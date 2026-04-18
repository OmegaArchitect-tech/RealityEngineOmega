// RealityEngineOmega.cpp — PATCHED v3
#include "RealityEngineOmega.h"
#include "Engine/Engine.h"

// ── Box-Muller Gaussian sampler ───────────────────────────────────────────────
// Replaces uniform U[-1,1] — KPZ requires Gaussian white noise N(0,D²)
float URealityEngineOmega::GaussianNoise(float Amplitude)
{
	float U1 = FMath::Max(FMath::FRand(), 1e-7f);
	float U2 = FMath::FRand();
	float Z  = FMath::Sqrt(-2.0f * FMath::Loge(U1)) * FMath::Cos(2.0f * PI * U2);
	return Amplitude * Z;
}

// ── Initialize ────────────────────────────────────────────────────────────────
void URealityEngineOmega::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	// FIX F-07: required for UWorldSubsystem — without this Tick() never fires.
	// Do NOT use FTickableGameObject — it is incompatible with UCLASS/GENERATED_BODY().
	bCanEverTick = true;

	DarkEnergyEquationOfState_W = COSMO_W0;
	UniversalScaleFactor_A      = 1.0;
	TotalSystemEntropy          = 0.0;

	UE_LOG(LogTemp, Warning, TEXT("Reality Engine Omega: Initialized."));
}

// ── CalculateDynamicExpansion ─────────────────────────────────────────────────
// CPL dark energy: w(a) = w₀ + wₐ(1 − a), clamped to [-2, 0]
// Friedmann: H²(a) = Ωm·a⁻³ + ΩΛ·a^{-3(1+w)}
void URealityEngineOmega::CalculateDynamicExpansion(float DeltaTime)
{
	DarkEnergyEquationOfState_W = FMath::Clamp(
		COSMO_W0 + COSMO_WA * (1.0 - UniversalScaleFactor_A),
		-2.0, 0.0
	);

	const double A    = UniversalScaleFactor_A;
	const double W    = DarkEnergyEquationOfState_W;
	const double H_sq = COSMO_OMEGA_M * FMath::Pow(A, -3.0)
	                  + COSMO_OMEGA_L * FMath::Pow(A, -3.0 * (1.0 + W));
	const double H    = FMath::Sqrt(FMath::Max(H_sq, 0.0));

	UniversalScaleFactor_A *= FMath::Exp(H * static_cast<double>(DeltaTime));
}

// ── ResolveQuantumDecoherence ─────────────────────────────────────────────────
// Passes CONST_HBAR (ħ = h/2π) — not CONST_H. FIX F-03.
void URealityEngineOmega::ResolveQuantumDecoherence(float DeltaTime)
{
	for (auto& QuantumGrid : ActiveSubatomicGrids)
	{
		if (QuantumGrid.ObserverCount > 0 || QuantumGrid.EnvironmentalNoise > DecoherenceThreshold)
		{
			QuantumGrid.CollapseToMacroState();
		}
		else
		{
			QuantumGrid.UpdateSchrodingerEquation(CONST_HBAR, DeltaTime);
		}
	}
}

// ── EnforceThermodynamics ─────────────────────────────────────────────────────
// ΔS = Q/T [J K⁻¹]. Landauer floor: Q ≥ k_B·T·ln2·BitErasureCount. FIX F-04.
void URealityEngineOmega::EnforceThermodynamics(float DeltaTime)
{
	for (auto& Actor : ActiveActors)
	{
		const double T = FMath::Max(Actor.Temperature, 1e-9);

		double WasteHeat    = T * (Actor.Complexity / FMath::Max(Actor.Efficiency, 1e-9))
		                    * static_cast<double>(DeltaTime);
		double LandauerHeat = CONST_KB * T * FMath::Loge(2.0)
		                    * static_cast<double>(Actor.BitErasureCount);
		double TotalHeat    = FMath::Max(WasteHeat, LandauerHeat);

		Actor.InternalEnergy = FMath::Max(0.0, Actor.InternalEnergy - TotalHeat);
		TotalSystemEntropy  += TotalHeat / T;   // accumulate in [J K⁻¹]
	}
}

// ── ProcessKPZGrowth ──────────────────────────────────────────────────────────
// Full KPZ: ∂h/∂t = ν∇²h + (λ/2)|∇h|² + η(x,t). FIX F-02.
// Cell double-buffer prevents within-frame write-read aliasing.
void URealityEngineOmega::ProcessKPZGrowth(float DeltaTime)
{
	for (auto& Surface : ActiveGrowthSurfaces)
	{
		const int32 N = Surface.GridSize;
		TArray<float> NewHeightMap = Surface.HeightMap;

		for (int32 Y = 0; Y < N; ++Y)
		{
			for (int32 X = 0; X < N; ++X)
			{
				const int32 Idx = Y * N + X;
				if (!Surface.HeightMap.IsValidIndex(Idx)) continue;

				float Diffusion = Surface.DiffusionConstant * Surface.Laplacian(X, Y);
				float NonLinear = 0.5f * Surface.LambdaKPZ  * Surface.GradientSquared(X, Y);
				float Noise     = GaussianNoise(Surface.NoiseAmplitude);

				NewHeightMap[Idx] += (Diffusion + NonLinear + Noise) * DeltaTime;
			}
		}

		Surface.HeightMap = MoveTemp(NewHeightMap);
	}
}

// ── Tick ──────────────────────────────────────────────────────────────────────
// FIX F-08: Thermodynamics precedes KPZ so energy budgets are settled first.
// FIX N-09: Super::Tick() restored.
void URealityEngineOmega::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);   // FIX N-09: required for UWorldSubsystem bookkeeping

	CalculateDynamicExpansion(DeltaTime);
	ResolveQuantumDecoherence(DeltaTime);
	EnforceThermodynamics(DeltaTime);   // settle budgets before growth draws from them
	ProcessKPZGrowth(DeltaTime);

	// God-mode HUD overlay
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1, 0.0f, FColor::Orange,
			FString::Printf(TEXT("Entropy (S): %.4e J/K"), TotalSystemEntropy));
		GEngine->AddOnScreenDebugMessage(2, 0.0f, FColor::Cyan,
			FString::Printf(TEXT("Scale factor (a): %.6f"), UniversalScaleFactor_A));
		GEngine->AddOnScreenDebugMessage(3, 0.0f, FColor::Yellow,
			FString::Printf(TEXT("Dark energy w(a): %.4f"), DarkEnergyEquationOfState_W));
	}
}

// ── Public Injection API ──────────────────────────────────────────────────────
// FIX N-04: External actors call these instead of accessing ActiveActors directly.

void URealityEngineOmega::InjectActor(const FThermodynamicActor& NewActor)
{
	ActiveActors.Add(NewActor);
	UE_LOG(LogTemp, Log, TEXT("Omega API: Actor injected. T=%.1fK E=%.1fJ"),
		NewActor.Temperature, NewActor.InternalEnergy);
}

void URealityEngineOmega::InjectMatter(float StartingTemp, float Energy)
{
	FThermodynamicActor NewMatter;
	NewMatter.Temperature    = static_cast<double>(StartingTemp);
	NewMatter.InternalEnergy = static_cast<double>(Energy);
	NewMatter.Complexity     = 1.5;
	NewMatter.Efficiency     = 0.8;

	ActiveActors.Add(NewMatter);
	UE_LOG(LogTemp, Log, TEXT("Omega API: Matter injected. T=%.1fK E=%.1fJ"),
		StartingTemp, Energy);
}

// ── Deinitialize ──────────────────────────────────────────────────────────────
void URealityEngineOmega::Deinitialize()
{
	UE_LOG(LogTemp, Warning, TEXT("Reality Engine Terminated. Final entropy: %.6e J/K"),
		TotalSystemEntropy);
	Super::Deinitialize();
}

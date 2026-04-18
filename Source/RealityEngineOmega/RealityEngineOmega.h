// RealityEngineOmega.h — PATCHED v3
#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "RealityEngineOmega.generated.h"

// ── Fundamental Constants (2019 SI exact definitions) ────────────────────────
constexpr double CONST_C    = 299792458.0;              // Speed of light      [m s⁻¹]
constexpr double CONST_G    = 6.67430e-11;               // Gravitational const [m³ kg⁻¹ s⁻²]
constexpr double CONST_H    = 6.62607015e-34;            // Planck constant     [J s]
constexpr double CONST_HBAR = CONST_H / (2.0 * UE_PI);  // FIX N-03: UE_PI not PI
constexpr double CONST_KB   = 1.380649e-23;              // Boltzmann constant  [J K⁻¹]

// ── Cosmological Parameters (DESI 2024 / Planck 2018) ────────────────────────
constexpr double COSMO_OMEGA_M = 0.315;
constexpr double COSMO_OMEGA_L = 0.685;
constexpr double COSMO_W0      = -0.99;
constexpr double COSMO_WA      =  0.30;

// ── FQuantumGrid ─────────────────────────────────────────────────────────────
USTRUCT(BlueprintType)
struct FQuantumGrid
{
	GENERATED_BODY()

	UPROPERTY() int32 ObserverCount      = 0;
	UPROPERTY() float EnvironmentalNoise = 0.0f;

	TArray<double> PsiReal;
	TArray<double> PsiImag;

	void CollapseToMacroState()
	{
		double MaxProb = 0.0;
		for (int32 i = 0; i < PsiReal.Num(); ++i)
		{
			double P = PsiReal[i]*PsiReal[i] + PsiImag[i]*PsiImag[i];
			if (P > MaxProb) MaxProb = P;
		}
	}

	void UpdateSchrodingerEquation(double Hbar, float DeltaTime)
	{
		const double Phase = -static_cast<double>(DeltaTime) / Hbar;
		for (int32 i = 0; i < PsiReal.Num(); ++i)
		{
			double R  = PsiReal[i]*FMath::Cos(Phase) - PsiImag[i]*FMath::Sin(Phase);
			double Im = PsiReal[i]*FMath::Sin(Phase) + PsiImag[i]*FMath::Cos(Phase);
			PsiReal[i] = R;
			PsiImag[i] = Im;
		}
	}
};

// ── FThermodynamicActor ───────────────────────────────────────────────────────
USTRUCT(BlueprintType)
struct FThermodynamicActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite) double Temperature    = 300.0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) double Complexity     = 1.0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) double Efficiency     = 1.0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) double InternalEnergy = 1000.0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) int64  BitErasureCount = 0;
};

// ── FGrowthSurface ────────────────────────────────────────────────────────────
USTRUCT(BlueprintType)
struct FGrowthSurface
{
	GENERATED_BODY()

	TArray<float> HeightMap;
	UPROPERTY() int32 GridSize          = 32;
	UPROPERTY() float DiffusionConstant = 0.1f;
	UPROPERTY() float LambdaKPZ         = 0.5f;
	UPROPERTY() float NoiseAmplitude    = 1.0f;

	float Laplacian(int32 X, int32 Y) const
	{
		auto H = [&](int32 ix, int32 iy) -> float {
			int32 cx = (ix + GridSize) % GridSize;
			int32 cy = (iy + GridSize) % GridSize;
			return HeightMap.IsValidIndex(cy*GridSize+cx) ? HeightMap[cy*GridSize+cx] : 0.f;
		};
		return H(X+1,Y) + H(X-1,Y) + H(X,Y+1) + H(X,Y-1) - 4.f*H(X,Y);
	}

	float GradientSquared(int32 X, int32 Y) const
	{
		auto H = [&](int32 ix, int32 iy) -> float {
			int32 cx = (ix + GridSize) % GridSize;
			int32 cy = (iy + GridSize) % GridSize;
			return HeightMap.IsValidIndex(cy*GridSize+cx) ? HeightMap[cy*GridSize+cx] : 0.f;
		};
		float dX = (H(X+1,Y) - H(X-1,Y)) * 0.5f;
		float dY = (H(X,Y+1) - H(X,Y-1)) * 0.5f;
		return dX*dX + dY*dY;
	}
};

// ── URealityEngineOmega ───────────────────────────────────────────────────────
// Derives ONLY from UWorldSubsystem — never add FTickableGameObject.
// Tick enabled via bCanEverTick = true in Initialize().
UCLASS()
class MYPROJECT2_API URealityEngineOmega : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	virtual void Tick(float DeltaTime) override;

	void CalculateDynamicExpansion(float DeltaTime);
	void ResolveQuantumDecoherence(float DeltaTime);
	void EnforceThermodynamics(float DeltaTime);
	void ProcessKPZGrowth(float DeltaTime);

	// Public injection API — FIX N-04: replaces direct ActiveActors access
	UFUNCTION(BlueprintCallable, Category = "Omega API")
	void InjectActor(const FThermodynamicActor& NewActor);

	UFUNCTION(BlueprintCallable, Category = "Omega API")
	void InjectMatter(float StartingTemp, float Energy);

private:
	double DarkEnergyEquationOfState_W = COSMO_W0;
	double UniversalScaleFactor_A      = 1.0;
	double TotalSystemEntropy          = 0.0;   // [J K⁻¹]

	double DecoherenceThreshold = 0.01;

	TArray<FQuantumGrid>        ActiveSubatomicGrids;
	TArray<FThermodynamicActor> ActiveActors;
	TArray<FGrowthSurface>      ActiveGrowthSurfaces;

	static float GaussianNoise(float Amplitude);
};

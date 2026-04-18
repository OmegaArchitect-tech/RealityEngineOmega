// AnomalyCube.cpp — PATCHED v3
// FIX N-01/N-05/N-06: Actor implementation separated from subsystem .cpp.
// FIX N-04: Uses InjectActor() public API — no direct ActiveActors access.
#include "AnomalyCube.h"

AAnomalyCube::AAnomalyCube()
{
	PrimaryActorTick.bCanEverTick = true;

	CubeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CubeMesh"));
	RootComponent = CubeMesh;

	// Default thermodynamic profile: hot, complex, large energy pool
	PhysicsData.Temperature    = 5000.0;   // [K]
	PhysicsData.Complexity     = 5.0;
	PhysicsData.Efficiency     = 0.8;
	PhysicsData.InternalEnergy = 100000.0; // [J]
}

void AAnomalyCube::BeginPlay()
{
	Super::BeginPlay();

	if (GetWorld())
	{
		RealityEngine = GetWorld()->GetSubsystem<URealityEngineOmega>();

		if (RealityEngine)
		{
			// FIX N-04: Call the public API instead of accessing private ActiveActors directly
			RealityEngine->InjectActor(PhysicsData);
			UE_LOG(LogTemp, Warning, TEXT("Anomaly Cube injected into the Reality Engine."));
		}
	}
}

void AAnomalyCube::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// Future: sync subsystem state back to visual mesh (scale, color, heat shimmer)
}

void AAnomalyCube::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	// Future: remove this actor's FThermodynamicActor from ActiveActors on destroy
}

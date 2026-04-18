// AnomalyCube.h — PATCHED v3
// FIX N-01: Separated from RealityEngineOmega.h into its own file pair.
// FIX N-02: AAnomalyCube is now in its own header with correct .generated.h.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RealityEngineOmega.h"
#include "AnomalyCube.generated.h"

UCLASS(BlueprintType)
class MYPROJECT2_API AAnomalyCube : public AActor
{
	GENERATED_BODY()

public:
	AAnomalyCube();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* CubeMesh;

	// Thermodynamic properties exposed for Blueprint editing
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physics")
	FThermodynamicActor PhysicsData;

	// Cached pointer to the universe
	UPROPERTY()
	URealityEngineOmega* RealityEngine;
};

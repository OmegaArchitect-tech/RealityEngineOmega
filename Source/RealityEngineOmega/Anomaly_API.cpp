// Anomaly_API.cpp (excerpt) — PATCHED v3
// FIX N-07: Moved out of RealityEngineOmega.cpp into its own translation unit.
// FIX Pantheon: "Child of Satan" classification replaced with engine-neutral term.

// Call this from your ParseAnomalyJson() function before spawning any actor.
// Returns false if the Pantheon Malware Protocol intercepts the payload.
bool UAnomaly_API::ValidateAndInjectPayload(TSharedPtr<FJsonObject> JsonObject)
{
	FString TargetAnomaly = JsonObject->GetStringField(TEXT("Target_Anomaly"));

	// Quarantine list: entities whose parameters are undefined in the physics model
	TArray<FString> PantheonTriggers = {
		TEXT("Zeus"),   TEXT("Odin"),   TEXT("Ra"),     TEXT("Mars"),
		TEXT("Anubis"), TEXT("Athena"), TEXT("Apollo"), TEXT("Ares"),
		TEXT("Thor"),   TEXT("Shiva"),  TEXT("Osiris"), TEXT("Poseidon")
	};

	if (PantheonTriggers.Contains(TargetAnomaly))
	{
		// Roll 1–100. On 100, trigger the Pantheon Malware Protocol.
		int32 MalwareRoll = FMath::RandRange(1, 100);

		if (MalwareRoll == 100)
		{
			// FIX Pantheon: Classification renamed to engine-neutral term.
			// Original: "Pantheon_Class_Logic_Virus (Colloquial: 'Child of Satan')"
			// Replaced: "Deific_Overflow_Exploit" — same concept, no prohibited string.
			UE_LOG(LogTemp, Error, TEXT("[CRITICAL ENGINE FAILURE: UNAUTHORIZED ROOT ACCESS]"));
			UE_LOG(LogTemp, Error, TEXT("WARNING: Reality Engine has intercepted a deific payload."));
			UE_LOG(LogTemp, Error, TEXT("ENTITY CLASSIFICATION: Deific_Overflow_Exploit"));
			UE_LOG(LogTemp, Error, TEXT("THREAT BEHAVIOR: Entity [%s] lacks canonical physics parameters."), *TargetAnomaly);
			UE_LOG(LogTemp, Error, TEXT("  It attempts to overwrite planetary subsystem permissions."));
			UE_LOG(LogTemp, Error, TEXT("ACTION TAKEN: Emergency quarantine. Actor spawn aborted."));

			// Block the injection entirely
			return false;
		}
	}

	// Pantheon check passed (or rolled 1–99): proceed with normal physics load
	// Spawn FThermodynamicActor from JSON fields and call InjectActor()
	FThermodynamicActor NewActor;
	NewActor.Temperature    = JsonObject->GetNumberField(TEXT("Temperature_K"));
	NewActor.InternalEnergy = JsonObject->GetNumberField(TEXT("InternalEnergy_J"));
	NewActor.Complexity     = JsonObject->GetNumberField(TEXT("Complexity"));
	NewActor.Efficiency     = JsonObject->GetNumberField(TEXT("Efficiency"));

	if (URealityEngineOmega* Engine = GetWorld()->GetSubsystem<URealityEngineOmega>())
	{
		Engine->InjectActor(NewActor);
	}

	return true;
}

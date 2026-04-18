================================================================================
RealityEngineOmega (v1.3)
The 4D Render Renaissance Catalyst
================================================================================

OVERVIEW
RealityEngineOmega is an Unreal Engine 5 C++ World Subsystem designed to completely replace standard video game logic with immutable, real-world physics. Instead of calculating hit points or mana, the engine calculates joules, entropy, quantum wave functions, and spacetime expansion. By injecting FThermodynamicActor data into the engine, developers can force digital entities, AI, and environmental mechanics to perfectly obey thermodynamics, quantum mechanics, and general relativity.

CORE FEATURES & FUNCTIONALITY
The engine evaluates all in-game actions through five primary physical constraints:

1. EnforceThermodynamics(): Enforces the First and Second Laws of Thermodynamics. Spontaneous energy generation is disabled. Actions requiring energy must drain directly from an actor's InternalEnergy. Localized decreases in entropy automatically generate proportional chaotic waste heat in the surrounding environment.
2. ResolveQuantumDecoherence(): Replaces standard "line-of-sight" mechanics. Uses a discrete Eulerian Time-Dependent Schrödinger Equation (TDSE) propagator. Entities existing in a state of probability instantly collapse into a definitive macro-state the moment the local ObserverCount > 0.
3. ProcessKPZGrowth(): Replaces simple scale-transformations for structural changes (e.g., petrification, ice spreading). It dynamically calculates the mathematical diffusion constant and Gaussian noise (seeded by a hardware TRNG pipeline) to render physically accurate surface growth.
4. CalculateDynamicExpansion(): Monitors the UniversalScaleFactor_A using the Friedmann equations to simulate cosmological dark energy expansion.
5. ProcessLocalCurvature(): Implements the localized stress-energy tensor. Massive injections of InternalEnergy (e=mc^2) actively warp a 64x64x64 3D grid, simulating localized gravitational lensing and spatial distortion relative to the Einstein constant.

POTENTIAL USAGE
* The Ultimate Hardware Stress Test: Calculating cascading thermal equations, real-time KPZ surface growth, and 4D spatial tensors across multiple grids is computationally devastating. This engine serves as a brutal test of hardware stability, server thread processing, and localized entropy math.
* Educational & Hardcore Physics Simulators: A sandbox for theoretical physicists or hardcore simulation enthusiasts to test the actual physical viability of extreme energy transfers.
* Next-Generation VR Immersion: NPC AI and environmental rendering can be driven by the localized shifts in the thermodynamic state of the room, polling the GetSimulationState() interface to adjust volumetric rendering.

HOW TO USE
1. Integration: Add the `RealityEngineOmega.h` and `RealityEngineOmega.cpp` files to your UE5 C++ project's source directory.
2. Initialization: As a UWorldSubsystem, the engine initializes automatically upon world load. It will immediately begin ticking and calculating background cosmological expansion.
3. Rendering Hooks: External volumetric renderers or particle systems can call `GetSimulationState()` via Blueprint or C++ to access the current Universal Scale Factor, Total Entropy, and Max Local Curvature to drive visual distortions.
4. Spawning Matter: Use the public API function `InjectMatter(float StartingTemp, float Energy, FVector SpawnLoc)` to spawn new thermodynamic masses into the simulation.
5. Custom Actors: Create custom actors utilizing the `FThermodynamicActor` struct and inject them using `InjectActor()` to expose them to the engine's physics tick.

================================================================================
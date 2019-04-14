// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Characters/CharacterHealthComponent.h"
#include "Engine/Classes/Camera/CameraShake.h"
#include "Weapons/BaseGun.h"
#include "Engine/Public/TimerManager.h"
#include "MyCharacter.generated.h"

// ENUM to determine position of wall against the player
UENUM()
enum EWallNeighborhood
{
	WN_None,
	WN_Front,
	WN_Right,
	WN_Left
};

UCLASS()
class FPPLATFORMER_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyCharacter();

	// Base turn rate, in deg/sec. Other scaling may affect final turn rate
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;

	// Base look up/down rate, in deg/sec. Other scaling may affect final rate
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseLookUpRate;

	// Camera shake to play on jumping
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
		TSubclassOf<UCameraShake> JumpCameraShake;

	// Camera shake to play on landing
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
		TSubclassOf<UCameraShake> LandCameraShake;

	// Camera shake to play on running
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
		TSubclassOf<UCameraShake> RunningCameraShake;

	// First person camera 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FirstPersonCameraComponent;

	// Linetrace range for shooting
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gun)
		float ShootRange = 3000.0f;

	// How many additional jumps can player perform airborne
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Parkour)
		int JumpLimit = 2;

	// Horizontal speed applied to player when performing double jump
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Parkour)
		float DoubleJumpForce = 150;

	// Line trace start location for wall detection
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Parkour)
		FVector LineTraceStart;

	// Line trace range for wall detection
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Parkour)
		float LineTraceRange = 40;

	// Gravity scale during vertical wallrunning
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Parkour)
		float VerticalParkourGravityScale = 0.15f;

	// Gravity scale during horizontal wallrunning
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Parkour)
		float HorizontalParkourGravityScale = 0.1f;

	// Force applied to player when beginning vertical wallrun
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Parkour)
		float VerticalWallRunForce = 300.0f;
	
	// Force applied to player when beginning horizontal wallrun
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Parkour)
		float HorizontalWallRunForce = 350.0f;

	// Force applied to player when beginning vertical wallrun
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Parkour)
		float WallStickForce = 250.0f;

	// Minimum Z velocity for the player to keep wallrunning vertically
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Parkour)
		float MinVerticalZSpeed = 0.0f;

	// Minimum Z velocity for the player to keep wallrunning horizontally
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Parkour)
		float MinHorizontalZSpeed = -100.0f;

	// Wallrun cooldown to prevent running the wall we just jumped of
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Parkour)
		float WallrunCooldown = 0.5f;

	// Distance from what player can attach to green spheres
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GraplingHook)
		float GraplingRange = 500.0f;

	// Grapling speed
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GraplingHook)
		float GraplingSpeed = 2500.0f;

	// Velocity applied to the player on walljump
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Parkour)
		float WallJumpForce = 500.0f;

	// Dodge speed
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Dodge)
		float DodgeForce = 500.0f;

	// Dodge duration
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Dodge)
		float DodgeTime = 0.2f;

	// Dodge cooldown
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Dodge)
		float DodgeCooldown = 1.0f;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Handles jumping
	virtual void Jump() override;

	// Handles releasing of jump button
	virtual void StopJumping() override;

	// Handles landing
	virtual void Landed(const FHitResult & Hit) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Health Component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Health, meta = (AllowPrivateAccess = "true"))
		UCharacterHealthComponent* CharacterHealth;

	// Gets point the player's looking at
	FVector GetLookedPoint();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Primary gun pressed action
	void OnFirePressed();

	// Dodge action
	void Dodge();

	// Dodge end
	void StopDodging();

	// Reset dodge
	void ResetDodge();

	// Enable wallrunning
	void EnableWallrunning();

	// Primary gun released action
	void OnFireReleased();

	// Secondary gun pressed action
	void OnAltFirePressed();

	// Secondary gun released action
	void OnAltFireReleased();

	// Handles moving forward/backward
	void MoveForward(float Val);

	// Handles stafing movement, left and right
	void MoveRight(float Val);

	// Called via input to turn at a given rate
	void TurnAtRate(float Rate);

	// Called via input to turn look up/down at a given rate
	void LookUpAtRate(float Rate);

	// Switch to gun from slot 1
	void GetSlotOne();

	// Switch to gun from slot 2
	void GetSlotTwo();

	// Switch to gun from slot 3
	void GetSlotThree();

	// Switch to gun from given slot
	void SwitchWeapon(int id);

	// Grapling to sphere action
	void Grip();

	// Dropping from walls/grappling // Switch to gun from slot 1
	void Drop();

	// Timer handle for dodges
	FTimerHandle DodgeTimerHandle;

	// Timer handle wallrunning cooldown 
	FTimerHandle WallrunCooldownTimerHandle;

public:
	// For clearing timers when bullet is destroyed
	virtual void Destroyed() override;

	// For clearing timers when game stops
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	// Handle overlaps
	UFUNCTION()
		virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

	// When character's HP reaches 0
	UFUNCTION()
		void Die();

	virtual void OnWalkingOffLedge_Implementation(const FVector & PreviousFloorImpactNormal, const FVector & PreviousFloorContactNormal, const FVector & PreviousLocation, float TimeDelta) override;

private:
	// Double jumps count
	int	JumpCount;

	// If the player stands close to a wall
	bool IsCloseToWall;

	// If player holds parkour key down
	bool JumpKeyDown;

	// Can player start wallrunning during current jump
	bool CanWallRun;

	// If the player is wallrunning
	bool IsWallRunning;

	// If the player is dodging
	bool IsDodging;

	// Can player dodge in current frame
	bool CanDodge;

	// Is player attached to sphere
	bool IsGrapling;

	// Original friction to apply after dodge ended
	float OriginalFriction;

	// Normal of the wall the player is close to/running on
	FVector AttachedWallNormal;

	// Status of the close walls
	EWallNeighborhood WallNeighborhood;

	// Search for walls nearby
	EWallNeighborhood CheckForWallsNearby(FVector &wallNormal);

	// ID of currently equipped gun
	int CurrentGunID = 0;
	
	// Get currently equipped gun object
	ABaseGun* GetCurrentGun();

	// Performed when player stops wallrunning
	void StopWallrun();

	// Performed when player stops grapling
	void StopGrapling();

	// Called during tick to check if player can continue wallrunning
	void HandleWallrunning();

	// Can player climb on the object he's currently running on
	bool CanClimb;

	// All player's guns
	TArray<ABaseGun*> Guns;

	// Grapling direction that player should maintain
	FVector GraplingMovementNormal;

	// Last checkpoint visited
	FVector LastCheckpointLocation;

	// Pointer to to shake being applied constantly when running
	UCameraShake* RunningShake;
};

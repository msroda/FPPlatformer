// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyCharacter.generated.h"

UENUM()
enum EWallNeighborhood
{
	WN_None,
	WN_Front,
	WN_Side
};

UCLASS()
class FPPLATFORMER_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyCharacter();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Primary gun action */
	void OnFire();

	/** Secondary gun action */
	void OnAltFire();

	/** Start parkour input */
	void ParkourStart();

	/** Stop parkour input */
	void ParkourEnd();

	/** Resets HMD orientation and position in VR. */
	void OnResetVR();

	/** Handles jumping and parkour */
	void CustomJump();

	/** Handles moving forward/backward */
	void MoveForward(float Val);

	/** Handles stafing movement, left and right */
	void MoveRight(float Val);

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);
public:
	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseLookUpRate;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FirstPersonCameraComponent;

	/** Gun local position */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gun)
		FVector GunOffset;

	/** How many additional jumps can player perform airborne*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Parkour)
		int JumpLimit = 2;

	/** Horizontal speed applied to player when performing double jump*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Parkour)
		float DoubleJumpForce = 150;

	/** Line trace start location for wall detection*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Parkour)
		FVector LineTraceStart;

	/** Line trace range for wall detection*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Parkour)
		float LineTraceRange = 40;

	/** Line trace start location offset for additional wall detection*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Parkour)
		float LineTraceShift = 20;

	/** Gravity scale during vertical wallrunning*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Parkour)
		float VerticalParkourGravityScale = 0.15f;

	/** Gravity scale during horizontal wallrunning*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Parkour)
		float HorizontalParkourGravityScale = 0.1f;

	/** Force applied to player when beginning vertical wallrun*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Parkour)
		float VerticalWallRunForce = 300.0f;

	/** Force applied to player when beginning horizontal wallrun*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Parkour)
		float HorizontalWallRunForce = 350.0f;

	/** Force applied to player when beginning vertical wallrun*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Parkour)
		float WallStickForce = 250.0f;

	/** Minimum Z velocity for the player to keep wallrunning vertically*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Parkour)
		float MinVerticalZSpeed = 0.0f;

	/** Minimum Z velocity for the player to keep wallrunning horizontally*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Parkour)
		float MinHorizontalZSpeed = -100.0f;

	/** Velocity applied to the player on walljump*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Parkour)
		float WallJumpForce = 500.0f;

	/** Gun class */
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
		TSubclassOf<class AActor> GunClass;

	/** Called every frame */
	virtual void Tick(float DeltaTime) override;

	/** Handles jumping */
	virtual void Jump() override;

	/** Handles landing */
	virtual void Landed(const FHitResult & Hit) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	/** Double jumps count */
	int	JumpCount;

	/** If the player stands close to a wall */
	bool IsCloseToWall;

	/** If player holds parkour key down */
	bool ParkourKeyDown;

	/** Can player start wallrunning during current jump */
	bool CanWallRun;

	/** If the player is wallrunning */
	bool IsWallRunning;

	/** Normal of the wall the player is close to/running on */
	FVector AttachedWallNormal;

	/** Status of the close walls */
	EWallNeighborhood WallNeighborhood;

	/** Search for walls  */
	EWallNeighborhood CheckForWallsNearby(FVector &wallNormal);

	/** Performed when player stops wallrunning*/
	void StopWallrun();
};

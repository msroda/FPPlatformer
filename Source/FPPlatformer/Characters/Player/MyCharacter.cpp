// Fill out your copyright notice in the Description page of Project Settings.

#include "MyCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Runtime/Engine/Classes/GameFramework/CharacterMovementComponent.h"
#include "Runtime/Engine/Public/DrawDebugHelpers.h"

// Sets default values
AMyCharacter::AMyCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->RelativeLocation = FVector(-39.56f, 1.75f, 64.f); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;
	
	LineTraceStart = FVector(0.0f, 0.0f, -50.0f);

	JumpCount = 0;

	IsWallRunning = false;
	CanWallRun = true;
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsWallRunning)
	{
		//check if player wants to stop wallrunning
		if (!ParkourKeyDown)
		{
			StopWallrun();
		}
		//check if the wallrunning needs to be stopped by the gravity
		else if ((WallNeighborhood == WN_Front && GetVelocity().Z < MinVerticalZSpeed) || (WallNeighborhood == WN_Side && GetVelocity().Z < MinHorizontalZSpeed))
		{
			StopWallrun();
		}
		//check if there's still a wall to run on
		else
		{
			const FRotator rot(0.0f, 0.0f, 90.0f);
			FVector startPosShift = rot.RotateVector(AttachedWallNormal) * LineTraceShift;

			FHitResult outHit;
			bool isHit = false;

			FVector startLocation = GetActorLocation() + LineTraceStart + startPosShift;
			FCollisionQueryParams collisionParams;

			//1st check
			isHit = GetWorld()->LineTraceSingleByChannel(outHit, startLocation, startLocation - AttachedWallNormal * LineTraceRange, ECC_Visibility, collisionParams);

			if (isHit)
			{
				AttachedWallNormal = outHit.ImpactNormal;
				DrawDebugLine(GetWorld(), outHit.ImpactPoint, outHit.ImpactPoint + outHit.ImpactNormal * LineTraceRange, FColor::Green, false, 1, 0, 1);
			}
			//2nd check
			else
			{
				startLocation -= 2 * startPosShift;
				isHit = GetWorld()->LineTraceSingleByChannel(outHit, startLocation, startLocation - AttachedWallNormal * LineTraceRange, ECC_Visibility, collisionParams);

				if (isHit)
				{
					AttachedWallNormal = outHit.ImpactNormal;
					DrawDebugLine(GetWorld(), outHit.ImpactPoint, outHit.ImpactPoint + outHit.ImpactNormal * LineTraceRange, FColor::Green, false, 1, 0, 1);
				}
				else
				{
					StopWallrun();
				}
			}
		}
	}
	else
	{
		WallNeighborhood = CheckForWallsNearby(AttachedWallNormal);

		//if player wants to wallrun and there's a wall nearby, that is vertical
		if (ParkourKeyDown && CanWallRun && WallNeighborhood != WN_None && AttachedWallNormal.Z >= -0.05f && AttachedWallNormal.Z < 0.15f)
		{
			CanWallRun = false;			
			IsWallRunning = true;
			//wall is in front of the player - run upwards
			if (WallNeighborhood == WN_Front)
			{
				GetCharacterMovement()->GravityScale = VerticalParkourGravityScale;
				LaunchCharacter(FVector(0.0f, 0.0f, VerticalWallRunForce) - AttachedWallNormal * WallStickForce, true, true);
			}
			//wall on the side
			else
			{
				GetCharacterMovement()->GravityScale = HorizontalParkourGravityScale;
				LaunchCharacter(FVector::VectorPlaneProject(FirstPersonCameraComponent->GetForwardVector(), AttachedWallNormal).GetUnsafeNormal2D() * HorizontalWallRunForce - AttachedWallNormal * WallStickForce, true, true);
			}
		}
	}
}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	check(PlayerInputComponent);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("Parkour", IE_Pressed, this, &AMyCharacter::ParkourStart);
	PlayerInputComponent->BindAction("Parkour", IE_Released, this, &AMyCharacter::ParkourEnd);

// Bind fire event
PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AMyCharacter::OnFire);

// Bind fire event
PlayerInputComponent->BindAction("Fire2", IE_Pressed, this, &AMyCharacter::OnAltFire);

// Bind movement events
PlayerInputComponent->BindAxis("MoveForward", this, &AMyCharacter::MoveForward);
PlayerInputComponent->BindAxis("MoveRight", this, &AMyCharacter::MoveRight);

// We have 2 versions of the rotation bindings to handle different kinds of devices differently
// "turn" handles devices that provide an absolute delta, such as a mouse.
// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
PlayerInputComponent->BindAxis("TurnRate", this, &AMyCharacter::TurnAtRate);
PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
PlayerInputComponent->BindAxis("LookUpRate", this, &AMyCharacter::LookUpAtRate);

}

void AMyCharacter::OnFire()
{
	// try and fire a projectile
	if (GunClass)
	{
		/*UWorld* const World = GetWorld();
		if (World != NULL)
		{
			if (bUsingMotionControllers)
			{
				const FRotator SpawnRotation = VR_MuzzleLocation->GetComponentRotation();
				const FVector SpawnLocation = VR_MuzzleLocation->GetComponentLocation();
				World->SpawnActor<APlatformer3DProjectile>(ProjectileClass, SpawnLocation, SpawnRotation);
			}
			else
			{
				const FRotator SpawnRotation = GetControlRotation();
				// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
				const FVector SpawnLocation = ((FP_MuzzleLocation != nullptr) ? FP_MuzzleLocation->GetComponentLocation() : GetActorLocation()) + SpawnRotation.RotateVector(GunOffset);

				//Set Spawn Collision Handling Override
				FActorSpawnParameters ActorSpawnParams;
				ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

				// spawn the projectile at the muzzle
				World->SpawnActor<APlatformer3DProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
			}*/
	}
}

void AMyCharacter::OnAltFire()
{
	// try and fire a projectile
	if (GunClass)
	{

	}
}

void AMyCharacter::ParkourStart()
{
	ParkourKeyDown = true;
}

void AMyCharacter::ParkourEnd()
{
	ParkourKeyDown = false;
}

void AMyCharacter::Jump()
{
	if (IsWallRunning)
	{
		//walljump
		FVector desiredVelocity = AttachedWallNormal * WallJumpForce;
		//desiredVelocity.Z = GetCharacterMovement()->JumpZVelocity;
		LaunchCharacter(desiredVelocity, true, true);
		//StopWallrun();
		JumpCount = 1;
		CanWallRun = true;
	}
	else
	{
		// normal jump
		if (JumpCount == 0)
		{
			Super::Jump();
			JumpCount++;
		}
		// second jump (air launch)
		else if (JumpCount < JumpLimit)
		{
			// desired air launch velocity based on the input
			FVector desiredVelocity = FVector(InputComponent->GetAxisValue(TEXT("MoveForward")), InputComponent->GetAxisValue(TEXT("MoveRight")), 0.0f);

			// if there's input from player, launch him in the desired direction, making sure he doesn't lose too much of the current velocity
			if (desiredVelocity.Size() > 0.0f)
			{
				desiredVelocity = GetActorRotation().RotateVector(desiredVelocity.GetUnsafeNormal2D());

				float dotProduct = FVector::DotProduct(desiredVelocity, GetVelocity().GetUnsafeNormal2D());

				float horizontalSpeed = FMath::Max(DoubleJumpForce, GetVelocity().Size2D() * dotProduct);
				float verticalSpeed = FMath::Max(GetCharacterMovement()->JumpZVelocity, GetVelocity().Z * dotProduct);

				desiredVelocity *= horizontalSpeed;
				desiredVelocity.Z = verticalSpeed;

				LaunchCharacter(desiredVelocity, true, true);
				JumpCount++;
			}
			// else don't change his horizontal velocity
			else
			{
				// if launch can affect vertical speed
				if (GetCharacterMovement()->JumpZVelocity > GetVelocity().Z)
				{
					LaunchCharacter(FVector(0.0f, 0.0f, GetCharacterMovement()->JumpZVelocity), false, true);
					JumpCount++;
				}
			}
		}
	}
}

void AMyCharacter::MoveForward(float Value)
{
	if (Value != 0.0f && !IsWallRunning)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void AMyCharacter::MoveRight(float Value)
{
	if (Value != 0.0f && !IsWallRunning)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void AMyCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AMyCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AMyCharacter::Landed(const FHitResult & Hit)
{
	Super::Landed(Hit);
	JumpCount = 0;
	CanWallRun = true;
	IsWallRunning = false;
}

EWallNeighborhood AMyCharacter::CheckForWallsNearby(FVector &wallNormal)
{
	FHitResult outHit;
	bool isHit = false;

	FVector startLocation = GetActorLocation() + LineTraceStart;

	FCollisionQueryParams collisionParams;	

	// front
	isHit = GetWorld()->LineTraceSingleByChannel(outHit, startLocation, startLocation + GetActorForwardVector() * (LineTraceRange + LineTraceShift), ECC_Visibility, collisionParams);

	if (isHit)
	{
		wallNormal = outHit.ImpactNormal;
		return WN_Front;
	}

	//right side 1st check
	startLocation.X += LineTraceShift;
	isHit = GetWorld()->LineTraceSingleByChannel(outHit, startLocation, startLocation + GetActorForwardVector() * LineTraceRange, ECC_Visibility, collisionParams);
	if (isHit)
	{
		DrawDebugLine(GetWorld(), outHit.ImpactPoint, outHit.ImpactPoint + outHit.ImpactNormal * LineTraceRange, FColor::Green, false, 1, 0, 1);
		wallNormal = outHit.ImpactNormal;
		return WN_Side;
	}

	//right side 2nd check
	startLocation.X -= 2 * LineTraceShift;
	isHit = GetWorld()->LineTraceSingleByChannel(outHit, startLocation, startLocation + GetActorForwardVector() * LineTraceRange, ECC_Visibility, collisionParams);
	if (isHit)
	{
		DrawDebugLine(GetWorld(), outHit.ImpactPoint, outHit.ImpactPoint + outHit.ImpactNormal * LineTraceRange, FColor::Green, false, 1, 0, 1);
		wallNormal = outHit.ImpactNormal;
		return WN_Side;
	}

	//left side 1st check
	isHit = GetWorld()->LineTraceSingleByChannel(outHit, startLocation, startLocation - GetActorForwardVector() * LineTraceRange, ECC_Visibility, collisionParams);
	if (isHit)
	{
		DrawDebugLine(GetWorld(), outHit.ImpactPoint, outHit.ImpactPoint + outHit.ImpactNormal * LineTraceRange, FColor::Green, false, 1, 0, 1);
		wallNormal = outHit.ImpactNormal;
		return WN_Side;
	}

	//left side 2nd check
	startLocation.X += 2 * LineTraceShift;
	isHit = GetWorld()->LineTraceSingleByChannel(outHit, startLocation, startLocation - GetActorForwardVector() * LineTraceRange, ECC_Visibility, collisionParams);
	if (isHit)
	{
		DrawDebugLine(GetWorld(), outHit.ImpactPoint, outHit.ImpactPoint + outHit.ImpactNormal * LineTraceRange, FColor::Green, false, 1, 0, 1);
		wallNormal = outHit.ImpactNormal;
		return WN_Side;
	}

	return WN_None;
}

void AMyCharacter::StopWallrun()
{
	IsWallRunning = false;
	GetCharacterMovement()->GravityScale = 1.0f;
}
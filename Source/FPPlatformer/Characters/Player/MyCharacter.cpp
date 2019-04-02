// Fill out your copyright notice in the Description page of Project Settings.

#include "MyCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Runtime/Engine/Classes/GameFramework/CharacterMovementComponent.h"

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

	TArray<AActor*> tempArray;
	GetAllChildActors(tempArray, false);

	tempGun = dynamic_cast<ABaseGun*>(tempArray[0]);
	
	/*for (auto GunClass : GunsClasses)
	{
		ABaseGun* spawned;
		FActorSpawnParameters params;
		spawned = GetWorld()->SpawnActorAbsolute(GunClass, GetActorLocation() + gunffset, params);
		spawned->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
		spawned->SetOwner(this);
		Guns.Add(spawned);
	}*/
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsWallRunning)
	{
		//check if the wallrunning needs to be stopped by the gravity
		if ((WallNeighborhood == WN_Front && GetVelocity().Z < MinVerticalZSpeed) || (WallNeighborhood == WN_Side && GetVelocity().Z < MinHorizontalZSpeed))
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
			}
			//2nd check
			else
			{
				startLocation -= 2 * startPosShift;
				isHit = GetWorld()->LineTraceSingleByChannel(outHit, startLocation, startLocation - AttachedWallNormal * LineTraceRange, ECC_Visibility, collisionParams);

				if (isHit)
				{
					AttachedWallNormal = outHit.ImpactNormal;
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
		if (JumpKeyDown && CanWallRun && WallNeighborhood != WN_None && AttachedWallNormal.Z >= -0.05f && AttachedWallNormal.Z < 0.15f)
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
				LaunchCharacter(FVector::VectorPlaneProject(FirstPersonCameraComponent->GetForwardVector(), AttachedWallNormal).GetUnsafeNormal2D() * HorizontalWallRunForce + FVector(0.0f, 0.0f, VerticalWallRunForce / 3) - AttachedWallNormal * WallStickForce, true, true);
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

// Bind fire events
PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AMyCharacter::OnFirePressed);
PlayerInputComponent->BindAction("Fire", IE_Released, this, &AMyCharacter::OnFireReleased);
PlayerInputComponent->BindAction("AltFire", IE_Pressed, this, &AMyCharacter::OnAltFirePressed);
PlayerInputComponent->BindAction("AltFire", IE_Released, this, &AMyCharacter::OnAltFireReleased);

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

void AMyCharacter::OnFirePressed()
{
	FHitResult outHit;
	bool isHit = false;

	FVector startLocation = FirstPersonCameraComponent->GetComponentLocation();

	FCollisionQueryParams collisionParams;

	// front
	isHit = GetWorld()->LineTraceSingleByChannel(outHit, startLocation, startLocation + FirstPersonCameraComponent->GetForwardVector() * ShootRange, ECC_Visibility, collisionParams);

	if (isHit)
	{
		tempGun->OnFirePressed(outHit.ImpactPoint);
	}
	else
	{
		tempGun->OnFirePressed(startLocation + FirstPersonCameraComponent->GetForwardVector() * ShootRange);
	}
}

void AMyCharacter::OnFireReleased()
{
	FHitResult outHit;
	bool isHit = false;

	FVector startLocation = FirstPersonCameraComponent->GetComponentLocation();

	FCollisionQueryParams collisionParams;

	// front
	isHit = GetWorld()->LineTraceSingleByChannel(outHit, startLocation, startLocation + FirstPersonCameraComponent->GetForwardVector() * ShootRange, ECC_Visibility, collisionParams);

	if (isHit)
	{
		tempGun->OnFireReleased(outHit.ImpactPoint);
	}
	else
	{
		tempGun->OnFireReleased(startLocation + FirstPersonCameraComponent->GetForwardVector() * ShootRange);
	}
}

void AMyCharacter::OnAltFirePressed()
{
	FHitResult outHit;
	bool isHit = false;

	FVector startLocation = FirstPersonCameraComponent->GetComponentLocation();

	FCollisionQueryParams collisionParams;

	// front
	isHit = GetWorld()->LineTraceSingleByChannel(outHit, startLocation, startLocation + FirstPersonCameraComponent->GetForwardVector() * ShootRange, ECC_Visibility, collisionParams);

	if (isHit)
	{
		tempGun->OnAltFirePressed(outHit.ImpactPoint);
	}
	else
	{
		tempGun->OnAltFirePressed(startLocation + FirstPersonCameraComponent->GetForwardVector() * ShootRange);
	}
}

void AMyCharacter::OnAltFireReleased()
{
	FHitResult outHit;
	bool isHit = false;

	FVector startLocation = FirstPersonCameraComponent->GetComponentLocation();

	FCollisionQueryParams collisionParams;

	// front
	isHit = GetWorld()->LineTraceSingleByChannel(outHit, startLocation, startLocation + FirstPersonCameraComponent->GetForwardVector() * ShootRange, ECC_Visibility, collisionParams);

	if (isHit)
	{
		tempGun->OnAltFireReleased(outHit.ImpactPoint);
	}
	else
	{
		tempGun->OnAltFireReleased(startLocation + FirstPersonCameraComponent->GetForwardVector() * ShootRange);
	}
}

void AMyCharacter::Jump()
{
	JumpKeyDown = true;

	if (IsWallRunning)
	{
		//walljump
		FVector desiredVelocity = AttachedWallNormal * WallJumpForce;
		desiredVelocity.Z = GetCharacterMovement()->JumpZVelocity;
		LaunchCharacter(desiredVelocity, true, true);
		StopWallrun();
		JumpCount = 1;
		CanWallRun = true;
		JumpKeyDown = false;
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

void AMyCharacter::StopJumping()
{
	JumpKeyDown = false;
	Super::StopJumping();
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
	isHit = GetWorld()->LineTraceSingleByChannel(outHit, startLocation, startLocation + GetActorForwardVector() * LineTraceRange, ECC_Visibility, collisionParams);

	if (isHit)
	{
		wallNormal = outHit.ImpactNormal;
		return WN_Front;
	}

	//right side 1st check
	startLocation += GetActorForwardVector() * LineTraceShift;
	isHit = GetWorld()->LineTraceSingleByChannel(outHit, startLocation, startLocation + GetActorRightVector() * LineTraceRange, ECC_Visibility, collisionParams);
	if (isHit)
	{
		wallNormal = outHit.ImpactNormal;
		return WN_Side;
	}

	//right side 2nd check
	startLocation -= GetActorForwardVector() * LineTraceShift * 2;
	isHit = GetWorld()->LineTraceSingleByChannel(outHit, startLocation, startLocation + GetActorRightVector() * LineTraceRange, ECC_Visibility, collisionParams);
	if (isHit)
	{
		wallNormal = outHit.ImpactNormal;
		return WN_Side;
	}

	//left side 1st check
	isHit = GetWorld()->LineTraceSingleByChannel(outHit, startLocation, startLocation - GetActorRightVector() * LineTraceRange, ECC_Visibility, collisionParams);
	if (isHit)
	{
		wallNormal = outHit.ImpactNormal;
		return WN_Side;
	}

	//left side 2nd check
	startLocation += GetActorForwardVector() * LineTraceShift * 2;
	isHit = GetWorld()->LineTraceSingleByChannel(outHit, startLocation, startLocation - GetActorRightVector() * LineTraceRange, ECC_Visibility, collisionParams);
	if (isHit)
	{
		wallNormal = outHit.ImpactNormal;
		return WN_Side;
	}

	return WN_None;
}

ABaseGun * AMyCharacter::GetCurrentGun()
{
	return Guns[CurrentGunID];
}

void AMyCharacter::StopWallrun()
{
	IsWallRunning = false;
	GetCharacterMovement()->GravityScale = 1.0f;
}
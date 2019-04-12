// Fill out your copyright notice in the Description page of Project Settings.

#include "MyCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Engine/Classes/GameFramework/CharacterMovementComponent.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"

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

	CharacterHealth = CreateDefaultSubobject<UCharacterHealthComponent>(TEXT("CharacterHealth"));
	CharacterHealth->KillCharacter.AddDynamic(this, &AMyCharacter::Die);

	CharacterHealth->MaxHealth = 250.0f;
	CharacterHealth->RegenerationRate = 50.0f;
	CharacterHealth->RegenerationCooldown = 5.0f;
	
	LineTraceStart = FVector(0.0f, 0.0f, -50.0f);

	JumpCount = 0;

	IsWallRunning = false;
	CanWallRun = true;
	CanDodge = true;
	IsDodging = false;
	IsGrapling = false;
}

FVector AMyCharacter::GetLookedPoint()
{
	FHitResult outHit;
	bool isHit;

	TArray<AActor*> actorsToIgnore;
	actorsToIgnore.Add(this);

	isHit = UKismetSystemLibrary::LineTraceSingle(this, FirstPersonCameraComponent->GetComponentLocation(), FirstPersonCameraComponent->GetComponentLocation() + FirstPersonCameraComponent->GetForwardVector() * ShootRange, UEngineTypes::ConvertToTraceType(ECC_Visibility), false, actorsToIgnore, EDrawDebugTrace::ForOneFrame, outHit, true, FLinearColor::Red, FLinearColor::Green, 0.0f);

	if (isHit)
		return outHit.ImpactPoint;
	return FirstPersonCameraComponent->GetComponentLocation() + FirstPersonCameraComponent->GetForwardVector() * ShootRange;
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> children;
	GetAllChildActors(children, false);

	for (auto child : children)
	{
		ABaseGun* gun = Cast<ABaseGun>(child);
		if (gun)
		{
			Guns.Add(gun);
			gun->SetActive(false);
		}
	}

	if (Guns.Num() > 0)
	{
		Guns[0]->SetActive(true);
	}

	UCharacterMovementComponent* movement = Cast<UCharacterMovementComponent>(GetMovementComponent());
	if (movement)
		OriginalFriction = movement->GroundFriction;

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AMyCharacter::OnOverlapBegin);
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	if (IsGrapling)
	{
		if (GetVelocity().Size() == 0.0f || FVector::DotProduct(GraplingMovementNormal, GetVelocity().GetUnsafeNormal()) < 0.95f)
			StopGrapling();
	}

	if (IsWallRunning)
	{
		HandleWallrunning();
	}
	else if (JumpKeyDown && CanWallRun && !IsDodging)
	{
		WallNeighborhood = CheckForWallsNearby(AttachedWallNormal);

		//if player wants to wallrun and there's a wall nearby, that is vertical
		if (WallNeighborhood != WN_None && AttachedWallNormal.Z >= -0.05f && AttachedWallNormal.Z < 0.15f)
		{
			
			if (IsGrapling)
				StopGrapling();
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
				LaunchCharacter(FVector::VectorPlaneProject(FirstPersonCameraComponent->GetForwardVector(), AttachedWallNormal).GetUnsafeNormal2D() * HorizontalWallRunForce + FVector(0.0f, 0.0f, VerticalWallRunForce * 0.25f) - AttachedWallNormal * WallStickForce, true, true);
			}
		}
	}

	for (auto gun : Guns)
	{
		gun->Aim(GetLookedPoint(), DeltaTime);
	}
}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	check(PlayerInputComponent);

	// Bind movement events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("Dodge", IE_Pressed, this, &AMyCharacter::Dodge);
	PlayerInputComponent->BindAction("Grip", IE_Pressed, this, &AMyCharacter::Grip);
	PlayerInputComponent->BindAction("Drop", IE_Pressed, this, &AMyCharacter::Drop);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMyCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMyCharacter::MoveRight);

	// Camera input
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AMyCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AMyCharacter::LookUpAtRate);

	// Bind fire events
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AMyCharacter::OnFirePressed);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &AMyCharacter::OnFireReleased);
	PlayerInputComponent->BindAction("AltFire", IE_Pressed, this, &AMyCharacter::OnAltFirePressed);
	PlayerInputComponent->BindAction("AltFire", IE_Released, this, &AMyCharacter::OnAltFireReleased);

	// Bind weapon slot change events
	PlayerInputComponent->BindAction("WeaponSlot1", IE_Pressed, this, &AMyCharacter::GetSlotOne);
	PlayerInputComponent->BindAction("WeaponSlot2", IE_Pressed, this, &AMyCharacter::GetSlotTwo);
	PlayerInputComponent->BindAction("WeaponSlot3", IE_Pressed, this, &AMyCharacter::GetSlotThree);

}

void AMyCharacter::Dodge()
{
	UCharacterMovementComponent* movement = Cast<UCharacterMovementComponent>(GetMovementComponent());	
	
	if (CanDodge && movement && movement->IsMovingOnGround())
	{
		FVector desiredVelocity = FVector(InputComponent->GetAxisValue(TEXT("MoveForward")), InputComponent->GetAxisValue(TEXT("MoveRight")), 0.0f);		

// if there's input from player, launch him in the desired direction, making sure he doesn't lose too much of the current velocity
if (desiredVelocity.Size() > 0.0f)
{
	desiredVelocity = GetActorRotation().RotateVector(desiredVelocity.GetUnsafeNormal2D());

	if (FVector::DotProduct(desiredVelocity, GetActorForwardVector()) > 0.1f)
		return;

	desiredVelocity = desiredVelocity * DodgeForce, true, true;
}
// else don't change his horizontal velocity
else
{
	desiredVelocity = GetActorForwardVector() * -DodgeForce, true, true;
}

movement->GroundFriction = 0.0f;
LaunchCharacter(desiredVelocity, true, true);

CanDodge = false;
IsDodging = true;
GetWorldTimerManager().SetTimer(DodgeTimerHandle, this, &AMyCharacter::StopDodging, DodgeTime, false, DodgeTime);
	}
}

void AMyCharacter::StopDodging()
{
	GetWorldTimerManager().ClearTimer(DodgeTimerHandle);
	UCharacterMovementComponent* movement = Cast<UCharacterMovementComponent>(GetMovementComponent());
	if (movement)
		movement->GroundFriction = OriginalFriction;
	//GetCharacterMovement()->StopMovementImmediately();
	IsDodging = false;
	GetWorldTimerManager().SetTimer(DodgeTimerHandle, this, &AMyCharacter::ResetDodge, DodgeCooldown, false, DodgeCooldown);
}

void AMyCharacter::OnFirePressed()
{
	GetCurrentGun()->OnFirePressed();
}


void AMyCharacter::ResetDodge()
{
	GetWorldTimerManager().ClearTimer(DodgeTimerHandle);
	CanDodge = true;
}

void AMyCharacter::EnableWallrunning()
{
	GetWorldTimerManager().ClearTimer(WallrunCooldownTimerHandle);
	CanWallRun = true;
}

void AMyCharacter::OnFireReleased()
{
	GetCurrentGun()->OnFireReleased();
}

void AMyCharacter::OnAltFirePressed()
{
	GetCurrentGun()->OnAltFirePressed();
}

void AMyCharacter::OnAltFireReleased()
{
	GetCurrentGun()->OnAltFireReleased();
}

void AMyCharacter::Jump()
{
	JumpKeyDown = true;

	if (IsDodging)
		return;
	if (IsWallRunning)
	{
		//walljump
		FVector desiredVelocity = GetActorRotation().RotateVector(FVector(InputComponent->GetAxisValue(TEXT("MoveForward")), InputComponent->GetAxisValue(TEXT("MoveRight")), 0.0f));

		if (desiredVelocity.Size() == 0.0f)
			desiredVelocity = GetActorForwardVector().GetUnsafeNormal2D();
		else
			desiredVelocity = desiredVelocity.GetUnsafeNormal2D();

		FVector outputVelocity = AttachedWallNormal * WallJumpForce;

		if (WallNeighborhood == WN_Right)
		{
			if(FVector::DotProduct(desiredVelocity, AttachedWallNormal.RotateAngleAxis(90.0f, FVector::UpVector)) < 0.0f)
			{
				outputVelocity = AttachedWallNormal * WallJumpForce;
			}
			else if (FVector::DotProduct(desiredVelocity, AttachedWallNormal.RotateAngleAxis(-30.0f, FVector::UpVector)) < 0.0f)
			{
				outputVelocity = (AttachedWallNormal * WallJumpForce).RotateAngleAxis(60.0f, FVector::UpVector);
			}
			else
				outputVelocity = desiredVelocity * WallJumpForce;
		}
		else if(WallNeighborhood == WN_Left)
		{
			if (FVector::DotProduct(desiredVelocity, AttachedWallNormal.RotateAngleAxis(-90.0f, FVector::UpVector)) < 0.0f)
			{
				outputVelocity = AttachedWallNormal * WallJumpForce;
			}
			else if (FVector::DotProduct(desiredVelocity, AttachedWallNormal.RotateAngleAxis(30.0f, FVector::UpVector)) < 0.0f)
			{
				outputVelocity = (AttachedWallNormal * WallJumpForce).RotateAngleAxis(-60.0f, FVector::UpVector);
			}
			else
				outputVelocity = desiredVelocity * WallJumpForce;
		}
		else
		{
			if (FVector::DotProduct(desiredVelocity, AttachedWallNormal) < 0.0f)
			{
				outputVelocity = AttachedWallNormal * WallJumpForce;
			}
			else
			{
				if (FVector::DotProduct(desiredVelocity, AttachedWallNormal.RotateAngleAxis(45.0f, FVector::UpVector)) < 0.0f)
					outputVelocity = (AttachedWallNormal * WallJumpForce).RotateAngleAxis(-45.0f, FVector::UpVector);

				else if (FVector::DotProduct(desiredVelocity, AttachedWallNormal.RotateAngleAxis(-45.0f, FVector::UpVector)) < 0.0f)
					outputVelocity = (AttachedWallNormal * WallJumpForce).RotateAngleAxis(45.0f, FVector::UpVector);
				else
					outputVelocity = desiredVelocity * WallJumpForce;
			}
		}

		outputVelocity.Z = GetCharacterMovement()->JumpZVelocity;
		LaunchCharacter(outputVelocity, true, true);
		StopWallrun();
		JumpCount = 1;
		GetWorldTimerManager().SetTimer(WallrunCooldownTimerHandle, this, &AMyCharacter::EnableWallrunning, WallrunCooldown, false, WallrunCooldown);
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

void AMyCharacter::GetSlotOne()
{
	SwitchWeapon(0);
}

void AMyCharacter::GetSlotTwo()
{
	SwitchWeapon(1);
}

void AMyCharacter::GetSlotThree()
{
	SwitchWeapon(2);
}

void AMyCharacter::SwitchWeapon(int id)
{
	if (Guns.Num() > 0)
	{
		CurrentGunID = id;
		for (int i = 0; i < Guns.Num(); i++)
		{
			Guns[i]->SetActive(i == CurrentGunID ? true : false);
		}
	}
}

void AMyCharacter::Grip()
{
	FHitResult outHit;
	bool isHit;

	TArray<AActor*> actorsToIgnore;
	actorsToIgnore.Add(this);

	isHit = UKismetSystemLibrary::SphereTraceSingle(this, FirstPersonCameraComponent->GetComponentLocation(), FirstPersonCameraComponent->GetComponentLocation() + FirstPersonCameraComponent->GetForwardVector() * GraplingRange, 0.5f, UEngineTypes::ConvertToTraceType(ECC_Visibility), false, actorsToIgnore, EDrawDebugTrace::Persistent, outHit, true, FLinearColor::Red, FLinearColor::Green, 0.0f);
	
	if (isHit)
	{
		AActor* other = outHit.Actor.Get();
		
		TArray<FName> tags = outHit.Actor->Tags;
		
		if (outHit.Actor->ActorHasTag("Slingshot") && !IsDodging)
		{
			if (IsWallRunning)
				StopWallrun();
			GraplingMovementNormal = (outHit.Actor->GetActorLocation() - GetActorLocation()).GetUnsafeNormal();
			LaunchCharacter(GraplingMovementNormal * GraplingSpeed, true, true);
			GetCharacterMovement()->GravityScale = 0.0f;
			IsGrapling = true;
			CanWallRun = true;
		}
	}
}

void AMyCharacter::Drop()
{
	if (IsWallRunning)
	{
		StopWallrun();
		GetMovementComponent()->StopMovementImmediately();
	}

	if (IsGrapling)
	{
		StopGrapling();
	}
}

void AMyCharacter::Destroyed()
{
	Super::Destroyed();
	GetWorldTimerManager().ClearTimer(DodgeTimerHandle);
}

void AMyCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	GetWorldTimerManager().ClearTimer(DodgeTimerHandle);
	Super::EndPlay(EndPlayReason);
}

void AMyCharacter::OnOverlapBegin(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor->ActorHasTag("Slingshot") && IsGrapling)
	{
		StopGrapling();
		LaunchCharacter(OtherActor->GetActorForwardVector() * GraplingSpeed, true, true);
	}

	if (OtherActor->ActorHasTag("Checkpoint"))
        LastCheckpointLocation = OtherActor->GetActorLocation();

}

void AMyCharacter::Die()
{
	GetMovementComponent()->StopMovementImmediately();
	TeleportTo(LastCheckpointLocation, GetActorRotation());
}

void AMyCharacter::Landed(const FHitResult & Hit)
{
	Super::Landed(Hit);
	JumpCount = 0;
	CanWallRun = true;
	IsWallRunning = false;
	GetCharacterMovement()->GravityScale = 1.0f;
}

EWallNeighborhood AMyCharacter::CheckForWallsNearby(FVector &wallNormal)
{
	FHitResult outHit;
	bool isHit = false;

	TArray<AActor*> actorsToIgnore;
	actorsToIgnore.Add(this);

	FVector startLocation = GetActorLocation();

	FCollisionQueryParams collisionParams;	

	// front
	isHit = UKismetSystemLibrary::BoxTraceSingle(this, startLocation, startLocation + GetActorForwardVector() * LineTraceRange, FVector(5.0f, 10.0f, 5.0f), GetActorRotation(), UEngineTypes::ConvertToTraceType(ECC_Visibility), false, actorsToIgnore, EDrawDebugTrace::ForOneFrame, outHit, true, FLinearColor::Red, FLinearColor::Green, 0.0f);

	if (isHit)
	{
		wallNormal = outHit.ImpactNormal.GetUnsafeNormal2D();
		return WN_Front;
	}

	//right side
	isHit = UKismetSystemLibrary::BoxTraceSingle(this, startLocation, startLocation + GetActorRightVector() * LineTraceRange, FVector(20.0f, 5.0f, 5.0f), GetActorRotation(), UEngineTypes::ConvertToTraceType(ECC_Visibility), false, actorsToIgnore, EDrawDebugTrace::ForOneFrame, outHit, true, FLinearColor::Red, FLinearColor::Green, 0.0f);
	if (isHit)
	{
		wallNormal = outHit.ImpactNormal.GetUnsafeNormal2D();
		return WN_Right;
	}

	//left side
	isHit = UKismetSystemLibrary::BoxTraceSingle(this, startLocation, startLocation - GetActorRightVector() * LineTraceRange, FVector(20.0f, 5.0f, 5.0f), GetActorRotation(), UEngineTypes::ConvertToTraceType(ECC_Visibility), false, actorsToIgnore, EDrawDebugTrace::ForOneFrame, outHit, true, FLinearColor::Red, FLinearColor::Green, 0.0f);
	if (isHit)
	{
		wallNormal = outHit.ImpactNormal.GetUnsafeNormal2D();
		return WN_Left;
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

void AMyCharacter::StopGrapling()
{
	IsGrapling = false;
	GetCharacterMovement()->GravityScale = 1.0f;
	LaunchCharacter(GetVelocity().GetUnsafeNormal() * GraplingSpeed, true, true);
}

void AMyCharacter::HandleWallrunning()
{
	//check if the wallrunning needs to be stopped by the gravity
	if ((WallNeighborhood == WN_Front && GetVelocity().Z < MinVerticalZSpeed) || ((WallNeighborhood == WN_Left || WallNeighborhood == WN_Right) && GetVelocity().Z < MinHorizontalZSpeed))
	{
		StopWallrun();
	}
	//check if there's still a wall to run on
	else
	{
		FHitResult outHit;
		bool isHit = false;		

		TArray<AActor*> actorsToIgnore;
		actorsToIgnore.Add(this);

		FVector startLocation = GetActorLocation();

		isHit = UKismetSystemLibrary::BoxTraceSingle(this, startLocation, startLocation - AttachedWallNormal * LineTraceRange, FVector(20.0f, 5.0f, 5.0f), GetActorRotation(), UEngineTypes::ConvertToTraceType(ECC_Visibility), false, actorsToIgnore, EDrawDebugTrace::ForOneFrame, outHit, true, FLinearColor::Red, FLinearColor::Green, 0.0f);
		if (isHit)
		{
			AttachedWallNormal = outHit.ImpactNormal.GetUnsafeNormal2D();
		}
		else
		{
			if (WallNeighborhood == WN_Front)
				LaunchCharacter(FVector(0.0f, 0.0f, VerticalWallRunForce) - AttachedWallNormal * WallStickForce * 2, true, true);
         				StopWallrun();		
		}		
	}
}

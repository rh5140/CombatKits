// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CombatCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ACombatCharacter::ACombatCharacter():
	DefaultTurnRate(45.f),
	DefaultLookUpRate(45.f),
	WalkSpeed(300.f),
	RunSpeed(600.f)
{
 	// Create spring arm
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->TargetArmLength = 300.f;
	SpringArmComponent->bUsePawnControlRotation = true;

	// Create camera
	FollowCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera Component"));
	FollowCameraComponent->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName);
	FollowCameraComponent->bUsePawnControlRotation = false;

	// Set up pitch, yaw, roll
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	// Configure character movement
	// Only rotate yaw
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f);

	// Jumping
	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = 0.1f;

	RightWeaponCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Right Weapon Box"));
	RightWeaponCollision->SetupAttachment(GetMesh(), FName("RightWeaponBone"));

}

// Called when the game starts or when spawned
void ACombatCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Bind function to overlap event for weapon box
	RightWeaponCollision->OnComponentBeginOverlap.AddDynamic(this, &ACombatCharacter::OnRightWeaponOverlap);

	// Set up right weapon collision box
	RightWeaponCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RightWeaponCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	RightWeaponCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	RightWeaponCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
}

// Move forward and backward
void ACombatCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.f))
	{
		const FRotator Rotation{ Controller->GetControlRotation() };

		const FRotator YawRotation{ 0, Rotation.Yaw, 0 };

		const FVector Direction{ FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X) };

		AddMovementInput(Direction, Value);
	}

}

// Move right and left
void ACombatCharacter::MoveRight(float Value)
{
	if ((Controller != nullptr) && (Value != 0.f))
	{
		const FRotator Rotation{ Controller->GetControlRotation() };

		const FRotator YawRotation{ 0, Rotation.Yaw, 0 };

		const FVector Direction{ FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y) };

		AddMovementInput(Direction, Value);
	}

}

void ACombatCharacter::TurnRate(float Rate)
{
	// Turning degrees per delta seconds
	AddControllerYawInput(Rate * DefaultTurnRate * GetWorld()->GetDeltaSeconds());

}

void ACombatCharacter::LookUpRate(float Rate)
{
	// Lookup rate per delta seconds
	AddControllerPitchInput(Rate * DefaultLookUpRate * GetWorld()->GetDeltaSeconds());

}

void ACombatCharacter::Running()
{
	if (GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.0f)
	{
		GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
	}

}

void ACombatCharacter::StopRunning()
{
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

}

void ACombatCharacter::Recall()
{
	PlayAnimMontage(RecallMontage, "Recall");
}

void ACombatCharacter::PlayAnimMontage(UAnimMontage* MontageToPlay, FName SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && MontageToPlay)
	{
		if (!AnimInstance->Montage_IsPlaying(MontageToPlay))
		{
			GetCharacterMovement()->DisableMovement();

			int32 const SectionIndex = MontageToPlay->GetSectionIndex(SectionName);
			int32 const SectionLength = MontageToPlay->GetSectionLength(SectionIndex);

			AnimInstance->Montage_Play(MontageToPlay);
			AnimInstance->Montage_JumpToSection(SectionName);

			GetWorldTimerManager().SetTimer(TimerMovementWalking, this, &ACombatCharacter::EnableWalk, SectionLength);
		}
	}
}

void ACombatCharacter::EnableWalk()
{
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);
}

void ACombatCharacter::MainAttack()
{
	PlayAnimMontage(MainAttackMontage, "MainAttack");
}

void ACombatCharacter::OnRightWeaponOverlap(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Apply Damage"));
}

void ACombatCharacter::ActivateRightWeapon()
{
	RightWeaponCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Activate weapon"));
}

void ACombatCharacter::DeactivateRightWeapon()
{
	RightWeaponCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Deactivate weapon"));
}

// Called to bind functionality to input
void ACombatCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Make sure component is valid
	check(PlayerInputComponent);

	// Player movement
	PlayerInputComponent->BindAxis("MoveForward", this, &ACombatCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACombatCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("Running", IE_Pressed, this, &ACombatCharacter::Running);
	PlayerInputComponent->BindAction("Running", IE_Released, this, &ACombatCharacter::StopRunning);

	// Combat abilities
	PlayerInputComponent->BindAction("Recall", IE_Pressed, this, &ACombatCharacter::Recall);
	PlayerInputComponent->BindAction("MainAttack", IE_Pressed, this, &ACombatCharacter::MainAttack);
}


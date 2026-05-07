// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/EnemyMelee.h"
#include "Character/CombatCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/UObjectBaseUtility.h"

AEnemyMelee::AEnemyMelee() :
	RightWeaponDamage(10.f),
	LeftWeaponDamage(10.f)
{
	RightWeaponCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Right Weapon Box"));
	RightWeaponCollision->SetupAttachment(GetMesh(), FName("RightWeaponSocket"));

	LeftWeaponCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Left Weapon Box"));
	LeftWeaponCollision->SetupAttachment(GetMesh(), FName("LeftWeaponSocket"));
}

void AEnemyMelee::BeginPlay()
{
	Super::BeginPlay();

	RightWeaponCollision->OnComponentBeginOverlap.AddDynamic(this, &AEnemyMelee::OnRightWeaponOverlap);
	SetUpCollisionBox(RightWeaponCollision);
	LeftWeaponCollision->OnComponentBeginOverlap.AddDynamic(this, &AEnemyMelee::OnLeftWeaponOverlap);
	SetUpCollisionBox(LeftWeaponCollision);
}

void AEnemyMelee::SetUpCollisionBox(UBoxComponent* WeaponCollision)
{
	WeaponCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	WeaponCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	WeaponCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
}

// Play random enemy attack montage
void AEnemyMelee::MeleeAttack()
{
	// TODO: Cache ref to AnimInstance in BeginPlay()
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance && MeleeAttackMontage)
	{
		int32 const SectionCount = MeleeAttackMontage->CompositeSections.Num();

		FName SectionName = GetAttackSectionName(SectionCount);
		int32 const SectionIndex = MeleeAttackMontage->GetSectionIndex(SectionName);
		float const SectionLength = MeleeAttackMontage->GetSectionLength(SectionIndex);

		// Disable enemy movement then re-enable after timer
		GetCharacterMovement()->DisableMovement();

		AnimInstance->Montage_Play(MeleeAttackMontage);
		AnimInstance->Montage_JumpToSection(SectionName, MeleeAttackMontage);
		GetWorldTimerManager().SetTimer(TimerAttack, this, &AEnemyMelee::ResetMovementWalking, SectionLength);
	}
}

void AEnemyMelee::ResetMovementWalking()
{
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);
}

// Get random attack section name
FName AEnemyMelee::GetAttackSectionName(int32 SectionCount)
{
	FName SectionName;

	// Get random section in montage
	const int32 Section{ FMath::RandRange(1, SectionCount) };

	switch (Section)
	{
		case 1:
			SectionName = FName("Attack 1");
			break;
		case 2:
			SectionName = FName("Attack 2");
			break;
	}

	return SectionName;
}

// TODO: Refactor when building behavior tree
void AEnemyMelee::MeleeMainAttack()
{
	MeleeAttack();
}

void AEnemyMelee::ActivateRightWeapon()
{
	RightWeaponCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AEnemyMelee::DeactivateRightWeapon()
{
	RightWeaponCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AEnemyMelee::ActivateLeftWeapon()
{
	LeftWeaponCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AEnemyMelee::DeactivateLeftWeapon()
{
	LeftWeaponCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AEnemyMelee::OnRightWeaponOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == nullptr) return;
	if (OtherActor->IsA(ACombatCharacter::StaticClass()))
	{
		// Temporarily hardcoded
		float const WeaponDamage = RightWeaponDamage;

		UGameplayStatics::ApplyDamage(
			OtherActor,
			WeaponDamage,
			AIController,
			this,
			UDamageType::StaticClass());
	}
}

void AEnemyMelee::OnLeftWeaponOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == nullptr) return;
	if (OtherActor->IsA(ACombatCharacter::StaticClass()))
	{
		// Temporarily hardcoded
		float const WeaponDamage = LeftWeaponDamage;

		UGameplayStatics::ApplyDamage(
			OtherActor,
			WeaponDamage,
			AIController,
			this,
			UDamageType::StaticClass());
	}
}

float AEnemyMelee::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	// TODO: Cache ref to AnimInstance in BeginPlay()
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && HitReactMontage)
	{
		// Cancel existing attack if it's happening
		if (AnimInstance->Montage_IsPlaying(MeleeAttackMontage)) AnimInstance->Montage_Stop(0.f, MeleeAttackMontage);

		int32 const SectionCount = MeleeAttackMontage->CompositeSections.Num();

		FName SectionName = GetAttackSectionName(SectionCount);
		int32 const SectionIndex = MeleeAttackMontage->GetSectionIndex(SectionName);
		float const SectionLength = MeleeAttackMontage->GetSectionLength(SectionIndex);

		// Disable enemy movement then re-enable after timer
		GetCharacterMovement()->DisableMovement();

		AnimInstance->Montage_Play(MeleeAttackMontage);
		AnimInstance->Montage_JumpToSection(SectionName, MeleeAttackMontage);
		GetWorldTimerManager().SetTimer(TimerAttack, this, &AEnemyMelee::ResetMovementWalking, SectionLength);
	}

	return DamageAmount;
}
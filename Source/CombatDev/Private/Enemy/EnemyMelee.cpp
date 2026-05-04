// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/EnemyMelee.h"
#include "GameFramework/CharacterMovementComponent.h"

AEnemyMelee::AEnemyMelee()
{
	RightWeaponCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Right Weapon Box"));
	RightWeaponCollision->SetupAttachment(GetMesh(), FName("RightWeaponBone"));
}

void AEnemyMelee::BeginPlay()
{
	Super::BeginPlay();

	RightWeaponCollision->OnComponentBeginOverlap.AddDynamic(this, &AEnemyMelee::OnRightWeaponOverlap);

	RightWeaponCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RightWeaponCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	RightWeaponCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	RightWeaponCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
}

// Play random enemy attack montage
void AEnemyMelee::MeleeAttack()
{
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
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Activate weapon"));
}

void AEnemyMelee::DeactivateRightWeapon()
{
	RightWeaponCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Deactivate weapon"));
}

void AEnemyMelee::OnRightWeaponOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IsValid(SweepResult.GetActor()))
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Hit Player"));
	}
}
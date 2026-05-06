// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/Enemy.h"
#include "EnemyMelee.generated.h"

/**
 * 
 */
UCLASS()
class COMBATDEV_API AEnemyMelee : public AEnemy
{
	GENERATED_BODY()

public:
	AEnemyMelee();

	virtual void ActivateRightWeapon();
	virtual void DeactivateRightWeapon();

	virtual void ActivateLeftWeapon();
	virtual void DeactivateLeftWeapon();

protected:
	virtual void BeginPlay() override;

	void MeleeAttack();

	void ResetMovementWalking();

	FName GetAttackSectionName(int32 SectionCount);

	void SetUpCollisionBox(UBoxComponent* WeaponCollision);

	UFUNCTION()
	void OnRightWeaponOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnLeftWeaponOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* MeleeAttackMontage;

	FTimerHandle TimerAttack;

	UFUNCTION(BlueprintCallable)
	void MeleeMainAttack();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* RightWeaponCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* LeftWeaponCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	float RightWeaponDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	float LeftWeaponDamage;
};

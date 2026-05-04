// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/BoxComponent.h"
#include "CombatCharacterInterface.h"
#include "CombatCharacter.generated.h"

// Declarations
class USpringArmComponent;
class UCameraComponent;
class UAnimMontage;

UCLASS()
class COMBATDEV_API ACombatCharacter : public ACharacter, public ICombatCharacterInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACombatCharacter();
	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void ActivateRightWeapon();
	virtual void DeactivateRightWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Move forward and backward
	void MoveForward(float Value);

	// Move left and right
	void MoveRight(float Value);

	// Input to turn at normalized rate given
	void TurnRate(float rate);

	// Input to lookup or down at normalized rate given
	void LookUpRate(float rate);

	// Start and stop running
	void Running();
	void StopRunning();

	// Recall ability
	void Recall();

	// Disable movement and play montage
	void PlayAnimMontage(UAnimMontage* MontageToPlay, FName SectionName = "Default");

	void EnableWalk();

	// LMB main attack
	void MainAttack();

	// Right weapon overlap
	UFUNCTION()
	void OnRightWeaponOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Camera", meta=(AllowPrivateAccess="true"))
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	float DefaultTurnRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	float DefaultLookUpRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float WalkSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float RunSpeed;

	// Timer handle to disable movement during montages
	FTimerHandle TimerMovementWalking;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* RecallMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* MainAttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* RightWeaponCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	float BaseDamage;
};

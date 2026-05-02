// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CombatAnimInstance.generated.h"

// Declarations
class ACombatCharacter;

/**
 * 
 */
UCLASS()
class COMBATDEV_API UCombatAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:

	// Used in animation bp
	UFUNCTION(BlueprintCallable)
	void UpdateAnimationProperties(float DeltaTime);

	// Intialize variables to ouse in BP
	virtual void NativeInitializeAnimation() override;

private:
	// Reference to player character
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"));
	ACombatCharacter* CombatCharacter;

	// Movement speed
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"));
	float Speed;

	// T/F if player is in air
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"));
	bool bIsInAir;

	// Movement offset Yaw
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"));
	float MovementOffsetYaw;

};

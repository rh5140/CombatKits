// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "Character/CombatCharacter.h"
#include "NotifyStateRightWeapon.generated.h"

/**
 * 
 */
UCLASS()
class COMBATDEV_API UNotifyStateRightWeapon : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	UPROPERTY();
	ACombatCharacter* CombatCharacter;

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	
};

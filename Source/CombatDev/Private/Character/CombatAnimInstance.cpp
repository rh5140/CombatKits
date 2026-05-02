// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CombatAnimInstance.h"
#include "Character/CombatCharacter.h"

// Act like animation tick. Get speed
void UCombatAnimInstance::UpdateAnimationProperties(float DeltaTime)
{
	if (CombatCharacter == nullptr)
	{
		CombatCharacter = Cast<ACombatCharacter>(TryGetPawnOwner());
	}

	if (CombatCharacter)
	{
		// Get speed of player from velocity
		FVector Velocity(CombatCharacter->GetVelocity());
		Velocity.Z = 0;
		Speed = Velocity.Size();
	}

}

// Get reference to player character
void UCombatAnimInstance::NativeInitializeAnimation()
{
	CombatCharacter = Cast<ACombatCharacter>(TryGetPawnOwner());
}
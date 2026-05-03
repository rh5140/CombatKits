// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Enemy.h"

// Sets default values
AEnemy::AEnemy() :
	Health(100.f),
	MaxHealth(100.f)
{

}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::MeleeHit_Implementation(FHitResult HitResult)
{
	// Impact sfx
	
	// VFX

	// Hit montage
}

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (Health - DamageAmount < -0.f)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Enemy Dies"));

	}
	else
	{
		Health -= DamageAmount;
	}

	return DamageAmount;
}


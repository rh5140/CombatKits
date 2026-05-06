// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Enemy.h"
#include "Enemy/EnemyAIController.h"

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
	
	AIController = Cast<AEnemyAIController>(GetController());
}

void AEnemy::MainAttack()
{
	

}

void AEnemy::MeleeHit_Implementation(FHitResult HitResult)
{
	// Impact sfx
	
	// VFX

	// Hit montage
}

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Health -= DamageAmount;
	if (Health - DamageAmount <= 0.f)
	{
		GetMesh()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		DeathOfEnemy(); // Implemented in BP
	}
	else
	{
	}

	return DamageAmount;
}


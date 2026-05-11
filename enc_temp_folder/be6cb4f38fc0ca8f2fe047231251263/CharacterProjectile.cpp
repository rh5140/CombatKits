// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CharacterProjectile.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ACharacterProjectile::ACharacterProjectile() :
	BaseDamage(20.f)
{
	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));
}

// Called when the game starts or when spawned
void ACharacterProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACharacterProjectile::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
}



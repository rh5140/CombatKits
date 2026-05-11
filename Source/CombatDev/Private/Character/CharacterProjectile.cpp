// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CharacterProjectile.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ACharacterProjectile::ACharacterProjectile() :
	BaseDamage(20.f)
{
	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));
	ProjectileComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement Component"));
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	ProjectileMesh->SetupAttachment(CollisionComponent);
	CollisionComponent->InitSphereRadius(5.0f);
	CollisionComponent->OnComponentHit.AddDynamic(this, &ACharacterProjectile::OnHit);
	RootComponent = CollisionComponent;
	ProjectileComponent->UpdatedComponent = CollisionComponent;
}

// Called when the game starts or when spawned
void ACharacterProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Projectile code from https://dev.epicgames.com/documentation/unreal-engine/coder-08-implement-a-projectile-in-unreal-engine?lang=en-US
void ACharacterProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Only add impulse and destroy projectile if we hit a physics object
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) && OtherComp->IsSimulatingPhysics())
	{
		OtherComp->AddImpulseAtLocation(GetVelocity() * PhysicsForce, GetActorLocation());

		Destroy();
	}
}



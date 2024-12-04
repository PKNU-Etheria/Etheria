// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/EProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"

// Sets default values
AEProjectile::AEProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	CollisionComponent->InitSphereRadius(Radius);
	RootComponent = CollisionComponent;

	// ProjectileMovementComponent
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);
	ProjectileMovementComponent->InitialSpeed = 0.0f;
	ProjectileMovementComponent->MaxSpeed = 3000.0f;
	ProjectileMovementComponent->bShouldBounce = false;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->ProjectileGravityScale = 0.0f;

	// projectile life time 4 second
	InitialLifeSpan = 5.0f;

	// collision
	CollisionComponent->BodyInstance.SetCollisionProfileName(TEXT("Collision"));

	Damage = 0.0f;
}

// Called when the game starts or when spawned
void AEProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEProjectile::FireInDirection(const FVector& ShootDirection, float LaunchSpeed, float GravityScale)
{
	ProjectileMovementComponent->MaxSpeed = MaxSpeed;
	ProjectileMovementComponent->InitialSpeed = LaunchSpeed;
	ProjectileMovementComponent->ProjectileGravityScale = GravityScale;
	ProjectileMovementComponent->Velocity = ShootDirection * ProjectileMovementComponent->InitialSpeed;
}


// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EProjectile.generated.h"

UCLASS()
class ETHERIA_API AEProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION(BlueprintCallable, Category = Projectile)
	void FireInDirection(const FVector& ShootDirection, float LaunchSpeed, float GravityScale = 1.0f);

	UFUNCTION(BlueprintCallable, Category = Projectile)
	float GetDamage() { return Damage; }

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Projectile)
	TObjectPtr<class USphereComponent> CollisionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Movement)
	TObjectPtr<class UProjectileMovementComponent> ProjectileMovementComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Collision)
	float Radius = 20.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage)
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage)
	float MaxSpeed;
};

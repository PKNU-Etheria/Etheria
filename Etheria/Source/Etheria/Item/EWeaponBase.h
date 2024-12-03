// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EWeaponBase.generated.h"

UCLASS()
class ETHERIA_API AEWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEWeaponBase();

	UPROPERTY(VisibleAnywhere, Category = Weapon)
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere, Category = Weapon)
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(VisibleAnywhere, Category = Weapon)
	USkeletalMeshComponent* SkeletalMesh;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

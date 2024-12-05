// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/EWeaponBase.h"
#include "Components/SceneComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AEWeaponBase::AEWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    Root = CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneComponent"));
    RootComponent = Root;

    SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
    SkeletalMesh->SetupAttachment(RootComponent);

    StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
    StaticMesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AEWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


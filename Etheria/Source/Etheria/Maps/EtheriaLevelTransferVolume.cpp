// Fill out your copyright notice in the Description page of Project Settings.


#include "EtheriaLevelTransferVolume.h"

#include "Components/BoxComponent.h"
#include "Character/Player/EPlayer.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AEtheriaLevelTransferVolume::AEtheriaLevelTransferVolume()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TransferVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("TransferVolume"));
	RootComponent = TransferVolume;
	TransferVolume->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));
}

// Called when the game starts or when spawned
void AEtheriaLevelTransferVolume::BeginPlay()
{
	Super::BeginPlay();
}

void AEtheriaLevelTransferVolume::NotifyActorBeginOverlap(AActor* OtherActor)
{
	if(AEPlayer* Character = Cast<AEPlayer>(OtherActor))
	{
		UGameplayStatics::OpenLevel(GetWorld(), FName(TransferLevelName));
	}
}

// Called every frame
void AEtheriaLevelTransferVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


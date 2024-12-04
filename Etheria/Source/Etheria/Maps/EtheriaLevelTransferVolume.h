// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EtheriaLevelTransferVolume.generated.h"

UCLASS()
class ETHERIA_API AEtheriaLevelTransferVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEtheriaLevelTransferVolume();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess=true))
	FString TransferLevelName;
	
	UPROPERTY()
	TObjectPtr<class UBoxComponent> TransferVolume;
};

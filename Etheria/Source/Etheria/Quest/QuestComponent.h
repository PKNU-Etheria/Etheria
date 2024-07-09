// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EtheriaStructs.h"
#include "QuestComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ETHERIA_API UQuestComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UQuestComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	void Interact();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Interact_Range = 500;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Interact_Radius = 50;


	TMap<int, FQuestStruct*> UnavailableQuests;
	TMap<int, FQuestStruct*> AvailableQuests;
	TMap<int, FQuestStruct*> ProgressingQuests;
	TMap<int, FQuestStruct*> ClearableQuests;
	TMap<int, FQuestStruct*> ClearedQuests;
};

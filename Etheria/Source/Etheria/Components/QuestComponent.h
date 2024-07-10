// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EtheriaStructs.h"
#include "EtheriaEnums.h"
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

	void TryAcceptQuest(int QuestID);
	void TryClearQuest(int QuestID);

	void StartDialogue(int QuestID);
	void ShowNextDialgoue();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Interact_Range = 500;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Interact_Radius = 50;

private:
	EQuestInteractStatus InteractingStatus = EQuestInteractStatus::EQIS_None;

	TArray<FDialogueStruct*>& CurrentDialgoues;
	int CurrentScriptIdx = 0;

	UPROPERTY()
	class UQuestSubSystem* QuestSubSystem;
};

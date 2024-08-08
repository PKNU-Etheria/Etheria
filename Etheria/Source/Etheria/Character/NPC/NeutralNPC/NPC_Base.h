// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "NPCInterface.h"
#include "EtheriaStructs.h"
#include "Etheria/Character/ECharacter.h"
#include "NPC_Base.generated.h"

UCLASS()
class ETHERIA_API ANPC_Base : public AECharacter, public INPCInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ANPC_Base();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	// Interact
public:
	virtual void Interact_With_Implementation(class UQuestComponent* QuestComponent) override;


	// CallBack
public:
	UFUNCTION()
	void QuestAvailable_Callback(int NPCID, int QuestID);

	UFUNCTION()
	void QuestClearable_Callback(int NPCID, int QuestID);

	UFUNCTION()
	void AcceptQuest_Callback(int InNPCID, int InQuestID);

	UFUNCTION()
	void ClearQuest_Callback(int InNPCID, int InQuestID);

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int NPCID = -1;

	TMap<int, FQuestStruct*> UnavailableQuests;
	TMap<int, FQuestStruct*> AvailableQuests;
	TMap<int, FQuestStruct*> ProgressingQuests;
	TMap<int, FQuestStruct*> ClearableQuests;
	TMap<int, FQuestStruct*> ClearedQuests;

private:
	class UQuestSubSystem* QuestSubSystem;
};

// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/NPC/NeutralNPC/NPC_Base.h"
#include "Quest/QuestSubSystem.h"

// Sets default values
ANPC_Base::ANPC_Base()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ANPC_Base::BeginPlay()
{
	Super::BeginPlay();
	
	UGameInstance* GameInst = GetGameInstance();

	if (!GameInst) return;

	QuestSubSystem = GameInst->GetSubsystem<UQuestSubSystem>();
	if (!QuestSubSystem)
	{
		UE_LOG(LogTemp, Warning, TEXT("ANPC_Base : Failed to get Quest Sub System."));
		return;
	}

	QuestSubSystem->Available_Delegate.AddUObject(this, &ANPC_Base::QuestAvailable_Callback);
	QuestSubSystem->Clearable_Delegate.AddUObject(this, &ANPC_Base::QuestClearable_Callback);

	QuestSubSystem->InitializeNPC(this);
}

// Called every frame
void ANPC_Base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ANPC_Base::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ANPC_Base::Interact_With_Implementation(UQuestComponent* QuestComponent)
{
	UE_LOG(LogTemp, Display, TEXT("ANPC_Base : Interact_With_Implementation"));

	if (!QuestSubSystem || !QuestComponent) return;

	// Clear Quest
	for (const auto& Quest : ClearableQuests)
	{
		UE_LOG(LogTemp, Display, TEXT("ANPC_Base : Clear Quest < %s >"), *Quest.Value->QuestName.ToString());

		int QuestID = Quest.Value->QuestID;

		// Clearable -> Cleared
		ClearedQuests.Add(TTuple<int, FQuestStruct*>(QuestID, Quest.Value));
		ClearableQuests.Remove(QuestID);

		// Clearing Quest
		QuestSubSystem->ClearQuest(QuestID);

		// Show Quest Clear Widget

		return;
	}


	// Accept Quest
	for (const auto& Quest : AvailableQuests)
	{
		UE_LOG(LogTemp, Display, TEXT("ANPC_Base : Accept Quest < %s >"), *Quest.Value->QuestName.ToString());

		int QuestID = Quest.Value->QuestID;

		// Available -> Progressing
		ProgressingQuests.Add(TTuple<int, FQuestStruct*>(QuestID, Quest.Value));
		AvailableQuests.Remove(QuestID);

		// Accepting Quest
		QuestSubSystem->AcceptQuest(QuestID);


		// Show Quest Accept Widget

		return;
	}

	// Normal Interact


}

void ANPC_Base::AcceptQuest()
{
}

void ANPC_Base::ClearQuest()
{
}

void ANPC_Base::QuestAvailable_Callback(int InNPCID, int InQuestID)
{
	if (NPCID != InNPCID) 
		return;

	UE_LOG(LogTemp, Display, TEXT("ANPC_Base : QuestAvailable_Callback"));

	FQuestStruct** QuestRefPtr = UnavailableQuests.Find(InQuestID);
	if (!QuestRefPtr) return;

	FQuestStruct* Quest = *QuestRefPtr;
	if (!Quest) return;

	UnavailableQuests.Remove(InQuestID);
	AvailableQuests.Add(TTuple<int, FQuestStruct*>(InQuestID, Quest));

	// if(AvailableQuests.Num() > 0)
}

void ANPC_Base::QuestClearable_Callback(int InNPCID, int InQuestID)
{
	if (NPCID != InNPCID)
		return;

	UE_LOG(LogTemp, Display, TEXT("ANPC_Base : QuestClearable_Callback"));

	FQuestStruct** QuestRefPtr = ProgressingQuests.Find(InQuestID);
	if (!QuestRefPtr) return;

	FQuestStruct* Quest = *QuestRefPtr;
	if (!Quest) return;

	ProgressingQuests.Remove(InQuestID);
	ClearableQuests.Add(TTuple<int, FQuestStruct*>(InQuestID, Quest));

	// if(ClearableQuests.Num() > 0)
}


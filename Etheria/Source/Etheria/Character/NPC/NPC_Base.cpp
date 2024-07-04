// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/NPC/NPC_Base.h"
#include "Quest/QuestSubSystem.h"

// Sets default values
ANPC_Base::ANPC_Base()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	UGameInstance* GameInst = GetGameInstance();
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

// Called when the game starts or when spawned
void ANPC_Base::BeginPlay()
{
	Super::BeginPlay();
	
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

void ANPC_Base::QuestAvailable_Callback(int QuestID)
{
	FQuestStruct* Quest = *UnavailableQuests.Find(QuestID);
	if (!Quest) return;

	UnavailableQuests.Remove(QuestID);
	AvailableQuests.Add(TTuple<int, FQuestStruct*>(QuestID, Quest));

	// if(AvailableQuests.Num() > 0)
}

void ANPC_Base::QuestClearable_Callback(int QuestID)
{
	FQuestStruct* Quest = *ProgressingQuests.Find(QuestID);
	if (!Quest) return;

	ProgressingQuests.Remove(QuestID);
	ClearableQuests.Add(TTuple<int, FQuestStruct*>(QuestID, Quest));

	// if(ClearableQuests.Num() > 0)
}


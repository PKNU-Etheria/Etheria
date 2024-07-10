// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/QuestComponent.h"
#include "Quest/QuestSubSystem.h"
#include "Character/NPC/NeutralNPC/NPCInterface.h"
#include "Character/NPC/NeutralNPC/NPC_Base.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values for this component's properties
UQuestComponent::UQuestComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UQuestComponent::BeginPlay()
{
	Super::BeginPlay();
	
	UGameInstance* GameInst = UGameplayStatics::GetGameInstance(this);
	if (!GameInst) return;

	QuestSubSystem = GameInst->GetSubsystem<UQuestSubSystem>();
	if (!QuestSubSystem)
	{
		UE_LOG(LogTemp, Warning, TEXT("UQuestComponent : Failed to get Quest Sub System."));
		return;
	}

}


// Called every frame
void UQuestComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UQuestComponent::Interact()
{
	if (InteractingStatus != EQuestInteractStatus::EQIS_None)
		ShowNextDialgoue();

	ACharacter* player = UGameplayStatics::GetPlayerCharacter(this, 0);
	if (!player) return;

	AController* controller = player->GetController();
	if (!controller) return;

	TArray<AActor*> IgnoreActors;	FHitResult hitResult;
	IgnoreActors.Add(player);

	FVector start = player->GetActorLocation();
	FRotator ControllerRot = controller->GetControlRotation();
	FVector ControllerForwardVec = UKismetMathLibrary::GetForwardVector(ControllerRot);
	FVector end = start + Interact_Range * ControllerForwardVec;

	ETraceTypeQuery TraceType = UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Pawn);

	UKismetSystemLibrary::SphereTraceSingle(player, start, end, Interact_Radius,
		TraceType, false, IgnoreActors,
		EDrawDebugTrace::ForDuration, hitResult, true);

	INPCInterface* hitActor = Cast<INPCInterface>(hitResult.GetActor());
	ANPC_Base* NPC = Cast<ANPC_Base>(hitActor);
	if (IsValid(NPC))
	{
		NPC->Execute_Interact_With(NPC, this);
		return;
	}
}

void UQuestComponent::TryAcceptQuest(int QuestID)
{
	InteractingStatus = EQuestInteractStatus::EQIS_Accepting;
	StartDialogue(QuestID);
}

void UQuestComponent::TryClearQuest(int QuestID)
{
	InteractingStatus = EQuestInteractStatus::EQIS_Clearing;
	StartDialogue(QuestID);
}

void UQuestComponent::StartDialogue(int QuestID)
{
	if (!QuestSubSystem) return;

	const FQuestDialogueDataStruct* DialogueData = QuestSubSystem->GetQuestDialgoue(QuestID);
	if (!DialogueData) return;

	if (InteractingStatus == EQuestInteractStatus::EQIS_Accepting)
	{
		if (!DialogueData->AcceptDialogueDB) return;

		DialogueData->AcceptDialogueDB->GetAllRows<FDialogueStruct>("UQuestComponent::StartDialogue", CurrentDialgoues);
	}
	else if (InteractingStatus == EQuestInteractStatus::EQIS_Clearing)
	{
		if (!DialogueData->ClearDialogueDB) return;

		DialogueData->ClearDialogueDB->GetAllRows<FDialogueStruct>("UQuestComponent::StartDialogue", CurrentDialgoues);
	}

	if (CurrentDialgoues.IsValidIndex(CurrentScriptIdx))
	{
		UE_LOG(LogTemp, Display, TEXT("%s : %s"), 
			*CurrentDialgoues[CurrentScriptIdx]->NPCName.ToString(), *CurrentDialgoues[CurrentScriptIdx]->Script.ToString());
		CurrentScriptIdx++;
	}
}

void UQuestComponent::ShowNextDialgoue()
{
	// Show Next Dialogue
	if (CurrentDialgoues.IsValidIndex(CurrentScriptIdx))
	{
		UE_LOG(LogTemp, Display, TEXT("%s : %s"),
			*CurrentDialgoues[CurrentScriptIdx]->NPCName.ToString(), *CurrentDialgoues[CurrentScriptIdx]->Script.ToString());
		CurrentScriptIdx++;
	}
	// Close Dialogue
	else
	{
		if (InteractingStatus == EQuestInteractStatus::EQIS_Accepting)
		{
			UE_LOG(LogTemp, Display, TEXT("UQuestComponent : Close Dialogue & Accept Quest"));

		}
		else if (InteractingStatus == EQuestInteractStatus::EQIS_Clearing)
		{
			UE_LOG(LogTemp, Display, TEXT("UQuestComponent : Close Dialogue & Clear Quest"));

		}

		CurrentScriptIdx = 0;
	}
}


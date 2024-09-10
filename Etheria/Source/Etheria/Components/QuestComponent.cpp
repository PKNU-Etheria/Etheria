// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/QuestComponent.h"
#include "Quest/QuestSubSystem.h"
#include "Quest/Widget_Dialogue.h"
#include "Quest/Widgets/Widget_Quest.h"
#include "Character/Player/EPlayer.h"
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

	AEPlayer* player = Cast<AEPlayer>(GetOwner());
	if (player)
	{
		player->Delegate_ShowQuest.AddUObject(this, &UQuestComponent::ShowQuest);
	}
}


// Called every frame
void UQuestComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UQuestComponent::Interact(INPCInterface* InNPC)
{
	if (InteractingStatus != EQuestInteractStatus::EQIS_None)
	{
		if (BranchNum > 0)
			return;

		ShowNextDialgoue();
		return;
	}

	ANPC_Base* NPC = Cast<ANPC_Base>(InNPC);
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

	// Set Input Mode
	APlayerController* controller = UGameplayStatics::GetPlayerController(this, 0);
	if(controller)
	{
		FInputModeGameAndUI InputMode;
		controller->SetInputMode(InputMode);
		controller->bShowMouseCursor = true;
	}

	// Get Dialogue Data
	const FQuestDialogueDataStruct* DialogueData = QuestSubSystem->GetQuestDialgoue(QuestID);
	if (!DialogueData) return;

	CurrentDialgoues.Empty();

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

	DialogueQuestID = QuestID;

	if (CurrentDialgoues.IsValidIndex(CurrentScriptIdx))
	{
		ShowDialgoue(*CurrentDialgoues[CurrentScriptIdx]);
		CurrentScriptIdx = CurrentDialgoues[CurrentScriptIdx]->NextScriptID;
	}
}

void UQuestComponent::ShowNextDialgoue()
{
	if (!QuestSubSystem) return;

	// Close Dialogue
	if (CurrentScriptIdx == -1 || !CurrentDialgoues.IsValidIndex(CurrentScriptIdx))
	{
		if (InteractingStatus == EQuestInteractStatus::EQIS_Accepting)
		{
			CloseDialogue();
			QuestSubSystem->AcceptQuest(DialogueQuestID);
		}
		else if (InteractingStatus == EQuestInteractStatus::EQIS_Clearing)
		{
			CloseDialogue();
			QuestSubSystem->ClearQuest(DialogueQuestID);
		}

		DialogueQuestID = -1;
		CurrentScriptIdx = 0;
		InteractingStatus = EQuestInteractStatus::EQIS_None;
	}
	// Show Next Dialogue
	else
	{
		UE_LOG(LogTemp, Display, TEXT("Branch Num : %d"), CurrentDialgoues[CurrentScriptIdx]->Branchs.Num());
		int i = (*CurrentDialgoues[CurrentScriptIdx]).Branchs.Num();
		BranchNum = (*CurrentDialgoues[CurrentScriptIdx]).Branchs.Num();
		BranchNum = i;

		ShowDialgoue(*CurrentDialgoues[CurrentScriptIdx]);
		CurrentScriptIdx = CurrentDialgoues[CurrentScriptIdx]->NextScriptID;
	}
	
}

void UQuestComponent::ShowDialgoue(const FDialogueStruct& DialogueInfo)
{
	if (!DialogueWidgetClass) return;

	if (!DialogueWidget)
	{
		APawn* owningPawn = Cast<APawn>(GetOwner());
		if (!owningPawn) return;
		APlayerController* playerController = Cast<APlayerController>(owningPawn->GetController());
		if (!playerController) return;
		DialogueWidget = CreateWidget<UWidget_Dialogue>(playerController, DialogueWidgetClass);
		DialogueWidget->AddToViewport();
	}

	DialogueWidget->ShowDialogue(DialogueInfo);
}

void UQuestComponent::BranchSelected(const FBranchStruct& BranchInfo)
{
	BranchNum = 0;
	CurrentScriptIdx = BranchInfo.NextScriptID;
	ShowNextDialgoue();
}

void UQuestComponent::CloseDialogue()
{
	// Set Input Mode
	APlayerController* controller = UGameplayStatics::GetPlayerController(this, 0);
	if (controller)
	{
		FInputModeGameAndUI InputMode;
		controller->SetInputMode(InputMode);
		controller->bShowMouseCursor = true;
	}

	if (DialogueWidget)
	{
		DialogueWidget->RemoveFromParent();
		DialogueWidget = nullptr;
	}
}

void UQuestComponent::ShowQuest()
{
	if (!QuestWidgetClass)
		return;

	if (!QuestWidget)
	{
		APawn* owningPawn = Cast<APawn>(GetOwner());
		if (!owningPawn) return;
		APlayerController* playerController = Cast<APlayerController>(owningPawn->GetController());
		if (!playerController) return;
		QuestWidget = CreateWidget<UWidget_Quest>(playerController, QuestWidgetClass);
		QuestWidget->AddToViewport();

		FInputModeGameAndUI InputMode;
		playerController->SetInputMode(InputMode);
		playerController->bShowMouseCursor = true;
	}
	else if (QuestWidget)
	{
		// Toggle Input Mode
		ESlateVisibility newVisibility = 
			QuestWidget->GetVisibility() == ESlateVisibility::Collapsed ?
			ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed;
		QuestWidget->SetVisibility(newVisibility);

		APlayerController* controller = UGameplayStatics::GetPlayerController(this, 0);
		if (!controller) return;

		if (newVisibility == ESlateVisibility::Collapsed)
		{
			FInputModeGameOnly InputMode;
			controller->SetInputMode(InputMode);
			controller->bShowMouseCursor = false;
		}
		else if (newVisibility == ESlateVisibility::SelfHitTestInvisible)
		{
			FInputModeGameAndUI InputMode;
			controller->SetInputMode(InputMode);
			controller->bShowMouseCursor = true;
		}
	}
}


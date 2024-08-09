// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractComponent.h"
#include "Components/QuestComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Character/NPC/NeutralNPC/NPCInterface.h"
#include "Character/NPC/NeutralNPC/NPC_Base.h"
#include "Kismet/KismetMathLibrary.h"
#include "Public/Interfaces/InteractionInterface.h"

// Sets default values for this component's properties
UInteractComponent::UInteractComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	QuestComponent = CreateDefaultSubobject<UQuestComponent>(FName("QuestComponent"));
}


// Called when the game starts
void UInteractComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UInteractComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

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

	INPCInterface* NPC_If = Cast<INPCInterface>(hitResult.GetActor());
	if (NPC_If)
	{
		InteractingNPC = NPC_If;
	}

	IInteractionInterface* Item_If = Cast<IInteractionInterface>(hitResult.GetActor());
	if (Item_If)
	{
		InteractingItem = Item_If;
	}
}

void UInteractComponent::Interact()
{
	if (InteractingNPC)
	{
		QuestComponent->Interact(InteractingNPC);
	}
}


// Fill out your copyright notice in the Description page of Project Settings.


#include "Quest/QuestComponent.h"
#include "Quest/QuestSubSystem.h"
#include "Character/NPC/NeutralNPC/NPCInterface.h"
#include "Character/NPC/NeutralNPC/NPC_Base.h"
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

	// ...
	
}


// Called every frame
void UQuestComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UQuestComponent::Interact()
{
	AController* controller = Cast<AController>(GetOwner());
	if (!controller) return;

	APawn* pawn = controller->GetPawn();
	if (!pawn) return;

	TArray<AActor*> IgnoreActors;	FHitResult hitResult;
	IgnoreActors.Add(pawn);

	FVector start = pawn->GetActorLocation();
	FRotator ControllerRot = controller->GetControlRotation();
	FVector ControllerForwardVec = UKismetMathLibrary::GetForwardVector(ControllerRot);
	FVector end = start + Interact_Range * ControllerForwardVec;

	ETraceTypeQuery TraceType = UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_GameTraceChannel1);

	UKismetSystemLibrary::SphereTraceSingle(pawn, start, end, Interact_Radius,
		TraceType, false, IgnoreActors,
		EDrawDebugTrace::ForOneFrame, hitResult, true);

	INPCInterface* hitActor = Cast<INPCInterface>(hitResult.GetActor());
	ANPC_Base* NPC = Cast<ANPC_Base>(hitActor);
	if (IsValid(NPC))
	{
		NPC->Execute_Interact_With(NPC, this);
		return;
	}
}


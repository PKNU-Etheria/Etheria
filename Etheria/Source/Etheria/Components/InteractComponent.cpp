// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/QuestComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Character/Player/EPlayer.h"
#include "Character/NPC/NeutralNPC/NPCInterface.h"
#include "Character/NPC/NeutralNPC/NPC_Base.h"
#include "Kismet/KismetMathLibrary.h"
#include "Public/Interfaces/InteractionInterface.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

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
	InteractionCheckFrequency = 0.1f;
	InteractionCheckDistance = 225.0f;
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

	//UKismetSystemLibrary::SphereTraceSingle(player, start, end, Interact_Radius,
	//	TraceType, false, IgnoreActors,
	//	EDrawDebugTrace::ForDuration, hitResult, true);

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

void UInteractComponent::SetPlayer(AEPlayer* InPlayer)
{
	Player = InPlayer;
}

void UInteractComponent::Interact()
{
	if (InteractingNPC)
	{
		QuestComponent->Interact(InteractingNPC);
	}

	if (InteractingItem)
	{
		Player->GetWorldTimerManager().ClearTimer(TimerHandle_Interaction);

		if (IsValid(TargetInteractable.GetObject()))
		{
			TargetInteractable->Interact(Player);
		}
	}
}

void UInteractComponent::UpdateInteractionWidget() const
{
	if (IsValid(TargetInteractable.GetObject()))
	{
		Player->GetHUD()->UpdateInteractionWidget(&TargetInteractable->InteractableData);
	}
}

void UInteractComponent::PerformInteractionCheck()
{
	// line trace
	InteractionData.LastInteractionCheckTime = GetWorld()->GetTimeSeconds();

	FVector TraceStart{ FVector::ZeroVector };

	if (!Player->bAiming)
	{
		InteractionCheckDistance = 225.0f;
		TraceStart = Player->GetPawnViewLocation();
	}
	else
	{
		InteractionCheckDistance = 325.0f;
		TraceStart = Player->GetCameraComponent()->GetComponentLocation();
	}

	//FVector TraceStart{ GetPawnViewLocation()};
	FVector TraceEnd{ TraceStart + (Player->GetViewRotation().Vector() * InteractionCheckDistance) };	


	float LookDirection = FVector::DotProduct(Player->GetActorForwardVector(), Player->GetViewRotation().Vector());

	if (LookDirection > 0) 
	{
		//DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, false, 1.0f, 0, 2.0f);

		FCollisionQueryParams QueryParams;	
		QueryParams.AddIgnoredActor(Player);	
		FHitResult TraceHit;	

		if (GetWorld()->LineTraceSingleByChannel(TraceHit, TraceStart, TraceEnd, ECC_Visibility, QueryParams))
		{	
			if (TraceHit.GetActor()->GetClass()->ImplementsInterface(UInteractionInterface::StaticClass()))
			{
				if (TraceHit.GetActor() != InteractionData.CurrentInteractable)
				{
					FoundInteractable(TraceHit.GetActor());
					return;
				}

				if (TraceHit.GetActor() == InteractionData.CurrentInteractable)
				{
					return;
				}
			}
		}
	}

	NoInteractableFound();
}

void UInteractComponent::FoundInteractable(AActor* NewInteractable)
{
	if (IsInteracting())
	{
		EndInteract();
	}

	if (InteractionData.CurrentInteractable)
	{
		TargetInteractable = InteractionData.CurrentInteractable;
		TargetInteractable->EndFocus();
	}

	InteractionData.CurrentInteractable = NewInteractable;
	TargetInteractable = NewInteractable;

	Player->GetHUD()->UpdateInteractionWidget(&TargetInteractable->InteractableData);

	TargetInteractable->BeginFocus();
}

void UInteractComponent::NoInteractableFound()
{
	if (IsInteracting())
	{
		Player->GetWorldTimerManager().ClearTimer(TimerHandle_Interaction);
	}

	if (InteractionData.CurrentInteractable)	
	{	
		if (IsValid(TargetInteractable.GetObject()))
		{
			TargetInteractable->EndFocus();
		}

		Player->GetHUD()->HideInteractionWidget();

		InteractionData.CurrentInteractable = nullptr;
		TargetInteractable = nullptr;
	}
}

void UInteractComponent::BeginInteract()
{
	PerformInteractionCheck();

	if (InteractionData.CurrentInteractable)
	{
		if (IsValid(TargetInteractable.GetObject()))
		{
			TargetInteractable->BeginInteract();

			if (FMath::IsNearlyZero(TargetInteractable->InteractableData.InteractionDuration, 0.1f))
			{
				Interact();
			}
			else
			{
				Player->GetWorldTimerManager().SetTimer(TimerHandle_Interaction, this, &UInteractComponent::Interact, TargetInteractable->InteractableData.InteractionDuration, false);
			}
		}
	}
}

void UInteractComponent::EndInteract()
{
	Player->GetWorldTimerManager().ClearTimer(TimerHandle_Interaction);

	if (IsValid(TargetInteractable.GetObject()))
	{
		TargetInteractable->EndInteract();
	}
}

bool UInteractComponent::IsInteracting() const
{
	return Player->GetWorldTimerManager().IsTimerActive(TimerHandle_Interaction);
}


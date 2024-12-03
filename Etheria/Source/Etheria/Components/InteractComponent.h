// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Public/Interfaces/InteractionInterface.h"
#include "InteractComponent.generated.h"

class AEPlayer;

USTRUCT()
struct FInteractionData	// 상호작용에 대한 데이터
{
	GENERATED_USTRUCT_BODY()

	FInteractionData() : CurrentInteractable(nullptr), LastInteractionCheckTime(0.0f)
	{

	};

	UPROPERTY()
	AActor* CurrentInteractable;	// 상호작용 액터

	UPROPERTY()
	float LastInteractionCheckTime;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ETHERIA_API UInteractComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInteractComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetPlayer(AEPlayer* InPlayer);

	void Interact();

	void UpdateInteractionWidget() const;

	void PerformInteractionCheck();
	void FoundInteractable(AActor* NewInteractable);
	void NoInteractableFound();
	void BeginInteract();
	void EndInteract();

	// if interaction time is zero = false 
	bool IsInteracting() const;
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UQuestComponent* QuestComponent;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Interact_Range = 500;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Interact_Radius = 50;

protected:
	// Interaction
	UPROPERTY(VisibleAnywhere, Category = "Character | Interaction")
	TScriptInterface<IInteractionInterface> TargetInteractable;	// Interaction Actor Data

	float InteractionCheckFrequency;	// Interaction Frequency

	float InteractionCheckDistance;	// Interaction Distance

	FTimerHandle TimerHandle_Interaction;	// Check Frequency

	FInteractionData InteractionData;	// Can interact Actor Info

private:
	class INPCInterface* InteractingNPC;
	class IInteractionInterface* InteractingItem;
	
	AEPlayer* Player;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Etheria/Character/ECharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Enemy_Base.generated.h"

/**
 * 
 */
UCLASS()
class ETHERIA_API AEnemy_Base : public AECharacter
{
	GENERATED_BODY()

public:
	AEnemy_Base();

	FORCEINLINE class UAnimMontage* GetDeadMontage() const { return DeadMontage; }



	// AI
	void SetupStimulus();

protected:
	virtual void SetDead();
	void PlayDeadAnimation();

public:
	// AI
	class UAIPerceptionStimuliSourceComponent* Stimulus;



protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> DeadMontage;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Etheria/Character/ECharacter.h"
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

	// AI
	void SetupStimulus();
	

public:
	// AI
	class UAIPerceptionStimuliSourceComponent* Stimulus;
};

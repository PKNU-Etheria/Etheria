// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy_Base.h"
#include "Perception/AISense_Sight.h"
#include "Components/ArrowComponent.h"
#include "Components/SceneComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

AEnemy_Base::AEnemy_Base()
{
	SetupStimulus();
}

// AI perception component Stimulus(자극) 구조체 생성
// TODO : Enemy, NeutralNPC 공통 부모 생성해서 거기 붙이기
void AEnemy_Base::SetupStimulus()
{
	Stimulus = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("Stimulus"));

	if (Stimulus) 
	{
		Stimulus->RegisterForSense(TSubclassOf<UAISense_Sight>());
		Stimulus->RegisterWithPerceptionSystem();
	}
}
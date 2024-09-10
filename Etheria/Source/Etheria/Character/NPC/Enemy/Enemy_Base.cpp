// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy_Base.h"
#include "Perception/AISense_Sight.h"
#include "GameFramework/CharacterMovementComponent.h"

AEnemy_Base::AEnemy_Base()
{
	SetupStimulus();
}

// AI perception component Stimulus(�ڱ�) ����ü ����
// TODO : Enemy, NeutralNPC ���� �θ� �����ؼ� �ű� ���̱�
void AEnemy_Base::SetupStimulus()
{
	Stimulus = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("Stimulus"));
	Stimulus->RegisterForSense(TSubclassOf<UAISense_Sight>());
	Stimulus->RegisterWithPerceptionSystem();
}
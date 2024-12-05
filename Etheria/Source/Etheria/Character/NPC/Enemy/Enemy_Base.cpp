// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy_Base.h"
#include "Perception/AISense_Sight.h"
#include "Components/ArrowComponent.h"
#include "Components/SceneComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Character/ECharacterAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Character/NPC/AIController/EAIController.h"

AEnemy_Base::AEnemy_Base()
{
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
	AttributeSet = CreateDefaultSubobject<UECharacterAttributeSet>(TEXT("AttributeSet"));

	SetupStimulus();
}

void AEnemy_Base::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (!ASC) 
	{
		return;
	}

	ASC->InitAbilityActorInfo(this, this);

	for (const auto& StartAbility : StartAbilities)
	{
		FGameplayAbilitySpec StartSpec(StartAbility);
		ASC->GiveAbility(StartSpec);
	}
	
	AttributeSet->OnOutOfHealth.AddDynamic(this, &AEnemy_Base::OnOutOfHealth);
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

void AEnemy_Base::SetDead()
{
	UE_LOG(LogTemp, Log, TEXT("AEnemy_Base : %s Dead"), *GetName());
	
	

	PlayDeadAnimation();
	SetActorEnableCollision(false);
}

void AEnemy_Base::PlayDeadAnimation()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->StopAllMontages(0.0f);
	AnimInstance->Montage_Play(DeadMontage, 1.0f);

}

void AEnemy_Base::OnOutOfHealth()
{
	SetDead();
}

int AEnemy_Base::BasicAttack_Implementation()
{
	if (AttackActionMontage)
	{
		PlayAnimMontage(AttackActionMontage);
	}

	return 0;
}

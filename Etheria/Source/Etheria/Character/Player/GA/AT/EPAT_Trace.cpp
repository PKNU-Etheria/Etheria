// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Player/GA/AT/EPAT_Trace.h"
#include "Character/Player/GA/TA/EPTA_Trace.h"
#include "AbilitySystemComponent.h"

UEPAT_Trace::UEPAT_Trace()
{
}

UEPAT_Trace* UEPAT_Trace::CreateTask(UGameplayAbility* OwningAbility, TSubclassOf<class AEPTA_Trace> TargetActorClass)
{
	UEPAT_Trace* NewTask = NewAbilityTask<UEPAT_Trace>(OwningAbility);
	NewTask->TargetActorClass = TargetActorClass;
	return NewTask;
}

void UEPAT_Trace::Activate()
{
	Super::Activate();

	SpawnAndInitializeTargetActor();
	FinializeTargetActor();

	SetWaitingOnAvatar();
}

void UEPAT_Trace::OnDestroy(bool AbilityEnded)
{
	if (SpawnedTargetActor)
	{
		SpawnedTargetActor->Destroy();
	}

	Super::OnDestroy(AbilityEnded);
}

void UEPAT_Trace::SpawnAndInitializeTargetActor()
{
	SpawnedTargetActor = Cast<AEPTA_Trace>(Ability->GetWorld()->SpawnActorDeferred<AGameplayAbilityTargetActor>(TargetActorClass, FTransform::Identity, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn));
	if (SpawnedTargetActor)
	{
		SpawnedTargetActor->SetShowDebug(true);
		SpawnedTargetActor->TargetDataReadyDelegate.AddUObject(this, &UEPAT_Trace::OnTargetDataReadyCallback);
	}
}

void UEPAT_Trace::FinializeTargetActor()
{
	UAbilitySystemComponent* ASC = AbilitySystemComponent.Get();
	if (ASC)
	{
		const FTransform SpawnTransform = ASC->GetAvatarActor()->GetTransform();
		SpawnedTargetActor->FinishSpawning(SpawnTransform);

		ASC->SpawnedTargetActors.Push(SpawnedTargetActor);
		SpawnedTargetActor->StartTargeting(Ability);
		SpawnedTargetActor->ConfirmTargeting();
	}
}

void UEPAT_Trace::OnTargetDataReadyCallback(const FGameplayAbilityTargetDataHandle& DataHandle)
{
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnComplete.Broadcast(DataHandle);
	}

	EndTask();
}

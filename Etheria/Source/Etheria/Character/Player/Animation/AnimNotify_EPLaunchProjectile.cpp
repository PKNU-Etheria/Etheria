// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Player/Animation/AnimNotify_EPLaunchProjectile.h"
#include "AbilitySystemBlueprintLibrary.h"

UAnimNotify_EPLaunchProjectile::UAnimNotify_EPLaunchProjectile()
{
}

FString UAnimNotify_EPLaunchProjectile::GetNotifyName_Implementation() const
{
	return TEXT("EPLaunchProjectile");
}

void UAnimNotify_EPLaunchProjectile::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp)
	{
		AActor* OwnerActor = MeshComp->GetOwner();
		if (OwnerActor)
		{
			FGameplayEventData PayloadData;
			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(OwnerActor, TriggerGameplayTag, PayloadData);
		}
	}
}

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "GameplayTagContainer.h"
#include "AnimNotify_EPLaunchProjectile.generated.h"

/**
 * 
 */
UCLASS()
class ETHERIA_API UAnimNotify_EPLaunchProjectile : public UAnimNotify
{
	GENERATED_BODY()

public:
	UAnimNotify_EPLaunchProjectile();

protected:
	virtual FString GetNotifyName_Implementation() const override;
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

protected:
	UPROPERTY(EditAnywhere)
	FGameplayTag TriggerGameplayTag;
	
};

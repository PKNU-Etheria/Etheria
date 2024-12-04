// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Etheria/Character/ECharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "Character/NPC/AIController/ECombatInterface.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Enemy_Base.generated.h"

/**
 * 
 */
UCLASS()
class ETHERIA_API AEnemy_Base : public AECharacter, public IECombatInterface
{
	GENERATED_BODY()

public:
	AEnemy_Base();

	virtual void PossessedBy(AController* NewController) override;

	FORCEINLINE class UAnimMontage* GetDeadMontage() const { return DeadMontage; }
	FORCEINLINE class UAnimMontage* GetGetHitMontage() const { return GetHitMontage; }
	FORCEINLINE class UAnimMontage* GetAttackActionMontage() const { return AttackActionMontage; }

	int BasicAttack_Implementation() override;

	// AI
	void SetupStimulus();

protected:
	virtual void SetDead() override;
	void PlayDeadAnimation();

	UFUNCTION()
	void OnOutOfHealth();

public:
	// AI
	class UAIPerceptionStimuliSourceComponent* Stimulus;

protected:
	UPROPERTY(EditAnywhere, Category = GAS)
	TArray<TSubclassOf<class UGameplayAbility>> StartAbilities;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Animation, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> DeadMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Animation, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> AttackActionMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Animation, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> GetHitMontage;
};

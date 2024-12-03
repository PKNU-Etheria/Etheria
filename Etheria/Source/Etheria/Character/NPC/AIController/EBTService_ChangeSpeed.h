// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "EBTService_ChangeSpeed.generated.h"

/**
 * 
 */
UCLASS()
class ETHERIA_API UEBTService_ChangeSpeed : public UBTService_BlackboardBase
{
	GENERATED_BODY()

public:
	UEBTService_ChangeSpeed();
	void OnBecomeRelevant(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory) override;

protected:
	FString GetStaticServiceDescription() const override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	float speed = 500.f;
};

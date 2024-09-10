// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EAIController.generated.h"

/**
 * 
 */
class UBehaviorTree;
class UBehaviorTreeComponent;
class UBlackboardComponent;

UCLASS(config=game)
class ETHERIA_API AEAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	AEAIController(FObjectInitializer const& ObjectInitializer);

	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void Tick(float DeltaSeconds) override;
	virtual FRotator GetControlRotation() const override;

	UFUNCTION()
	void OnPawnDetected(const TArray<AActor*>& DetectedPawns);
	UFUNCTION()
	void OnTargetDetected(AActor* Actor, const FAIStimulus Stimulus);

	class UBlackboardComponent* GetBlackboard() const;

protected:
	void SetupPerceptionSystem();


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
	float AISightRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
	float AISightAge;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
	float AILoseSightRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
	float AIFieldOfView;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
	bool bIsPlayerDetected = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
	float DistanceToPlayer;

protected:
	class UAISenseConfig_Sight* SightConfig;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI, meta = (AllowPrivateAccess = true))
	UBlackboardComponent* BlackboardComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI, meta = (AllowPrivateAccess = true))
	UBehaviorTreeComponent* BehaviorTreeComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI, meta = (AllowPrivateAccess = true))
	UBehaviorTree* BehaviorTree;
};

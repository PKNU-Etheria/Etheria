// Fill out your copyright notice in the Description page of Project Settings.


#include "EAIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Etheria/Character/ECharacter.h"
#include "Etheria/Character/NPC/AIController/BlackboardKeys.h"

AEAIController::AEAIController(FObjectInitializer const& ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BehaviourTreeObject(TEXT("/Script/AIModule.BehaviorTree'/Game/Character/NPC/AIContorller/BP_EAIBehaviorTree.BP_EAIBehaviorTree'"));

	if (BehaviourTreeObject.Succeeded())
	{
		UE_LOG(LogTemp, Log, TEXT("EAIController : Behaviour Tree Load Success!"));
		BehaviorTree = BehaviourTreeObject.Object;
	}

	BehaviorTreeComponent = ObjectInitializer.CreateDefaultSubobject<UBehaviorTreeComponent>(this, TEXT("BehaviorComponent"));
	Blackboard = ObjectInitializer.CreateDefaultSubobject<UBlackboardComponent>(this, TEXT("Blackboard"));

	SetupPerceptionSystem();
}

void AEAIController::BeginPlay()
{
	Super::BeginPlay();

	RunBehaviorTree(BehaviorTree);
	BehaviorTreeComponent->StartTree(*BehaviorTree);

	if (GetPerceptionComponent()) 
	{
		UE_LOG(LogTemp, Log, TEXT("EAIController : Perception Component Set!"));
	}
	else 
	{
		UE_LOG(LogTemp, Warning, TEXT("EAIController : No Perception Component!"));
	}
}

void AEAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (Blackboard)
	{
		UE_LOG(LogTemp, Log, TEXT("EAIController : Blackboard Initialize Success!"));
		Blackboard->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
	}
}

void AEAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	//if (bIsPlayerDetected) 
	//{
	//	AECharacter* Player = Cast<AECharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	//	if (Player) 
	//	{
	//		MoveToActor(Player, 5.0f);
	//	}
	//}
}

FRotator AEAIController::GetControlRotation() const
{
	if (!GetPawn())
	{
		return FRotator(0.f, 0.f, 0.f);
	}

	return FRotator(0.f, GetPawn()->GetActorRotation().Yaw, 0.f);
}

void AEAIController::OnPawnDetected(const TArray<AActor*>& DetectedPawns)
{
	//if (DetectedPawns.Num() > 0) 
	//{
	//	UE_LOG(LogTemp, Log, TEXT("EAIController : Player Enter!"));
	//}

	for (size_t i = 0; i < DetectedPawns.Num(); i++)
	{
		DistanceToPlayer = GetPawn()->GetDistanceTo(DetectedPawns[i]);

		//UE_LOG(LogTemp, Log, TEXT("EAIController : Distance %f"), DistanceToPlayer);
	}

	bIsPlayerDetected = true;
}

void AEAIController::OnTargetDetected(AActor* Actor, const FAIStimulus Stimulus)
{
	UE_LOG(LogTemp, Log, TEXT("EAIController : Find Player!!"));

	if (auto const character = Cast<AECharacter>(Actor))
	{
		//UE_LOG(LogTemp, Log, TEXT("EAIController : Find Player!!"));
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, "Find Player!!");
		Blackboard->SetValueAsBool(BlackboardKeys::CanSeePlayer, Stimulus.WasSuccessfullySensed());
	}
}

UBlackboardComponent* AEAIController::GetBlackboard() const
{
	return Blackboard;
}

void AEAIController::SetupPerceptionSystem()
{
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component")));

	SightConfig->SightRadius = AISightRadius;
	SightConfig->LoseSightRadius = AILoseSightRadius;
	SightConfig->PeripheralVisionAngleDegrees = AIFieldOfView;
	SightConfig->SetMaxAge(AISightAge);

	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

	GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation());
	GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AEAIController::OnTargetDetected);
	GetPerceptionComponent()->ConfigureSense(*SightConfig);
}
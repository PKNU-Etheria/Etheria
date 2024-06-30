// Fill out your copyright notice in the Description page of Project Settings.


#include "E_Item.h"
#include "AbilitySystemComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AE_Item::AE_Item()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	// PrimaryActorTick.bCanEverTick = true;

	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
	Trigger = CreateDefaultSubobject<USphereComponent>(TEXT("Trigger"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	RootComponent = Trigger;
	Mesh->SetupAttachment(Trigger);
	// 트리거 프로필 설정
	//Trigger->SetCollisionProfileName(CPROFILE_ABTRIGGER);
	Trigger->InitSphereRadius(200.0f);

	// 메쉬 가져오기
	static ConstructorHelpers::FObjectFinder<UStaticMesh> BoxMeshRef(TEXT("/Script/Engine.StaticMesh'/Game/ArenaBattle/Environment/Props/SM_Env_Breakables_Box1.SM_Env_Breakables_Box1'"));
	if (BoxMeshRef.Object)
	{
		Mesh->SetStaticMesh(BoxMeshRef.Object);
	}

	Mesh->SetRelativeLocation(FVector(0.0f, -3.5f, -30.0f));
	Mesh->SetCollisionProfileName(TEXT("NoCollision"));
}

UAbilitySystemComponent* AE_Item::GetAbilitySystemComponent() const
{
	return nullptr;
}

void AE_Item::NotifyActorBeginOverlap(AActor* Other)
{
	Super::NotifyActorBeginOverlap(Other);

	// 먹었을 때 수행할 로직
	InvokeGameplayCue(Other);
	ApplyEffectToTarget(Other);

	// 캐릭터가 부딫혔을 때 메쉬 숨겨주기
	Mesh->SetHiddenInGame(true);
	SetActorEnableCollision(false);	// 충돌 처리
	SetLifeSpan(2.0f);		// 시한부 선언 2초뒤 자동으로 사라지게하기
}

// Called when the game starts or when spawned
void AE_Item::BeginPlay()
{
	Super::BeginPlay();
	
}

void AE_Item::PostInitializeComponents()
{
}

void AE_Item::ApplyEffectToTarget(AActor* Target)
{
}

void AE_Item::InvokeGameplayCue(AActor* Target)
{
}

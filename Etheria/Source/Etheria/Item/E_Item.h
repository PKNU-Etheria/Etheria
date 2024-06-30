// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "E_Item.generated.h"

UCLASS()
class ETHERIA_API AE_Item : public AActor, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AE_Item();

	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual void NotifyActorBeginOverlap(class AActor* Other) override;	// 아이템 상자와 캐릭터가 오버랩이 발생할 때 발동되는 함수.

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// 상자를 초기화 시킬 때 사용할 함수
	virtual void PostInitializeComponents() override;
	// 상자를 먹었을 때 먹은 대상에게 게임플레이 이펙트를 적용시켜주는 함수 
	void ApplyEffectToTarget(AActor* Target);
	// 상자에 대한 자체 이펙트 재생하기 위한 함수
	void InvokeGameplayCue(AActor* Target);

protected:
	// 상자를 구성할 주요 클래스 객체들을 생성

	UPROPERTY()
	TObjectPtr<class UAbilitySystemComponent> ASC;
	// 트리거를 위한 원 컴포넌트

	UPROPERTY(VisibleAnywhere, Category = Sphere)
	TObjectPtr<class USphereComponent> Trigger;
	// 스태틱 메쉬 컴포넌트

	UPROPERTY(VisibleAnywhere, Category = Box)
	TObjectPtr<class UStaticMeshComponent> Mesh;

	UPROPERTY(EditAnywhere, Category = GAS)
	TSubclassOf<class UGameplayEffect> GameplayEffectClass;

	// 큐에 관련된 태그. 구조체라서 정방 선언 할 수 없어서 바로 쓰기 때문에. 헤더를 추가함
	// 메타를 넣으면 ? -> 이 태그가 게임플레이 큐로 시작하는 태그만 걸러서 보여주게 됨.

	UPROPERTY(EditAnywhere, Category = GAS, Meta = (Categories = GameplayCue))
	FGameplayTag GameplayCueTag;

};

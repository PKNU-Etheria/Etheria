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
	virtual void NotifyActorBeginOverlap(class AActor* Other) override;	// ������ ���ڿ� ĳ���Ͱ� �������� �߻��� �� �ߵ��Ǵ� �Լ�.

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// ���ڸ� �ʱ�ȭ ��ų �� ����� �Լ�
	virtual void PostInitializeComponents() override;
	// ���ڸ� �Ծ��� �� ���� ��󿡰� �����÷��� ����Ʈ�� ��������ִ� �Լ� 
	void ApplyEffectToTarget(AActor* Target);
	// ���ڿ� ���� ��ü ����Ʈ ����ϱ� ���� �Լ�
	void InvokeGameplayCue(AActor* Target);

protected:
	// ���ڸ� ������ �ֿ� Ŭ���� ��ü���� ����

	UPROPERTY()
	TObjectPtr<class UAbilitySystemComponent> ASC;
	// Ʈ���Ÿ� ���� �� ������Ʈ

	UPROPERTY(VisibleAnywhere, Category = Sphere)
	TObjectPtr<class USphereComponent> Trigger;
	// ����ƽ �޽� ������Ʈ

	UPROPERTY(VisibleAnywhere, Category = Box)
	TObjectPtr<class UStaticMeshComponent> Mesh;

	UPROPERTY(EditAnywhere, Category = GAS)
	TSubclassOf<class UGameplayEffect> GameplayEffectClass;

	// ť�� ���õ� �±�. ����ü�� ���� ���� �� �� ��� �ٷ� ���� ������. ����� �߰���
	// ��Ÿ�� ������ ? -> �� �±װ� �����÷��� ť�� �����ϴ� �±׸� �ɷ��� �����ְ� ��.

	UPROPERTY(EditAnywhere, Category = GAS, Meta = (Categories = GameplayCue))
	FGameplayTag GameplayCueTag;

};

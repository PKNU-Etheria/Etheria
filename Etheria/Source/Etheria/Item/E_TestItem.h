// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "E_TestItem.generated.h"

UENUM(BlueprintType)
enum class EItemState : uint8
{
    Idle,
    MovingPlayer,
    Collected
};

UCLASS()
class ETHERIA_API AE_TestItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AE_TestItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    USphereComponent* CollisionSphere;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* ItemMesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Properties")
    float DetectionRadius;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Properties")
    FString ItemName;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item Properties")
    EItemState CurrentState;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Properties")
    float MoveSpeed;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Properties")
    float AccelerationRate;

private:
    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    void MoveToPlayer(float DeltaTime);
    void PickupItem(AActor* OtherActor);

    AActor* TargetPlayer;
    float CurrentMoveSpeed;
};

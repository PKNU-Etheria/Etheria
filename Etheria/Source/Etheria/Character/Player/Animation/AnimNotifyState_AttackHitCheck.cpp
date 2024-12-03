// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Player/Animation/AnimNotifyState_AttackHitCheck.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"

UAnimNotifyState_AttackHitCheck::UAnimNotifyState_AttackHitCheck()
{

}

FString UAnimNotifyState_AttackHitCheck::GetNotifyName_Implementation() const
{
	return TEXT("EPAttackHitCheck");
}

void UAnimNotifyState_AttackHitCheck::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
}

void UAnimNotifyState_AttackHitCheck::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);

    TArray<FHitResult> hitResults;

    FVector startLocation = MeshComp->GetSocketLocation(StartSocket);
    FVector endLocation = MeshComp->GetSocketLocation(EndSocket);

    TArray<TEnumAsByte<EObjectTypeQuery>> objectType;
    objectType.Emplace(UEngineTypes::ConvertToObjectType(ECC_Pawn));

    TArray<AActor*> ignore;
    ignore.Emplace(MeshComp->GetOwner());

    UKismetSystemLibrary::SphereTraceMultiForObjects(MeshComp, startLocation, endLocation, Radius, objectType,
        false, ignore, EDrawDebugTrace::ForDuration, hitResults, true);

    for (FHitResult& hit : hitResults)
    {
        AActor* DamagedActor = hit.GetActor();

        if (DamagedActor)
        {
            if (!DamagedActors.Contains(DamagedActor))
            {
                DamagedActors.Add(DamagedActor);

                AActor* OwnerActor = MeshComp->GetOwner();
                if (OwnerActor)
                {
                    FGameplayEventData PayloadData;
                    PayloadData.EventMagnitude = Damage;
                    // 내가 지정한 ASC를 가진 특정 액터에 태그를 넣어서 이벤트를 발동시키는 함수
                    UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(OwnerActor, TriggerGameplayTag, PayloadData);
                }
            }
        }
    }
}

void UAnimNotifyState_AttackHitCheck::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
}

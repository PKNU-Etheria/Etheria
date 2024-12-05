// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EPlayerAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class ETHERIA_API UEPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	UEPlayerAnimInstance();
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

public:
	void SetDeadAnim() { IsDead = true; }
	void SetAiming(bool Aiming) { IsAiming = Aiming; }

public:
	float GetCurrentSpeed() { return CurrentPawnSpeed; }
	bool GetIsAiming() { return IsAiming; }
	bool GetIsInAir() { return IsInAir; }

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	float CurrentPawnSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool IsInAir;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool IsDead;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool IsAiming;
};

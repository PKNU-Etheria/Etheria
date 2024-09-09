// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Player/EPlayerAnimInstance.h"
#include "Character/Player/EPlayer.h"
#include "GameFramework/PawnMovementComponent.h"

UEPlayerAnimInstance::UEPlayerAnimInstance()
{
	CurrentPawnSpeed = 0.0f;
	IsInAir = false;
	IsDead = false;
	IsAiming = false;
}

void UEPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto Pawn = TryGetPawnOwner();
	if (!::IsValid(Pawn)) return;

	if (!IsDead)
	{
		CurrentPawnSpeed = Pawn->GetVelocity().Size();
		AEPlayer* Character = Cast<AEPlayer>(Pawn);
		if (Character)
		{
			IsInAir = Character->GetMovementComponent()->IsFalling();
		}
	}
}

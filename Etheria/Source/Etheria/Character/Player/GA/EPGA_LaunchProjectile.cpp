// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Player/GA/EPGA_LaunchProjectile.h"
#include "Character/Player/EPlayer.h"
#include "Item/EProjectile.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"

UEPGA_LaunchProjectile::UEPGA_LaunchProjectile()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	static ConstructorHelpers::FClassFinder<AActor> PROJECTILE(TEXT("/Script/Engine.Blueprint'/Game/Item/BP_EBomb.BP_EBomb_C'"));
	if (PROJECTILE.Succeeded())
	{
		BP_Projectile = PROJECTILE.Class;
	}

	Projectile = nullptr;
}

void UEPGA_LaunchProjectile::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AEPlayer* EPlayer = CastChecked<AEPlayer>(ActorInfo->AvatarActor.Get());

	if (IsValid(EPlayer))
	{
		UAbilityTask_PlayMontageAndWait* PlayAttackTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("PlayAttack"), EPlayer->GetSkillMontage());

		PlayAttackTask->OnCompleted.AddDynamic(this, &UEPGA_LaunchProjectile::OnCompleteCallback);
		PlayAttackTask->OnInterrupted.AddDynamic(this, &UEPGA_LaunchProjectile::OnInterruptedCallback);

		// 투사체 발사
		UE_LOG(LogTemp, Warning, TEXT("UEPGA_LaunchProjectile : Launch!"));

		UWorld* world = EPlayer->GetWorld();

		float launchSpeed = 3000.0f;
		float gravityScale = 1.0f;

		if (world)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = EPlayer;
			SpawnParams.Instigator = EPlayer;

			FVector muzzleLocation = EPlayer->GetMesh()->GetSocketLocation(FName("Projectile_Socket"));
			FRotator muzzleRotation = EPlayer->GetMesh()->GetSocketRotation(FName("Projectile_Socket"));

			// spawn fireball
			Projectile = world->SpawnActor<AEProjectile>(BP_Projectile, muzzleLocation, muzzleRotation, SpawnParams);
		}

		// projectile launch
		if (IsValid(Projectile))
		{
			Projectile->DetachAllSceneComponents(EPlayer->GetMesh(), FDetachmentTransformRules::KeepWorldTransform);
			Projectile->FireInDirection(EPlayer->GetActorForwardVector() * 0.5f, launchSpeed, gravityScale);

			PlayAttackTask->ReadyForActivation();
		}
	}
}

void UEPGA_LaunchProjectile::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

void UEPGA_LaunchProjectile::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	AEPlayer* EPlayer = CastChecked<AEPlayer>(ActorInfo->AvatarActor.Get());
}

void UEPGA_LaunchProjectile::OnCompleteCallback()
{
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}

void UEPGA_LaunchProjectile::OnInterruptedCallback()
{
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = true;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}

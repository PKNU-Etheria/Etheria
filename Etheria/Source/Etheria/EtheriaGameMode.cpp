// Copyright Epic Games, Inc. All Rights Reserved.

#include "EtheriaGameMode.h"
#include "Character/Player/EPlayer.h"
#include "Character/Player/EPlayerController.h"
#include "Character/Player/EPlayerState.h"
#include "UObject/ConstructorHelpers.h"

AEtheriaGameMode::AEtheriaGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Character/Player/BP_EPlayer.BP_EPlayer_C"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(TEXT("/Game/Character/Player/BP_EPlayerController.BP_EPlayerController_C"));
	if (PlayerControllerBPClass.Class != NULL)
	{
		PlayerControllerClass = PlayerControllerBPClass.Class;
	}

	static ConstructorHelpers::FClassFinder<APlayerState> PlayerStateBPClass(TEXT("/Script/Engine.Blueprint'/Game/Character/Player/BP_EPlayerState.BP_EPlayerState_C'"));
	if (PlayerStateBPClass.Class != NULL)
	{
		PlayerStateClass = PlayerStateBPClass.Class;
	}
}

void AEtheriaGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	UE_LOG(LogTemp, Warning, TEXT("AEtheriaGameMode : PostLogin"));
}

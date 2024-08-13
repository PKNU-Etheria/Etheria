// Fill out your copyright notice in the Description page of Project Settings.


#include "World/ItemTestGameMode.h"
#include "UObject/ConstructorHelpers.h"
#include "GameFramework/HUD.h"

AItemTestGameMode::AItemTestGameMode()
{
	/// Script / Engine.Blueprint'/Game/Character/Player/BP_EPlayer.BP_EPlayer'
		/// Script / Engine.Blueprint'/Game/ThirdPerson/Blueprints/ItemTest_ThirdPersonCharacter.ItemTest_ThirdPersonCharacter'
		//static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/ItemTest_ThirdPersonCharacter"));

	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Character/Player/BP_EPlayer.BP_EPlayer_C"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	static ConstructorHelpers::FClassFinder<AHUD> HUDBPClass(TEXT("/Game/Item/BP_TutorialHUD.BP_TutorialHUD_C"));
	if (HUDBPClass.Succeeded())
	{
		HUDClass = HUDBPClass.Class;
	}
}

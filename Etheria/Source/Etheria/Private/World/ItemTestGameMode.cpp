// Fill out your copyright notice in the Description page of Project Settings.


#include "World/ItemTestGameMode.h"
#include "UObject/ConstructorHelpers.h"
#include "GameFramework/HUD.h"

AItemTestGameMode::AItemTestGameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/ItemTest_ThirdPersonCharacter"));
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

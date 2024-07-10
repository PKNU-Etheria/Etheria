// Fill out your copyright notice in the Description page of Project Settings.


#include "World/ItemTestGameMode.h"

AItemTestGameMode::AItemTestGameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/ItemTest_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

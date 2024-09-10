// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ECharacter.h"
#include "Character/ECharacterAttributeSet.h"


// Sets default values
AECharacter::AECharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ASC = nullptr;
	AttributeSet = nullptr;
}

UAbilitySystemComponent* AECharacter::GetAbilitySystemComponent() const
{
	return ASC;
}

// Called when the game starts or when spawned
void AECharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AECharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AECharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AECharacter::InitializeDelegate()
{
	
}

void AECharacter::SetDead()
{

}


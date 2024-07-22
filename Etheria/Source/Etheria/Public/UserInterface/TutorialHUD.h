// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Public/Interfaces/InteractionInterface.h"
#include "TutorialHUD.generated.h"

class UMainMenu;
class UInteractionWidget;

UCLASS()
class ETHERIA_API ATutorialHUD : public AHUD
{// HUD는 위젯을 생성하기에 완벽한 장소 -> 무언가에 부착되어야 하는 입장임 우리는.
	GENERATED_BODY()
	
public:
	/// <summary>
	/// Variables
	/// </summary>
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UMainMenu> MainMenuClass;	// TSubclassOf는 컴파일 때 먼저 해당 클래스에 일치하는지 검사함.

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UInteractionWidget> InteractionWidgetClass;

	bool bIsMenuVisble;

	/// <summary>
	/// Functions
	/// </summary>
	ATutorialHUD();

	void DisplayMenu();
	void HideMenu();
	void ToggleMenu();

	void ShowInteractionWidget() const;
	void HideInteractionWidget() const;
	void UpdateInteractionWidget(const FInteractableData* InteractableData) const;

protected:
	/// <summary>
	/// Variables
	/// </summary>
	UPROPERTY()
	UMainMenu* MainMenuWidget;

	UPROPERTY()
	UInteractionWidget* InteractionWidget;

	/// <summary>
	/// Functions
	/// </summary>
	virtual void BeginPlay() override;
};

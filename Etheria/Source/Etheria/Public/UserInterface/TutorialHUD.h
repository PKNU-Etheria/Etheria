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
{// HUD�� ������ �����ϱ⿡ �Ϻ��� ��� -> ���𰡿� �����Ǿ�� �ϴ� ������ �츮��.
	GENERATED_BODY()
	
public:
	/// <summary>
	/// Variables
	/// </summary>
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UMainMenu> MainMenuClass;	// TSubclassOf�� ������ �� ���� �ش� Ŭ������ ��ġ�ϴ��� �˻���.

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UInteractionWidget> InteractionWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UUserWidget> CrosshairWidgetClass;

	bool bIsMenuVisble;

	/// <summary>
	/// Functions
	/// </summary>
	ATutorialHUD();

	void DisplayMenu();
	void HideMenu();
	void ToggleMenu();

	void ShowCrosshair();
	void HideCrosshair();

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

	UPROPERTY()
	UUserWidget* CrosshairWidget;

	/// <summary>
	/// Functions
	/// </summary>
	virtual void BeginPlay() override;
};

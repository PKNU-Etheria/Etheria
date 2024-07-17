// Fill out your copyright notice in the Description page of Project Settings.


#include "UserInterface/TutorialHUD.h"
#include "UserInterface/MainMenu.h"
#include "UserInterface/Interaction/InteractionWidget.h"

ATutorialHUD::ATutorialHUD()
{
}

void ATutorialHUD::DisplayMenu()
{
	if (MainMenuWidget)
	{
		bIsMenuVisble = true;
		MainMenuWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void ATutorialHUD::HideMenu()
{
	if (MainMenuWidget)
	{
		bIsMenuVisble = false;
		MainMenuWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void ATutorialHUD::ShowInteractionWidget() const
{
	if (InteractionWidget)
	{
		InteractionWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void ATutorialHUD::HideInteractionWidget() const
{
	if (InteractionWidget)
	{
		InteractionWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void ATutorialHUD::UpdateInteractionWidget(const FInteractableData* InteractableData) const
{
	if (InteractionWidget)
	{
		if (InteractionWidget->GetVisibility() == ESlateVisibility::Collapsed)
		{
			InteractionWidget->SetVisibility(ESlateVisibility::Visible);
		}

		InteractionWidget->UpdateWidget(InteractableData);
	}
}

void ATutorialHUD::BeginPlay()
{
	Super::BeginPlay();

	if (MainMenuClass)
	{	// 메인 메뉴 위젯이 있다면
		MainMenuWidget = CreateWidget<UMainMenu>(GetWorld(), MainMenuClass); // 해당 위젯으로 세팅
		MainMenuWidget->AddToViewport(5);	// 전체화면 설정, 매개 변수는 z축 선정
		MainMenuWidget->SetVisibility(ESlateVisibility::Collapsed);		// 숨김
	}

	if (InteractionWidgetClass)
	{
		InteractionWidget = CreateWidget<UInteractionWidget>(GetWorld(), InteractionWidgetClass);
		InteractionWidget->AddToViewport(-1);
		InteractionWidget->SetVisibility(ESlateVisibility::Collapsed);
	}


}

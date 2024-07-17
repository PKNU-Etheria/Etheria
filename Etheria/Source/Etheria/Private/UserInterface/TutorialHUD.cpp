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
	{	// ���� �޴� ������ �ִٸ�
		MainMenuWidget = CreateWidget<UMainMenu>(GetWorld(), MainMenuClass); // �ش� �������� ����
		MainMenuWidget->AddToViewport(5);	// ��üȭ�� ����, �Ű� ������ z�� ����
		MainMenuWidget->SetVisibility(ESlateVisibility::Collapsed);		// ����
	}

	if (InteractionWidgetClass)
	{
		InteractionWidget = CreateWidget<UInteractionWidget>(GetWorld(), InteractionWidgetClass);
		InteractionWidget->AddToViewport(-1);
		InteractionWidget->SetVisibility(ESlateVisibility::Collapsed);
	}


}

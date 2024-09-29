// Fill out your copyright notice in the Description page of Project Settings.


#include "TutorialHUD.h"
#include "Inventory.h"
#include "Item/UserInterface/Interaction/InteractionWidget.h"
#include "Item/UserInterface/Inventory/WeaponWheel.h"

ATutorialHUD::ATutorialHUD()
{
}

void ATutorialHUD::DisplayMenu()
{
	if (InventoryWidget)
	{
		CenterMouseCursor();
		bIsInventoryVisble = true;
		InventoryWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void ATutorialHUD::HideMenu()
{
	if (InventoryWidget)
	{
		bIsInventoryVisble = false;
		InventoryWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void ATutorialHUD::ToggleMenu()
{	// �޴�â�� ��������
	if (bIsInventoryVisble)
	{	// �޴�â�� ���� ����
		HideMenu();

		const FInputModeGameOnly InputMode;
		GetOwningPlayerController()->SetInputMode(InputMode);
		GetOwningPlayerController()->SetShowMouseCursor(false);
	}
	else
	{	// �޴��� �������� �� UI��ҵ��� Ŭ���� �� ����. ĳ���� ���� , ī�޶� ���� � ����.
		DisplayMenu();

		const FInputModeGameAndUI InputMode;
		GetOwningPlayerController()->SetInputMode(InputMode);
		GetOwningPlayerController()->SetShowMouseCursor(true);
	}
}

void ATutorialHUD::ShowCrosshair()
{
	if (CrosshairWidget)
	{
		CrosshairWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void ATutorialHUD::HideCrosshair()
{
	if (CrosshairWidget)
	{
		CrosshairWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void ATutorialHUD::ShowWeaponWheel()
{
	if (WeaponWheelWidget)
	{
		CenterMouseCursor();
		bIsWeaponWheelVisible = true;
		WeaponWheelWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void ATutorialHUD::HideWeaponWheel()
{
	if (WeaponWheelWidget)
	{
		bIsWeaponWheelVisible = false;
		WeaponWheelWidget->SetVisibility(ESlateVisibility::Collapsed);
		WeaponWheelWidget->CheckSection();
	}
}

void ATutorialHUD::ToggleWeaponWheel()
{
	// if WeaponWheel VIsible
	if (bIsWeaponWheelVisible)
	{	// �޴�â�� ���� ����
		HideWeaponWheel();

		const FInputModeGameOnly InputMode;
		GetOwningPlayerController()->SetInputMode(InputMode);
		GetOwningPlayerController()->SetShowMouseCursor(false);
	}
	else
	{	// �޴��� �������� �� UI��ҵ��� Ŭ���� �� ����. ĳ���� ���� , ī�޶� ���� � ����.
		ShowWeaponWheel();

		const FInputModeGameAndUI InputMode;
		GetOwningPlayerController()->SetInputMode(InputMode);
		GetOwningPlayerController()->SetShowMouseCursor(true);
	}
}

void ATutorialHUD::InteractWeaponWheel()
{
	if (WeaponWheelWidget)
	{
		WeaponWheelWidget->CalculateSection();
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
		InventoryWidget = CreateWidget<UInventory>(GetWorld(), MainMenuClass); // �ش� �������� ����
		InventoryWidget->AddToViewport(5);	// ��üȭ�� ����, �Ű� ������ z�� ����
		InventoryWidget->SetVisibility(ESlateVisibility::Collapsed);		// ����
	}

	if (InteractionWidgetClass)
	{
		InteractionWidget = CreateWidget<UInteractionWidget>(GetWorld(), InteractionWidgetClass);
		InteractionWidget->AddToViewport(-1);
		InteractionWidget->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (CrosshairWidgetClass)
	{
		CrosshairWidget = CreateWidget<UUserWidget>(GetWorld(), CrosshairWidgetClass);
		CrosshairWidget->AddToViewport();
		CrosshairWidget->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (WeaponWheelClass)
	{
		WeaponWheelWidget = CreateWidget<UWeaponWheel>(GetWorld(), WeaponWheelClass);
		WeaponWheelWidget->AddToViewport();
		WeaponWheelWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void ATutorialHUD::CenterMouseCursor()
{
	if (APlayerController * PC = GetOwningPlayerController())
	{
		FViewport* Viewport = PC->GetLocalPlayer()->ViewportClient->Viewport;
		if (Viewport)
		{
			int32 ViewportSizeX, ViewportSizeY;
			PC->GetViewportSize(ViewportSizeX, ViewportSizeY);

			int32 X = ViewportSizeX / 2;
			int32 Y = ViewportSizeY / 2;

			Viewport->SetMouse(X, Y);
		}
	}
}

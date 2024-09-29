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
{	// 메뉴창이 떠있으면
	if (bIsInventoryVisble)
	{	// 메뉴창을 먼저 숨김
		HideMenu();

		const FInputModeGameOnly InputMode;
		GetOwningPlayerController()->SetInputMode(InputMode);
		GetOwningPlayerController()->SetShowMouseCursor(false);
	}
	else
	{	// 메뉴가 열려있을 때 UI요소들을 클릭할 수 있음. 캐릭터 제어 , 카메라 무빙 등도 포함.
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
	{	// 메뉴창을 먼저 숨김
		HideWeaponWheel();

		const FInputModeGameOnly InputMode;
		GetOwningPlayerController()->SetInputMode(InputMode);
		GetOwningPlayerController()->SetShowMouseCursor(false);
	}
	else
	{	// 메뉴가 열려있을 때 UI요소들을 클릭할 수 있음. 캐릭터 제어 , 카메라 무빙 등도 포함.
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
	{	// 메인 메뉴 위젯이 있다면
		InventoryWidget = CreateWidget<UInventory>(GetWorld(), MainMenuClass); // 해당 위젯으로 세팅
		InventoryWidget->AddToViewport(5);	// 전체화면 설정, 매개 변수는 z축 선정
		InventoryWidget->SetVisibility(ESlateVisibility::Collapsed);		// 숨김
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

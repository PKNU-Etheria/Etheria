// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UInteractionWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	InteractionProgressBar->PercentDelegate.BindUFunction(this, "UpdateInteractionProgress");
}

void UInteractionWidget::NativeConstruct()
{
	Super::NativeConstruct();

	KeyPressText->SetText(FText::FromString("Press"));
	CurrentInteractionDuration = 0.0f;
}

void UInteractionWidget::UpdateWidget(const FInteractableData* InteractableData) const
{
	switch (InteractableData->InteractableType)
	{
	case EInteractableType::Pickup:	// 아이템에 대한 위젯 정보 업데이트.
		KeyPressText->SetText(FText::FromString("Press"));	// 상호작용 워딩
		InteractionProgressBar->SetVisibility(ESlateVisibility::Collapsed);	// 숨김

		if (InteractableData->Quantity < 2)	// 수량이 1개일 경우
		{	
			QuantityText->SetVisibility(ESlateVisibility::Collapsed);
		}
		else  // 수량이 2개 이상일 경우.
		{
			QuantityText->SetText(FText::Format(NSLOCTEXT("InteractionWidget", "QuantityText", "x{0}"), InteractableData->Quantity));
			QuantityText->SetVisibility(ESlateVisibility::Visible);
		}

		break;

	case EInteractableType::NonPlayerCharacter:
		break;

	case EInteractableType::Device:
		break;

	case EInteractableType::Toggle:
		break;

	case EInteractableType::Container:
		break;

	default:
		break;
	}

	ActionText->SetText(InteractableData->Action);
	NameText->SetText(InteractableData->Name);

}

float UInteractionWidget::UpdateInteractionProgress()
{
	return 0.0f;
}

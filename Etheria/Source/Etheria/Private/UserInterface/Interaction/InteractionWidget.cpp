// Fill out your copyright notice in the Description page of Project Settings.


#include "UserInterface/Interaction/InteractionWidget.h"
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
	case EInteractableType::Pickup:	// �����ۿ� ���� ���� ���� ������Ʈ.
		KeyPressText->SetText(FText::FromString("Press"));	// ��ȣ�ۿ� ����
		InteractionProgressBar->SetVisibility(ESlateVisibility::Collapsed);	// ����

		if (InteractableData->Quantity < 2)	// ������ 1���� ���
		{	
			QuantityText->SetVisibility(ESlateVisibility::Collapsed);
		}
		else  // ������ 2�� �̻��� ���.
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

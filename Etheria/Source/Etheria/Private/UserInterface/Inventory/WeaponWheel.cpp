// Fill out your copyright notice in the Description page of Project Settings.


#include "UserInterface/Inventory/WeaponWheel.h"
#include "Character/Player/EPlayer.h"
#include "Components/WeaponWheelComponent.h"
#include "Public/UserInterface/Inventory/WeaponWheelSectionImage.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

void UWeaponWheel::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	PlayerCharacter = Cast<AEPlayer>(GetOwningPlayerPawn());
	if (PlayerCharacter)
	{
		WeaponWheelReference = PlayerCharacter->GetWeaponWheel();
		WeaponWheelReference->OnWeaponWheelWidgetUpdated.AddUObject(this, &UWeaponWheel::UpdateActiveSection);
	}

	if (WeaponSectionPanel)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, FString::Printf(TEXT("WeaponSectionPanel Connect!")));
	}

	// MI_WeaponWheel�̶�� ��Ƽ������ ������� ���� �ν��Ͻ� ����
	if (WeaponWheel && RadialMenuMat == nullptr)
	{
		UMaterialInstance* ParentMaterial = LoadObject<UMaterialInstance>(nullptr, TEXT("/Script/Engine.MaterialInstanceConstant'/Game/UserInterface/WeaponWheel/materials/MI_WeaponWheel.MI_WeaponWheel'"));
		if (ParentMaterial)
		{
			RadialMenuMat = UMaterialInstanceDynamic::Create(ParentMaterial, this);
			WeaponWheel->SetBrushFromMaterial(RadialMenuMat);
		}
	}

	// SectionCount�� SectionSize ����
	if (RadialMenuMat)
	{
		float RotationValue = (WeaponWheelReference->CurSectionAngle + (WeaponWheelReference->GetSectionCount() * 0.5f)) * WeaponWheelReference->GetSectionSize(); // Change Value

		RadialMenuMat->SetScalarParameterValue(FName("SectionCount"), WeaponWheelReference->GetSectionCount());
		RadialMenuMat->SetScalarParameterValue(FName("ActiveRotation"), RotationValue);
	}

	MaxBounds.Empty();
	MinBounds.Empty();

	CalculateSectionBounds();

	SettingSectionImage();
}

void UWeaponWheel::NativeConstruct()
{
	Super::NativeConstruct();

	PlayerCharacter = Cast<AEPlayer>(GetOwningPlayerPawn());
}

void UWeaponWheel::CalculateSectionBounds()
{
	float PartitionSectionSize = WeaponWheelReference->GetSectionSize()/ 2;
	int SectionCount = WeaponWheelReference->GetSectionCount();

	for (int32 i = 0; i < SectionCount; ++i)
	{
		int SectionSize = WeaponWheelReference->GetSectionSize() * i;
		float MinBound = SectionSize - PartitionSectionSize;
		float MaxBound = SectionSize + PartitionSectionSize;

		MaxBounds.Add(MaxBound);
		MinBounds.Add(MinBound);

		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, FString::Printf(TEXT("Check Bounds: %d, %d"), MinBounds[i], MaxBounds[i]));
	}

	// MaxBounds�� MinBounds�� ��Ƽ���� �Ķ���ͷ� ����
	if (RadialMenuMat)
	{
		RadialMenuMat->SetScalarParameterValue(FName("SectionCount"), SectionCount);
		//RadialMenuMat->SetScalarParameterValue(FName("SectionSize"), SectionSize);
	}
}

void UWeaponWheel::SettingSectionImage()
{
	if (!WeaponWheelSectionImageClass || !WeaponSectionPanel) return; // Ŭ���� �Ǵ� �г��� ������ ����

	if (!WeaponSectionPanel || !WeaponSectionPanel->IsA<UCanvasPanel>())
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("WeaponSectionPanel is not a Canvas Panel!"));
		return;
	}

	FVector InitialRotationAxis = FVector(0.0f, -1.0f, 0.0f);

	// 4���� ������ �гο� �߰�
	for (int32 i = 0; i < WeaponWheelReference->GetSectionCount(); ++i)
	{
		// UWeaponWheelSectionImage ������ ����
		UWeaponWheelSectionImage* NewSectionImage = CreateWidget<UWeaponWheelSectionImage>(this, WeaponWheelSectionImageClass);
		NewSectionImage->GetWeaponIcon()->SetBrushFromTexture(WeaponWheelReference->WeaponSectionDefaultImages[i]);

		if (NewSectionImage)
		{
			// �гο� �� ���� �߰�
			WeaponSectionPanel->AddChild(NewSectionImage);
			WeaponImageWidget.Add(NewSectionImage);
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, FString::Printf(TEXT("Check Section Children: %d"), WeaponSectionPanel->GetAllChildren().Num()));
		}

		UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(NewSectionImage->Slot);
		if (!CanvasSlot) continue;

		CanvasSlot->SetAnchors(FAnchors(0.5f, 0.5f, 0.5f, 0.5f));
		CanvasSlot->SetAlignment(FVector2D(0.5f, 0.5f));

		float AngleDegrees = i * WeaponWheelReference->GetSectionSize();

		FVector RotatedVector = InitialRotationAxis.RotateAngleAxis(AngleDegrees, FVector(0.0f, 0.0f, 1.0f));
		FVector2D Position2D(RotatedVector.X * 250.0f, RotatedVector.Y * 250.0f); // 250.0f is the radius from the center.

		// Set the position for the widget.
		CanvasSlot->SetPosition(Position2D);

		// Set the size of each widget (can be adjusted to fit icon dimensions).
		CanvasSlot->SetSize(FVector2D(200.0f, 200.0f));

		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, FString::Printf(TEXT("Add Weapon Section Image : %d"), i));
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, FString::Printf(TEXT("Widget Position: %f, %f"), Position2D.X, Position2D.Y));

	}
}

void UWeaponWheel::CalculateSection()
{
	// ���콺�� ȸ�� ���� ��������
	FRotator MouseRotation = GetMouseRotation();
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Blue, MouseRotation.ToString());
	float MouseYaw = MouseRotation.Yaw;

	int SectionCount = WeaponWheelReference->GetSectionCount();

	// ���� ��谪 Ȯ���� ���� ����
	for (int32 i = 0; i < SectionCount; ++i)
	{
		float MinBound = MinBounds[i];
		float MaxBound = MaxBounds[i];

		if (MinBound < 0.0f)
		{
			MinBound += 360.0f;
			if (MouseYaw >= MinBound || MouseYaw <= MaxBound)
			{
				SelectedAngle = i;
				break;
			}
		}
		else
		{
			if (MouseYaw >= MinBound && MouseYaw <= MaxBound)
			{
				SelectedAngle = i;
				break;
			}
		}
	}

	float RotationValue = (SelectedAngle + (SectionCount * 0.5f))* WeaponWheelReference->GetSectionSize(); // Change Value

	if (RadialMenuMat)
	{
		// ��Ƽ������ ��Į�� �Ķ���͸� ������Ʈ
		RadialMenuMat->SetScalarParameterValue(FName("SelectedRotation"), RotationValue);

		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("RotationValue : %d, %d"), SelectedAngle, WeaponWheelReference->CurSectionAngle));
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("Updated SelectedRotation: %d, %d"), MinBounds[SelectedAngle], MaxBounds[SelectedAngle]));
	}
}

void UWeaponWheel::CheckSection()
{
	if (WeaponWheelReference->CurSectionAngle == SelectedAngle) return;

	WeaponWheelReference->ChangeCurSectionValue(SelectedAngle);
}

void UWeaponWheel::UpdateActiveSection()
{
	float RotationValue = (WeaponWheelReference->CurSectionAngle + (WeaponWheelReference->GetSectionCount() * 0.5f)) * WeaponWheelReference->GetSectionSize(); // Change Value

	if (RadialMenuMat)
	{
		// ��Ƽ������ ��Į�� �Ķ���͸� ������Ʈ
		RadialMenuMat->SetScalarParameterValue(FName("ActiveRotation"), RotationValue);

		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("RotationValue : %f"), RotationValue));
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("Updated SelectedRotation: %d, %d"), MinBounds[SelectedAngle], MaxBounds[SelectedAngle]));
	}
}

FRotator UWeaponWheel::GetMouseRotation()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (!PlayerController) return FRotator::ZeroRotator;

	float MouseX, MouseY;
	bool bGotMousePosition = PlayerController->GetMousePosition(MouseX, MouseY);
	if (!bGotMousePosition)  return FRotator::ZeroRotator;

	FVector2D ViewportSize;
	GEngine->GameViewport->GetViewportSize(ViewportSize);

	FVector2D ScreenCenter = ViewportSize * 0.5f;

	FVector MousePositionVector(MouseX, MouseY, 0.0f);

	FVector ScreenCenterVector(ScreenCenter.X, ScreenCenter.Y, 0.0f);

	FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(ScreenCenterVector, MousePositionVector);

	LookAtRotation.Yaw = 180.0f - LookAtRotation.Yaw;

	return LookAtRotation;
}

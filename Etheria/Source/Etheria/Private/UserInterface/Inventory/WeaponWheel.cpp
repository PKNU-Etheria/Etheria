// Fill out your copyright notice in the Description page of Project Settings.


#include "UserInterface/Inventory/WeaponWheel.h"
#include "Character/Player/EPlayer.h"
#include "Components/WeaponWheelComponent.h"
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

	// MI_WeaponWheel이라는 머티리얼을 기반으로 동적 인스턴스 생성
	if (WeaponWheel && RadialMenuMat == nullptr)
	{
		UMaterialInstance* ParentMaterial = LoadObject<UMaterialInstance>(nullptr, TEXT("/Script/Engine.MaterialInstanceConstant'/Game/UserInterface/WeaponWheel/materials/MI_WeaponWheel.MI_WeaponWheel'"));
		if (ParentMaterial)
		{
			RadialMenuMat = UMaterialInstanceDynamic::Create(ParentMaterial, this);
			WeaponWheel->SetBrushFromMaterial(RadialMenuMat);
		}
	}

	// SectionCount와 SectionSize 설정
	if (RadialMenuMat)
	{
		RadialMenuMat->SetScalarParameterValue(FName("SectionCount"), WeaponWheelReference->GetSectionCount());
	}

	MaxBounds.Empty();
	MinBounds.Empty();

	CalculateSectionBounds();
}

void UWeaponWheel::NativeConstruct()
{
	Super::NativeConstruct();

	PlayerCharacter = Cast<AEPlayer>(GetOwningPlayerPawn());
}

void UWeaponWheel::NativePreConstruct()
{
	Super::NativePreConstruct();

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

	// MaxBounds와 MinBounds를 머티리얼 파라미터로 설정
	if (RadialMenuMat)
	{
		RadialMenuMat->SetScalarParameterValue(FName("SectionCount"), SectionCount);
		//RadialMenuMat->SetScalarParameterValue(FName("SectionSize"), SectionSize);
	}
}

void UWeaponWheel::CalculateSection()
{
	// 마우스의 회전 각도 가져오기
	FRotator MouseRotation = GetMouseRotation();
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Blue, MouseRotation.ToString());
	float MouseYaw = MouseRotation.Yaw;

	int SectionCount = WeaponWheelReference->GetSectionCount();

	// 섹션 경계값 확인을 위한 루프
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
		// 머티리얼의 스칼라 파라미터를 업데이트
		RadialMenuMat->SetScalarParameterValue(FName("SelectedRotation"), RotationValue);

		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("RotationValue : %f"), RotationValue));
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
		// 머티리얼의 스칼라 파라미터를 업데이트
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
	return FRotator();
}

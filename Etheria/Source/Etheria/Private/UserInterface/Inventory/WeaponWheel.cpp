// Fill out your copyright notice in the Description page of Project Settings.


#include "UserInterface/Inventory/WeaponWheel.h"
#include "Character/Player/EPlayer.h"
#include "Components/WeaponWheelComponent.h"

void UWeaponWheel::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	PlayerCharacter = Cast<AEPlayer>(GetOwningPlayerPawn());
	if (PlayerCharacter)
	{
		WeaponWheelReference = PlayerCharacter->GetWeaponWheel();
	}
}

void UWeaponWheel::NativeConstruct()
{
	Super::NativeConstruct();

	PlayerCharacter = Cast<AEPlayer>(GetOwningPlayerPawn());
}

void UWeaponWheel::NativePreConstruct()
{
	Super::NativePreConstruct();

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
		RadialMenuMat->SetScalarParameterValue(FName("SectionCount"), SectionCount);
		//RadialMenuMat->SetScalarParameterValue(FName("SectionSize"), 360.0f / SectionCount);
	}

	SectionSize = 360.0f / SectionCount;

	MaxBounds.Empty();
	MinBounds.Empty();

	CalculateSectionBounds();

}

void UWeaponWheel::CalculateSectionBounds()
{
	float PartitionSectionSize = SectionSize / 2;

	for (int32 i = 0; i < SectionCount; ++i)
	{
		//float MinBound = FMath::Fmod((SectionSize * i) - PartitionSectionSize + 360.0f, 360.0f);
		//float MaxBound = FMath::Fmod((SectionSize * (i + 1)) - PartitionSectionSize + 360.0f, 360.0f);
		float MinBound = (SectionSize * i) - PartitionSectionSize;
		float MaxBound = (SectionSize * i) + PartitionSectionSize;

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

void UWeaponWheel::CalculateSection()
{
	// ���콺�� ȸ�� ���� ��������
	FRotator MouseRotation = WeaponWheelReference->GetMouseRotation();
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Blue, MouseRotation.ToString());
	float MouseYaw = MouseRotation.Yaw;

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

	float RotationValue = (SelectedAngle + (SectionCount * 0.5f))*SectionSize; // Change Value

	if (RadialMenuMat)
	{
		// ��Ƽ������ ��Į�� �Ķ���͸� ������Ʈ
		RadialMenuMat->SetScalarParameterValue(FName("SelectedRotation"), RotationValue);

		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("RotationValue : %f"), RotationValue));
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("Updated SelectedRotation: %d, %d"), MinBounds[SelectedAngle], MaxBounds[SelectedAngle]));
	}
}

void UWeaponWheel::UpdateSection()
{
}

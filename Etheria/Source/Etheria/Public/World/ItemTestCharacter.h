// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Components/InteractComponent.h"
#include "Public/Interfaces/InteractionInterface.h"
#include "Public/UserInterface/TutorialHUD.h"
#include "ItemTestCharacter.generated.h"

class UInventoryComponent;
class UItemBase;
class UTimelineComponent;

//USTRUCT()
//struct FInteractionData	// ��ȣ�ۿ뿡 ���� ������
//{
//	GENERATED_USTRUCT_BODY()
//
//	FInteractionData() : CurrentInteractable(nullptr), LastInteractionCheckTime(0.0f)
//	{
//
//	};
//
//	UPROPERTY()
//	AActor* CurrentInteractable;	// ��ȣ�ۿ� ����
//
//	UPROPERTY()
//	float LastInteractionCheckTime;
//};

UCLASS(config = Game)
class ETHERIA_API AItemTestCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;//

	/** Interaction Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* InterAction;//

	/** Aim Zoom Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* AimAction;	//

	/** Toggle On/Off Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* ToggleAction;//

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;//

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;//

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;//

public:
	AItemTestCharacter();


protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);//

	/** Called for looking input */
	void Look(const FInputActionValue& Value);//

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;//

	// To add mapping context
	virtual void BeginPlay();//

	void ToggleMenu();	// ��Ŭ ��/�����ϴ� Ű�� ���ε� ��ų �Լ�.

	void Aim();	// �� �����
	void StopAiming();	// �� ���
	UFUNCTION()
	void UpdateCameraTimeline(const float TimelineValue) const;		// ���� Ÿ�Ӷ��ο� ���� �� ȿ�� (���� ���� -> ���� ����)
	UFUNCTION()
	void CameraTimelineEnd();	// Ÿ�� ���γ����� �̺�Ʈ

	void PerformInteractionCheck();

	void FoundInteractable(AActor* NewInteractable);

	void NoInteractableFound();

	void BeginInteract();

	void EndInteract();

	void Interact();

	virtual void Tick(float DeltaSeconds) override;

	/// <summary>
	/// variables
	/// </summary>
	
	UPROPERTY()
	ATutorialHUD* HUD;

	UPROPERTY(VisibleAnywhere, Category = "Character | Interaction")
	TScriptInterface<IInteractionInterface> TargetInteractable;	// ��ȣ�ۿ� ������ �׸��

	UPROPERTY(VisibleAnywhere, Category = "Character | Inventory")
	UInventoryComponent* PlayerInventory;

	// ��ȣ�ۿ� ������
	float InteractionCheckFrequency;	// ��ȣ�ۿ� �ֱ�

	float InteractionCheckDistance;	// ��ȣ�ۿ� �Ÿ�

	FTimerHandle TimerHandle_Interaction;	// üũ �ֱ�

	FInteractionData InteractionData;	// ��ȣ�ۿ� ������ �׸� -> �ֱٿ� ����
	
	// ī�޶� ���� ��ġ Ÿ�Ӷ��� �Ӽ�
	UPROPERTY(VisibleAnywhere, Category = "Character | Camera")
	FVector DefaultCameraLocation;
	UPROPERTY(VisibleAnywhere, Category = "Character | Camera")
	FVector AimingCameraLocation;

	TObjectPtr<UTimelineComponent> AimingCameraTimeline;

	UPROPERTY(EditDefaultsOnly, Category = "Character | Aim Timeline")
	UCurveFloat* AimingCameraCurve;

public:
	bool bAiming;

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	// ��ȣ�ۿ� �ð��� 0�� ��쿡�� false 
	FORCEINLINE bool IsInteracting() const { return GetWorldTimerManager().IsTimerActive(TimerHandle_Interaction); };

	FORCEINLINE UInventoryComponent* GetInventory() const { return PlayerInventory; };

	void UpdateInteractionWidget() const;

	void DropItem(UItemBase* ItemToDrop, const int32 QuantityToDrop);
};

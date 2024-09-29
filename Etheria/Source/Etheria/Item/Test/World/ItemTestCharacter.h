// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Components/InteractComponent.h"
#include "Item/Item/Interface/InteractionInterface.h"
#include "Item/UserInterface/TutorialHUD.h"
#include "ItemTestCharacter.generated.h"

class UInventoryComponent;
class UItemBase;
class UTimelineComponent;

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

	void ToggleMenu();	// 토클 온/오프하는 키와 바인딩 시킬 함수.

	void Aim();	// 줌 땡기기
	void StopAiming();	// 줌 취소
	UFUNCTION()
	void UpdateCameraTimeline(const float TimelineValue) const;		// 선형 타임라인에 따른 줌 효과 (기존 벡터 -> 수정 벡터)
	UFUNCTION()
	void CameraTimelineEnd();	// 타임 라인끝난뒤 이벤트

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
	TScriptInterface<IInteractionInterface> TargetInteractable;	// 상호작용 가능한 항목들

	UPROPERTY(VisibleAnywhere, Category = "Character | Inventory")
	UInventoryComponent* PlayerInventory;

	// 상호작용 변수들
	float InteractionCheckFrequency;	// 상호작용 주기

	float InteractionCheckDistance;	// 상호작용 거리

	FTimerHandle TimerHandle_Interaction;	// 체크 주기

	FInteractionData InteractionData;	// 상호작용 가능한 항목 -> 최근에 접촉
	
	// 카메라 초점 위치 타임라인 속성
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
	// 상호작용 시간이 0인 경우에는 false 
	FORCEINLINE bool IsInteracting() const { return GetWorldTimerManager().IsTimerActive(TimerHandle_Interaction); };

	FORCEINLINE UInventoryComponent* GetInventory() const { return PlayerInventory; };

	void UpdateInteractionWidget() const;

	void DropItem(UItemBase* ItemToDrop, const int32 QuantityToDrop);
};

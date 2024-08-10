// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ECharacter.h"
#include "InputActionValue.h"
#include "AbilitySystemInterface.h"
#include "Public/Interfaces/InteractionInterface.h"
#include "EPlayer.generated.h"

class ATutorialHUD;
class UInventoryComponent;
class UItemBase;
class UTimelineComponent;

USTRUCT()
struct FInteractionData	// 상호작용에 대한 데이터
{
	GENERATED_USTRUCT_BODY()

	FInteractionData() : CurrentInteractable(nullptr), LastInteractionCheckTime(0.0f)
	{

	};

	UPROPERTY()
	AActor* CurrentInteractable;	// 상호작용 액터

	UPROPERTY()
	float LastInteractionCheckTime;
};

UCLASS()
class ETHERIA_API AEPlayer : public AECharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AEPlayer();

	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void PostInitializeComponents() override;
	virtual void PossessedBy(AController* NewController) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

public:
	bool bAiming;	// Aim

	/** Returns SpringArmComponent subobject **/
	FORCEINLINE class USpringArmComponent* GetSpringArmComponent() const { return SpringArmComp; }
	/** Returns CameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetCameraComponent() const { return CameraComp; }
	// if interaction time is zero = false 
	FORCEINLINE bool IsInteracting() const { return GetWorldTimerManager().IsTimerActive(TimerHandle_Interaction); };

	FORCEINLINE UInventoryComponent* GetInventory() const { return PlayerInventory; };

	void UpdateInteractionWidget() const;

	void DropItem(UItemBase* ItemToDrop, const int32 QuantityToDrop);

protected:
	// ASC
	void SetupGASInputComponent();

	// Camera


	// Input
	void InitializeInputKey();

	void Move(const struct FInputActionInstance& Instance);
	void Look(const struct FInputActionInstance& Instance);

	void GASInputPressed(int32 InputID);
	void GASInputReleased(int32 InputID);
	void Interact(int32 InputID);
	void Attack(int32 InputID);
	void Skill(int32 InputID);
	void SpecialSkill(int32 InputID);
	// If you want to add input, add to here
	void Aim();	// Zoom In
	void StopAiming();	// Zoom Out

	// Invecntory
	void ToggleMenu();	 // Inventory ToggleOn/Off

	// Interaction 
	UFUNCTION()
	void UpdateCameraTimeline(const float TimelineValue) const;		// linear timeline Zoom In
	UFUNCTION()
	void CameraTimelineEnd();	// Event after timeline finish
	void PerformInteractionCheck();
	void FoundInteractable(AActor* NewInteractable);
	void NoInteractableFound();
	void BeginInteract();
	void EndInteract();

	// State


	// Skill & Weapon



//Variable
protected:
	//ASC
	UPROPERTY(EditAnywhere, Category = GAS)
	TObjectPtr<class UAbilitySystemComponent> ASC;

	UPROPERTY(EditAnywhere, Category = GAS)
	TArray<TSubclassOf<class UGameplayAbility>> StartAbilities;

	UPROPERTY(EditAnywhere, Category = GAS)
	TMap<int32, TSubclassOf<class UGameplayAbility>> StartInputAbilities;


	// Camera
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* SpringArmComp;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* CameraComp;


	// Interact Component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Interact, meta = (AllowPrivateAccess = "true"))
	class UInteractComponent* InteractComp;

	// Input
	// ���߿� �迭�� ����
	UPROPERTY(VisibleAnywhere, Category = Input)
	class UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction* LookAction;

	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction* InteractAction;

	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction* AttackAction;

	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction* SkillAction;

	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction* SpecialSkillAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* ToggleAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* AimAction;

	// Inventory 
	UPROPERTY()
	ATutorialHUD* HUD;

	UPROPERTY(VisibleAnywhere, Category = "Character | Inventory")
	UInventoryComponent* PlayerInventory;

	// Interaction
	UPROPERTY(VisibleAnywhere, Category = "Character | Interaction")
	TScriptInterface<IInteractionInterface> TargetInteractable;	// Interaction Actor Data

	float InteractionCheckFrequency;	// Interaction Frequency

	float InteractionCheckDistance;	// Interaction Distance

	FTimerHandle TimerHandle_Interaction;	// Check Frequency

	FInteractionData InteractionData;	// Can interact Actor Info

	// Zoom
	UPROPERTY(VisibleAnywhere, Category = "Character | Camera")
	FVector DefaultCameraLocation;
	UPROPERTY(VisibleAnywhere, Category = "Character | Camera")
	FVector AimingCameraLocation;

	TObjectPtr<UTimelineComponent> AimingCameraTimeline;

	UPROPERTY(EditDefaultsOnly, Category = "Character | Aim Timeline")
	UCurveFloat* AimingCameraCurve;
};

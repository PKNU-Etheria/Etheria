// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ECharacter.h"
#include "InputActionValue.h"
#include "AbilitySystemInterface.h"
#include "Public/Interfaces/InteractionInterface.h"
#include "Public/UserInterface/TutorialHUD.h"
#include "EPlayer.generated.h"

class UInventoryComponent;
class UItemBase;
class UTimelineComponent;

DECLARE_MULTICAST_DELEGATE(FShowQuest);

UCLASS()
class ETHERIA_API AEPlayer : public AECharacter//, public IAbilitySystemInterface
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

	FORCEINLINE class UInteractComponent* GetInteractComponent() const { return InteractComp; }

	FORCEINLINE class ATutorialHUD* GetHUD() const { return HUD; };

	FORCEINLINE UInventoryComponent* GetInventory() const { return PlayerInventory; };

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
	void Dash(int32 InputID);

	void InventoryUI(int32 InputID);
	void WeaponUI(int32 InputID);
	void QuickSlot(int32 InputID);
	// If you want to add input, add to here
	void Aim();	// Zoom In
	void StopAiming();	// Zoom Out

	// Inventory
	void InitializeInventorySet();
	void ToggleMenu();	 // Inventory ToggleOn/Off

	// Interaction 
	UFUNCTION()
	void UpdateCameraTimeline(const float TimelineValue) const;		// linear timeline Zoom In
	UFUNCTION()
	void CameraTimelineEnd();	// Event after timeline finish
	
	void ShowQuest(int32 InputID);

	// State
	virtual void InitializeDelegate() override;

	virtual void SetDead() override;

	UFUNCTION()
	virtual void OnOutOfHealth();

	// Skill & Weapon
	void SetWeapon();



public:
	// Animation
	void SetMontage(UAnimMontage* sourceMontage, UAnimMontage* targetMontage) { sourceMontage = targetMontage; }
	// To Change Montage needs some GA have UAnimMontage itself

	UAnimMontage* GetInteractMontage() { return InteractMontage; }
	UAnimMontage* GetAttackMontage() { return AttackMontage; }
	UAnimMontage* GetSkillMontage() { return SkillMontage; }
	UAnimMontage* GetSpecialSkillMontage() { return SpecialSkillMontage; }
	UAnimMontage* GetDashMontage() { return DashMontage; }



//Variable
protected:
	//ASC
	//UPROPERTY(EditAnywhere, Category = GAS)
	//TObjectPtr<class UAbilitySystemComponent> ASC;

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
	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction* QuestAction;

	// Delegates
public:
	FShowQuest Delegate_ShowQuest;
	
	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<class UInputAction> DashAction;

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<class UInputAction> InventoryUIAction;

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<class UInputAction> WeaponUIAction;

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<class UInputAction> QuickSlotAction_01;

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<class UInputAction> QuickSlotAction_02;

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<class UInputAction> QuickSlotAction_03;

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<class UInputAction> QuickSlotAction_04;

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<class UInputAction> QuickSlotAction_05;

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<class UInputAction> QuickSlotAction_06;


	// Animation
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> InteractMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> AttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> SkillMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> SpecialSkillMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> DashMontage;


	// Weapon
	// TObjectPtr<class EWeapon> Weapon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* AimAction;

	// Inventory 
	UPROPERTY()
	ATutorialHUD* HUD;

	UPROPERTY(VisibleAnywhere, Category = "Character | Inventory")
	UInventoryComponent* PlayerInventory;

	// Zoom
	UPROPERTY(VisibleAnywhere, Category = "Character | Camera")
	FVector DefaultCameraLocation;
	UPROPERTY(VisibleAnywhere, Category = "Character | Camera")
	FVector AimingCameraLocation;

	TObjectPtr<UTimelineComponent> AimingCameraTimeline;

	UPROPERTY(EditDefaultsOnly, Category = "Character | Aim Timeline")
	UCurveFloat* AimingCameraCurve;
};

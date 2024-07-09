// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ECharacter.h"
#include "InputActionValue.h"
#include "AbilitySystemInterface.h"
#include "EPlayer.generated.h"

/**
 * 
 */
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
	/** Returns SpringArmComponent subobject **/
	FORCEINLINE class USpringArmComponent* GetSpringArmComponent() const { return SpringArmComp; }
	/** Returns CameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetCameraComponent() const { return CameraComp; }


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


	// Input
	// 나중에 배열로 관리
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
	
};

// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Player/EPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components/TimelineComponent.h"
#include "Components/InventoryComponent.h"
#include "Public/World/Pickup.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "AbilitySystemComponent.h"
#include "EPlayerController.h"
#include "EPlayerState.h"
#include "Character/ECharacterAttributeSet.h"
//#include "Etheria/Weapon/EWeapon.h"
#include "Components/InteractComponent.h"
#include "DrawDebugHelpers.h"
#include "Public/UserInterface/TutorialHUD.h"
#include "Character/Player/Tag/EPlayerGameAbilityTag.h"

AEPlayer::AEPlayer()
{
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT("/Script/Engine.SkeletalMesh'/Plugins_AssetShare/BattleWizardPolyart/Meshes/WizardSM.WizardSM'"));
	if (TempMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(TempMesh.Object);
		// change this
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));
	}

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// ASC
	ASC = nullptr;

	// Create a SpringArmComp (pulls in towards the player if there is a collision)
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComp->SetupAttachment(GetCapsuleComponent());
	SpringArmComp->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	SpringArmComp->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a CameraComp
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	CameraComp->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Create Interact Component
	InteractComp = CreateDefaultSubobject<UInteractComponent>(TEXT("InteractComponent"));
	InteractComp->SetPlayer(this);

	// Animation
	InitializeMontage();

	// Input
	InitializeInputKey();

	// Inventory Setting
	InitializeInventorySet();
}

UAbilitySystemComponent* AEPlayer::GetAbilitySystemComponent() const
{
	return ASC;
}

void AEPlayer::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("AEPlayer : BeginPlay"));

	AEPlayerController* PlayerController = Cast<AEPlayerController>(Controller);
	if (PlayerController != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerController != nullptr"));
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem< UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		if (Subsystem != nullptr)
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerController == nullptr"));
	}

	HUD = Cast<ATutorialHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());

	FOnTimelineFloat AimLerpAlphaValue;
	FOnTimelineEvent TimelineFinishedEvent;
	AimLerpAlphaValue.BindUFunction(this, FName("UpdateCameraTimeline"));
	TimelineFinishedEvent.BindUFunction(this, FName("CameraTimelineEnd"));

	if (AimingCameraTimeline && AimingCameraCurve)
	{
		AimingCameraTimeline->AddInterpFloat(AimingCameraCurve, AimLerpAlphaValue);
		AimingCameraTimeline->SetTimelineFinishedFunc(TimelineFinishedEvent);
	}
}

void AEPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (this->GetCharacterMovement()->IsFalling())
	{
		if (IsValid(ASC) && !ASC->HasMatchingGameplayTag(PLAYERTAG_STATE_ISINAIR))
		{
			ASC->AddLooseGameplayTag(PLAYERTAG_STATE_ISINAIR);
		}
	}
	else
	{
		if (IsValid(ASC) && ASC->HasMatchingGameplayTag(PLAYERTAG_STATE_ISINAIR))
		{
			ASC->RemoveLooseGameplayTag(PLAYERTAG_STATE_ISINAIR);
		}
	}
}

void AEPlayer::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	UE_LOG(LogTemp, Warning, TEXT("AEPlayer : PostInitializeComponents"));
}

void AEPlayer::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	AEPlayerState* EPS = GetPlayerState<AEPlayerState>();
	if (EPS)
	{
		ASC = EPS->GetAbilitySystemComponent();
		ASC->InitAbilityActorInfo(EPS, this);

		FGameplayEffectContextHandle EffectContextHandle = ASC->MakeEffectContext();
		EffectContextHandle.AddSourceObject(this);
		FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec(InitStatEffect, 1, EffectContextHandle);
		if (EffectSpecHandle.IsValid())
		{
			ASC->BP_ApplyGameplayEffectSpecToSelf(EffectSpecHandle);
		}

		for (const auto& StartAbility : StartAbilities)
		{
			FGameplayAbilitySpec StartSpec(StartAbility);
			ASC->GiveAbility(StartSpec);
		}

		for (const auto& StartInputAbility : StartInputAbilities)
		{
			FGameplayAbilitySpec StartSpec(StartInputAbility.Value);
			StartSpec.InputID = StartInputAbility.Key;
			ASC->GiveAbility(StartSpec);
		}

		SetupGASInputComponent();

		APlayerController* PlayerController = CastChecked<APlayerController>(NewController);
		PlayerController->ConsoleCommand(TEXT("showdebug abilitysystem"));

		InitializeDelegate();
	}
}

void AEPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	// Default Input
	if (EnhancedInputComponent != nullptr)
	{
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AEPlayer::Look);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AEPlayer::Move);
	}

	// GAS Input
	SetupGASInputComponent();
}

void AEPlayer::DropItem(UItemBase* ItemToDrop, const int32 QuantityToDrop)
{
	if (PlayerInventory->FindMatchingItem(ItemToDrop))
	{	// ���ο� ���͸� ���忡 �����ϱ� ���� Ŭ���� ����(�پ��� ����ü �Լ� ����)
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.bNoFail = true;
		// ������ �������� �� ���ο� ���� ���� �������� �ʵ��� ��. � ������Ʈ�� �������̵� �ϴ��� üũ
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		// ĳ���� �տ� �����ٴ� ���� -> ĳ���� ���ο��� ����Ǵ� �ͺ��� ����.
		const FVector SpawnLocation{ GetActorLocation() + (GetActorForwardVector() * 50.0f) };
		const FTransform SpawnTransform(GetActorRotation(), SpawnLocation);

		const int32 RemovedQuantity = PlayerInventory->RemoveAmountOfItem(ItemToDrop, QuantityToDrop);

		APickup* Pickup = GetWorld()->SpawnActor<APickup>(APickup::StaticClass(), SpawnTransform, SpawnParams);

		Pickup->InitializeDrop(ItemToDrop, RemovedQuantity);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Item to drop was somehow null!"));
	}
}

void AEPlayer::SetupGASInputComponent()
{
	if (IsValid(ASC) && IsValid(InputComponent))
	{
		UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AEPlayer::GASInputPressed, EInputValue::E_Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AEPlayer::GASInputReleased, EInputValue::E_Jump);

		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &AEPlayer::Interact, EInputValue::E_Interact);
		EnhancedInputComponent->BindAction(QuestAction, ETriggerEvent::Started, this, &AEPlayer::ShowQuest, EInputValue::E_Quest);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &AEPlayer::GASInputPressed, EInputValue::E_Attack);
		EnhancedInputComponent->BindAction(SkillAction, ETriggerEvent::Triggered, this, &AEPlayer::GASInputPressed, EInputValue::E_Skill);
		EnhancedInputComponent->BindAction(SpecialSkillAction, ETriggerEvent::Triggered, this, &AEPlayer::SpecialSkill, EInputValue::E_SpecialSkill);
		EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Triggered, this, &AEPlayer::GASInputPressed, EInputValue::E_Dash);

		EnhancedInputComponent->BindAction(InventoryUIAction, ETriggerEvent::Triggered, this, &AEPlayer::InventoryUI, EInputValue::E_InventoryUI);
		EnhancedInputComponent->BindAction(WeaponUIAction, ETriggerEvent::Triggered, this, &AEPlayer::WeaponUI, EInputValue::E_WeaponUI);

		EnhancedInputComponent->BindAction(QuickSlotAction_01, ETriggerEvent::Triggered, this, &AEPlayer::QuickSlot, EInputValue::E_QuickSlot_01);
		EnhancedInputComponent->BindAction(QuickSlotAction_02, ETriggerEvent::Triggered, this, &AEPlayer::QuickSlot, EInputValue::E_QuickSlot_02);
		EnhancedInputComponent->BindAction(QuickSlotAction_03, ETriggerEvent::Triggered, this, &AEPlayer::QuickSlot, EInputValue::E_QuickSlot_03);
		EnhancedInputComponent->BindAction(QuickSlotAction_04, ETriggerEvent::Triggered, this, &AEPlayer::QuickSlot, EInputValue::E_QuickSlot_04);
		EnhancedInputComponent->BindAction(QuickSlotAction_05, ETriggerEvent::Triggered, this, &AEPlayer::QuickSlot, EInputValue::E_QuickSlot_05);
		EnhancedInputComponent->BindAction(QuickSlotAction_06, ETriggerEvent::Triggered, this, &AEPlayer::QuickSlot, EInputValue::E_QuickSlot_06);
	}
}

void AEPlayer::GASInputPressed(EInputValue InputID)
{
	FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromInputID(InputID);
	if (Spec)
	{
		Spec->InputPressed = true;

		if (Spec->IsActive())
		{
			ASC->AbilitySpecInputPressed(*Spec);
		}
		else
		{
			ASC->TryActivateAbility(Spec->Handle);
			UE_LOG(LogTemp, Warning, TEXT("AEPlayer : GASInputPressed"));
		}
	}
}

void AEPlayer::GASInputReleased(EInputValue InputID)
{
	FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromInputID(InputID);
	if (Spec)
	{
		Spec->InputPressed = false;

		if (Spec->IsActive())
		{
			ASC->AbilitySpecInputReleased(*Spec);
			UE_LOG(LogTemp, Warning, TEXT("AEPlayer : GASInputReleased"));
		}
	}
}

void AEPlayer::InitializeInputKey()
{
	static ConstructorHelpers::FObjectFinder<UInputMappingContext>DEFAULT_MAPPING_CONTEXT
	(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Character/Player/Input/IMC.IMC'"));
	if (DEFAULT_MAPPING_CONTEXT.Succeeded())
		DefaultMappingContext = DEFAULT_MAPPING_CONTEXT.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction>IA_JUMP
	(TEXT("/Script/EnhancedInput.InputAction'/Game/Character/Player/Input/Actions/IA_Jump.IA_Jump'"));
	if (IA_JUMP.Succeeded())
		JumpAction = IA_JUMP.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction>IA_LOOK
	(TEXT("/Script/EnhancedInput.InputAction'/Game/Character/Player/Input/Actions/IA_Look.IA_Look'"));
	if (IA_LOOK.Succeeded())
		LookAction = IA_LOOK.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction>IA_MOVE
	(TEXT("/Script/EnhancedInput.InputAction'/Game/Character/Player/Input/Actions/IA_Move.IA_Move'"));
	if (IA_MOVE.Succeeded())
		MoveAction = IA_MOVE.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction>IA_INTERACT
	(TEXT("/Script/EnhancedInput.InputAction'/Game/Character/Player/Input/Actions/IA_Interact.IA_Interact'"));
	if (IA_INTERACT.Succeeded())
		InteractAction = IA_INTERACT.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction>IA_ATTACK
	(TEXT("/Script/EnhancedInput.InputAction'/Game/Character/Player/Input/Actions/IA_Attack.IA_Attack'"));
	if (IA_ATTACK.Succeeded())
		AttackAction = IA_ATTACK.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction>IA_SKILL
	(TEXT("/Script/EnhancedInput.InputAction'/Game/Character/Player/Input/Actions/IA_Skill.IA_Skill'"));
	if (IA_SKILL.Succeeded())
		SkillAction = IA_SKILL.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction>IA_SPECIALSKILL
	(TEXT("/Script/EnhancedInput.InputAction'/Game/Character/Player/Input/Actions/IA_SpeicalSkill.IA_SpeicalSkill'"));
	if (IA_SPECIALSKILL.Succeeded())
		SpecialSkillAction = IA_SPECIALSKILL.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction>IA_DASH
	(TEXT("/Script/EnhancedInput.InputAction'/Game/Character/Player/Input/Actions/IA_Dash.IA_Dash'"));
	if (IA_DASH.Succeeded())
		DashAction = IA_DASH.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction>IA_INVENTORYUI
	(TEXT("/Script/EnhancedInput.InputAction'/Game/Character/Player/Input/Actions/IA_InventoryUI.IA_InventoryUI'"));
	if (IA_INVENTORYUI.Succeeded())
		InventoryUIAction = IA_INVENTORYUI.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction>IA_WEAPONUI
	(TEXT("/Script/EnhancedInput.InputAction'/Game/Character/Player/Input/Actions/IA_WeaponUI.IA_WeaponUI'"));
	if (IA_WEAPONUI.Succeeded())
		WeaponUIAction = IA_WEAPONUI.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction>IA_QUICKSLOT_01
	(TEXT("/Script/EnhancedInput.InputAction'/Game/Character/Player/Input/Actions/IA_QuickSlot_1.IA_QuickSlot_1'"));
	if (IA_QUICKSLOT_01.Succeeded())
		QuickSlotAction_01 = IA_QUICKSLOT_01.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction>IA_QUICKSLOT_02
	(TEXT("/Script/EnhancedInput.InputAction'/Game/Character/Player/Input/Actions/IA_QuickSlot_2.IA_QuickSlot_2'"));
	if (IA_QUICKSLOT_02.Succeeded())
		QuickSlotAction_02 = IA_QUICKSLOT_02.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction>IA_QUICKSLOT_03
	(TEXT("/Script/EnhancedInput.InputAction'/Game/Character/Player/Input/Actions/IA_QuickSlot_3.IA_QuickSlot_3'"));
	if (IA_QUICKSLOT_03.Succeeded())
		QuickSlotAction_03 = IA_QUICKSLOT_03.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction>IA_QUICKSLOT_04
	(TEXT("/Script/EnhancedInput.InputAction'/Game/Character/Player/Input/Actions/IA_QuickSlot_4.IA_QuickSlot_4'"));
	if (IA_QUICKSLOT_04.Succeeded())
		QuickSlotAction_04 = IA_QUICKSLOT_04.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction>IA_QUICKSLOT_05
	(TEXT("/Script/EnhancedInput.InputAction'/Game/Character/Player/Input/Actions/IA_QuickSlot_5.IA_QuickSlot_5'"));
	if (IA_QUICKSLOT_05.Succeeded())
		QuickSlotAction_05 = IA_QUICKSLOT_05.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction>IA_QUICKSLOT_06
	(TEXT("/Script/EnhancedInput.InputAction'/Game/Character/Player/Input/Actions/IA_QuickSlot_6.IA_QuickSlot_6'"));
	if (IA_QUICKSLOT_06.Succeeded())
		QuickSlotAction_06 = IA_QUICKSLOT_06.Object;

	 static ConstructorHelpers::FObjectFinder<UInputAction>IA_QUEST
	 (TEXT("/Script/EnhancedInput.InputAction'/Game/Character/Player/Input/Actions/IA_Quest.IA_Quest'"));
	 if (IA_QUEST.Succeeded())
	 	QuestAction = IA_QUEST.Object;
}

void AEPlayer::Move(const FInputActionInstance& Instance)
{
	FVector2D MovementVector = Instance.GetValue().Get<FVector2D>();

	if (Controller != nullptr)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector FowardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(FowardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AEPlayer::Look(const FInputActionInstance& Instance)
{
	FVector2D LookVector = Instance.GetValue().Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput(LookVector.X);
		AddControllerPitchInput(LookVector.Y);
	}
}

void AEPlayer::Interact(EInputValue InputID)
{
	UE_LOG(LogTemp, Warning, TEXT("AEPlayer : Interact"));
	InteractComp->Interact();
}

void AEPlayer::Attack(EInputValue InputID)
{
	UE_LOG(LogTemp, Warning, TEXT("AEPlayer : Attack"));
}

void AEPlayer::Skill(EInputValue InputID)
{
	UE_LOG(LogTemp, Warning, TEXT("AEPlayer : Skill"));
}

void AEPlayer::SpecialSkill(EInputValue InputID)
{
	UE_LOG(LogTemp, Warning, TEXT("AEPlayer : SpecialSkill"));
}

void AEPlayer::Dash(EInputValue InputID)
{
	UE_LOG(LogTemp, Warning, TEXT("AEPlayer : Dash"));
}

void AEPlayer::InventoryUI(EInputValue InputID)
{
	UE_LOG(LogTemp, Warning, TEXT("AEPlayer : InventoryUI"));
}

void AEPlayer::WeaponUI(EInputValue InputID)
{
	UE_LOG(LogTemp, Warning, TEXT("AEPlayer : WeaponUI"));
}

void AEPlayer::QuickSlot(EInputValue InputID)
{
	UE_LOG(LogTemp, Warning, TEXT("AEPlayer : QuickSlot"));
}

void AEPlayer::InitializeDelegate()
{
	//AttributeSet->OnOutOfHealth.AddDynamic(this, &ThisClass::OnOutOfHealth);
	const UECharacterAttributeSet* CurrentAttributeSet = ASC->GetSet<UECharacterAttributeSet>();
	// mutable로 제외를 델리게이트 등록이 가능
	if (CurrentAttributeSet)
	{
		CurrentAttributeSet->OnOutOfHealth.AddDynamic(this, &AEPlayer::OnOutOfHealth);
	}
}

void AEPlayer::SetDead()
{
	Super::SetDead();

	APlayerController* PlayerController = Cast<AEPlayerController>(GetController());
	if (PlayerController)
	{
		DisableInput(PlayerController);
	}
}

void AEPlayer::OnOutOfHealth()
{
	SetDead();
}

void AEPlayer::InitializeMontage()
{
	ConstructorHelpers::FObjectFinder<UAnimMontage> INTERACTMONTAGE(TEXT("/Script/Engine.AnimMontage'/Game/Character/Player/Animation/AM_Interact.AM_Interact'"));
	if (INTERACTMONTAGE.Succeeded())
	{
		//SetMontage(AttackMontage, TEMPMONTAGE.Object);
		InteractMontage = INTERACTMONTAGE.Object;
	}

	ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACKMONTAGE(TEXT("/Script/Engine.AnimMontage'/Game/Character/Player/Animation/Staff/AM_Staff_Attack.AM_Staff_Attack'"));
	if (ATTACKMONTAGE.Succeeded())
	{
		//SetMontage(AttackMontage, TEMPMONTAGE.Object);
		AttackMontage = ATTACKMONTAGE.Object;
	}

	ConstructorHelpers::FObjectFinder<UAnimMontage> SKILLMONTAGE(TEXT("/Script/Engine.AnimMontage'/Game/Character/Player/Animation/Staff/AM_Staff_Skill.AM_Staff_Skill'"));
	if (SKILLMONTAGE.Succeeded())
	{
		SkillMontage = SKILLMONTAGE.Object;
	}

	ConstructorHelpers::FObjectFinder<UAnimMontage> SPECIALSKILLMONTAGE(TEXT("/Script/Engine.AnimMontage'/Game/Character/Player/Animation/Staff/AM_Staff_SpecialSkill.AM_Staff_SpecialSkill'"));
	if (SPECIALSKILLMONTAGE.Succeeded())
	{
		SpecialSkillMontage = SPECIALSKILLMONTAGE.Object;
	}

	ConstructorHelpers::FObjectFinder<UAnimMontage> DASHMONTAGE(TEXT("/Script/Engine.AnimMontage'/Game/Character/Player/Animation/Staff/AM_Staff_Dash.AM_Staff_Dash'"));
	if (DASHMONTAGE.Succeeded())
	{
		DashMontage = DASHMONTAGE.Object;
	}

	ConstructorHelpers::FObjectFinder<UAnimMontage> DEATHMONTAGE(TEXT("/Script/Engine.AnimMontage'/Game/Character/Player/Animation/AM_Death.AM_Death'"));
	if (DEATHMONTAGE.Succeeded())
	{
		DeathMontage = DEATHMONTAGE.Object;
	}
}

void AEPlayer::Aim()
{
	if (!HUD->bIsMenuVisble)
	{	
		bAiming = true;
		bUseControllerRotationYaw = true;	
		GetCharacterMovement()->MaxWalkSpeed = 200.0f;

		if (AimingCameraTimeline)	
		{	
			AimingCameraTimeline->PlayFromStart();
		}
	}
}

void AEPlayer::StopAiming()
{
	if (bAiming)
	{
		bAiming = false;
		bUseControllerRotationYaw = false;	
		HUD->HideCrosshair();
		GetCharacterMovement()->MaxWalkSpeed = 500.0f;

		if (AimingCameraTimeline)
		{	
			AimingCameraTimeline->Reverse();
		}
	}
}

void AEPlayer::InitializeInventorySet()
{
	PlayerInventory = CreateDefaultSubobject<UInventoryComponent>(TEXT("PlayerInventory"));
	PlayerInventory->SetSlotsCapacity(20);
	PlayerInventory->SetWeightCapacity(50.0f);
}

void AEPlayer::ToggleMenu()
{
	HUD->ToggleMenu();

	if (HUD->bIsMenuVisble)
	{
		StopAiming();
	}
}

void AEPlayer::UpdateCameraTimeline(const float TimelineValue) const
{
	const FVector CameraLocation = FMath::Lerp(DefaultCameraLocation, AimingCameraLocation, TimelineValue);
	SpringArmComp->SocketOffset = CameraLocation;
}

void AEPlayer::CameraTimelineEnd()
{
	if (AimingCameraTimeline)
	{
		if (AimingCameraTimeline->GetPlaybackPosition() != 0.0f)
		{
			HUD->ShowCrosshair();
		}
	}
}

void AEPlayer::ShowQuest(EInputValue InputID)
{
	Delegate_ShowQuest.Broadcast();
}

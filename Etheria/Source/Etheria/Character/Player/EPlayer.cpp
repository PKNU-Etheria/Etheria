// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Player/EPlayer.h"
#include "Character/Player/EPlayerController.h"
#include "Character/Player/EPlayerState.h"
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
#include "Components/InteractComponent.h"
#include "DrawDebugHelpers.h"
#include "Public/UserInterface/TutorialHUD.h"

AEPlayer::AEPlayer()
{
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Mannequins/Meshes/SKM_Quinn.SKM_Quinn'"));

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

	APlayerController* PlayerController = Cast<AEPlayerController>(Controller);
	if (PlayerController != nullptr)
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem< UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		if (Subsystem != nullptr)
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
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

		int32 InputId = 0;
		for (const auto& StartAbility : StartAbilities)
		{
			// ����Ƽ �ο�
			FGameplayAbilitySpec StartSpec(StartAbility);
			StartSpec.InputID = InputId++;
			ASC->GiveAbility(StartSpec);

			SetupGASInputComponent();
		}

		//for (const auto& StartInputAbility : StartInputAbilities)
		//{
		//	// ����Ƽ �ο�
		//	FGameplayAbilitySpec StartSpec(StartInputAbility.Value);
		//	StartSpec.InputID = StartInputAbility.Key;
		//	ASC->GiveAbility(StartSpec);
		//}

		//// ���� �Լ��� ���������� ȣ��Ǳ� ������ Ŭ���̾�Ʈ������ �� �Լ��� ȣ����� �ʴ´�.
		//// �� �� ���൵ �������.
		//SetupGASInputComponent();

		//// Ŭ���� ��ü�� �÷��̾ ���� Ŭ�����̱� ������ CastChecked �ص� ��
		//APlayerController* PlayerController = CastChecked<APlayerController>(NewController);

		//// PgDn, PgUp�� ������ GAS Component�� ����ϴ� Actor�� ������ �� �ִ�. (Player->NPC �����ư��鼭 Ȯ�� ����)
		//// ������ NPC�� ���� �ƹ��͵� ���� �ʾƵ� �÷��̾��� �±׷� ��Ÿ���� ���� �� �� �ִ�. 
		//// �̴� ���״� �ƴ�, ������ ������ ����� �Ѵ�.
		//// Config -> DefaultGame.ini����
		//// [/Script/GameplayAbilities.AbilitySystemGlobals]
		//// bUseDebugTargetFromHud = True
		//// �� �� ���� �߰������ �Ѵ�.
		//PlayerController->ConsoleCommand(TEXT("showdebug abilitysystem"));
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

		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AEPlayer::GASInputPressed, 0);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AEPlayer::GASInputReleased, 0);

		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &AEPlayer::Interact, 1);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &AEPlayer::Attack, 2);
		EnhancedInputComponent->BindAction(SkillAction, ETriggerEvent::Triggered, this, &AEPlayer::Skill, 3);
		EnhancedInputComponent->BindAction(SpecialSkillAction, ETriggerEvent::Triggered, this, &AEPlayer::SpecialSkill, 4);
		EnhancedInputComponent->BindAction(ToggleAction, ETriggerEvent::Started, this, &AEPlayer::ToggleMenu, 5);
	}
}

void AEPlayer::GASInputPressed(int32 InputID)
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

void AEPlayer::GASInputReleased(int32 InputID)
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

	static ConstructorHelpers::FObjectFinder<UInputAction>IA_TOGGLE
	(TEXT("/Script/EnhancedInput.InputAction'/Game/Character/Player/Input/Actions/IA_ToggleMenu.IA_ToggleMenu'"));
	if (IA_TOGGLE.Succeeded())
		ToggleAction = IA_TOGGLE.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction>IA_AIM
	(TEXT("/Script/EnhancedInput.InputAction'/Game/Character/Player/Input/Actions/IA_Aim.IA_Aim'"));
	if (IA_AIM.Succeeded())
		AimAction = IA_AIM.Object;
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

void AEPlayer::Interact(int32 InputID)
{
	UE_LOG(LogTemp, Warning, TEXT("AEPlayer : Interact"));
	InteractComp->Interact();
}

void AEPlayer::Attack(int32 InputID)
{
	UE_LOG(LogTemp, Warning, TEXT("AEPlayer : Attack"));
}

void AEPlayer::Skill(int32 InputID)
{
	UE_LOG(LogTemp, Warning, TEXT("AEPlayer : Skill"));
}

void AEPlayer::SpecialSkill(int32 InputID)
{
	UE_LOG(LogTemp, Warning, TEXT("AEPlayer : SpecialSkill"));
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

void AEPlayer::ToggleMenu(int32 InputID)
{
	UE_LOG(LogTemp, Warning, TEXT("AEPlayer : SpecialSkill"));

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

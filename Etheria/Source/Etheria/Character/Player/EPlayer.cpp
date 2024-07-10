// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Player/EPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "AbilitySystemComponent.h"
#include "EPlayerController.h"
#include "EPlayerState.h"

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

	// Input
	InitializeInputKey();

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
			// 어빌리티 부여
			FGameplayAbilitySpec StartSpec(StartAbility);
			StartSpec.InputID = InputId++;
			ASC->GiveAbility(StartSpec);

			SetupGASInputComponent();
		}

		//for (const auto& StartInputAbility : StartInputAbilities)
		//{
		//	// 어빌리티 부여
		//	FGameplayAbilitySpec StartSpec(StartInputAbility.Value);
		//	StartSpec.InputID = StartInputAbility.Key;
		//	ASC->GiveAbility(StartSpec);
		//}

		//// 빙의 함수는 서버에서만 호출되기 때문에 클라이언트에서는 밑 함수가 호출되지 않는다.
		//// 둘 다 해줘도 상관없다.
		//SetupGASInputComponent();

		//// 클래스 자체는 플레이어를 위한 클래스이기 때문에 CastChecked 해도 됨
		//APlayerController* PlayerController = CastChecked<APlayerController>(NewController);

		//// PgDn, PgUp을 누르면 GAS Component를 사용하는 Actor를 변경할 수 있다. (Player->NPC 번갈아가면서 확인 가능)
		//// 하지만 NPC를 보면 아무것도 하지 않아도 플레이어의 태그로 나타나는 것을 볼 수 있다. 
		//// 이는 버그는 아님, 하지만 설정을 해줘야 한다.
		//// Config -> DefaultGame.ini에서
		//// [/Script/GameplayAbilities.AbilitySystemGlobals]
		//// bUseDebugTargetFromHud = True
		//// 위 두 줄을 추가해줘야 한다.
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

void AEPlayer::SetupGASInputComponent()
{
	if (IsValid(ASC) && IsValid(InputComponent))
	{
		UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AEPlayer::GASInputPressed, 0);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AEPlayer::GASInputReleased, 0);

		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &AEPlayer::Interact, 1);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &AEPlayer::Attack, 2);
		EnhancedInputComponent->BindAction(SkillAction, ETriggerEvent::Triggered, this, &AEPlayer::Skill, 3);
		EnhancedInputComponent->BindAction(SpecialSkillAction, ETriggerEvent::Triggered, this, &AEPlayer::SpecialSkill, 4);
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

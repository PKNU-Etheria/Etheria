// Fill out your copyright notice in the Description page of Project Settings.

#include "World/ItemTestCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components/TimelineComponent.h"
#include "Public/Components/InventoryComponent.h"
#include "Public/World/Pickup.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "DrawDebugHelpers.h"

// Sets default values
AItemTestCharacter::AItemTestCharacter()
{
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

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
	
	// 줌으로 쓸 카메라 위치들 세팅
	AimingCameraTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("AimingCameraTimeline"));
	DefaultCameraLocation = FVector{ 0.0f, 0.0f, 65.0f };
	AimingCameraLocation = FVector{ 175.0f, 50.0f, 55.0f };
	CameraBoom->SocketOffset = DefaultCameraLocation;
	
	// 상호작용 변수들 초기화
	InteractionCheckFrequency = 0.1f;
	InteractionCheckDistance = 225.0f;

	BaseEyeHeight = 76.0f;	 // Pawn 변수. 원래 64의 값으로 폰의 목부분에서 나타남.

	PlayerInventory = CreateDefaultSubobject<UInventoryComponent>(TEXT("PlayerInventory"));
	PlayerInventory->SetSlotsCapacity(20);
	PlayerInventory->SetWeightCapacity(50.0f);
}

// Called when the game starts or when spawned
void AItemTestCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
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

void AItemTestCharacter::PerformInteractionCheck()
{	// line trace
	InteractionData.LastInteractionCheckTime = GetWorld()->GetTimeSeconds();

	FVector TraceStart{ FVector::ZeroVector };

	if (!bAiming)
	{
		InteractionCheckDistance = 225.0f;
		TraceStart = GetPawnViewLocation();
	}
	else
	{
		InteractionCheckDistance = 325.0f;
		TraceStart = FollowCamera->GetComponentLocation();
	}

	//FVector TraceStart{ GetPawnViewLocation()};
	FVector TraceEnd{ TraceStart + (GetViewRotation().Vector() * InteractionCheckDistance) };	// 회전은 폰 메시가 아닌 캐릭터 컨트롤러에서 이루어짐. -> 마우스로 카메라 위치를 변경.

	// 벡터 내적. 정면 벡터와 회전 벡터.
	float LookDirection = FVector::DotProduct(GetActorForwardVector(), GetViewRotation().Vector());
	// 폰이 바라보고 있는 방향과 같은 방향을 바라보고 있지 않는 한 라인 트레이스를 전혀 진행하지 않음.
	if (LookDirection > 0) // 양수는 같은 방향, 음수는 다른 방향
	{
		DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, false, 1.0f, 0, 2.0f);

		FCollisionQueryParams QueryParams;	// tracing하는 구조체.
		QueryParams.AddIgnoredActor(this);	// 나 자신은 감지되어선 안됌
		FHitResult TraceHit;	// trace 결과 저장
		// 직선을 이용한 충돌 판정
		if (GetWorld()->LineTraceSingleByChannel(TraceHit, TraceStart, TraceEnd, ECC_Visibility, QueryParams))
		{	// 충돌하는 액터가 상호작용 인터페이스가 존재할 시에
			if (TraceHit.GetActor()->GetClass()->ImplementsInterface(UInteractionInterface::StaticClass()))
			{
				// 현재 상호작용 데이터와 다를 시. 현재 액터로 교체
				if (TraceHit.GetActor() != InteractionData.CurrentInteractable)
				{
					FoundInteractable(TraceHit.GetActor());
					return;
				}
				// 동일 시 스킵.
				if (TraceHit.GetActor() == InteractionData.CurrentInteractable)
				{
					return;
				}
			}
		}
	}
	// 상호작용 가능한 액터가 없음.
	NoInteractableFound();
}

void AItemTestCharacter::FoundInteractable(AActor* NewInteractable)
{	// 상호작용하는 경우
	if (IsInteracting())
	{
		EndInteract();
	}

	if (InteractionData.CurrentInteractable)
	{
		TargetInteractable = InteractionData.CurrentInteractable;
		TargetInteractable->EndFocus();	// 하이라이트 종료
	}

	InteractionData.CurrentInteractable = NewInteractable;
	TargetInteractable = NewInteractable;

	HUD->UpdateInteractionWidget(&TargetInteractable->InteractableData);

	TargetInteractable->BeginFocus(); // 하이라이트 시작 (새로운 타겟)
}

void AItemTestCharacter::NoInteractableFound()
{	// 상호작용 하고 있는 경우.
	if (IsInteracting())
	{
		GetWorldTimerManager().ClearTimer(TimerHandle_Interaction);
	}

	if (InteractionData.CurrentInteractable)	// 상호작용 가능한 액터가 존재할 경우
	{	// 포커스 하이라이트 중지
		if (IsValid(TargetInteractable.GetObject()))
		{
			TargetInteractable->EndFocus();
		}

		HUD->HideInteractionWidget();

		// 타겟 널 세팅
		InteractionData.CurrentInteractable = nullptr;
		TargetInteractable = nullptr;
	}
}

void AItemTestCharacter::BeginInteract()
{
	PerformInteractionCheck();	// 변한게 있는지 확인하기 위한 호출.

	if (InteractionData.CurrentInteractable)
	{
		if (IsValid(TargetInteractable.GetObject()))
		{
			TargetInteractable->BeginInteract();

			if (FMath::IsNearlyZero(TargetInteractable->InteractableData.InteractionDuration, 0.1f))
			{
				Interact();
			}
			else
			{
				GetWorldTimerManager().SetTimer(TimerHandle_Interaction, this, &AItemTestCharacter::Interact, TargetInteractable->InteractableData.InteractionDuration, false);
			}
		}
	}
}

void AItemTestCharacter::EndInteract()
{	// 타이머 초기화 -> 상호작용이 잘 마무리 되었다면 굳이 상호작용 중인지 확인할 필요가 없음.
	GetWorldTimerManager().ClearTimer(TimerHandle_Interaction);

	if (IsValid(TargetInteractable.GetObject()))
	{
		TargetInteractable->EndInteract();
	}
}

void AItemTestCharacter::Interact()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_Interaction);

	if (IsValid(TargetInteractable.GetObject()))
	{
		TargetInteractable->Interact(this);
	}
}

void AItemTestCharacter::UpdateInteractionWidget() const
{
	if (IsValid(TargetInteractable.GetObject()))
	{
		HUD->UpdateInteractionWidget(&TargetInteractable->InteractableData);
	}
}

void AItemTestCharacter::ToggleMenu()
{
	HUD->ToggleMenu();
	// 메뉴가 켜지면
	if (HUD->bIsMenuVisble)
	{	// 줌 취소
		StopAiming();
	}
}

void AItemTestCharacter::Aim()
{	// 줌
	if (!HUD->bIsMenuVisble)
	{	// 메뉴가 아닌 상태라면 줌 시전.
		bAiming = true;
		bUseControllerRotationYaw = true;	// 플레이어와 카메라는 같이 움직인다.
		GetCharacterMovement()->MaxWalkSpeed = 200.0f;

		if (AimingCameraTimeline)	// 타임라인이 존재할 경우
		{	// 해당 타임라인에 따른 플레이어 줌이 선형시간에 따라 움직임.
			AimingCameraTimeline->PlayFromStart();
		}
	}
}

void AItemTestCharacter::StopAiming()
{
	if (bAiming)
	{
		bAiming = false;
		bUseControllerRotationYaw = false;	// 플레이어와 카메라 독립
		HUD->HideCrosshair();
		GetCharacterMovement()->MaxWalkSpeed = 500.0f;

		if (AimingCameraTimeline)
		{	// 역으로 되돌림.
			AimingCameraTimeline->Reverse();
		}
	}
}

void AItemTestCharacter::UpdateCameraTimeline(const float TimelineValue) const
{	// 기존 카메라 위치와 줌 카메라 위치를 타임라인에 따른 확대 줌 기능.
	const FVector CameraLocation = FMath::Lerp(DefaultCameraLocation, AimingCameraLocation, TimelineValue);
	CameraBoom->SocketOffset = CameraLocation;
}

void AItemTestCharacter::CameraTimelineEnd()
{
	if (AimingCameraTimeline)
	{
		if (AimingCameraTimeline->GetPlaybackPosition() != 0.0f)
		{
			HUD->ShowCrosshair();
		}
	}
}

void AItemTestCharacter::DropItem(UItemBase* ItemToDrop, const int32 QuantityToDrop)
{	// 인벤토리 내에 드랍하고 싶은 아이템이 존재할 경우
	if (PlayerInventory->FindMatchingItem(ItemToDrop))
	{	// 새로운 액터를 월드에 생성하기 위한 클래스 변수(다양한 구조체 함수 함축)
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.bNoFail = true;
		// 생성된 아이템이 벽 내부와 같은 곳에 스폰되지 않도록 즉. 어떤 오브젝트와 오버라이드 하는지 체크
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		// 캐릭터 앞에 던진다는 느낌 -> 캐릭터 내부에서 드랍되는 것보다 나음.
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

void AItemTestCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (GetWorld()->TimeSince(InteractionData.LastInteractionCheckTime) > InteractionCheckFrequency)
	{
		PerformInteractionCheck();
	}
}

void AItemTestCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		//Interaction TEST
		EnhancedInputComponent->BindAction(InterAction, ETriggerEvent::Started, this, &AItemTestCharacter::BeginInteract);
		EnhancedInputComponent->BindAction(InterAction, ETriggerEvent::Completed, this, &AItemTestCharacter::EndInteract);

		EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Started, this, &AItemTestCharacter::Aim);
		EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Completed, this, &AItemTestCharacter::StopAiming);

		// Toggle On/Off
		EnhancedInputComponent->BindAction(ToggleAction, ETriggerEvent::Started, this, &AItemTestCharacter::ToggleMenu);

		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AItemTestCharacter::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AItemTestCharacter::Look);

	}

}

void AItemTestCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AItemTestCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}
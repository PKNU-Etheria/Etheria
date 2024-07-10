// Fill out your copyright notice in the Description page of Project Settings.

#include "World/ItemTestCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
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

	// ��ȣ�ۿ� ������ �ʱ�ȭ
	InteractionCheckFrequency = 0.1f;
	InteractionCheckDistance = 225.0f;

	BaseEyeHeight = 74.0f;	 // Pawn ����. ���� 64�� ������ ���� ��κп��� ��Ÿ��.
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
}

void AItemTestCharacter::PerformInteractionCheck()
{	// line trace
	InteractionData.LastInteractionCheckTime = GetWorld()->GetTimeSeconds();

	FVector TraceStart{ GetPawnViewLocation()};
	FVector TraceEnd{ TraceStart + (GetViewRotation().Vector() * InteractionCheckDistance) };	// ȸ���� �� �޽ð� �ƴ� ĳ���� ��Ʈ�ѷ����� �̷����. -> ���콺�� ī�޶� ��ġ�� ����.

	// ���� ����. ���� ���Ϳ� ȸ�� ����.
	float LookDirection = FVector::DotProduct(GetActorForwardVector(), GetViewRotation().Vector());
	// ���� �ٶ󺸰� �ִ� ����� ���� ������ �ٶ󺸰� ���� �ʴ� �� ���� Ʈ���̽��� ���� �������� ����.
	if (LookDirection > 0) // ����� ���� ����, ������ �ٸ� ����
	{
		DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, false, 1.0f, 0, 2.0f);

		FCollisionQueryParams QueryParams;	// tracing�ϴ� ����ü.
		QueryParams.AddIgnoredActor(this);	// �� �ڽ��� �����Ǿ �ȉ�
		FHitResult TraceHit;	// trace ��� ����
		// ������ �̿��� �浹 ����
		if (GetWorld()->LineTraceSingleByChannel(TraceHit, TraceStart, TraceEnd, ECC_Visibility, QueryParams))
		{	// �浹�ϴ� ���Ͱ� ��ȣ�ۿ� �������̽��� ������ �ÿ�
			if (TraceHit.GetActor()->GetClass()->ImplementsInterface(UInteractionInterface::StaticClass()))
			{
				const float Distance = (TraceStart - TraceHit.ImpactPoint).Size();
				// ���� ��ȣ�ۿ� �����Ϳ� �ٸ� ��. ���� ���ͷ� ��ü
				if (TraceHit.GetActor() != InteractionData.CurrentInteractable && Distance <= InteractionCheckDistance)
				{
					FoundInteractable(TraceHit.GetActor());
					return;
				}
				// ���� �� ��ŵ.
				if (TraceHit.GetActor() == InteractionData.CurrentInteractable)
				{
					return;
				}
			}
		}
	}
	// ��ȣ�ۿ� ������ ���Ͱ� ����.
	NoInteractableFound();
}

void AItemTestCharacter::FoundInteractable(AActor* NewInteractable)
{	// ��ȣ�ۿ��ϴ� ���
	if (IsInteracting())
	{
		EndInteract();
	}

	if (InteractionData.CurrentInteractable)
	{
		TargetInteractable = InteractionData.CurrentInteractable;
		TargetInteractable->EndFocus();	// ���̶���Ʈ ����
	}

	InteractionData.CurrentInteractable = NewInteractable;
	TargetInteractable = NewInteractable;

	TargetInteractable->BeginFocus(); // ���̶���Ʈ ���� (���ο� Ÿ��)
}

void AItemTestCharacter::NoInteractableFound()
{
}

void AItemTestCharacter::BeginInteract()
{
}

void AItemTestCharacter::EndInteract()
{
}

void AItemTestCharacter::Interact()
{
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
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {

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
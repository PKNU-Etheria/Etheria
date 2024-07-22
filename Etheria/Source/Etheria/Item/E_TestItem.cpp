// Fill out your copyright notice in the Description page of Project Settings.

#include "Item/E_TestItem.h"
#include "World/ItemTestCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

AE_TestItem::AE_TestItem()
{
    PrimaryActorTick.bCanEverTick = true;

    CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
    RootComponent = CollisionSphere;
    CollisionSphere->SetCollisionProfileName("OverlapAllDynamic");

    ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
    ItemMesh->SetupAttachment(RootComponent);
    ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    DetectionRadius = 200.0f;
    ItemName = "Test Item";

    CurrentState = EItemState::Idle;
    MoveSpeed = 200.0f;
    AccelerationRate = 2.0f;
    CurrentMoveSpeed = 0.0f;
}

void AE_TestItem::BeginPlay()
{
    Super::BeginPlay();

    CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &AE_TestItem::OnOverlapBegin);
    CollisionSphere->SetSphereRadius(DetectionRadius);

    // �߷� ��Ȱ��ȭ
    ItemMesh->SetEnableGravity(false);

    // ����� ��ü �׸���
    if (GetWorld())
    {
        DrawDebugSphere(
            GetWorld(),
            GetActorLocation(),
            DetectionRadius,
            32,  // ��ü�� ���׸�Ʈ ��
            FColor::Green,
            false,  // ���������� ǥ��
            -1,    // ���� �ð� (������)
            0,     // ���� �켱����
            1      // �β�
        );
    }
}

void AE_TestItem::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // ��� ���� ��
    if (CurrentState == EItemState::Idle)
    {
        // ������� ó�� �� �Ϸ� ������ �ݺ�.
        FVector NewLocation = GetActorLocation();
        NewLocation.Z += FMath::Sin(GetGameTimeSinceCreation() * 2.0f) * 0.5f;
        SetActorLocation(NewLocation);
    }
    // �������� �÷��̾�� �̵� ���� ��
    if (CurrentState == EItemState::MovingPlayer)
    {
        MoveToPlayer(DeltaTime);
    }

    // �� �����Ӹ��� ����� ��ü �׸���
    if (GetWorld())
    {
        DrawDebugSphere(
            GetWorld(),
            GetActorLocation(),
            DetectionRadius,
            32,
            FColor::Green,
            false,  // ���������� ǥ������ ����
            -1,
            0,
            1  // �β��� 1�� ����
        );
    }
}

void AE_TestItem::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    /*if (Cast<AItemTestCharacter>(OtherActor))
    {
        PickupItem(OtherActor);
    }*/

    if (CurrentState == EItemState::Idle && Cast<AItemTestCharacter>(OtherActor))
    {
        CurrentState = EItemState::MovingPlayer;
        TargetPlayer = OtherActor;
        CurrentMoveSpeed = MoveSpeed;
    }
}

void AE_TestItem::MoveToPlayer(float DeltaTime)
{
    if (TargetPlayer)
    {
        FVector Direction = (TargetPlayer->GetActorLocation() - GetActorLocation()).GetSafeNormal();
        FVector NewLocation = GetActorLocation() + Direction * CurrentMoveSpeed * DeltaTime;

        SetActorLocation(NewLocation);

        // �ӵ� ����
        CurrentMoveSpeed *= (1.0f + AccelerationRate * DeltaTime);

        // �÷��̾���� �Ÿ� üũ
        float DistanceToPlayer = FVector::Distance(GetActorLocation(), TargetPlayer->GetActorLocation());
        if (DistanceToPlayer < 50.0f) // ������ �Ÿ��� �����ϸ� ȹ��
        {
            PickupItem(TargetPlayer);
        }
    }
}

void AE_TestItem::PickupItem(AActor* OtherActor)
{
    AItemTestCharacter* PlayerCharacter = Cast<AItemTestCharacter>(OtherActor);
    if (PlayerCharacter)
    {
        //UInventoryComponent* InventoryComponent = PlayerCharacter->FindComponentByClass<UInventoryComponent>();
        //if (InventoryComponent && InventoryComponent->HasFreeSpace())
        //{
        //    InventoryComponent->AddItem(this);

        //    // ������ ȹ�� �޽��� ǥ��
        //    FString Message = FString::Printf(TEXT("%s ȹ��"), *ItemName);
        //    if (APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
        //    {
        //        // ���⼭ UI ���� ����.
        //    }

        //    // ���� ����
        //}
        Destroy();
    }
}


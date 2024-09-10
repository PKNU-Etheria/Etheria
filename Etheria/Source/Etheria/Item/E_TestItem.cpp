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

    // 중력 비활성화
    ItemMesh->SetEnableGravity(false);

    // 디버그 구체 그리기
    if (GetWorld())
    {
        DrawDebugSphere(
            GetWorld(),
            GetActorLocation(),
            DetectionRadius,
            32,  // 구체의 세그먼트 수
            FColor::Green,
            false,  // 지속적으로 표시
            -1,    // 지속 시간 (영구적)
            0,     // 뎁스 우선순위
            1      // 두께
        );
    }
}

void AE_TestItem::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // 대기 중일 때
    if (CurrentState == EItemState::Idle)
    {
        // 도깨비불 처럼 상 하로 움직임 반복.
        FVector NewLocation = GetActorLocation();
        NewLocation.Z += FMath::Sin(GetGameTimeSinceCreation() * 2.0f) * 0.5f;
        SetActorLocation(NewLocation);
    }
    // 아이템이 플레이어에게 이동 중일 때
    if (CurrentState == EItemState::MovingPlayer)
    {
        MoveToPlayer(DeltaTime);
    }

    // 매 프레임마다 디버그 구체 그리기
    if (GetWorld())
    {
        DrawDebugSphere(
            GetWorld(),
            GetActorLocation(),
            DetectionRadius,
            32,
            FColor::Green,
            false,  // 지속적으로 표시하지 않음
            -1,
            0,
            1  // 두께를 1로 설정
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

        // 속도 증가
        CurrentMoveSpeed *= (1.0f + AccelerationRate * DeltaTime);

        // 플레이어와의 거리 체크
        float DistanceToPlayer = FVector::Distance(GetActorLocation(), TargetPlayer->GetActorLocation());
        if (DistanceToPlayer < 50.0f) // 적당한 거리에 도달하면 획득
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

        //    // 아이템 획득 메시지 표시
        //    FString Message = FString::Printf(TEXT("%s 획득"), *ItemName);
        //    if (APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
        //    {
        //        // 여기서 UI 위젯 생성.
        //    }

        //    // 액터 제거
        //}
        Destroy();
    }
}


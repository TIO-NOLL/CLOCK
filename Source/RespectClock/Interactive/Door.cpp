

#include "Door.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"
#include "RespectClock/Characters/Player/PlayerCharacter.h"
#include "RespectClock/Interactive/RoomNameWidget.h"

ADoor::ADoor()
    :DoorStatus(EDoorStatus::Close)
{
    LerpPointSceneInRoom = CreateDefaultSubobject<USceneComponent>(TEXT("LerpPointInRoom"));
    LerpPointSceneInRoom->SetupAttachment(RootComponent);
}

void ADoor::BeginPlay()
{
	Super::BeginPlay();

    SetActorTickEnabled(false);
}

void ADoor::Tick(float DeltaTime)
{
    ElapsedTime += DeltaTime;

    if(ElapsedTime > ChangeLocationTime)
    {
        SetPlayerLocation();
        SetActorTickEnabled(false);
        IsInteractable = true;
    }
}

FVector ADoor::GetLerpPointPos()
{
    if (!IsValid(LerpPointSceneInRoom)) return FVector::ZeroVector;
    if (IsPlayerInFrontOf) return LerpPointScene->GetComponentLocation();
    return LerpPointSceneInRoom->GetComponentLocation();
}

FRotator ADoor::GetLerpPointRot()
{
    if (!IsValid(LerpPointSceneInRoom)) return FRotator::ZeroRotator;
    if (IsPlayerInFrontOf) return LerpPointScene->GetComponentRotation();
    return LerpPointSceneInRoom->GetComponentRotation();
}

void ADoor::Interactive()
{
    if (IsInteractable)
    {
        APlayerCharacter* Player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

        Player->bInvincible = true;

        //移動フラグとTick処理を有効にする
        ElapsedTime = 0.0f;
        IsInteractable = false;
        SetActorTickEnabled(true);
        PlaySound();
    }
}

EPlayerStatus ADoor::GetStatus()
{
    IsPlayerInFrontOf = IsPlayerInFrontOfActor();
    return EPlayerStatus::Idle;
}

void ADoor::ShowRoomName()
{
    // プレイヤーコントローラーの取得
    if(APlayerController* playerController = UGameplayStatics::GetPlayerController(this, 0))
    {
        // ウィジェットが生成されていないなら
        if(!RoomNameWidget)
        {
            // ウィジェットの生成
            if(RoomNameWidgetClass)
            {
                RoomNameWidget = CreateWidget<URoomNameWidget>(playerController, RoomNameWidgetClass);
                if(RoomNameWidget)
                {
                    RoomNameWidget->AddToViewport(1);
                }
            }
        }

        if(RoomNameWidget)
        {
            // 部屋名の設定
            if(IsPlayerInFrontOf)
            {
                RoomNameWidget->SetRoomName(RoomNameEnter);
            }
            else
            {
                RoomNameWidget->SetRoomName(RoomNameExit);
            }
            // 部屋名の表示
            RoomNameWidget->ShowRoomName();
        }
    }
}

bool ADoor::IsPlayerInFrontOfActor()
{
    // プレイヤーがドアの前にいるか後ろにいるか判定してドアを引くか押すかを返す
    APlayerCharacter* Player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
    if (Player != nullptr)
    {
        FVector PlayerPos = Player->GetActorLocation();
        FVector DoorPos = GetActorLocation();

        FVector DirectionToActor = DoorPos - PlayerPos;
        DirectionToActor.Normalize();

        FVector DoorForwardVector = -GetActorRightVector();
        float DotProduct = FVector::DotProduct(DoorForwardVector, DirectionToActor);

        if (DotProduct >= 0)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    return true;
}

void ADoor::SetPlayerLocation()
{
    APlayerCharacter* Player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
    if (Player != nullptr)
    {
        if(IsPlayerInFrontOf)
        {
            FVector setLocation = LerpPointSceneInRoom->GetComponentLocation();
            Player->SetActorLocation(FVector(setLocation.X, setLocation.Y, Player->GetActorLocation().Z));
            Player->SetActorRotation(Player->GetActorRotation());
        }
        else
        {
            FVector setLocation = LerpPointScene->GetComponentLocation();
            Player->SetActorLocation(FVector(setLocation.X, setLocation.Y, Player->GetActorLocation().Z));
        }
        Player->IsMoveable = true;
    }

    Player->bInvincible = false;
}

bool ADoor::bIsOpenable() const
{
    return IsOpenable;
}

// void ADoor::UpdateRotation(UCurveFloat* movementCurve)
// {
//     USceneComponent* RotationRootComponent = GetRotationRoot();
//
//     //支点コンポーネントは有効か
//     if (IsValid(movementCurve) && IsValid(RotationRootComponent))
//     {
//         //Curveの進行時間の最小値と最大値を取得
//         float MinCurveTime, MaxCurveTime;
//         movementCurve->GetTimeRange(MinCurveTime, MaxCurveTime);
//
//         //進行時間の範囲を超えないように丸め込み
//         ElapsedTime = FMath::Clamp(ElapsedTime, MinCurveTime, MaxCurveTime);
//
//         //閉じてる時から開いてる時までの割合(0.0~1.0)を取得
//         float MoveAlpha = movementCurve->GetFloatValue(ElapsedTime);
//
//         //座標の更新
//         FRotator NewRotation = RotationRootComponent->GetRelativeRotation();
//         NewRotation.Yaw = FMath::Lerp(ClosedDoorYawRotation, OpenedDoorYawRotation, MoveAlpha);
//         RotationRootComponent->SetRelativeRotation(NewRotation);
//
//         //進行時間がCurveの最大値を超えたらTickを停止する
//         if (ElapsedTime >= MaxCurveTime)
//         {
//             IsInteractable = true;
//             DoorStatus = EDoorStatus::Close;
//             ElapsedTime = 0;
//             SetActorTickEnabled(false);
//         }
//     }
// }

// Fill out your copyright notice in the Description page of Project Settings.


#include "CameraChangeArea.h"
#include "Components/BoxComponent.h"
#include "CameraManager.h"
#include "RespectClock/Characters/Player/PlayerCharacter.h"

ACameraChangeArea::ACameraChangeArea()
{
    // ボックスコンポーネントのデフォルトを作成
    OverlapVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("CameraProximityVolume"));
    // ボックスコンポネントをルートコンポネントに取り付ける
    OverlapVolume->SetupAttachment(RootComponent);

}

// オーバーラップ開始
void ACameraChangeArea::NotifyActorBeginOverlap(AActor* OtherActor)
{
    // オーバーラップしているアクタがプレイヤーキャラクターかどうかのキャストチェック
    if (APlayerCharacter* PlayerCharacterCheck = Cast<APlayerCharacter>(OtherActor))
    {
        // プレイヤー キャラクターの PlayerController へのキャスト
        if (APlayerController* PlayerCharacterController = Cast<APlayerController>(PlayerCharacterCheck->GetController()))
        {
            // 現在のカメラと更新後のカメラが同じでないか
            if (PlayerCharacterController->GetViewTarget() != CameraActor)
            {
                //CameraActor を プレイヤー コントローラーのビューに設定します
                PlayerCharacterController->SetViewTargetWithBlend(CameraActor, ChangeTime, EViewTargetBlendFunction::VTBlend_Linear);
            }
        }
    }

}
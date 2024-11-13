//--------------------------------------------------------------------------------------
// ファイル名	：BlendTriggerVolume.cpp
// 内容			：トリガーボックスの作成
// 作成者		：22cu0236 八町陸斗
// 更新日		：2024/04/09		作成
//              ：2024/04/12 カメラの切り替え方を変更(設定したカメラを切り替え)
//--------------------------------------------------------------------------------------

#include "BlendTriggerVolume.h"
#include "Components/BoxComponent.h"
#include "RespectClock/Characters/Player/PlayerCharacter.h"
#include "Camera/CameraActor.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABlendTriggerVolume::ABlendTriggerVolume()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// ボックスコンポーネントのデフォルトを作成
	OverlapVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("CameraProximityVolume"));
	// ボックスコンポネントをルートコンポネントに取り付ける
	OverlapVolume->SetupAttachment(RootComponent);
    // 見つけたカメラを管理する
    //CameraToFind = CreateDefaultSubobject<ACameraActor>(TEXT("CameraToFind"));

    //CameraBlendTime を設定
    CameraBlendTime = 0.0f;
}

// オーバーラップ開始
void ABlendTriggerVolume::NotifyActorBeginOverlap(AActor* OtherActor)
{
    // オーバーラップしているアクタがプレイヤーキャラクターかどうかのキャストチェック
    if (APlayerCharacter* PlayerCharacterCheck = Cast<APlayerCharacter>(OtherActor))
    {
        // プレイヤー キャラクターの PlayerController へのキャスト
        if (APlayerController* PlayerCharacterController = Cast<APlayerController>(PlayerCharacterCheck->GetController()))
        {
            //見つかったカメラ アクタを含む配列
            TArray<AActor*> FoundActors;

            //レベル内のすべてのカメラ アクタを配列に含めるためのユーティリティ関数
            UGameplayStatics::GetAllActorsOfClass(GetWorld(), CameraToFind, FoundActors);

            //見つかった最初の CameraActor にプレイヤー コントローラーのビューを設定します
            PlayerCharacterController->SetViewTargetWithBlend(FoundActors[0], CameraBlendTime, EViewTargetBlendFunction::VTBlend_Linear);
        }
    }

}

// オーバーラップ終了
void ABlendTriggerVolume::NotifyActorEndOverlap(AActor* OtherActor) 
{
    // オーバーラップしているアクタがプレイヤーキャラクターかどうかのキャストチェック
    if (APlayerCharacter* PlayerCharacterCheck = Cast<APlayerCharacter>(OtherActor))
    {
        if (APlayerController* PlayerCharacterController = Cast<APlayerController>(PlayerCharacterCheck->GetController()))
        {
            // プレイヤー キャラクターのカメラ コンポーネントにブレンド
            PlayerCharacterController->SetViewTargetWithBlend(PlayerCharacterController->GetPawn(), CameraBlendTime, EViewTargetBlendFunction::VTBlend_Linear);
        }
    }
}
/*
* FileName	: RazorKiller.cpp
* Created	: Misora Tanaka
* Summary	: キラー自体の制御を行う
* Update	: ヘッダファイルに記載
*/

#include "RazorKiller.h"
#include "Perception/PawnSensingComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "RespectClock/Characters/Player/PlayerCharacter.h"
#include "AIC_RazorKiller.h"

// コンストラクタ
ARazorKiller::ARazorKiller()
	: VisionAngle(60.f)
	, ChaseTime(5.f)
	, TrackTime(0.5f)
{
 	// 07/05現在Tick動かさなくていいので無効化
	PrimaryActorTick.bCanEverTick = false;

	SensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("SensingComp"));
	// 視野
	SensingComp->SetPeripheralVisionAngle(VisionAngle);

	// 見える範囲
	SensingComp->SightRadius = 2000;
	SensingComp->OnSeePawn.AddDynamic(this, &ARazorKiller::OnSeePlayer);
}

// 生成時か開始に動くメソッド
void ARazorKiller::BeginPlay()
{
	Super::BeginPlay();

	AIRazorKiller = Cast<AAIC_RazorKiller>(GetController());

	AIRazorKiller->SetWarpPointIDKey(-1);
}

/// <summary>
/// 何かキャラクターを視認した時動く
/// </summary>
/// <param name="Pawn">視認しているポーン</param>
void ARazorKiller::OnSeePlayer(APawn* Pawn)
{
	// プレイヤーキャラクターとしてキャスト
    TObjectPtr<APlayerCharacter> const Player = Cast<APlayerCharacter>(Pawn);
    if (!Player || !AIRazorKiller)
    {
        return;
    }

    // AIController にプレイヤー情報を設定
    AIRazorKiller->SetTrackingPointKey(100);
    AIRazorKiller->SetIsSeePlayerKey(true);

    // 既存のタイマーがあればクリア
    if (ChaseHandle.IsValid())
    {
        GetWorldTimerManager().ClearTimer(ChaseHandle);
    }

    if (TrackHandle.IsValid())
    {
        GetWorldTimerManager().ClearTimer(TrackHandle);
    }

    // 新たにタイマーをセット
    GetWorldTimerManager().SetTimer(TrackHandle, this, &ARazorKiller::SubTrackingPoint, ChaseTime, false);
    GetWorldTimerManager().SetTimer(ChaseHandle, this, &ARazorKiller::LostSeePlayer, ChaseTime, false);

	if (bLook == false)
	{
		// ビヘイビアツリーを指定時間後に再起動
		if (!RestartBehaviorTreeHandle.IsValid())
		{
			GetWorldTimerManager().SetTimer(RestartBehaviorTreeHandle, this, &ARazorKiller::RestartBehaviorTree, RestartDelay, false);
		}

		if (RestartBehaviorTreeHandle.IsValid())
		{
			UE_LOG(LogTemp, Error, TEXT("RestartBehaviorTreeHandle"));
		}
	}

    DebugPrintString("See", FColor::Blue, 1.f);
}

void ARazorKiller::RestartBehaviorTree()
{
	if (bLook == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("RestartBehaviorTree is being called"));

		if (AIRazorKiller && BehaviorTree)
		{
			AIRazorKiller->RunBehaviorTree(BehaviorTree);
			DebugPrintString("Behavior Tree Restarted", FColor::Green, 1.f);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("BehaviorTree or AIRazorKiller is NULL"));
		}

		bLook = true;
	}
}

void ARazorKiller::SubTrackingPoint()
{
	// ここそのうち可変にする
	AIRazorKiller->SubtractionTrackingPoint(1);
	GetWorldTimerManager().SetTimer(TrackHandle, this, &ARazorKiller::SubTrackingPoint, TrackTime, false);
}

/// <summary>
/// プレイヤーを見失ったときに呼ばれるメソッド
/// </summary>
void ARazorKiller::LostSeePlayer() const
{
	if (AIRazorKiller)
	{
		AIRazorKiller->SetIsSeePlayerKey(false);
	}

	DebugPrintString("Lost", FColor::Red, 2.f);
}

// 抵抗はじめ※後で直す
void ARazorKiller::Resisted()
{
	AIRazorKiller->BehaviorComp->StopTree();
	// アニメーションの管理をしなさーい！
}

// 抵抗終わり※後で直す
void ARazorKiller::CompResisted()
{
	AIRazorKiller->ReStartEventForTimer(5.f);
	// タイマー入れて再起動でいいんじゃないスカね
}

// 抵抗失敗※そのうちアニメーション処理いれる
void ARazorKiller::FailedResisted()
{
	
}

/// <summary>
/// デバッグプリントの管理
/// </summary>
/// <param name="Log">ログの内容</param>
/// <param name="Color">色</param>
/// <param name="PrintTime">表示時間</param>
void ARazorKiller::DebugPrintString(const FString& Log, const FColor Color, const float PrintTime) const
{
	if (!bShowLog)
	{
		return;
	}
	UKismetSystemLibrary::PrintString(this, Log, true, true, Color, PrintTime);
}

/// <summary>
/// デバッグログの管理
/// </summary>
/// <param name="Log">ログの内容</param>
void ARazorKiller::DebugLog(FString const& Log) const
{
	if (!bShowLog)
	{
		return;
	}
	UE_LOG(LogTemp, Error, TEXT("%s"), *Log);
}

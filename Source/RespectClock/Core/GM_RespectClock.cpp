//--------------------------------------------------------------------------------------
// ファイル名：GM_RespectClock.cpp
// 内容		：ゲームのルールを制御する
// 作成者	：22cu0127 丹代天
// 更新日	：2024/04/23		作成、関数宣言
//			：2024/04/30		ビューポートの追加、削除処理、レベルのリスタート処理(後に決められた地点へのリスポーン処理に変更)、リスポーン地点設定処理を記載
//--------------------------------------------------------------------------------------

#include "GM_RespectClock.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "SoundPlayer.h"
#include "LevelSequenceActor.h"
#include "LevelSequencePlayer.h"
#include "RespectClock/Interactive/Door.h"
#include "EventPlayer.h"
#include "MovieSceneSequencePlayer.h"
#include "RespectClock/Characters/Player/PlayerCharacter.h"
#include "RespectClock/Characters/RazorKiller/AIC_RazorKiller.h"

//コンストラクタ
AGM_RespectClock::AGM_RespectClock()
	: m_RespawnPos(FVector::ZeroVector)
	, m_pGameCOWidget(nullptr)
{

}

//開始時処理
void AGM_RespectClock::BeginPlay()
{
	Super::BeginPlay();

	HandleLevelReload();

	UEventPlayer* EP = UEventPlayer::GetInstance(GetWorld());

	if (EP) EP->SetKillerController(GetWorld());

	//スタート時点での座標を取得
}

//Viewport追加
UUserWidget* AGM_RespectClock::View(UUserWidget* _Widget)
{
	//変数に入れる
	if (m_pGameCOWidget == nullptr)
	{
		m_pGameCOWidget = _Widget;
	}
	else
	{
		m_pGameCOWidget = nullptr;
		m_pGameCOWidget = _Widget;
	}

	if (m_pGameCOWidget)
	{
		// Viewportに追加する
		m_pGameCOWidget->AddToViewport(0);
	}

	return m_pGameCOWidget;
}

void AGM_RespectClock::Respawn()
{

	//レベルを再ロード(Prototype的実装)
	//UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), true, "?game=RespectClockInstance");

	//リスポーン地点へ移動(ある程度のstatusをリセットするかも？)
	if (Player == nullptr)	Player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (Player)
	{
		Player->Respawn(m_RespawnPos);
		UE_LOG(LogTemp, Warning, TEXT("ttt"));
	}

}

//Viewport削除
void AGM_RespectClock::Remove()
{
	if (m_pGameCOWidget)
	{
		//ViewPortから削除
		m_pGameCOWidget->RemoveFromParent();
		//変数を初期化
		m_pGameCOWidget = nullptr;
	}
}

//再スタート地点を設定する関数
void AGM_RespectClock::SetRespawnPos(FVector _respawnPos)
{
	m_RespawnPos = _respawnPos;
}

void AGM_RespectClock::StartTrack()
{
	TrackingStart.Broadcast();
}

void AGM_RespectClock::EndTrack()
{
	TrackingEnd.Broadcast();
}

//サウンド再生イベント
void AGM_RespectClock::CallSoundEvent(int32 _id, FString _name, FVector _location, float _volume, float _pitch)
{
	USoundPlayer* SP = USoundPlayer::GetInstance(GetWorld());

	if (!SP) return;

	if (_id != 0)
	{
		SP->PlaySoundByID(GetWorld(), _id, _location, _volume, _pitch);
	}
	else if (_name != TEXT(""))
	{
		SP->PlaySoundByName(GetWorld(), _name, _location, _volume, _pitch);
	}
}

//キラー出現イベント
void AGM_RespectClock::CallKillerEvent()
{
	KillerEvent.Broadcast();
}

void AGM_RespectClock::CallSequenceEvent(ULevelSequence* Sequence)
{
	ALevelSequenceActor* SequenceActor = GetWorld()->SpawnActor<ALevelSequenceActor>(ALevelSequenceActor::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
	if (SequenceActor)
	{
		// シーケンスアクターにシーケンスを設定
		SequenceActor->SetSequence(Sequence);

		// シーケンスプレイヤーを取得
		ULevelSequencePlayer* SequencePlayer = SequenceActor->GetSequencePlayer();
		if (SequencePlayer)
		{
			// シーケンスプレイヤーを再生
			SequencePlayer->Play();
		}
	}
}

//部屋通知イベント
void AGM_RespectClock::CallEntryRoom(FString RoomName, TSoftObjectPtr<ADoor> DoorObj)
{
	EntryRoom.Broadcast(RoomName, DoorObj);
}

void AGM_RespectClock::CallProgreesUpdate(FString ProgressText)
{
	if (Player == nullptr)	Player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (Player)
	{
		Player->ProgressStateText = ProgressText;
		Player->IsDisplayed = false;
	}
}

// 進行状況更新

void AGM_RespectClock::HandleLevelReload()
{
	UE_LOG(LogTemp, Warning, TEXT("HandleLevelReload called"));

	if (GetWorld())
	{
		UE_LOG(LogTemp, Warning, TEXT("GetWorld() is valid"));

		USoundPlayer* SoundPlayer = USoundPlayer::GetInstance(GetWorld());
		if (SoundPlayer)
		{
			UE_LOG(LogTemp, Warning, TEXT("Resetting BGMComponent"));
			SoundPlayer->ResetBGMComponent(GetWorld());
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to get SoundPlayer instance"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("GetWorld() is nullptr"));
	}
}
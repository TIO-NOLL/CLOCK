//--------------------------------------------------------------------------------------
// ファイル名：GM_RespectClock.h
// 内容		：ゲームのルールを制御する
// 作成者	：22cu0127 丹代天
// 更新日	：2024/04/23		作成
//			：2024/04/30		ビューポートの追加、削除処理、レベルのリスタート処理(後に決められた地点へのリスポーン処理に変更)、リスポーン地点設定処理を記載
//--------------------------------------------------------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Sound/SoundBase.h"
#include "LevelSequence.h"
#include "Delegates/DelegateCombinations.h"
#include "GM_RespectClock.generated.h"

//前方宣言
class UUserWidget;
class ULevelSequence;
class ADoor;
class APlayerCharacter;

UCLASS()
class RESPECTCLOCK_API AGM_RespectClock : public AGameModeBase
{
	GENERATED_BODY()

protected:
	//コンストラクタ
	AGM_RespectClock();
	//開始時の処理
	void BeginPlay()override;

private:
	//Viewport追加
	UFUNCTION(BlueprintCallable, Category = "RespectClock")
	UUserWidget* View(UUserWidget* _widget);
	//ViewPort削除
	UFUNCTION(BlueprintCallable, Category = "RespectClock")
	void Remove();
	//リスポーン
	UFUNCTION(BlueprintCallable, Category = "RespectClock")
	void Respawn();

	void HandleLevelReload();

private:
	//リスポーン座標
	UPROPERTY()
	FVector m_RespawnPos;
	UPROPERTY()
	UUserWidget* m_pGameCOWidget;
	UPROPERTY()
	APlayerCharacter* Player;

public:
	void SetRespawnPos(FVector _respawnPos);

// イベントディスパッチャー
protected:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGameRuleDispatcer);

//ゲームクリア
public:
	UPROPERTY(BlueprintAssignable, Category = "Custom Events")
	FGameRuleDispatcer GameClearDispatcer;

private:
//チェイスの開始・終了
	UPROPERTY(BlueprintAssignable, Category = "Custom Events")
	FGameRuleDispatcer TrackingStart;
	UPROPERTY(BlueprintAssignable, Category = "Custom Events")
	FGameRuleDispatcer TrackingEnd;

	//イベント関係ディスパッチャー
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FKillerEvent);
	UPROPERTY(BlueprintAssignable, Category = "KillerEvent")
	FKillerEvent KillerEvent;

public:
	UFUNCTION(BlueprintCallable, Category = "Call Event")
	void StartTrack();
	UFUNCTION(BlueprintCallable, Category = "Call Event")
	void EndTrack();

	//サウンドイベントを呼ぶ関数
	void CallSoundEvent(int32 _id, FString Name, FVector _location = FVector::ZeroVector, float _volume = 1.f, float _pitch = 1.f);
	//キラー出現イベントを呼ぶ関数
	void CallKillerEvent();
	//エフェクト再生イベントを呼ぶ関数

	//シーケンサ再生イベントを呼ぶ関数
	void CallSequenceEvent(ULevelSequence* Sequence);
	//部屋に入ったかどうかの通知
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FEntryRoom, FString, RoomName, TSoftObjectPtr<ADoor>, DoorObject);

	UPROPERTY(BlueprintAssignable, Category = "KillerEvent")
	FEntryRoom EntryRoom;
	void CallEntryRoom(FString RoomName, TSoftObjectPtr<ADoor> DoorObj);

	void CallProgreesUpdate(FString ProgressText);
};
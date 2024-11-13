/*
* FileName	: AIC_RazorKiller.h
* Created	: Misora Tanaka
* Summary	: キラーの頭脳を制御する
* Update	: 24/04/08 作成
*			: 24/06/18 全体のリファクタリング、再設計を開始
*			: 24/06/20 最も近いアクタの検索機能をタグ経由の検索に修正(クラスで検索したかった)
*			: 24/06/25 不要なメソッドを削除
*			: 24/07/04 タイマースタートを追加(抵抗処理の後に動く)
*			: 24/07/05 Search...メソッドのバグ修正
*			: 24/07/06 不要なメソッド、プロパティの削除(cppにコメント書く)
*/

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AIC_RazorKiller.generated.h"	// ここより下にインクルード書かない

// 前方宣言
class UBehaviorTree;
class UBehaviorTreeComponent;
class UBlackboardComponent;
class APlayerCharacter;
class ARazorKiller;
class AWarpPoint;
class ATriggerBox;
class ADoor;

UCLASS()
class RESPECTCLOCK_API AAIC_RazorKiller : public AAIController
{
	GENERATED_BODY()

public:
	AAIC_RazorKiller(const class FObjectInitializer& ObjectInitializer);

protected:
	virtual void BeginPlay() override;

public:
	// BlackBoardにKeyを設定するやつら
	void SetTrackingPointKey(int Num) const;	// 使ってる
	void SetWarpPointIDKey(int Num) const;		// 使ってる(いる？)
	UFUNCTION(BlueprintCallable, Category = "AI")
	void AddWarpPointIDKey() const; // 使ってるらしいけど今後いるかは知らん
	void SetIsSeePlayerKey(bool bValue) const;	// 使ってる

public:
	// 動かすビヘイビアツリー
	UPROPERTY()
	TObjectPtr<UBehaviorTreeComponent> BehaviorComp;
	// 所有するブラックボード
	UPROPERTY()
	TObjectPtr<UBlackboardComponent> BlackboardComp;

	// BlackBoardキーの設定FNameで変えられるようにしたいね
	// 中身確定してからやろうね

protected:
	// AIControllerのPawn所持
	virtual void OnPossess(class APawn* InPawn) override;

protected:

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;

	// いらないかも
	FORCEINLINE UBehaviorTreeComponent* GetBehaviorComp() const { return BehaviorComp; }
	FORCEINLINE UBlackboardComponent* GetBlackboardComp() const { return BlackboardComp; }

public:
	// BPでワープポイント拾ってくるやつ(仮)
	TArray<TObjectPtr<AWarpPoint>> WarpPoint;
	UFUNCTION(BlueprintCallable, Category = "Trash")
	void SetWarpPoints(const TArray<AWarpPoint*> Point) { WarpPoint = Point; };
	UFUNCTION(BlueprintPure, Category = "Trash")
	TArray<AWarpPoint*> GetWarpPoints() { return WarpPoint; };

	// ===============================================ここより下に新しく必要になるモノを書く===============================================

public:

	// イベントが発生した時に呼ばれる関数
	UFUNCTION(BlueprintCallable, Category = "RC_AI")
	void SetEvent(float MoveStartTime);
	// 初期化なしで再起動する
	void StartEvent() const;
	// 抵抗終了後に呼ばれるメソッド
	UFUNCTION(BlueprintCallable, Category = "RC_AI")
	void ReStartEventForTimer(float Timer);

	// イベント管理マンから通知を受け取る
	UFUNCTION(BlueprintCallable, Category = "RC_AI")
	void StopEvent() const;
	// 最も近いオブジェクトをタグから探す
	TObjectPtr<AActor> SearchForNearestObjectWithTag(const FString& Tag) const;
	// 追跡度の減算処理
	void SubtractionTrackingPoint(const int Num) const;

	//-----2024/09/03 22cu0127 丹代天 記述：開始時にマップ全体のワープポイントを確保する配列、プレイヤーの近くのワープポイントに飛ぶ処理-----
	TArray<TObjectPtr<AWarpPoint>> m_AWarpPoints;
		void SetLocationNearestWarpPoint();
	//------------------------------------------------------------------------------------------------------------------------------
		void SetLocationClosestToPlayerWarpPoint();
	//---------------------------------------------------------------------------------------------------------------------------------------

	//-----2024/09/08 22cu0127 丹代天 記述：プレイヤーが部屋に入った際に部屋にワープする関数-----

	FTimerHandle WarpTimerHandle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RC_AI")
	float DelayTime = 8.0f; // 例えば5秒待機

	// ワープ処理の内部ロジックを関数化
	void HandleWarp(FString DoorName, TSoftObjectPtr<ADoor> DoorObj);

	UFUNCTION(BlueprintCallable, Category = "RC_AI")
	float WarpRoom(FString RoomName, TSoftObjectPtr<ADoor> DoorObj);
	//-------------------------------------------------------------------------------------------
	//-----2024/09/08 22cu0127 丹代天 記述：散策するための基準となるアクターを、部屋名をキーとして格納するMap,それをブラックボードに入力されてる部屋名をもとにして返す関数-----
	void SetStrollingActors(TArray<AActor*> StrollingActors);
	TArray<AActor*> GetStrollingActors();
	//-------------------------------------------------------------------------------------------------------------------------------------------------------------
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FItemDis);
	UPROPERTY(BlueprintAssignable, Category = "Custom Events")
	FItemDis ItemDispatcer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RC_AI")
	bool bOnce = false;
	UFUNCTION(BlueprintCallable, Category = "RC_AI")
	void SetbOnce(bool b) { bOnce = b; }

private:
	// ※カテゴリはリファクタリング後に直すよ
	UPROPERTY()
	FTimerHandle StartHandle;
	// リスタートまでのタイマー
	UPROPERTY()
	FTimerHandle ReStartHandle;
	// 待機位置を設定する
	UPROPERTY(VisibleAnywhere, Category = "RC_AI")
	TObjectPtr<AWarpPoint> WaitingPoint;
	// これは本体のポインタ
	UPROPERTY(VisibleAnywhere, Category = "RC_AI")
	TObjectPtr<ARazorKiller> RazorKiller;
	// これワープポイントのIDだった気がする(今のところ未使用、そのうち使うかも)
	UPROPERTY()
	int PlacementPointID;

	//-----2024/09/06 22cu0127 丹代天 記述：開始時にマップ全体のドアのポインタと所属するレベルインスタンス名をキーとするMap配列-----
	TMap<FString, TArray<TObjectPtr<ADoor>>> DoorMap;
	//----------------------------------------------------------------------------------------------------------------------

	TArray<AActor*> StrollingActorArray;

	bool WaitingWarp = false;
	bool GetWaitingflag() { return WaitingWarp; }
};

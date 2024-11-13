/*
* FileName	: RazorKiller.h
* Created	: Misora Tanaka
* Summary	: キラー自体の制御を行う
* Update	: 24/04/08 作成
*			: 24/06/18 コメント追記
*			: 24/07/04 抵抗処理をプレイヤーとやりとりする為のResistedInterfaceを追加
*			: 24/07/05 抵抗が動くように各所を調整
*			:	       追加でコメントなどを整理
*/

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ResistedInterface.h"
#include "RazorKiller.generated.h"

// 前方宣言
class UPawnSensingComponent;
class AAIC_RazorKiller;
class UBehaviorTree;

UCLASS()
class RESPECTCLOCK_API ARazorKiller : public ACharacter, public IResistedInterface
{
	GENERATED_BODY()

// == メソッド ==
public:
	// コンストラクタ
	ARazorKiller();

protected:
	// 生成時か開始に動くメソッド
	virtual void BeginPlay() override;

public:
	//抵抗開始(ResistedInterface)
	UFUNCTION()
	virtual void Resisted()override;
	//抵抗成功(ResistedInterface)
	UFUNCTION()
	virtual void CompResisted() override;
	//抵抗失敗(ResistedInterface)
	UFUNCTION()
	virtual void FailedResisted() override;

private:
	// SensingComponentで見たときに動くメソッド
	UFUNCTION(BlueprintCallable , Category = "Player")
	void OnSeePlayer(APawn* Pawn);
	// 見失ったときに動くメソッド
	UFUNCTION(BlueprintCallable , Category = "Player")
	void LostSeePlayer() const;
	// 追跡度の減少する再帰関数
	void SubTrackingPoint();
	UFUNCTION()
	void RestartBehaviorTree();

	
// == プロパティ ==
private:
	// 目線
	UPROPERTY(VisibleAnywhere, Category = "AI")
	TObjectPtr<UPawnSensingComponent> SensingComp;
	// 自らを制御するAIController
	UPROPERTY()
	TObjectPtr<AAIC_RazorKiller> AIRazorKiller;
	// 視野
	UPROPERTY(EditAnywhere, Category = "AI")
	float VisionAngle;
	// チェイス時に使うタイマー
	UPROPERTY()
	FTimerHandle ChaseHandle;
	// チェイスでプレイヤーを見失うまでの時間
	UPROPERTY(EditAnywhere, Category = "RC_AI")
	float ChaseTime;
	// 追跡度減少に使うタイマー
	UPROPERTY()
	FTimerHandle TrackHandle;
	// 追跡度を下げるタイミング(何秒に1回なのか)
	UPROPERTY(EditAnywhere, Category = "RC_AI")
	float TrackTime;
	
// == Debug用 ==
private:
	// プレビューに表示
	UFUNCTION()                                                                   
	void DebugPrintString(const FString& Log, FColor Color,float PrintTime) const;
	// コンソールに表示
	UFUNCTION()                                                                   
	void DebugLog(const FString& Log) const;
	// ログを表示する
	UPROPERTY(EditAnywhere, Category = "Debug")
	bool bShowLog;

protected:
	FTimerHandle RestartBehaviorTreeHandle;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RC_AI")
	UBehaviorTree* BehaviorTree;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RC_AI")
	float RestartDelay = 6.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RC_AI")
	bool bLook;
};
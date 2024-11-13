//--------------------------------------------------------------------------------------
// ファイル名：BTT_Strolling.h
// 内容		：追跡中、プレイヤーが隠れていた際にその部屋を探索するタスク
// 作成者	：22cu0127 丹代天
// 更新日	：2024/09/12		作成
//--------------------------------------------------------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "NavigationSystem.h"
#include "BTT_Strolling.generated.h"

//前方宣言
class AAIC_RazorKiller;
class APlayerCharacter;

UCLASS()
class RESPECTCLOCK_API UBTT_Strolling : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	//コンストラクタ
	UBTT_Strolling();

public:
	//探索するためのActor配列
	TArray<AActor*> m_StrollingActors;
	
	//移動速度
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RazorKiller")
	float m_MoveSpeed;
	//回転速度
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RazorKiller")
	float m_InterpolationSpeed;
	//到達したとみなす距離（目標Actorからの半径）
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RazorKiller")
	float m_Radias;
	//停止する時間
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RazorKiller")
	// メンバ変数として停止時間と現在の経過時間を宣言
	float PauseDuration = 3.0f; // 停止時間 (秒)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RazorKiller")
	float ElapsedPauseTime = 0.0f;

private:
	//タスクが実行されたときに実行される距離
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& _ownerComp, uint8* _nodeMemory) override;
	//移動処理
	void UpdateMove(UBehaviorTreeComponent& _ownerComp, float _deltaTime);
	//回転処理
	void UpdateRotate(const FVector _direction, const APawn* _pOwner, float _deltaTime);
	//移動の計算
	FVector CalcMovement(const APawn* _pOwner, float _deltaTime, FVector _targetpos);

	void PauseTask(UBehaviorTreeComponent& _ownerComp);

	void ResumeTask(UBehaviorTreeComponent& _ownerComp);

	void MoveToDoorLocation(UBehaviorTreeComponent& _ownerComp, float _deltaSeconds);

	void ResetRotation(APawn* _pOwner);

private:
	//キラーのAIControlleを格納する変数
	AAIC_RazorKiller* KillerController;
	//Controllerが取得しているPawnを格納する変数
	APawn* pKillerOwner;
	//配列の要素数となる変数
	int StrollingElem = 0;

	bool bIsPaused;
	bool bIsPausedBeforeDoor;
	bool bIsMovingToDoor;
	
	FRotator InitialMeshRotation; // メッシュの初期回転を保存する変数

	APlayerCharacter* PlayerCharacter;

protected:
	//フレームごとの処理
	virtual void TickTask(UBehaviorTreeComponent& _ownerComp, uint8* _nodeMemory, float _deltaSeconds) override;

protected:
	//ナビメッシュを格納する変数
	UNavigationSystemV1* navSys;

};

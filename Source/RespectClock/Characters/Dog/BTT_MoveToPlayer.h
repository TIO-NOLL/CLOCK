//--------------------------------------------------------------------------------------
// ファイル名：BTT_MoveToPlayer.h
// 内容		：犬の動きを制御するAIコントローラー
// 作成者	：22cu0127 丹代天
// 更新日	：2024/04/16		作成、プレイヤーへ近づく処理記述
//--------------------------------------------------------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "NavigationSystem.h"
#include "BTT_MoveToPlayer.generated.h"

UCLASS()
class RESPECTCLOCK_API UBTT_MoveToPlayer : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	//コンストラクタ
	UBTT_MoveToPlayer();

private:
	//タスク開始時に呼ばれる処理
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& _ownerComp, uint8* _nodeMemory) override;

private:
	//移動処理
	void UpdateMove(UBehaviorTreeComponent& _OwnerComp, float _deltaTime);
	//回転処理
	void UpdateRotate(const FVector _direction, const APawn* _pOwner, float _deltaTime);
	
	void UpdateDistancefromPlayer(UBehaviorTreeComponent& _ownerComp);
protected:
	//フレームごとに呼ばれる処理
	virtual void TickTask(UBehaviorTreeComponent& _ownerComp, uint8* _nodeMemory, float _deltaSeconds) override;
	//移動地点への計算処理
	virtual FVector CalcMovement(const APawn* _pOwner, float _deltaTime);

protected:
	//ナビゲーションのパスを格納する変数
	UNavigationSystemV1* navSys;

	//プレイヤーに近づく距離(半径)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.0", UIMin = "0.0"))
	float m_acceptableRadius;
	//移動速度
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.0", UIMin = "0.0"))
	float m_MoveSpeed;

	//PlayerActorを格納する変数
	APawn* pTarget;

	UPROPERTY(EditAnywhere, Category = "Move")
	float m_InterpolationSpeed;
};

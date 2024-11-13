// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "NavigationSystem.h"
#include "BTT_GoStraight.generated.h"

/**
 * 
 */
UCLASS()
class RESPECTCLOCK_API UBTT_GoStraight : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	//コンストラクタ
	UBTT_GoStraight();

private:
	APawn* m_pPlayerActor;

	FVector m_TargetPoint;

	//仮置き
	UPROPERTY(EditAnywhere)
	float m_TargetDistance;

	UPROPERTY(EditAnywhere)
	float m_MoveSpeed;

	UPROPERTY(EditAnywhere)
	float m_InterpolationSpeed;

	UPROPERTY(EditAnywhere)
	float m_Radias;

private:
	//タスクが実行された時に実行される処理
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& _ownerComp, uint8* _nodeMemory) override;

	void UpdateMove(UBehaviorTreeComponent& _ownerComp, float _deltaTime);

	void UpdateRotate(const FVector _direction, const APawn* _pOwner, float _deltaTime);

	FVector CalcMovement(const APawn* _pOwner, float _deltaTime);

protected:
	virtual void TickTask(UBehaviorTreeComponent& _ownerComp, uint8* _nodeMemory, float _deltaSeconds) override;

protected:
	UNavigationSystemV1* navSys;
};

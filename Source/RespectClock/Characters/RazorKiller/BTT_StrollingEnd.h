// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTT_StrollingEnd.generated.h"

//前方宣言
class UBlackboardComponent;
class AAIC_RazorKiller;

UCLASS()
class RESPECTCLOCK_API UBTT_StrollingEnd : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	//コンストラクタ
	UBTT_StrollingEnd();

private:
	//ブラックボードコンポーネント
	UBlackboardComponent* pBlackboardComp;

private:
	//タスクが実行された時に実行される処理
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& _ownerComp, uint8* _nodeMemory) override;
};
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTT_SetWarpPoint.generated.h"

/**
 * 
 */
UCLASS()
class RESPECTCLOCK_API UBTT_SetWarpPoint : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTT_SetWarpPoint();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};

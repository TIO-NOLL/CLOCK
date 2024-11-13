/*
* FileName	: BTTask_Patrol.cpp
* Created	: Misora Tanaka
* Summary	: Patrols patrolling the range
* Update	: 24/04/11 Create
*
*/

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTT_Patrol.generated.h"


UCLASS()
class RESPECTCLOCK_API UBTT_Patrol : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UBTT_Patrol();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};

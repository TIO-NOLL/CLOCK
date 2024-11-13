/*
* FileName	: BTT_Patrol.cpp
* Created	: Misora Tanaka
* Summary	: Patrols patrolling the range
* Update	: 24/04/11 Create
*
*/

#include "BTT_Patrol.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/TriggerBox.h"
#include "AITypes.h"

UBTT_Patrol::UBTT_Patrol()
{
	// Taskノードの名前
	NodeName = "Patrol";
}

/// <summary>
/// パトロールするタスク
/// </summary>
/// <param name="OwnerComp">実行しているキャラのPawn</param>
/// <param name="NodeMemory"></param>
/// <returns>タスクが完了したか</returns>
EBTNodeResult::Type UBTT_Patrol::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp,NodeMemory);
	// 何これキモすぎ後で直す
	TObjectPtr<AActor> const Owner = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(GetSelectedBlackboardKey()));
	FVector const OwnerLocation = Owner->GetActorLocation();
	TObjectPtr<UBlackboardComponent> const OwnerBB = OwnerComp.GetBlackboardComponent();

	FVector const PatrolLocation = OwnerBB->GetValueAsVector("PatrolPos");

	OwnerComp.GetAIOwner()->MoveToLocation(FVector(PatrolLocation.X, PatrolLocation.Y, OwnerLocation.Z), 0.0f, true, true, true, true, NULL, true);

	return EBTNodeResult::Succeeded;
}
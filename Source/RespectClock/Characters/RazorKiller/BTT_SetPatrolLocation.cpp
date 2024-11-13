// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_SetPatrolLocation.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/TriggerBox.h"
#include "AITypes.h"

UBTT_SetPatrolLocation::UBTT_SetPatrolLocation()
{
	// Taskの名前を設定
	NodeName = "SetPatrolLocation";
}

/// <summary>
/// パトロールの位置を設定するタスク
/// </summary>
/// <param name="OwnerComp"></param>
/// <param name="NodeMemory"></param>
/// <returns></returns>
EBTNodeResult::Type UBTT_SetPatrolLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	FVector OriginLocation;
	FVector BoxExtent;
	TObjectPtr<AActor> const MoveBox = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("MoveArea"));
	UKismetSystemLibrary::GetActorBounds(MoveBox, OriginLocation, BoxExtent);

	FVector const RandomLocation = UKismetMathLibrary::RandomPointInBoundingBox(OriginLocation, BoxExtent);

	const TObjectPtr<UBlackboardComponent> OwnerBB = OwnerComp.GetBlackboardComponent();

	OwnerBB->SetValueAsVector("PatrolPos", RandomLocation);
	OwnerBB->SetValueAsBool("IsCompPatrol", false);

	return EBTNodeResult::Succeeded;
}
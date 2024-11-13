// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_WarpTo.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTT_WarpTo::UBTT_WarpTo()
{
	NodeName = "Warp";

	// 選択するブラックボードキーにフィルターをかける
	BlackboardKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTT_WarpTo, BlackboardKey), AActor::StaticClass());
}

EBTNodeResult::Type UBTT_WarpTo::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	TObjectPtr<UBlackboardComponent> const OwnerBB = OwnerComp.GetBlackboardComponent();
	TObjectPtr<AActor> const WarpPoint = Cast<AActor>(OwnerBB->GetValueAsObject(GetSelectedBlackboardKey()));
	if(WarpPoint != nullptr)
	{
		FVector const WarpLocation = WarpPoint->GetActorLocation();
		TObjectPtr<AActor> const Owner = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("SelfActor"));
		OwnerBB->SetValueAsBool("IsInit",true);
		Owner->SetActorLocation(WarpLocation);
		return EBTNodeResult::Succeeded;
	}
	return  EBTNodeResult::Failed;
}



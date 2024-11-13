// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_SetWarpPoint.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "RespectClock/Core/WarpPoint.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

UBTT_SetWarpPoint::UBTT_SetWarpPoint()
{
	// タスク名を設定
	NodeName = "SetWarpPoint";
}

EBTNodeResult::Type UBTT_SetWarpPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	TSubclassOf<AWarpPoint> findClass;
	findClass = AWarpPoint::StaticClass();
	TArray<TObjectPtr<AActor>> WarpPoints;
	TObjectPtr<UBlackboardComponent> const OwnerBB = OwnerComp.GetBlackboardComponent();
	FName Tag = OwnerBB->GetValueAsName(GetSelectedBlackboardKey());
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), findClass, WarpPoints);

	FVector OwnerPos = OwnerComp.GetOwner()->GetActorLocation();
	float Difference = 0.f;
	float PreviousDifference = 10000000.f;
	int NearestNum = 0;
	for (int wi = 0; wi < WarpPoints.Num(); wi++)
	{
		FVector NowLocation = WarpPoints[wi]->GetActorLocation();
		Difference += UKismetMathLibrary::Abs(OwnerPos.X - NowLocation.X) + UKismetMathLibrary::Abs(OwnerPos.Y - NowLocation.Y) + UKismetMathLibrary::Abs(OwnerPos.Z - NowLocation.Z);
		if (Difference < PreviousDifference && WarpPoints[wi]->ActorHasTag(Tag))
		{
			PreviousDifference = Difference;
			NearestNum = wi;
		}
	}
	if (WarpPoints.IsValidIndex(0))
	{
		OwnerBB->SetValueAsObject("PlacementPoint", WarpPoints[NearestNum]);
		return EBTNodeResult::Succeeded;
	}
	else
	{
		return EBTNodeResult::Failed;
	}
}

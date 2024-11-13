// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_StrollingEnd.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "AIC_RazorKiller.h"
#include "RespectClock/Core/SoundPlayer.h"

UBTT_StrollingEnd::UBTT_StrollingEnd()
{
	NodeName = TEXT("散策終了");

	bNotifyTick = false;
}

EBTNodeResult::Type UBTT_StrollingEnd::ExecuteTask(UBehaviorTreeComponent& _ownerComp, uint8* _nodeMemory)
{
	// サウンドプレイヤーを取得
	USoundPlayer* SP = USoundPlayer::GetInstance(GetWorld());
	SP->StopBGM();

	// AIControllerとControlledPawnを取得
	AAIController* AIController = _ownerComp.GetAIOwner();
	APawn* ControlledPawn = AIController->GetPawn();

	// サウンド再生
	SP->PlaySoundByID(GetWorld(), 2, ControlledPawn->GetActorLocation(), 1.f, 1.f);

	// タスクの終了処理を行い、Succeededでタスクを終了
	FinishLatentTask(_ownerComp, EBTNodeResult::Succeeded);

	// タスク終了後にビヘイビアツリーを停止する
	AAIC_RazorKiller* KillerController = Cast<AAIC_RazorKiller>(AIController);
	if (KillerController)
	{
		KillerController->StopEvent();  // ビヘイビアツリー停止処理
	}

	return EBTNodeResult::Succeeded;
}
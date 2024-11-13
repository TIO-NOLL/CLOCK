// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_GoStraight.h"
#include "NavigationPath.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "AIC_Dog.h"
#include "Math/UnrealMathUtility.h"
#include "DrawDebugHelpers.h"

UBTT_GoStraight::UBTT_GoStraight()
	: m_pPlayerActor(nullptr)
	, m_TargetPoint(FVector::ZeroVector)
{
	NodeName = TEXT("IsStroke");

	bNotifyTick = true;
	navSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
}

EBTNodeResult::Type UBTT_GoStraight::ExecuteTask(UBehaviorTreeComponent& _ownerComp, uint8* _nodeMemory)
{
	UBlackboardComponent* pBlackboard = _ownerComp.GetBlackboardComponent();
	m_pPlayerActor = Cast<APawn>(pBlackboard->GetValueAsObject(TEXT("PlayerActor")));

	FVector PlayerForward = m_pPlayerActor->GetActorForwardVector();

	// AIコントローラー取得
	AAIController* pController = _ownerComp.GetAIOwner();
	//コントローラーが保持している、Pawnを取得
	APawn* pOwner = pController->GetPawn();

	m_TargetPoint = m_pPlayerActor->GetActorLocation() + PlayerForward * m_TargetDistance;

	// Tickで移動処理するのでInProgressを返す
	return EBTNodeResult::InProgress;
}

void UBTT_GoStraight::TickTask(UBehaviorTreeComponent& _ownerComp, uint8* _nodeMemory, float _deltaSeconds)
{
	UpdateMove(_ownerComp, _deltaSeconds);
}

//移動処理
void UBTT_GoStraight::UpdateMove(UBehaviorTreeComponent& _ownerComp, float _deltaTime)
{
	//AIコントローラー取得
	AAIController* pController = _ownerComp.GetAIOwner();
	//コントローラーが保持している、Pawnを取得
	APawn* pOwner = pController->GetPawn();

	//目標までの距離を格納する変数
	FVector distance;

	// ターゲットに到達しているのかの判定
	//Pawnとコントローラーが存在している時
	if (m_pPlayerActor)
	{
		m_TargetPoint.Z = pOwner->GetActorLocation().Z;
		//distanceにターゲットまでの距離を格納する
		distance = pOwner->GetActorLocation() - m_TargetPoint;

		////レイを飛ばす
	    ////始点 Start：Actor
		//FVector Start = m_pPlayerActor->GetActorLocation();
		////終点 End：Startから前方向m_ActionForwardLengthの距離
		//FVector End = m_TargetPoint;

		////デバッグ用にラインを描画
		//DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 1.f);
	}

	if (distance.SquaredLength() > m_Radias * m_Radias)
	{
		//1フレーム間の移動距離を計算
		FVector movement = CalcMovement(pOwner, _deltaTime);

		//移動距離がゼロでなかった場合
		if (!movement.IsZero())
		{
			//移動
			pOwner->AddMovementInput(movement, m_MoveSpeed);
		}
		else
		{
			//Enumの値を変更
			Cast<AAIC_Dog>(pController)->SwitchDogState(EDogState::Tracking);
			// タスクを終了させる
			FinishLatentTask(_ownerComp, EBTNodeResult::Succeeded);
		}
	}
	else
	{
		//Enumの値を変更
		Cast<AAIC_Dog>(pController)->SwitchDogState(EDogState::Tracking);
		// タスクを終了させる
		FinishLatentTask(_ownerComp, EBTNodeResult::Succeeded);
	}

	return;
}

//移動経路の計算
FVector UBTT_GoStraight::CalcMovement(const APawn* _pOwner, float _deltaTime)
{
	// NavigationMeshを用いて移動経路を探索
	UNavigationPath* pPath = navSys->FindPathToLocationSynchronously(GetWorld(), _pOwner->GetActorLocation(), m_TargetPoint);

	//パスが見つからなかった場合、処理を返す
	if (!pPath) return FVector::Zero();

	//Pathpointの値を配列に格納する	
	TArray<FVector> pathPoints = pPath->PathPoints;

	if (pathPoints.Num() >= 2)
	{
		// 自身の座標から初めの地点への方向を返す
		FVector dir = pathPoints[1] - pathPoints[0];
		dir.Normalize();

		//デバッグ用
		//pPathがNULLでない場合
		if (pPath)
		{
			for (int pointiter = 0; pointiter < pPath->PathPoints.Num(); pointiter++)
			{
				//パスポイントに球を描画する
				DrawDebugSphere(GetWorld(), pPath->PathPoints[pointiter], 10.0f, 12, FColor(255, 0, 0));
			}
		}

		//移動方向をもとにMeshを回転させる
		UpdateRotate(dir, _pOwner, _deltaTime);

		return dir;
	}
	else
	{
		return FVector::Zero();
	}
}

//Meshの回転処理
void UBTT_GoStraight::UpdateRotate(const FVector _direction, const APawn* _pOwner, float _deltaTime)
{
	//プレイヤーのメッシュ取得
	USkeletalMeshComponent* pMeshComp = Cast<ACharacter>(_pOwner)->GetMesh();
	//角度はDegreeの角度の範囲で表記(-180°～180°)
	if (pMeshComp != NULL)
	{
		//現在の回転を保持
		FRotator CurrentRotation = pMeshComp->GetRelativeRotation();

		//アークタンジェントを使って進行方向の角度を求める
		float angle = atan2(-_direction.X, _direction.Y);
		//Radian値をDegreeに変換
		float angleDeg = FMath::RadiansToDegrees(angle);

		//新しい回転を保持
		FRotator NewRotation = FRotator(pMeshComp->GetRelativeRotation().Pitch, angleDeg, pMeshComp->GetRelativeRotation().Roll);

		//deltaTimeが0の場合、もしくは現在の回転と新しい回転が同じだった場合、処理を返す
		if (_deltaTime == 0.f || CurrentRotation == NewRotation) { return; }

		//回転速度が0以下だった場合、即時新たな回転に設定する
		if (m_InterpolationSpeed <= 0.f) { pMeshComp->SetRelativeRotation(NewRotation); }

		//回転速度を計算
		float DeltaInterpSpeed = m_InterpolationSpeed * _deltaTime;
		//回転量の最短を求める
		const FRotator DeltaRotate = (NewRotation - CurrentRotation).GetNormalized();

		//フレームごとの回転が0に近かった場合、即時新しい回転を設定する。
		if (DeltaRotate.IsNearlyZero()) { pMeshComp->SetRelativeRotation(NewRotation); }

		//一フレームの回転量（最短の回転量 × 回転速度[0～1]）
		const FRotator DeltaMove = DeltaRotate * FMath::Clamp<float>(DeltaInterpSpeed, 0.f, 1.f);

		//現在の回転に一フレームの回転量を足した物を返す
		pMeshComp->SetRelativeRotation((CurrentRotation + DeltaMove).GetNormalized());
	}
}
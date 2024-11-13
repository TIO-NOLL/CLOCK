//--------------------------------------------------------------------------------------
// ファイル名：BTT_MoveToPlayer.h
// 内容		：犬の動きを制御するAIコントローラー
// 作成者	：22cu0127 丹代天
// 更新日	：2024/04/16		作成、プレイヤーへ近づく処理記述
//--------------------------------------------------------------------------------------

#include "BTT_MoveToPlayer.h"
#include "AIController.h"
#include "NavigationPath.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "VisualLogger/VisualLogger.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

//コンストラクタ
UBTT_MoveToPlayer::UBTT_MoveToPlayer()
	: m_acceptableRadius(200.f)
	, m_MoveSpeed(1.f)
	, m_InterpolationSpeed(5.f)
{
	//ビヘイビアツリーノードの名前を記述
	NodeName = TEXT("MoveToPlayer");
	//Tickを回すかどうか判断
	bNotifyTick = true;
	//WorldContextから、現在使用しているナビゲーションシステムを取得
	navSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());

	//ブラックボードキーにフィルターをかける
	BlackboardKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTT_MoveToPlayer, BlackboardKey), APawn::StaticClass());
}

//タスク開始時に呼ばれる処理
EBTNodeResult::Type UBTT_MoveToPlayer::ExecuteTask(UBehaviorTreeComponent& _ownerComp, uint8* _nodeMemory)
{
	// Targetを取得
	UBlackboardComponent* pBlackboard = _ownerComp.GetBlackboardComponent();
	pTarget = Cast<APawn>(pBlackboard->GetValueAsObject(TEXT("PlayerActor")));

	// Tickで移動処理するのでInProgressを返す
	return EBTNodeResult::InProgress;
}

//フレームごとに呼ばれる処理
void UBTT_MoveToPlayer::TickTask(UBehaviorTreeComponent& _ownerComp, uint8* _nodeMemory, float _deltaSeconds)
{
	//移動処理
	UpdateMove(_ownerComp, _deltaSeconds);
}

//移動処理
void UBTT_MoveToPlayer::UpdateMove(UBehaviorTreeComponent& _ownerComp, float _deltaTime)
{
	//AIコントローラー取得
	AAIController* pController = _ownerComp.GetAIOwner();
	//コントローラーが保持している、Pawnを取得
	APawn* pOwner = pController->GetPawn();

	//目標までの距離を格納する変数
	FVector distance;

	// ターゲットに到達しているのかの判定
	//Pawnとコントローラーが存在している時
	if (pOwner && pTarget)
	{
		//distanceにターゲットまでの距離を格納する
		distance = pOwner->GetActorLocation() - pTarget->GetActorLocation();
	}

	//対象に向かって動く際に、どこまで近づくかを計算する
	//移動距離が半径よりも長かった時
	if (distance.SquaredLength() > m_acceptableRadius * m_acceptableRadius)
	{
		//位置フレーム間の移動距離を計算
		FVector movement = CalcMovement(pOwner, _deltaTime);

		//移動距離がゼロでなかった場合
		if (!movement.IsZero())
		{
			//移動
			pOwner->AddMovementInput(movement);
		}
	}
	//移動距離が近づける距離よりも短かった時
	else
	{
		// タスクを終了させる
		FinishLatentTask(_ownerComp, EBTNodeResult::Succeeded);
	}

	return;
}

//移動経路の計算
FVector UBTT_MoveToPlayer::CalcMovement(const APawn* _pOwner, float _deltaTime)
{
	// NavigationMeshを用いて移動経路を探索
	UNavigationPath* pPath = navSys->FindPathToActorSynchronously(GetWorld(), _pOwner->GetActorLocation(), pTarget);
	//パスが見つからなかった場合、処理を返す
	if (!pPath) { return FVector::Zero(); }

	//Pathpointの値を配列に格納する	
	TArray<FVector> pathPoints = pPath->PathPoints;

	//pathpointが二つ以上の場合(移動する必要がある場合)
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
void UBTT_MoveToPlayer::UpdateRotate(const FVector _direction, const APawn* _pOwner, float _deltaTime)
{
	//プレイヤーのメッシュ取得
	USkeletalMeshComponent* pMeshComp = Cast<ACharacter>(_pOwner)->GetMesh();
	//角度はDegreeの角度の範囲で表記(-180°～180°)
	if (pMeshComp != NULL)
	{
		//現在の回転を保持
		FRotator CurrentRotation = pMeshComp->GetRelativeRotation();

		//アークタンジェントを使ってコントローラーの入力方向がなす角度を求める
		float angle = atan2(-_direction.X, _direction.Y);
		//Radian値をDegreeに変換
		float angleDeg = FMath::RadiansToDegrees(angle);

		/*pMeshComp->SetRelativeRotation(FRotator(pMeshComp->GetRelativeRotation().Pitch, angleDeg, pMeshComp->GetRelativeRotation().Roll));*/

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

//プレイヤーとの距離を更新
void UBTT_MoveToPlayer::UpdateDistancefromPlayer(UBehaviorTreeComponent& _ownerComp)
{
	//ブラックボード取得
	UBlackboardComponent* pBlackboard = _ownerComp.GetBlackboardComponent();

	//AIコントローラー取得
	AAIController* pController = _ownerComp.GetAIOwner();
	//コントローラーが保持している、Pawnを取得
	APawn* pOwner = pController->GetPawn();

	float Distance = FVector::Dist(pTarget->GetActorLocation(), pOwner->GetActorLocation());

	//プレイヤーとの距離をブラックボードに記述
	pBlackboard->SetValueAsFloat(TEXT("DistanceFromPlayer"), Distance);
}
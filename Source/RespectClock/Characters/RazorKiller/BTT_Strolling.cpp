#include "BTT_Strolling.h"
#include "NavigationPath.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "AIC_RazorKiller.h"
#include "Math/UnrealMathUtility.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "RespectClock/Characters/Player/PlayerCharacter.h"
#include "DrawDebugHelpers.h"

// コンストラクタ
UBTT_Strolling::UBTT_Strolling()
{
    NodeName = TEXT("散策");
    bNotifyTick = true;  // タスク開始直後からTickを有効化する
    navSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());

    // 停止時間を初期化（秒単位）
    PauseDuration = 3.0f;
    // DoorLocationへの移動処理が完了したかを判定するフラグ
    bIsMovingToDoor = false;
    bIsPausedBeforeDoor = false;  // DoorLocationに移動する前に停止するフラグ
}

// タスクの実行
EBTNodeResult::Type UBTT_Strolling::ExecuteTask(UBehaviorTreeComponent& _ownerComp, uint8* _nodeMemory)
{
    // Tickを有効化
    bNotifyTick = true;

    // キラーのコントローラーを取得
    KillerController = Cast<AAIC_RazorKiller>(_ownerComp.GetAIOwner());
    // コントローラーが保持しているPawnを取得
    pKillerOwner = KillerController->GetPawn();
    // 散策用基準Actorを取得
    m_StrollingActors = KillerController->GetStrollingActors();
    // 要素数の初期化
    StrollingElem = 0;

    // 一時停止フラグをリセット
    bIsPaused = true;
    bIsPausedBeforeDoor = false;  // DoorLocation前の停止フラグリセット

    // タスク開始時に一定時間停止
    ElapsedPauseTime = 0.0f;

    // DoorLocationへの移動をまだ行っていない
    bIsMovingToDoor = false;

    ACharacter* Character = Cast<ACharacter>(_ownerComp.GetAIOwner()->GetPawn());
    if (Character)
    {
        InitialMeshRotation = Character->GetMesh()->GetRelativeRotation();
    }

    UBlackboardComponent* BlackboardComp = _ownerComp.GetBlackboardComponent();
    PlayerCharacter = Cast<APlayerCharacter>(BlackboardComp->GetValueAsObject("PlayerActor"));

    // タスクはInProgressで返すが、開始時に停止する
    return EBTNodeResult::InProgress;
}

// Tickで実行される処理
void UBTT_Strolling::TickTask(UBehaviorTreeComponent& _ownerComp, uint8* _nodeMemory, float _deltaSeconds)
{
    // 一時停止中であれば経過時間を計測
    if (bIsPaused)
    {
        ElapsedPauseTime += _deltaSeconds;

        // 経過時間が停止時間を超えたら再開
        if (ElapsedPauseTime >= PauseDuration)
        {
            ResumeTask(_ownerComp);
        }
        return;
    }

    // DoorLocationに移動する前の一時停止
    if (bIsPausedBeforeDoor)
    {
        ElapsedPauseTime += _deltaSeconds;

        if (ElapsedPauseTime >= PauseDuration)
        {
            // 一時停止を解除して、DoorLocationへの移動を開始
            bIsPausedBeforeDoor = false;
            bIsMovingToDoor = true;
            ElapsedPauseTime = 0.0f;
        }
        return;
    }

    // すべてのアクターを巡回し終わったら、DoorLocationに移動
    if (bIsMovingToDoor)
    {
        MoveToDoorLocation(_ownerComp, _deltaSeconds);
    }
    else
    {
        // 通常の移動処理
        UpdateMove(_ownerComp, _deltaSeconds);
    }
}

// 通常の移動処理
void UBTT_Strolling::UpdateMove(UBehaviorTreeComponent& _ownerComp, float _deltaSeconds)
{
    if (m_StrollingActors.Num() == 0 || StrollingElem >= m_StrollingActors.Num())
    {
        // ターゲットが存在しない、またはインデックスが範囲外の場合はタスクを終了
        FinishLatentTask(_ownerComp, EBTNodeResult::Failed);
        return;
    }

    FVector TargetLocation = m_StrollingActors[StrollingElem]->GetActorLocation();
    FVector Distance;

    if (pKillerOwner)
    {
        TargetLocation.Z = pKillerOwner->GetActorLocation().Z;
        Distance = pKillerOwner->GetActorLocation() - TargetLocation;
    }

    FVector Movement = CalcMovement(pKillerOwner, _deltaSeconds, TargetLocation);

    if (Distance.SizeSquared() > m_Radias * m_Radias)
    {
        if (!Movement.IsZero())
        {
            pKillerOwner->AddMovementInput(Movement, m_MoveSpeed);
        }
    }
    else
    {
        // Actorに到達した時点で一時停止
        StrollingElem++;
        PauseTask(_ownerComp);  // 一時停止処理
    }

    // すべてのActorを巡回し終わったらDoorLocationに移動開始
    if (StrollingElem >= m_StrollingActors.Num())
    {
        // DoorLocationに移動する前に一時停止
        bIsPausedBeforeDoor = true;
        ElapsedPauseTime = 0.0f;  // 経過時間をリセット
    }
}

// DoorLocationへの移動処理
void UBTT_Strolling::MoveToDoorLocation(UBehaviorTreeComponent& _ownerComp, float _deltaSeconds)
{
    // BlackboardからDoorLocationを取得
    UBlackboardComponent* BlackboardComp = _ownerComp.GetBlackboardComponent();
    FVector DoorLocation = BlackboardComp->GetValueAsVector("DoorLocation");

    FVector Distance;

    if (pKillerOwner)
    {
        DoorLocation.Z = pKillerOwner->GetActorLocation().Z;
        Distance = pKillerOwner->GetActorLocation() - DoorLocation;
    }

    FVector Movement = CalcMovement(pKillerOwner, _deltaSeconds, DoorLocation);

    if (Distance.SizeSquared() > m_Radias * m_Radias)
    {
        if (!Movement.IsZero())
        {
            pKillerOwner->AddMovementInput(Movement, m_MoveSpeed);
        }
    }
    else
    {
        // DoorLocationに到達した時点で停止
        BlackboardComp->SetValueAsBool("IsEndStrolling", true);

        ResetRotation(pKillerOwner);

        FinishLatentTask(_ownerComp, EBTNodeResult::Succeeded);
    }
}

// 一時停止処理
void UBTT_Strolling::PauseTask(UBehaviorTreeComponent& _ownerComp)
{
    bIsPaused = true;
    ElapsedPauseTime = 0.0f;  // 経過時間をリセット
}

// 再開処理
void UBTT_Strolling::ResumeTask(UBehaviorTreeComponent& _ownerComp)
{
    bIsPaused = false;
}

// 移動計算
FVector UBTT_Strolling::CalcMovement(const APawn* _pOwner, float _deltaTime, FVector _targetpos)
{
    UNavigationPath* pPath = navSys->FindPathToLocationSynchronously(GetWorld(), _pOwner->GetActorLocation(), _targetpos);

    if (!pPath) return FVector::Zero();

    TArray<FVector> pathPoints = pPath->PathPoints;

    if (pathPoints.Num() >= 2)
    {
        FVector dir = pathPoints[1] - pathPoints[0];
        dir.Normalize();

        UpdateRotate(dir, _pOwner, _deltaTime);

        return dir;
    }
    else
    {
        return FVector::Zero();
    }
}

// 回転計算
void UBTT_Strolling::UpdateRotate(const FVector _direction, const APawn* _pOwner, float _deltaTime)
{
    USkeletalMeshComponent* pMeshComp = Cast<ACharacter>(_pOwner)->GetMesh();
    if (pMeshComp != NULL)
    {
        FRotator CurrentRotation = pMeshComp->GetRelativeRotation();

        float angle = atan2(-_direction.X, _direction.Y);
        float angleDeg = FMath::RadiansToDegrees(angle);

        FRotator NewRotation = FRotator(pMeshComp->GetRelativeRotation().Pitch, angleDeg, pMeshComp->GetRelativeRotation().Roll);

        if (_deltaTime == 0.f || CurrentRotation == NewRotation) { return; }

        if (m_InterpolationSpeed <= 0.f) { pMeshComp->SetRelativeRotation(NewRotation); }

        float DeltaInterpSpeed = m_InterpolationSpeed * _deltaTime;
        const FRotator DeltaRotate = (NewRotation - CurrentRotation).GetNormalized();

        if (DeltaRotate.IsNearlyZero()) { pMeshComp->SetRelativeRotation(NewRotation); }

        const FRotator DeltaMove = DeltaRotate * FMath::Clamp<float>(DeltaInterpSpeed, 0.f, 1.f);

        pMeshComp->SetRelativeRotation((CurrentRotation + DeltaMove).GetNormalized());
    }
}

void UBTT_Strolling::ResetRotation(APawn* _pOwner)
{
    // メッシュの回転を初期状態に戻す
    USkeletalMeshComponent* pMeshComp = Cast<ACharacter>(_pOwner)->GetMesh();
    if (pMeshComp != NULL)
    {
        pMeshComp->SetRelativeRotation(InitialMeshRotation);
    }
}
// Fill out your copyright notice in the Description page of Project Settings.


#include "Curtain.h"

ACurtain::ACurtain()
{
	SkeltalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeltalMesh"));
	SkeltalMesh->SetupAttachment(RootComponent);
}

void ACurtain::BeginPlay()
{
	Super::BeginPlay();
	if (IsValid(SkeltalMesh))
	{
		ClosedCurtainScale = SkeltalMesh->GetRelativeScale3D().X;
		OpenedCurtainScale = ClosedCurtainScale + OpenScale;
	}
}

void ACurtain::Tick(float DeltaTime)
{
	// カーテンが開閉中か
	if ((MoveDir == 1) || (MoveDir == -1))
	{
		if (IsValid(MovementCurve))
		{
			// 進行時間を更新する
			ElapsedTime += DeltaTime * MoveDir;

			// Curveの進行時間の最大値と最小値を取得
			float MinCurveTime, MaxCurveTime;
			MovementCurve->GetTimeRange(MinCurveTime, MaxCurveTime);

			// 進行時間の範囲を超えないように丸め込み
			ElapsedTime = FMath::Clamp(ElapsedTime, MinCurveTime, MaxCurveTime);

			// 閉じてる時から開いてる時までの割合(0.0~1.0)を取得
			float MoveAlpha = MovementCurve->GetFloatValue(ElapsedTime);

			// スケールの更新
			FVector NewScale = SkeltalMesh->GetRelativeScale3D();
			NewScale.X = FMath::Lerp(ClosedCurtainScale, OpenedCurtainScale, MoveAlpha);
			SkeltalMesh->SetRelativeScale3D(NewScale);

			// 進行時間がCurveの最大値を超えたらTickを停止する
			if (ElapsedTime >= MaxCurveTime)
			{
				MoveDir = -1;
				SetPlayerLocation();
			}
			else if (ElapsedTime <= MinCurveTime)
			{
				if (MoveDir == -1)
				{
					MoveDir = 0;
					IsInteractable = true;
					SetActorTickEnabled(false);
				}
			}
		}
	}
}

void ACurtain::Interactive()
{
	// 移動フラグとTick処理を有効にする
	if (IsInteractive())
	{
		MoveDir = 1;
		IsInteractable = false;
		SetActorTickEnabled(true);
	}
}

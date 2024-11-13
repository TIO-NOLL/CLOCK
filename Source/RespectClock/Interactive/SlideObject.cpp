// Fill out your copyright notice in the Description page of Project Settings.

#include "SlideObject.h"
#include "RespectClock/Characters/Player/PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"

void ASlideObject::BeginPlay()
{
	Super::BeginPlay();
	ClosedXLocation = StaticMesh->GetRelativeLocation().X;
	OpenedXLocation = ClosedXLocation + OpenValue;
	SetActorTickEnabled(false);
}

void ASlideObject::Tick(float DeltaTime)
{
	// 時間を進める
	ElapsedTime += DeltaTime;

	if (IsValid(MovementCurve))
	{
		// Curveの進行時間の最小値と最大値を取得
		float MinCurveTime, MaxCurveTime;
		MovementCurve->GetTimeRange(MinCurveTime, MaxCurveTime);

		// 進行時間の範囲を超えないように丸め込み
		ElapsedTime = FMath::Clamp(ElapsedTime, MinCurveTime, MaxCurveTime);

		// 閉じてる時から開いてる時までの割合を取得
		float MoveAlpha = MovementCurve->GetFloatValue(ElapsedTime);

		// 座標の更新
		FVector NewLocation = StaticMesh->GetRelativeLocation();
		NewLocation.X = FMath::Lerp(ClosedXLocation, OpenedXLocation, MoveAlpha);
		StaticMesh->SetRelativeLocation(NewLocation);

		// 進行時間がCurveの最大値を超えたらTickを停止する
		if (ElapsedTime >= MaxCurveTime)
		{
			APlayerCharacter* Player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
			if (Player) Player->IsMoveable = true;
			//IsInteractable = true;
			ElapsedTime = 0;
			SetActorTickEnabled(false);
		}
	}
}

FVector ASlideObject::GetLerpPointPos()
{
	if (!IsValid(LerpPointScene)) return FVector::ZeroVector;
	return LerpPointScene->GetComponentLocation();
}

FRotator ASlideObject::GetLerpPointRot()
{
	if (!IsValid(LerpPointScene)) return FRotator::ZeroRotator;
	return LerpPointScene->GetComponentRotation();
}

void ASlideObject::Interactive()
{
	if (IsInteractive())
	{
		//移動フラグとTick処理を有効にする
		IsInteractable = false;
		SetActorTickEnabled(true);
	}
}

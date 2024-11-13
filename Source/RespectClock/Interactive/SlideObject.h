// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractiveObject.h"
#include "SlideObject.generated.h"

UCLASS()
class RESPECTCLOCK_API ASlideObject : public AInteractiveObject
{
	GENERATED_BODY()
	
protected:
	void BeginPlay();

public:
	void Tick(float DeltaTime) override;

	FVector GetLerpPointPos();
	FRotator GetLerpPointRot();

	void Interactive() override;

public:
	// 開いた時の座標
	UPROPERTY(EditDefaultsOnly)
	float OpenValue = 120.0f;

	// 開閉の仕方を指定するカーブアセット
	UPROPERTY(EditDefaultsOnly)
	UCurveFloat* MovementCurve;

private:
	// 進行時間
	UPROPERTY()
	float ElapsedTime;
	//閉まっているときのアクターのローケーション
	UPROPERTY()
	float ClosedXLocation;
	//開ききっているのアクターのローケーション
	UPROPERTY()
	float OpenedXLocation;

};

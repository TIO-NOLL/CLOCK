// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HideObject.h"
#include "Curtain.generated.h"

/**
 * 
 */
UCLASS()
class RESPECTCLOCK_API ACurtain : public AHideObject
{
	GENERATED_BODY()

public:
	ACurtain();
	
protected:
	void BeginPlay();

public:
	void Tick(float DeltaTime) override;

	void Interactive() override;

public:
	//開いた時のスケール
	UPROPERTY(EditDefaultsOnly)
	float OpenScale;

	//開閉の仕方を指定するカーブアセット
	UPROPERTY(EditDefaultsOnly)
	UCurveFloat* MovementCurve;

private:
	//開閉処理の進行時間
	UPROPERTY()
	float ElapsedTime;
	//開閉方向　1=開く　0=何もしない　-1=閉じる
	UPROPERTY()
	int MoveDir = 0;
	//カーテンが閉まっているときの
	UPROPERTY()
	float ClosedCurtainScale;
	UPROPERTY()
	//カーテンが開ききっているときのスケール
	float OpenedCurtainScale;
	
private:
	UPROPERTY(EditAnywhere)
	TObjectPtr<USkeletalMeshComponent> SkeltalMesh;
};

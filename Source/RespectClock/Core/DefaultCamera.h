// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraActor.h"
#include "DefaultCamera.generated.h"

/**
 * 
 */
UCLASS()
class RESPECTCLOCK_API ADefaultCamera : public ACameraActor
{
	GENERATED_BODY()

	ADefaultCamera();
	
protected:
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
    // プレイヤーを追う
    void UpdateCameraDirection();

	// ターゲットキャラクター
	AActor* TargetCharacter;

protected:
	
	// 首振りをするか
	UPROPERTY(EditAnywhere)
	bool bTargetCamera = false;

	// カメラのYawの最小値と最大値
	UPROPERTY(EditAnywhere, Category = "Camera Limits")
	float MinYaw = -180.0f;

	UPROPERTY(EditAnywhere, Category = "Camera Limits")
	float MaxYaw = 180.0f;

	// カメラのPitchの最小値と最大値
	UPROPERTY(EditAnywhere, Category = "Camera Limits")
	float MinPitch = -90.0f;

	UPROPERTY(EditAnywhere, Category = "Camera Limits")
	float MaxPitch = 90.0f;

};

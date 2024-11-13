// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractiveObject.h"
#include "Camera/CameraComponent.h"
#include "HideObject.generated.h"

/**
 * 
 */
UCLASS()
class RESPECTCLOCK_API AHideObject : public AInteractiveObject
{
	GENERATED_BODY()

public:
	AHideObject();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Interactive() override;

	FVector GetSecondLerpPointPos();
	FRotator GetSecondLerpPointRot();

	virtual EPlayerStatus GetStatus() override;

	UFUNCTION(BlueprintCallable)
	virtual void ChangeCamera() override;

	/// @brief プレイヤーの位置を移動
	void SetPlayerLocation();

protected:
	/// @brief 隠れた際のプレイヤーの位置
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USceneComponent> HidePointScene;
	/// @brief 隠れた際のカメラ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> CameraComp;
	
	/// @brief 隠れる前に使用していたカメラ
	AActor* BeforeCamera;
	/// @brief 経過時間
	float ElapsedTime;
	/// @brief カメラを変える時間
	UPROPERTY(EditAnyWhere)
	float CameraChangeTime = 2.0f;
	/// @brief プレイヤーが隠れている状態か
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool IsHiddenIn;

public:
	UFUNCTION(BlueprintCallable, BlueprintPure)
	UCameraComponent* GetHideCamera() const{ return CameraComp; }
};

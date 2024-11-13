// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MoveInput.generated.h"

class APlayerCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RESPECTCLOCK_API UMoveInput : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMoveInput();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	/// @brief 移動
	void Move(APlayerCharacter* player, FVector2D value);
	/// @brief 走り
	void Run(APlayerCharacter* player);
	/// @brief 走り止める
	void StopRunning(APlayerCharacter* player);
	/// @brief しゃがみ
	void Squat(APlayerCharacter* player);
	/// @brief 戻り始める 
	void BeginBack(APlayerCharacter* player);

private:
	/// @brief 後ろに歩く
	void MoveBack(float deltaTime);

public:
	/// @brief 歩行中か
	UPROPERTY()
	bool IsWalking;
	/// @brief しゃがみ中か
	UPROPERTY()
	bool IsCrouching;

private:
	/// @brief 移動時のカメラ
	UPROPERTY()
	AActor* Camera;
	/// @brief 一度のみのフラグ
	UPROPERTY()
	bool IsKeepControlValue;
	/// @brief 移動時の入力値
	UPROPERTY()
	FVector2D ControlValue;
	/// @brief 後ろの方向
	UPROPERTY()
	FVector BackVector;
	/// @brief 経過時間
	UPROPERTY()
	float ElapsedTime;
};

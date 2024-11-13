// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractiveInput.generated.h"

class APlayerCharacter;
class ARazorKiller;

/// @brief 正気度ごとの抵抗値をまとめた構造体
USTRUCT(BlueprintType)
struct FResistanceThreshold
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	int ThresholdAtFull;
	UPROPERTY(EditAnywhere)
	int ThresholdAtHigh;	
	UPROPERTY(EditAnywhere)
	int ThresholdAtMedium;	
	UPROPERTY(EditAnywhere)
	int ThresholdAtLow;
	UPROPERTY(EditAnywhere)
	int ThresholdAtExhausted;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RESPECTCLOCK_API UInteractiveInput : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInteractiveInput();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	/// @brief インタラクティブ
	void Interactive(APlayerCharacter* player);
	/// @brief 抵抗
	void Resist(APlayerCharacter* player , ARazorKiller* razorKiller);

private:
	/// @brief 現在の抵抗値
	UPROPERTY(EditAnywhere)
	int Resistance;
	/// @brief 抵抗する度に増える値
	UPROPERTY(EditAnywhere)
	int ResistanceIncrement;
	/// @brief 抵抗に必要な値
	UPROPERTY(EditAnywhere)
	FResistanceThreshold ResistanceThreshold;
	/// @brief 抵抗の制限時間
	UPROPERTY(EditAnywhere)
	float LimitTime;
	/// @brief 現在の経過時間
	UPROPERTY()
	float ElapsedTime;
	/// @brief 抵抗成功時に増加させる正気度の値
	UPROPERTY(EditAnywhere)
	float AddSanityValueAfterResistance;

	// =======田中追記(抵抗)=======
	UPROPERTY()
	TObjectPtr<ARazorKiller> RazorKiller;

public:
	UFUNCTION()
	void SuccessResist(APlayerCharacter* _player, ARazorKiller* _razorKiller);
};

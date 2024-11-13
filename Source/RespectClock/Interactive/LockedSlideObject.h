// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SlideObject.h"
#include "LockedSlideObject.generated.h"

class APlayerCharacter;

UCLASS()
class RESPECTCLOCK_API ALockedSlideObject : public ASlideObject
{
	GENERATED_BODY()
	
private:
	APlayerCharacter* Player;
	bool IsOpenable;

public:
	void Interactive() override;

	EPlayerStatus GetStatus();

	bool bIsOpenable()const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ID");
	int32 DoorID;	// ドアID

	UPROPERTY(BlueprintReadWrite)
	FString OpenableMessage;	// ドアを開けた時のメッセージ

	UPROPERTY(BlueprintReadWrite)
	FString DisableMessage;		// ドアを開けられない時のメッセージ

	UPROPERTY(BlueprintReadWrite)
	FString DifferentMessage;	// 鍵の番号が違う時のメッセージ
};

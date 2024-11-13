// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Door.h"
#include "LockedDoor.generated.h"

class APlayerCharacter;

UCLASS()
class RESPECTCLOCK_API ALockedDoor : public ADoor
{
	GENERATED_BODY()
	
private:
	UPROPERTY()
	APlayerCharacter* Player;
public:
	void Interactive() override;

	EPlayerStatus GetStatus();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door");
	int32 DoorID;	// ドアID

	UPROPERTY(BlueprintReadWrite)
	FString OpenableMessage;	// ドアを開けた時のメッセージ

	UPROPERTY(BlueprintReadWrite)
	FString DisableMessage;		// ドアを開けられない時のメッセージ

	UPROPERTY(BlueprintReadWrite)
	FString DifferentMessage;	// 鍵の番号が違う時のメッセージ
};
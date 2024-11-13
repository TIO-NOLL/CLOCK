// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "RespectClock/Characters/Player/E_PlayerState.h"
#include "InteractableInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteractableInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class RESPECTCLOCK_API IInteractableInterface
{
	GENERATED_BODY()

public:
	virtual void Interactive() = 0;

	virtual bool IsInteractive() = 0;

	virtual EPlayerStatus GetStatus() = 0;

	virtual FVector GetLerpPointPos() = 0;
	virtual FRotator GetLerpPointRot() = 0;

	virtual FVector GetSecondLerpPointPos() = 0;
	virtual FRotator GetSecondLerpPointRot() = 0;

	virtual void ChangeCamera() = 0;
};

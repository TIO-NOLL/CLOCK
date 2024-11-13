// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "E_PlayerState.generated.h"

UENUM(BlueprintType)
enum class EPlayerStatus : uint8
{
	None,
	Idle,
	Walk,
	PickUp,
	Run,
	Sit,
	Fall,
	Resist,
};

UENUM(BlueprintType)
enum class ESanityLevel : uint8
{
	Full = 100,
	High = 75,
	Medium = 50,
	Low = 25,
	Exhausted = 0
};

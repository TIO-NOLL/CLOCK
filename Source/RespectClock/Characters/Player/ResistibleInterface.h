// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ResistibleInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UResistibleInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class RESPECTCLOCK_API IResistibleInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual bool IsGetResistance() = 0;
};

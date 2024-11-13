// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HideObject.h"
#include "Bed.generated.h"

/**
 * 
 */
UCLASS()
class RESPECTCLOCK_API ABed : public AHideObject
{
	GENERATED_BODY()
public:
	ABed();

protected:
	void BeginPlay();
	
public:
	// Called every frame
	void Tick(float DeltaTime) override;

	void Interactive() override;

};

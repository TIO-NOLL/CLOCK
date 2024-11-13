// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HideObject.h"
#include "Closet.generated.h"

UCLASS()
class RESPECTCLOCK_API ACloset : public AHideObject
{
	GENERATED_BODY()
	
protected:
    void BeginPlay();

public:
    // Called every frame
    void Tick(float DeltaTime) override;

    void Interactive() override;

protected:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void ChangeDoorRot();
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraActor.h"
#include "FirstPersonCamera.generated.h"

/**
 * 
 */
UCLASS()
class RESPECTCLOCK_API AFirstPersonCamera : public ACameraActor
{
	GENERATED_BODY()

public:
	AFirstPersonCamera();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	AActor* TargetCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HeadOffset;

};

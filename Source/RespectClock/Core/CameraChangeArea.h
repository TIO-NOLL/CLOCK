// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CameraManager.h"
#include "CameraChangeArea.generated.h"

UCLASS()
class RESPECTCLOCK_API ACameraChangeArea : public AActor
{
	GENERATED_BODY()
	
public:
	ACameraChangeArea();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UBoxComponent* OverlapVolume;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	ACameraActor* CameraActor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ChangeTime;

protected:
	void NotifyActorBeginOverlap(AActor* OtherActor);

};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OverlapObject.h"
#include "EventPlayVolume.generated.h"

/**
 * 
 */
UCLASS()
class RESPECTCLOCK_API AEventPlayVolume : public AOverlapObject
{
	GENERATED_BODY()

	void BeginPlay()override;
	
public:
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Event")
	int32 EventID = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Event")
	FString EventName = TEXT("");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Event")
	bool m_bIsOncePlayEvent = false;

private:
	class UEventPlayer* EP;

};

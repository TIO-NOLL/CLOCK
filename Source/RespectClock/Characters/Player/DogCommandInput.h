// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DogCommandInput.generated.h"

class AAIC_Dog;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RESPECTCLOCK_API UDogCommandInput : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDogCommandInput();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void Call();

	void Stay();

	void MoveForward();

private:
	void GetDog();

private:
	AAIC_Dog* Dog;
};

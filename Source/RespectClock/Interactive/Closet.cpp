// Fill out your copyright notice in the Description page of Project Settings.


#include "Closet.h"

void ACloset::BeginPlay()
{
    Super::BeginPlay();
}

void ACloset::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if(ElapsedTime > CameraChangeTime)
    {
        ChangeDoorRot();
    }
}

void ACloset::Interactive()
{
    Super::Interactive();
}

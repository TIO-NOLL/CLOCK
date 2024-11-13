// Fill out your copyright notice in the Description page of Project Settings.


#include "DefaultCamera.h"
#include "Kismet/GameplayStatics.h"

ADefaultCamera::ADefaultCamera() 
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;


}

void ADefaultCamera::BeginPlay()
{
	Super::BeginPlay();

    // プレイヤを取得
    APawn* aPlayerPawn = UGameplayStatics::GetPlayerPawn(this->GetWorld(), 0);
    TargetCharacter = (AActor*)aPlayerPawn;
}

// Called every frame
void ADefaultCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// プレイヤーを追う
	UpdateCameraDirection();
}

void ADefaultCamera::UpdateCameraDirection()
{
    if (bTargetCamera)
    {
        // Get the direction from camera to player
        FVector DirectionToPlayer = TargetCharacter->GetActorLocation() - GetActorLocation();
        DirectionToPlayer.Normalize();

        // Calculate the rotation to look at the player
        FRotator NewCameraRotation = DirectionToPlayer.Rotation();

        // Limit the yaw within the specified range
        NewCameraRotation.Yaw = FMath::Clamp(NewCameraRotation.Yaw, MinYaw, MaxYaw);

        // Limit the pitch within the specified range
        NewCameraRotation.Pitch = FMath::Clamp(NewCameraRotation.Pitch, MinPitch, MaxPitch);

        // Set the camera's rotation to the limited rotation
        SetActorRotation(NewCameraRotation);
    }
}
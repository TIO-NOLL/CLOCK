// Fill out your copyright notice in the Description page of Project Settings.


#include "CameraManager.h"
#include "Kismet/GameplayStatics.h"

// Called when the game starts or when spawned
void ACameraManager::BeginPlay()
{
	Super::BeginPlay();

	// APawn の取得
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this->GetWorld(), 0);

	// カメラを設定
	APlayerController* PlayerCharacterController = Cast<APlayerController>(PlayerPawn->GetController());
	PlayerCharacterController->SetViewTargetWithBlend(StartCameraActor, 0.0f, EViewTargetBlendFunction::VTBlend_Linear);

}
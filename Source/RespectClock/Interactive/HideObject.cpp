// Fill out your copyright notice in the Description page of Project Settings.


#include "HideObject.h"
#include "RespectClock/Characters/Player/PlayerCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"

AHideObject::AHideObject()
{
	HidePointScene = CreateDefaultSubobject<USceneComponent>(TEXT("HidePoint"));
	HidePointScene->SetupAttachment(RootComponent);

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(RootComponent);
}

void AHideObject::BeginPlay()
{
	Super::BeginPlay();
	SetActorTickEnabled(false);
}

void AHideObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 時間を加算
	ElapsedTime += DeltaTime;

	if (ElapsedTime > CameraChangeTime)
	{
		// 隠れているなら
		if(IsHiddenIn)
		{
			IsHiddenIn = false;
			UE_LOG(LogTemp, Display, TEXT("SetPlayer"));
		}
		else
		{
			IsHiddenIn = true;
		}
		// プレイヤーを所定の位置に
		SetPlayerLocation();
		ChangeCamera();
		IsInteractable = true;
		SetActorTickEnabled(false);
	}
}

void AHideObject::Interactive()
{
	Super::Interactive();

	// 移動フラグとTick処理を有効にする
	if (IsInteractable)
	{
		ElapsedTime = 0.0f;
		IsInteractable = false;
		SetActorTickEnabled(true);
	}
}

FVector AHideObject::GetSecondLerpPointPos()
{
	if (!IsValid(HidePointScene)) return FVector::ZeroVector;
	return HidePointScene->GetComponentLocation();
}

FRotator AHideObject::GetSecondLerpPointRot()
{
	if (!IsValid(HidePointScene)) return FRotator::ZeroRotator;
	FRotator rot = HidePointScene->GetComponentRotation();
	rot.Yaw += 90.0f;
	return rot;
}

EPlayerStatus AHideObject::GetStatus()
{
	return EPlayerStatus::Idle;
}

void AHideObject::ChangeCamera()
{
	Super::ChangeCamera();

	// 現在のカメラを取得
	AActor* currentCamera = GetWorld()->GetFirstPlayerController()->GetViewTarget();
	// 同じじゃないならカメラを保存
	if (currentCamera != this || BeforeCamera == nullptr)
	{
		BeforeCamera = currentCamera;
	}

	TArray<AActor*> Actors;
	APlayerCharacter* Player = nullptr;
	// オーバーラップしているActorを返す
	Box->GetOverlappingActors(Actors);
	
	for (AActor* actor : Actors)
	{
		// アクターがプレイヤーか
		if (APlayerCharacter* PlayerCharacterCheck = Cast<APlayerCharacter>(actor))
		{
			// プレイヤー キャラクターの PlayerController へのキャスト
			if (APlayerController* PlayerCharacterController = Cast<APlayerController>(PlayerCharacterCheck->GetController()))
			{
				if (currentCamera == this)
				{
					// 隠れた際のカメラに切り替え
					PlayerCharacterController->SetViewTargetWithBlend(BeforeCamera, 0.0f, EViewTargetBlendFunction::VTBlend_Linear);
				}
				else
				{
					// 隠れる前のカメラに切り替え
					PlayerCharacterController->SetViewTargetWithBlend(this, 0.0f, EViewTargetBlendFunction::VTBlend_Linear);
				}
				UE_LOG(LogTemp, Warning, TEXT("%s"), *FString(currentCamera->GetName()));
			}
		}
	}
}

void AHideObject::SetPlayerLocation()
{
	TArray<AActor*> Actors;
	APlayerCharacter* Player = nullptr;
	// OverlapされているActorを返す
	Box->GetOverlappingActors(Actors);

	for (AActor* actor : Actors)
	{
		Player = Cast<APlayerCharacter>(actor);
		// もしプレイヤーなら
		if (Player != nullptr)
		{
			if (!IsHiddenIn)
			{
				Player->SetActorLocation(FVector(GetLerpPointPos().X, GetLerpPointPos().Y, Player->GetActorLocation().Z));
				Player->IsMoveable = true;
				Player->IsHiding = false;
				Player->SetHidden();
				Player->PlayerStatus = EPlayerStatus::Idle;
			}
			else
			{
				Player->Hide();
			}
		}
	}
}

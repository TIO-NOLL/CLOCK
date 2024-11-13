// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractiveInput.h"
#include "PlayerCharacter.h"
#include "RespectClock/Characters/RazorKiller/RazorKiller.h"
#include "RespectClock/Interactive/InteractableInterface.h"
#include "Kismet/KismetMathLibrary.h"
#include "Camera/CameraComponent.h"
#include "RespectClock/Interactive/Door.h"
#include "Kismet/GameplayStatics.h"
#include "RespectClock/Core/Message.h"
#include "RespectClock/Core/RespectClockInstance.h"
#include "RespectClock/Interactive/HideObject.h"

// Sets default values for this component's properties
UInteractiveInput::UInteractiveInput()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	// ...
}


// Called when the game starts
void UInteractiveInput::BeginPlay()
{
	Super::BeginPlay();
	SetComponentTickEnabled(false);
	// ...
}


// Called every frame
void UInteractiveInput::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	// 時間を進める
	ElapsedTime += DeltaTime;
	// 制限時間を超えたか
	if (ElapsedTime > LimitTime)
	{
		// ゲームオーバー処理
		APlayerCharacter* Player = Cast<APlayerCharacter>(GetOwner());
		Player->IsResistible = false;
		Player->GameOverDispatcer.Broadcast();
		SetComponentTickEnabled(false);
	}
	// ...
}

void UInteractiveInput::Interactive(APlayerCharacter* player)
{
	if (!IsValid(player))return;
	
	// プレイヤーがオーバーラップしているアクターを取得
	TArray<AActor*> Actors;
	player->GetOverlappingActors(Actors);
	IInteractableInterface* InteractableActor = nullptr; // インタラクティブ先のオブジェクト

	for (AActor* actor : Actors)
	{
		InteractableActor = Cast<IInteractableInterface>(actor); 
		// インタラクト可能か
		if (InteractableActor != nullptr)
		{
			if (player->IsInteractable && InteractableActor->IsInteractive())
			{
				if (!player->IsHiding && player->IsMoveable)
				{
					// インタラクティブ処理
					player->IsMoveable = false;
					player->PlayerStatus = InteractableActor->GetStatus();
					player->SetActorLocation(FVector(InteractableActor->GetLerpPointPos().X, InteractableActor->GetLerpPointPos().Y, player->GetActorLocation().Z));
					player->SetActorRotation(InteractableActor->GetLerpPointRot());
					// ドアか
					if(ADoor* door = Cast<ADoor>(actor))
					{
						if (door->bIsOpenable())
						{
							player->InFade();
							door->ShowRoomName();
						}
					}
					// 隠れられるオブジェクトか
					else if (actor->ActorHasTag("HideObject"))
					{
						// 隠れる
						player->HidePos = InteractableActor->GetSecondLerpPointPos();
						player->HideRot = InteractableActor->GetSecondLerpPointRot();
						player->IsHiding = true;
						player->InFade();
						player->SetHidden();
						// カメラを設定
						if(AHideObject* hideObj = Cast<AHideObject>(actor))
						{
							player->HideObject = hideObj;
						}
					}
				}
				else if (player->IsHiding)
				{
					player->IsHiding = false;

					player->InFade();
				}
				InteractableActor->Interactive();
			}
			break;	// 最初に見つけた一つだけ
		}
	}
}

void UInteractiveInput::Resist(APlayerCharacter* player, ARazorKiller* razorKiller)
{
	// 抵抗値の増加
	Resistance += ResistanceIncrement;

	//==== 抵抗を動かす為に処理追加(田中)
	if (RazorKiller == nullptr)
	{
		RazorKiller = razorKiller;
	}

	// 正気度による抵抗値の取得
	int threshold;
	switch (player->GetSanityLevel())
	{
	case ESanityLevel::Full:
		threshold = ResistanceThreshold.ThresholdAtFull;
		break;
	case ESanityLevel::High:
		threshold = ResistanceThreshold.ThresholdAtHigh;
		break;
	case ESanityLevel::Medium:
		threshold = ResistanceThreshold.ThresholdAtMedium;
		break;
	case ESanityLevel::Low:
		threshold = ResistanceThreshold.ThresholdAtLow;
		break;
	case ESanityLevel::Exhausted:
		threshold = ResistanceThreshold.ThresholdAtExhausted;
		break;
	}

	UE_LOG(LogTemp, Warning, TEXT("threshold=%i, NowResistance=%i"), threshold, Resistance);

	// 現在の抵抗値が抵抗に必要な値を超えたか
	if (Resistance >= threshold)
	{
		// 抵抗成功処理
		SuccessResist(player, razorKiller);

		// 正気度上昇
		player->AddSanity(AddSanityValueAfterResistance);

	}
}

void UInteractiveInput::SuccessResist(APlayerCharacter* _player, ARazorKiller* _razorKiller)
{
	if (RazorKiller == nullptr)
	{
		RazorKiller = _razorKiller;
	}

	SetComponentTickEnabled(false);
	_player->IsResistible = true;
	_player->PlayerStatus = EPlayerStatus::Idle;
	_player->IsMoveable = true;
	_player->CanRecover = true;
	_player->SetResistWidgetVisibility(false);
	Resistance = 0.0f;
	ElapsedTime = 0.0f;

	//==== 抵抗を動かす為に処理追加(田中)
	RazorKiller->CompResisted();
}


// Fill out your copyright notice in the Description page of Project Settings.


#include "MoveInput.h"
#include "PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Controller.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "RespectClock/Interactive/MsgType.h"
#include "RespectClock/Core/Message.h"
#include "RespectClock/Core/RespectClockInstance.h"

// Sets default values for this component's properties
UMoveInput::UMoveInput()
	:IsWalking(false)
	,IsKeepControlValue(true)
	,ControlValue(FVector2D::ZeroVector)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	// ...
}

// Called when the game starts
void UMoveInput::BeginPlay()
{
	Super::BeginPlay();
	SetComponentTickEnabled(false);

	Camera = GetWorld()->GetFirstPlayerController()->GetViewTarget();
	// ...
}

// Called every frame
void UMoveInput::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	MoveBack(DeltaTime);
}

void UMoveInput::Move(APlayerCharacter* player, FVector2D invalue)
{
	// 歩行中に
	IsWalking = true;

	// 現在のカメラを取得
	AActor* currentCamera = GetWorld()->GetFirstPlayerController()->GetViewTarget();
	// カメラが変わったか
	if (Camera != currentCamera)
	{
		// 入力値を保存したか
		if (IsKeepControlValue)
		{		
			// 入力値を保存
			ControlValue = invalue;
			IsKeepControlValue = false;
		}
		// 入力値が変わったか
		if(ControlValue != invalue)
		{
			// 使用するカメラを現在のカメラに変更
			Camera = currentCamera;
			IsKeepControlValue = true;
		}
	}

	// カメラの前方向を取得
	FVector ForwardDirection;
	// カメラの角度によってカメラの上方向、前方向を取得
	if (Camera->GetActorRotation().Pitch > 45.0f || Camera->GetActorRotation().Pitch < -45.0f)
	{
		ForwardDirection = Camera->GetActorUpVector();
	}
	else
	{
		ForwardDirection = Camera->GetActorForwardVector();
	}
	// カメラの横方向を取得
	FVector RightDirection = Camera->GetActorRightVector();

	// 移動
	player->AddMovementInput(ForwardDirection, invalue.Y);
	player->AddMovementInput(RightDirection, invalue.X);
	
	// 走っていないなら歩きに
	if (player->PlayerStatus != EPlayerStatus::Run)
	{
		player->PlayerStatus = EPlayerStatus::Walk;
	}
}

void UMoveInput::Run(APlayerCharacter* player)
{
	// 走れる状態か
	if (IsWalking && !IsCrouching)
	{
		player->SetActorTickEnabled(false);
		// スタミナがあるなら減少させる
		if (player->PlayerParam.Sanity > 0 && player->IsDoFall) player->PlayerParam.Sanity -= player->PlayerParam.SanityReductionValueAtIdling;
		UE_LOG(LogTemp, Warning, TEXT("%f") , player->PlayerParam.Sanity);

		// スタミナの量によって走るスピードを変更
		switch (player->GetSanityLevel())
		{
		case ESanityLevel::Full:
			player->GetCharacterMovement()->MaxWalkSpeed = player->PlayerParam.FullStaminaRunSpeed;
			player->PlayerStatus = EPlayerStatus::Run;
			break;

		case ESanityLevel::High:
			player->GetCharacterMovement()->MaxWalkSpeed = player->PlayerParam.HighStaminaRunSpeed;
			player->PlayerStatus = EPlayerStatus::Run;
			break;

		case ESanityLevel::Medium:
			player->GetCharacterMovement()->MaxWalkSpeed = player->PlayerParam.MediumStaminaRunSpeed;
			player->PlayerStatus = EPlayerStatus::Run;
			break;

		case ESanityLevel::Low:
			player->GetCharacterMovement()->MaxWalkSpeed = player->PlayerParam.LowStaminaRunSpeed;
			player->PlayerStatus = EPlayerStatus::Run;
			break;

		case ESanityLevel::Exhausted:
			player->PlayerStatus = EPlayerStatus::Sit;
			player->PlayerParam.Sanity -= 20;
			player->IsMoveable = false;
			IsCrouching = true;
			player->SetActorTickEnabled(true);
			break;
		}

		// 画面エフェクトの更新
		player->UpdateVignette();
	}
}

void UMoveInput::StopRunning(APlayerCharacter* player)
{
	player->SetActorTickEnabled(true);
	// 移動速度を歩きに戻す
	player->GetCharacterMovement()->MaxWalkSpeed = player->PlayerParam.WalkSpeed;
	// 座っていないなら
	if (!IsCrouching && player->PlayerStatus != EPlayerStatus::Resist)
	{
		// 状態をアイドルに
		player->PlayerStatus = EPlayerStatus::Idle;
	}
}

void UMoveInput::Squat(APlayerCharacter* player)
{
	// アニメーションの更新
	player->SetAnimation();
	// 動けるなら
	if (!IsCrouching && player->IsMoveable)
	{
		IsCrouching = true;
		player->IsMoveable = false;
		player->PlayerStatus = EPlayerStatus::Sit;
		if (!player->IsDisplayed)
		{
			TObjectPtr<URespectClockInstance> GameInstance;
			GameInstance = Cast<URespectClockInstance>(UGameplayStatics::GetGameInstance(this));
			GameInstance->WidgetInstance->DisplayMessage(player->ProgressStateText, EMsgType::Story);
			player->IsDisplayed = true;
			UE_LOG(LogTemp, Log, TEXT("aaaaa:%s"), *player->ProgressStateText);
		}
	}
	else
	{
		if(player->PlayerParam.Sanity > 20)
		{
			IsCrouching = false;
			player->IsMoveable = true;
			player->PlayerStatus = EPlayerStatus::Idle;
		}
	}
}

void UMoveInput::BeginBack(APlayerCharacter* player)
{
	// プレイヤーの後ろの方向を取得して後ろに歩かせ始める
	BackVector = -player->GetActorForwardVector();
	player->IsMoveable = false;
	player->PlayerStatus = EPlayerStatus::Walk;
	SetComponentTickEnabled(true);
}

void UMoveInput::MoveBack(float deltaTime)
{
	// プレイヤーをキャストして強制的に後ろに歩かせる
	APlayerCharacter* Player = Cast<APlayerCharacter>(GetOwner());
	if (Player) Player->AddMovementInput(BackVector, 0.5f);
	ElapsedTime += deltaTime;
	// 1秒移動したら移動を終わらせる
	if (ElapsedTime > 1.0f)
	{
		Player->PlayerStatus = EPlayerStatus::Idle;
		Player->IsMoveable = true;
		ElapsedTime = 0.0f;
		SetComponentTickEnabled(false);
	}
}
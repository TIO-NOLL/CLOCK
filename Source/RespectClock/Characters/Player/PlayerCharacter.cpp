//-------------------------------------------------------------------------------------
// ファイル名：PlayerCharacter.cpp
// 処理内容	：Playerの挙動を制御をするキャラクタークラス
// 作成者	：日向野大聖
// 作成日	：04/08
// 更新日	：04/08		プレイヤーの移動処理追加
//			　
//-------------------------------------------------------------------------------------


#include "PlayerCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "MoveInput.h"
#include "InteractiveInput.h"
#include "DogCommandInput.h"
#include "Inventory.h"
#include "RespectClock/Interactive/ItemBase.h"	// 05/10追加
//-----22cu0127 丹代天 2024/05/07 追記：カプセルコンポーネント、レイザーキラーインクルード-----
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "FramePro/FramePro.h"
#include "RespectClock/Characters/RazorKiller/RazorKiller.h"
#include "GameFramework/SpringArmComponent.h"
#include "RespectClock/Interactive/HideObject.h"
//-----------------------------------------------------------------------------

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// ATestClockTowerCharacter


FPlayerParam::FPlayerParam()
:WalkSpeed(300.0f)
,FullStaminaRunSpeed(400.0f)
,HighStaminaRunSpeed(375.0f)
,MediumStaminaRunSpeed(350.0f)
,LowStaminaRunSpeed(325.0f)
,SanityReductionValueAtSitting(0.1f)
,SanityReductionValueAtIdling(0.06f)
,SanityReductionValueAtWalking(0.03f)
,Sanity(100.0f)
{}

APlayerCharacter::APlayerCharacter()
	: DefaultMappingContext(nullptr)
	, MoveAction(nullptr)
	, RunAction(nullptr)
	, SitAction(nullptr)
	, InteractiveAction(nullptr)
	, DogAction(nullptr)
	, InventoryAction(nullptr)
	, Inventory(nullptr)
	, DeadZone(0.7f)
	, IsMoveable(true)
	, IsHiding(false)
	, IsResistible(false)
	, IsInteractable(false)
	, CanRecover(true)
	, PlayerStatus(EPlayerStatus::Idle)
	, HidePos(FVector::ZeroVector)
	, HideRot(FRotator::ZeroRotator)
	, IsItemUsing(false)
	, IsInventoryOpening(false)
	, InventorySlotNum(0)
	, InventorySlotNumMax(4)
{
	// CapsuleComponentの初期化
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// MovementComponentの初期化
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->MaxWalkSpeed = PlayerParam.WalkSpeed;
	
	MoveInput = CreateDefaultSubobject<UMoveInput>(TEXT("MoveInputComponent"));
	InteractiveInput = CreateDefaultSubobject<UInteractiveInput>(TEXT("InteractiveInputComponent"));
	DogCommandInput = CreateDefaultSubobject<UDogCommandInput>(TEXT("DogCommandInputComponent"));
	Inventory = CreateDefaultSubobject<UInventory>(TEXT("PlayerInventory"));

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));

	//-----22cu0127 丹代天 2024/05/07 追記：ヒットイベント関数登録-----
	m_pBoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Enemy"));
	if (m_pBoxComp)
	{
		m_pBoxComp->SetupAttachment(RootComponent);
	}
	//-----------------------------------------------------------------
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	SetActorTickEnabled(false);
	
	// マッピングコンテキストの設定
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	if (m_pBoxComp)
	{
		m_pBoxComp->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::OnHitActor);
	}

	GetCharacterMovement()->MaxWalkSpeed = PlayerParam.WalkSpeed;

	// 画面エフェクトの初期化
	UpdateVignette();
}

bool APlayerCharacter::IsGetResistance()
{
	return IsResistible;
}

ESanityLevel APlayerCharacter::GetSanityLevel() const
{
	if (PlayerParam.Sanity > static_cast<int32>(ESanityLevel::High)) {
		return ESanityLevel::Full;
	}
	else if (PlayerParam.Sanity > static_cast<int32>(ESanityLevel::Medium)) {
		return ESanityLevel::High;
	}
	else if (PlayerParam.Sanity > static_cast<int32>(ESanityLevel::Low)) {
		return ESanityLevel::Medium;
	}
	else if (PlayerParam.Sanity > static_cast<int32>(ESanityLevel::Exhausted)) {
		return ESanityLevel::Low;
	}
	else {
		return ESanityLevel::Exhausted;
	}
}


void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// スタミナの回復
	RecoverSanity();
	// 画面エフェクトの更新
	UpdateVignette();
}
// 入力
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// バインドアクションの設定
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) 
	{
		// Move
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &APlayerCharacter::StopMoving);
		// Run Press
		EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Run);
		// Run Release
		EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Completed, this, &APlayerCharacter::StopRunning);
		// Sit
		EnhancedInputComponent->BindAction(SitAction, ETriggerEvent::Started, this, &APlayerCharacter::Squat);
		// Interactive
		EnhancedInputComponent->BindAction(InteractiveAction, ETriggerEvent::Started, this, &APlayerCharacter::Interactive);
		EnhancedInputComponent->BindAction(InteractiveAction, ETriggerEvent::Completed, this, &APlayerCharacter::EndInteractive);
		// Dog
		EnhancedInputComponent->BindAction(DogAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Dog);
		// Item
		EnhancedInputComponent->BindAction(InventoryAction, ETriggerEvent::Started, this, &APlayerCharacter::InventoryOperarion);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void APlayerCharacter::Move(const FInputActionValue& Value)
{
	if (Controller != nullptr)
	{
		// 移動可能か
		if (IsMoveable)
		{
			// 入力値を変換
			FVector2D MovementVector = Value.Get<FVector2D>();
			// 移動
			if(MoveInput != nullptr) MoveInput->Move(this, MovementVector);
		}
	}
}

void APlayerCharacter::StopMoving()
{
	// 移動可能か
	if (IsMoveable)
	{
		// 移動の停止
		if (MoveInput != nullptr)MoveInput->IsWalking = false;
		if (PlayerStatus == EPlayerStatus::Walk || PlayerStatus == EPlayerStatus::Run) PlayerStatus = EPlayerStatus::Idle;
	}
}

void APlayerCharacter::Run()
{
	// 移動可能か
	if (IsMoveable)
	{
		// 走る
		if (MoveInput != nullptr)MoveInput->Run(this);
	}
}

void APlayerCharacter::StopRunning()
{
	if(MoveInput != nullptr) MoveInput->StopRunning(this);
}

void APlayerCharacter::Squat()
{
	if (MoveInput != nullptr)MoveInput->Squat(this);
}

void APlayerCharacter::Interactive()
{

	if (InteractiveInput != nullptr)
	{
		// プレイヤーの状態が抵抗以外か
		if (PlayerStatus != EPlayerStatus::Resist)
		{
			// インタラクティブ
			IsInteractable = true;
			InteractiveInput->Interactive(this);
		}
		// 抵抗
		else
		{
			InteractiveInput->Resist(this,RazorKiller);
		}
	}
	else return;
}

void APlayerCharacter::EndInteractive()
{
	IsInteractable = false;
}

void APlayerCharacter::dragout()
{
	Interactive();
}

void APlayerCharacter::Dog(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D InputVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		if(IsHiding)
		{
			if(HideObject)
			{
				// カメラの現在の回転を取得
				FRotator CurrentRotation = HideObject->GetHideCamera()->GetComponentRotation();

				// 入力からの変更量（例えば、マウスの動きやスティックの入力）
				float YawInput =InputVector.X/* プレイヤーからのYaw（左右）入力を取得 */;
				float PitchInput = -InputVector.Y/* プレイヤーからのPitch（上下）入力を取得 */;

				// 例えば、Sensitivityを定義してカメラの動きを滑らかにする
				const float Sensitivity = 0.2f;

				// 新しい回転を計算
				CurrentRotation.Yaw += YawInput * Sensitivity;
				CurrentRotation.Pitch = FMath::Clamp(CurrentRotation.Pitch + PitchInput * Sensitivity, -40.0f, 40.0f); // ピッチの制限

				// 新しい回転をカメラに適用
				HideObject->GetHideCamera()->SetWorldRotation(CurrentRotation);
			}
		}
	}
}

void APlayerCharacter::InventoryOperarion(const FInputActionValue& Value)
{
	// 入力値をFVector2Dに変換
	FVector2D InputVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// 上方向入力
		if (InputVector.Y < -DeadZone)
		{
			//UE_LOG(LogTemplateCharacter, Error, TEXT("十字キー上"), *GetNameSafe(this));
			BPCallInventoryOpen();
			IsInventoryOpening = !IsInventoryOpening;
			BPCallUpdateInventory(InventorySlotNum);
		}
		// 下方向入力
		else if (InputVector.Y > DeadZone)
		{
			//UE_LOG(LogTemplateCharacter, Error, TEXT("十字キー下"), *GetNameSafe(this));
			if (IsInventoryOpening && Inventory->MatchSlotNum(InventorySlotNum))
			{
				Inventory->UseItem(InventorySlotNum);
				Interactive();
				BPCallInventoryOpen();
				IsInventoryOpening = !IsInventoryOpening;
			}
			Inventory->LastUsedItem = nullptr;
			InventorySlotNum = 0;
			BPCallUpdateInventory(0);
		}
		// 左方向入力
		else if (InputVector.X < -DeadZone)
		{
			//UE_LOG(LogTemplateCharacter, Error, TEXT("十字キー左"), *GetNameSafe(this));
			if (IsInventoryOpening)
			{
				if (InventorySlotNum > 0)
					InventorySlotNum--;
				else
					InventorySlotNum = InventorySlotNumMax;
			}
			BPCallUpdateInventory(InventorySlotNum);
		}
		// 右方向入力
		else if (InputVector.X > DeadZone)
		{
			//UE_LOG(LogTemplateCharacter, Error, TEXT("十字キー右"), *GetNameSafe(this));
			if (IsInventoryOpening)
			{
				if (InventorySlotNum < InventorySlotNumMax)
					InventorySlotNum++;
				else
					InventorySlotNum = 0;
			}
			BPCallUpdateInventory(InventorySlotNum);
		}
	}
}

void APlayerCharacter::RecoverSanity()
{
	if(CanRecover)
	{
		// 正気度が最大値未満か
		if (PlayerParam.Sanity < 100)
		{
			// ステート毎で回復量を変化
			switch (PlayerStatus)
			{
			case EPlayerStatus::Sit:
				PlayerParam.Sanity += PlayerParam.SanityReductionValueAtSitting;
				break;

			case EPlayerStatus::Idle:
				PlayerParam.Sanity += PlayerParam.SanityReductionValueAtIdling;
				break;

			case EPlayerStatus::Walk:
				PlayerParam.Sanity += PlayerParam.SanityReductionValueAtWalking;
				break;
			}
			// 画面エフェクトの更新
			UpdateVignette();
			UE_LOG(LogTemplateCharacter, Error, TEXT("%f"), PlayerParam.Sanity);
		}
		else
		{
			// 正気度が最大なら回復を停止
			SetActorTickEnabled(false);
		}
	}
}

void APlayerCharacter::Hide()
{
	SetActorLocation(HidePos);
	SetActorRotation(HideRot);
}

void APlayerCharacter::AddSanity(float value)
{
	// 正気度を増加させる
	PlayerParam.Sanity += value;
	// エフェクトの更新
	UpdateVignette();
	if(PlayerParam.Sanity < 0)
	{
		PlayerParam.Sanity = 0;
	}
}

//-----22cu0127 丹代天 2024/05/07 追記：関数定義-----
// 衝突判定
void APlayerCharacter::OnHitActor(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IsEnemy(OtherActor) && m_bCollisionCheck)
	{
		if (bInvincible == true) return;

		// 抵抗しないならゲームオーバー
		if(!IsDoResist) GameOverDispatcer.Broadcast();
		// 抵抗開始
		PlayerStatus = EPlayerStatus::Resist;
		InteractiveInput->SetComponentTickEnabled(true);
		IsMoveable = false;
		CanRecover = false;
		// 抵抗(田中)追加 07/05
		RazorKiller->Resisted();
		// UIの表示
		SetResistWidgetVisibility(true);
	}
	else if (OtherActor->ActorHasTag("Back"))
	{
		MoveInput->BeginBack(this);
	}
}

// エネミー判断
bool APlayerCharacter::IsEnemy(AActor* OtherActor)
{
	if (OtherActor)
	{
		// 衝突したアクターがARazorKillerクラスのインスタンスであるかをチェックする
		if (OtherActor->IsA(ARazorKiller::StaticClass()))
		{
			// 抵抗(田中)追加 07/05
			RazorKiller = Cast<ARazorKiller>(OtherActor);
			//デバッグ用ログ
			//UE_LOG(LogTemp, Warning, TEXT("Correct Now"));
			return true;
		}
		else
		{
			return false;
		}
	}
	return false;
}

void APlayerCharacter::Respawn(FVector _respawnLocation)
{
	// 抵抗成功扱いにする
	InteractiveInput->SuccessResist(this, RazorKiller);
	// パニックエフェクトの更新
	UpdateVignette();
	// 座標移動
	SetActorLocation(_respawnLocation);
}

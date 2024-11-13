//-------------------------------------------------------------------------------------
// ファイル名：PlayerCharacter.h
// 処理内容	：Playerの挙動を制御をするキャラクタークラス
// 作成者	：日向野大聖
// 作成日	：04/08
// 更新日	：04/08		プレイヤーの移動処理追加
//			　
//-------------------------------------------------------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "E_PlayerState.h"
#include "RespectClock/Characters/Player/ResistibleInterface.h"
#include "Camera/CameraComponent.h"
#include "PlayerCharacter.generated.h"

//前方宣言
class UInputMappingContext;
class UInputAction;
class UMoveInput;
class UInteractiveInput;
class UDogCommandInput;
class AItemBase;	// 05/10追加
class UBoxComponent;
struct FInputActionValue;
class UInventory;
class ARazorKiller;	// 抵抗(田中)追加 07/05
class UCameraComponent;
class AHideObject;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

// プレイヤーのパラメータ構造体
USTRUCT(BlueprintType)
struct FPlayerParam
{
	GENERATED_BODY()
	
	/// @brief 歩きの速度
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CharaState")
	float WalkSpeed;
	/// @brief 走りの速度スタミナ100
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CharaState")
	float FullStaminaRunSpeed;
	/// @brief 走りの速度スタミナ75
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CharaState")
	float HighStaminaRunSpeed;
	/// @brief 走りの速度スタミナ50
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CharaState")
	float MediumStaminaRunSpeed;
	/// @brief 走りの速度スタミナ25
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CharaState")
	float LowStaminaRunSpeed;
	/// @brief しゃがみ時の正気度の上昇量
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CharaState")
	float SanityReductionValueAtSitting;
	/// @brief 立ち時の正気度の上昇量
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CharaState")
	float SanityReductionValueAtIdling;
	/// @brief 歩き時の正気度の上昇量
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CharaState")
	float SanityReductionValueAtWalking;
	/// @brief 正気度（実質HP）
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CharaState")
	float Sanity;

	FPlayerParam();
};

UCLASS(config = Game)
class RESPECTCLOCK_API APlayerCharacter : public ACharacter, public IResistibleInterface
{

	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* RunAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SitAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* InteractiveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* DogAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* InventoryAction;

public:
	APlayerCharacter();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/// @brief フェード処理
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void InFade();
	/// @brief ヴィネットの更新
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void UpdateVignette();
	/// @brief アニメーションの設定
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetAnimation();
	/// @brief 抵抗UIの表示非表示
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetResistWidgetVisibility(bool _isibility);
	/// @brief キャラクターの表示非表示
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetHidden();
	
	/*
	 * 取得メソッド
	 */
	UFUNCTION(BlueprintPure, BlueprintCallable)
	float GetWalkSpeed() const{ return PlayerParam.WalkSpeed; }
	UFUNCTION(BlueprintPure, BlueprintCallable)
	float GetFullStaminaRunSpeed() const{ return PlayerParam.FullStaminaRunSpeed; }
	UFUNCTION(BlueprintPure, BlueprintCallable)
	float GetHighStaminaRunSpeed() const{ return PlayerParam.HighStaminaRunSpeed; }
	UFUNCTION(BlueprintPure, BlueprintCallable)
	float GetMediumStaminaRunSpeed() const{ return PlayerParam.MediumStaminaRunSpeed; }
	UFUNCTION(BlueprintPure, BlueprintCallable)
	float GetLowStaminaRunSpeed() const{ return PlayerParam.LowStaminaRunSpeed; }
	UFUNCTION(BlueprintPure, BlueprintCallable)
	float GetSanityReductionValueAtSitting() const{ return PlayerParam.SanityReductionValueAtSitting; }
	UFUNCTION(BlueprintPure, BlueprintCallable)
	float GetSanityReductionValueAtIdling() const{ return PlayerParam.SanityReductionValueAtIdling; }
	UFUNCTION(BlueprintPure, BlueprintCallable)
	float GetSanityReductionValueAtWalking() const{ return PlayerParam.SanityReductionValueAtWalking; }
	UFUNCTION(BlueprintPure, BlueprintCallable)
	float GetSanity() const{ return PlayerParam.Sanity; }
	UFUNCTION(BlueprintPure, BlueprintCallable)
	UCameraComponent* GetCamera() const{ return CameraComp; }
	
protected:
	/// @brief 移動
	void Move(const FInputActionValue& MoveValue);
	/// @brief 移動停止
	void StopMoving();
	/// @brief 走り
	void Run();
	/// @brief 走り停止
	void StopRunning();
	/// @brief しゃがみ
	void Squat();
	/// @brief インタラクト
	void Interactive();
	/// @brief インタラクト終了
	void EndInteractive();
	/// @brief 犬への指示
	void Dog(const FInputActionValue& InputValue);
	/// @brief インベントリ操作
	void InventoryOperarion(const FInputActionValue& InputValue);

public:
	/// @brief 隠れる
	UFUNCTION(BlueprintCallable, category = "CharacterAction")
	void Hide();

	UFUNCTION(BlueprintCallable, category = "CharacterAction")
	void dragout();
	
private:
	/// @brief スタミナ回復
	void RecoverSanity();

public:
	/// @brief 抵抗成功かどうか
	/// @return 抵抗成功？
	bool IsGetResistance();
	/// @brief 現在のスタミナの状態を返す
	/// @return スタミナの状態
	UFUNCTION(BlueprintCallable)
	ESanityLevel GetSanityLevel() const;
	/// @brief パニック値を増加させる
	void AddSanity(float value);
	/// @brief inventory取得
	UInventory* GetInventory() { return Inventory; }

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// To add mapping context
	virtual void BeginPlay();

	UPROPERTY(EditAnyWhere, BlueprintReadOnly)
	UInventory* Inventory;	// インベントリ

private:
	/// @brief 入力のデッドゾーン
	UPROPERTY(EditAnywhere, Category = "CharaState")
	float DeadZone;

	/*
	 * コンポーネント
	 */
	UPROPERTY(EditAnywhere)
	UMoveInput* MoveInput;
	UPROPERTY(EditAnywhere)
	UInteractiveInput* InteractiveInput;
	UPROPERTY(EditAnywhere)
	UDogCommandInput* DogCommandInput;
	UPROPERTY(EditAnywhere)
	UCameraComponent* CameraComp;
	
public:
	/*
	 * フラグ
	 */
	/// @brief 移動可能か
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsMoveable;
	/// @brief 隠れているか
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsHiding;
	/// @brief 抵抗可能か
	UPROPERTY()
	bool IsResistible;
	/// @brief インタラクト可能か
	UPROPERTY()
	bool IsInteractable;
	/// @brief 回復可能か
	UPROPERTY()
	bool CanRecover;
	
	/// @brief プレイヤーのパラメータ
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CharaState")
	FPlayerParam PlayerParam;
	/// @brief プレイヤーの状態
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EPlayerStatus PlayerStatus;
	/// @brief 隠れる座標
	UPROPERTY()
	FVector HidePos;
	/// @brief 隠れる角度
	UPROPERTY()
	FRotator HideRot;
	/// @brief ハイドオブジェ
	AHideObject* HideObject;
	

	/*
	 * デバッグ用
	 */
	/// @brief 抵抗するかどうか
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
	bool IsDoResist = true;
	/// @brief 転ぶかどうか
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
	bool IsDoFall = true;
	/// @brief キラーとの当たり判定をつけるか
	UPROPERTY(EditAnywhere, Category = "CollisionCheck")
	bool m_bCollisionCheck = false;

	//-----22cu0127 丹代天 2024/05/07 追記：衝突判定・ゲームオーバー時に呼び出すイベントディスパッチャー-----
// イベントディスパッチャー
protected:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGameOverDispatcer);
public:
	UPROPERTY(BlueprintAssignable, Category = "Custom Events")
	FGameOverDispatcer GameOverDispatcer;

private:
	/// @brief 衝突判定
	UFUNCTION()
	void OnHitActor(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	/// @brief エネミー判断
	bool IsEnemy(AActor* OtherActor);
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBoxComponent* m_pBoxComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bInvincible = false;

	//------------------------------------------------------------------------------------------------------

	// カスコード(田中)
	UPROPERTY()
	TObjectPtr<ARazorKiller> RazorKiller;

	// インベントリ開閉操作
	UFUNCTION(BlueprintImplementableEvent)
	void BPCallInventoryOpen();


	//------------------------------------------------------------------------------------------------------
	/// @brief アイテム使用中か？
	UPROPERTY()
	bool IsItemUsing;

	/// @brief インベントリ開閉フラグ
	UPROPERTY()
	bool IsInventoryOpening;
	/// @brief 選択中のインベントリのスロット番号
	UPROPERTY(BlueprintReadWrite)
	int32 InventorySlotNum;
	/// @brief インベントリスロットの最大数
	UPROPERTY()
	int32 InventorySlotNumMax;
	/// @brief インベントリ更新処理
	UFUNCTION(BlueprintImplementableEvent)
	void BPCallUpdateInventory(int32 _SlotNum);
	//------------------------------------------------------------------------------------------------------

	/// @brief 現在の進行状況テキスト
	FString ProgressStateText;
	/// @brief 同じテキストを2度表示しない
	bool IsDisplayed;

	/// @brief リスポーン処理
	UFUNCTION()
	void Respawn(FVector _respawnLocation);
};

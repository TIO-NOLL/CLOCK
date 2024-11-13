// ItemBase.h

#pragma once

#include "CoreMinimal.h"
#include "InteractiveObject.h"
#include "ItemData.h"
#include "ItemBase.generated.h"

class APlayerCharacter;

// 仮イベント
UENUM(BlueprintType)
enum class EEventStatusItem : uint8
{
	NONE		UMETA(DisplayName = "NONE"),
	Killer      UMETA(DisplayName = "Killer"),
	NASHI		UMETA(DisplayName = "NASHI"),
	NAIYO		UMETA(DisplayName = "ABC"),
};

UCLASS()
class RESPECTCLOCK_API AItemBase : public AInteractiveObject
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemBase();

protected:
	// Called when the game starts or when spawned
	void BeginPlay();

	// ゲーム開始時に表示するかどうか
	UPROPERTY(BlueprintReadWrite, EditInstanceOnly, Category = "ItemData", DisplayName = "ゲーム開始時に非表示")
	bool IsActiveBeginPlay;

	// インタラクトされたときに呼ばれる関数
	UFUNCTION()
	void Interactive() override;

	// プレイヤー状態を返す
	EPlayerStatus GetStatus() override;

	// アイテムがインベントリに追加されたときの処理
	// 取ったらイベントやアイテムの削除などを行う
	virtual void AddedToInventory();

	// アイテムデータ
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "ItemData", DisplayName = "アイテムデータ")
	FItemData ItemData;
	// 行名
	UPROPERTY(BlueprintReadWrite, EditInstanceOnly, meta = (GetOptions = "GetRowNames"), Category = "ItemData", DisplayName = "アイテム名")
	FName RowName;
	// データテーブル
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "ItemData")
	TObjectPtr<UDataTable> DataTable;

	UPROPERTY()
	APlayerCharacter* Player;	// プレイヤーを取得するため

	// イベントを呼び出し可能か
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ItemData", DisplayName = "イベント呼び出し可能？")
	bool IsEventCallable;

	// BPで呼び出す用　いずれイベントデータに対応したら消す
	UFUNCTION(BlueprintImplementableEvent)
	void BPCallEventItem();

public:	

	// アイテムを取ったときのアニメーション、座標、回転
	FRotator GetLerpPointRot() override;
	FVector GetLerpPointPos() override;

	// 情報取得
	UFUNCTION()
	int32 GetID() const { return ItemData.ID; }	
	UFUNCTION()
	FText GetDescription() const { return ItemData.Description; }

	// アイテム設定用関数
	UFUNCTION()
	TArray<FName> GetRowNames() const;
	
	// 仮イベントデータ
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ItemData", meta = (EditCondition = "IsEventCallable"))
	EEventStatusItem EventData;

	// アイテム有無効化処理
	UFUNCTION()
	void ActiveItem(bool _IsActive);
};

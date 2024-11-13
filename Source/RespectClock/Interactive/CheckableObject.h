// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractiveObject.h"
#include "ObjectData.h"
#include "CheckableObject.generated.h"

class APlayerCharacter;


// 仮イベント
UENUM(BlueprintType)
enum class EEventStatus : uint8
{
	NONE		UMETA(DisplayName = "NONE"),
	Killer      UMETA(DisplayName = "Killer"),
	NASHI		UMETA(DisplayName = "NASHI"),
};

UCLASS()
class RESPECTCLOCK_API ACheckableObject : public AInteractiveObject
{
	GENERATED_BODY()
	
public:
	ACheckableObject();
	void BeginPlay() override;

protected:
	// オブジェクトデータ
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "ObjectData")
	FObjectData ObjectData;
	// 行名
	UPROPERTY(BlueprintReadWrite, EditInstanceOnly, meta = (GetOptions = "GetRowNames"), Category = "ObjectData", DisplayName = "オブジェクト名")
	FName RowName;
	// データテーブル
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "ObjectData")
	TObjectPtr<UDataTable> DataTable;
	// イベントを呼び出し可能か
	UPROPERTY(BlueprintReadWrite,EditAnywhere, Category = "ObjectData", DisplayName = "イベント呼び出し可能？")
	bool IsEventCallable;

	// イベント呼び出し
	UFUNCTION(BlueprintCallable)
	void CallEvent();

	// BPで呼び出す用　いずれイベントデータに対応したら消す
	UFUNCTION(BlueprintImplementableEvent)
	void BPCallEvent();

	APlayerCharacter* Player;
	void Interactive() override;

public:
	virtual EPlayerStatus GetStatus() override;
	virtual FRotator GetLerpPointRot() override;
	virtual FVector GetLerpPointPos() override;

	UFUNCTION()
	TArray<FName> GetRowNames() const;

// ここをイベントアセットのデータを渡すやつにする
// イベント名取得してそれ設定するだけで渡せるようにするとか
//protected:
//	// 仮のイベントデータ用変数
//	UPROPERTY(BlueprintReadWrite, EditInstanceOnly, meta = (GetOptions = "GetEventNames"), Category = "ObjectData", meta = (EditCondition = "IsEventCallable"), DisplayName = "イベント名")
//	FString EventName;

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ObjectData", meta = (EditCondition = "IsEventCallable"))
	EEventStatus EventData;
};

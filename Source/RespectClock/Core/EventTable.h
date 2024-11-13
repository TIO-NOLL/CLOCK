//--------------------------------------------------------------------------------------
// ファイル名：EventTable.h
// 内容		：イベント関係データテーブルのベースクラス
// 作成者	：22cu0127 丹代天
// 更新日	：2024/06/20		作成
//--------------------------------------------------------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Sound/SoundBase.h"
#include "LevelSequence.h"
#include "RespectClock/Interactive/ItemBase.h"
#include "RespectClock/Interactive/Door.h"
#include "EventTable.generated.h"

UENUM(BlueprintType)
enum class EEventType : uint8
{
    None = 0,
    Sound,
    Killer,
    Effect,
    Sequence,
    EntryRoom,
    Item,
    Strolling,
    Respawn,
    ProgressState,
};

USTRUCT(BlueprintType)
struct FSoundValue
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 ID;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Name;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector Location;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Volume;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Pitch;

    FSoundValue();
};

USTRUCT(BlueprintType)
struct FItemActivate
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSoftObjectPtr<AItemBase> Item;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool IsActivate;

    FItemActivate()
        : Item(nullptr), IsActivate(false)
    {}
};

USTRUCT(BlueprintType)
struct FEventTableBase : public FTableRowBase
{
    GENERATED_BODY()

public:

    FEventTableBase()
        : ID(0), Name(TEXT("")), eType(EEventType::None)
    {}

    //ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Event")
    int32 ID;
    //名前
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Event")
    FString Name;
    //イベントタイプ
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Event")
    EEventType eType;
//-------------↓イベント発火に必要なあれこれ,場合によっては変更します-----------
    //サウンド系
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DedicatedEventsValue")
    FSoundValue SoundValue;
    
    //エフェクト系


    //シーケンサ系
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DedicatedEventsValue")
    ULevelSequence* SequenceActor;

    //部屋通知系
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DedicatedEventsValue|Room")
    FString RoomName;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DedicatedEventsValue|Room")
    TSoftObjectPtr<ADoor> DoorObj;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DedicatedEventsValue|Room")
    FString DoorName;

    //散策用のActorを格納
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DedicatedEventsValue|Room")
    TArray<TSoftObjectPtr<AActor>> StrollingActors;

    //アイテム系
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DedicatedEventsValue|Item")
    TSoftObjectPtr<AItemBase> Item;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DedicatedEventsValue|Item")
    bool IsActivate;

    // リスポーン
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DedicatedEventsValue|Item")
    FVector RespawnPos;

    // 進行状況テキスト
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DedicatedEventsValue|Item")
    FString ProgressText;

#if WITH_EDITOR
    virtual void OnDataTableChanged(const UDataTable* InDataTable, const FName InRowName) override;
#endif
};
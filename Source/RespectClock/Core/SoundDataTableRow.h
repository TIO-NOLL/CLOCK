//--------------------------------------------------------------------------------------
// ファイル名：SoundDataTableRow.h
// 内容		：サウンドを管理するデータテーブル用の構造体
// 作成者	：22cu0127 丹代天
// 更新日	：2024/05/30		作成
//--------------------------------------------------------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Sound/SoundBase.h"
#include "SoundDataTableRow.generated.h"

USTRUCT(BlueprintType)
struct FSoundDataTableRow : public FTableRowBase
{
    GENERATED_BODY()

public:
    FSoundDataTableRow()
        : ID(0), Name(TEXT("")), SoundObj(nullptr)
    {}

    // データテーブルのカラムに対応するプロパティを定義
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
    int32 ID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
    FString Name;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
    USoundBase* SoundObj;
};
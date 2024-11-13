//--------------------------------------------------------------------------------------
// ファイル名：SoundDataAsset.h
// 内容		：SoundDataTableRowのテータテーブルを保持、リインポートができるデータアセット
// 作成者	：22cu0127 丹代天
// 更新日	：2024/05/30		作成、リインポート用の関数追加
//--------------------------------------------------------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Engine/DataTable.h"
#include "SoundDataTableRow.h"
#include "SoundDataAsset.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class RESPECTCLOCK_API USoundDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
protected:
	// データテーブルへの参照
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SoundDataTable")
	UDataTable* SoundDataTable;

public:
	//サウンドデータ（TArrayだったりTMapだったりのデータ配列）
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TArray<FSoundDataTableRow> SoundDataMap;

	// データ取り込み用ファンクション
	UFUNCTION(CallInEditor)
	void LoadData();
};

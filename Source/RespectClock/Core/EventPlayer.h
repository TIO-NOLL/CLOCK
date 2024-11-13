//--------------------------------------------------------------------------------------
// ファイル名：EventPlayer.cpp
// 内容		：EventDataTableAssetからデータを受け取り、ID又は名前から検索してEventをプレイするクラス
// 作成者	：22cu0127 丹代天
// 更新日	：2024/07/12		作成
//--------------------------------------------------------------------------------------
#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "EventTable.h"
#include "EventPlayer.generated.h"

//前方宣言
class AAIC_RazorKiller;

UCLASS()
class RESPECTCLOCK_API UEventPlayer : public UObject
{
	GENERATED_BODY()

public:
	//シングルトンインスタンスを取得
	UFUNCTION(BlueprintCallable, Category = "EventPlayer")
	static UEventPlayer* GetInstance(UObject* WorldContextObject);

	//イベント再生関数
	UFUNCTION(BlueprintCallable, Category = "EventPlayer")
	void PlayEventByName(const UWorld* World, const FString& _name);
	UFUNCTION(BlueprintCallable, Category = "EventPlayer")
	void PlayEventByID(const UWorld* World, int32 _iD);

	//マップをキャッシュする関数
	void CacheEventData(const UWorld* World);

	void SetKillerController(const UWorld* World);

private:
	UEventPlayer();

	//シングルトンインスタンス
	static UEventPlayer* Instance;

	//イベントデータマップ
	TMap<FString, FEventTableBase> NameToEventDataMap;
	TMap<int32, FEventTableBase> IDToEventDataMap;

	AAIC_RazorKiller* KillerController;
	TArray<AActor*> ActorArray;
};

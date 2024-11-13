//--------------------------------------------------------------------------------------
// ファイル名：GameObject.h
// 内容		：ゲームのゴールを示すオーバーラップコリジョン
// 作成者	：22cu0127 丹代天
// 更新日	：2024/05/10		作成、ゲームクリアのイベントを作成
//--------------------------------------------------------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "OverlapObject.h"
#include "GoalObject.generated.h"

//前方宣言
class AGM_RespectClock;

UCLASS()
class RESPECTCLOCK_API AGoalObject : public AOverlapObject
{
	GENERATED_BODY()
	
public:
	//開始時の処理
	void BeginPlay()override;

private:
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	AGM_RespectClock* CurrentGameMode;

	bool IsPlayer(AActor* OtherActor);

	bool IsPlayedOnce = false;
};

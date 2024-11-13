//--------------------------------------------------------------------------------------
// ファイル名：GM_RespectClock.h
// 内容		：オーバーラップできるオブジェクト。プレイヤーが触れた際にイベントを起こす
// 作成者	：22cu0127 丹代天
// 更新日	：2024/04/23		作成
//--------------------------------------------------------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OverlapObject.generated.h"

//前方宣言
class UBoxComponent;

UCLASS()
class RESPECTCLOCK_API AOverlapObject : public AActor
{
	GENERATED_BODY()
	
public:	
	//コンストラクタ
	AOverlapObject();

protected:
	//生成時の処理
	virtual void BeginPlay() override;

public:
	// オーバーラップ開始時の処理
	UFUNCTION()
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UBoxComponent* m_pBoxComp;

	bool m_bOncePlay;
};

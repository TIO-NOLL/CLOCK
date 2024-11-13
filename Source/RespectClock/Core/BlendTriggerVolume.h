//--------------------------------------------------------------------------------------
// ファイル名	：BlendTriggerVolume.h
// 内容			：トリガーボックスの作成
// 作成者		：22cu0236 八町陸斗
// 更新日		：2024/04/09		作成
//				：2024/04/12 NewCamera の追加
//				：2024/04/16 Target の追加
//--------------------------------------------------------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BlendTriggerVolume.generated.h"

UCLASS()
class RESPECTCLOCK_API ABlendTriggerVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABlendTriggerVolume();

protected:
	// アクターボリュームのコリジョン境界
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class UBoxComponent* OverlapVolume;

	// 見つけたカメラを管理
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ACameraActor> CameraToFind;

	// カメラのトランジションのブレインド時間
	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = 0.0f))
	float CameraBlendTime;

	// OverlapVolume アクターのための Begin および End Overlap メソッド
	virtual void NotifyActorBeginOverlap(AActor* OtherActor);
	virtual void NotifyActorEndOverlap(AActor* OtherActor);

};

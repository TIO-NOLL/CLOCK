//--------------------------------------------------------------------------------------
// ファイル名：AIC_Dog.h
// 内容		：犬の動きを制御するAIコントローラー
// 作成者	：22cu0127 丹代天
// 更新日	：2024/04/08		作成
//			：2024/04/09		ブラックボードキーへの書き込み
//			：2024/04/23		Enumを更新する処理を追加
//--------------------------------------------------------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AIC_Dog.generated.h"

//前方宣言
class UBlackboardComponent;

UENUM(BlueprintType)
enum class EDogState : uint8
{
	None = 0,
	Tracking UMETA(DisplayName = "TRACKING"),
	Stay UMETA(DisplayName = "STAY"),
	Stroke UMETA(DisplayName = "STROKE"),
	Imidiation UMETA(DisplayName = "IMIDIATION"),
};

UCLASS()
class RESPECTCLOCK_API AAIC_Dog : public AAIController				
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

private:
	UBlackboardComponent* m_pBlackBoardComponent;

public:
	void SwitchDogState(EDogState _state);

};

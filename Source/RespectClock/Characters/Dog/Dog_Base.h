//--------------------------------------------------------------------------------------
// ファイル名：Dog_Base.h
// 内容		：プレイヤーをサポートするNPCのベースクラス
// 作成者	：22cu0127 丹代天
// 更新日	：2024/04/08		作成
//--------------------------------------------------------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Dog_Base.generated.h"

UCLASS()
class RESPECTCLOCK_API ADog_Base : public ACharacter
{
	GENERATED_BODY()

public:
	// コンストラクタ
	ADog_Base();

protected:
	// 生成時の処理
	virtual void BeginPlay() override;

public:	
	// フレームごとの処理
	virtual void Tick(float DeltaTime) override;
};

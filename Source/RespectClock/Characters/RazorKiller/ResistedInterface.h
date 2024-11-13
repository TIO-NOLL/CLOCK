/*
* FileName	: ResistedInterface.h
* Created	: Misora Tanaka
* Summary	: キラーがプレイヤーと抵抗処理のやりとりをするためのインターフェース
* Update	: 24/05/17 作成
*			: 24/07/04 Failed(抵抗失敗時)のメソッドを追加
*/

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ResistedInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UResistedInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 *
 */
class RESPECTCLOCK_API IResistedInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	// 抵抗開始
	virtual void Resisted() = 0;
	// 抵抗完了
	virtual void CompResisted() = 0;
	// 抵抗失敗(今後追加される予定のアニメーション用)
	virtual void FailedResisted() = 0;
	
};
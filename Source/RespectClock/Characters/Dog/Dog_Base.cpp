//--------------------------------------------------------------------------------------
// ファイル名：Dog_Base.cpp
// 内容		：プレイヤーをサポートするNPCのベースクラス
// 作成者	：22cu0127 丹代天
// 更新日	：2024/04/08		作成
//--------------------------------------------------------------------------------------

#include "Dog_Base.h"

// コンストラクタ
ADog_Base::ADog_Base()
{
 	//Tickを使うかどうか判別
	PrimaryActorTick.bCanEverTick = true;
}

// 生成時の処理
void ADog_Base::BeginPlay()
{
	Super::BeginPlay();
	
}

// フレームごとの処理
void ADog_Base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
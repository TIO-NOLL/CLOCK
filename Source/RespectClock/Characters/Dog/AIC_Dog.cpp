//--------------------------------------------------------------------------------------
// ファイル名：AIC_Dog.cpp
// 内容		：犬の動きを制御するAIコントローラー
// 作成者	：22cu0127 丹代天
// 更新日	：2024/04/08		作成
//			：2024/04/09		ブラックボードキーへの書き込み
//			：2024/04/23		Enumを更新する処理を追加
//--------------------------------------------------------------------------------------

#include "AIC_Dog.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

void AAIC_Dog::BeginPlay()
{
	Super::BeginPlay();

	m_pBlackBoardComponent = GetBlackboardComponent();
	if (m_pBlackBoardComponent)
	{
		m_pBlackBoardComponent->SetValueAsObject(TEXT("PlayerActor"), UGameplayStatics::GetPlayerPawn(this, 0));
		m_pBlackBoardComponent->SetValueAsEnum(TEXT("DogState"), static_cast<uint8>(EDogState::None));
	}
}

void AAIC_Dog::SwitchDogState(EDogState _state)
{
	if (m_pBlackBoardComponent)
	{
		m_pBlackBoardComponent->SetValueAsEnum(TEXT("DogState"), static_cast<uint8>(_state));
	}
}
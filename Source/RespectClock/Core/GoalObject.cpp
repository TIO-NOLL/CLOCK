///--------------------------------------------------------------------------------------
// ファイル名：GameObject.cpp
// 内容		：ゲームのゴールを示すオーバーラップコリジョン
// 作成者	：22cu0127 丹代天
// 更新日	：2024/05/10		作成、ゲームクリアのイベントを作成
//--------------------------------------------------------------------------------------

#include "GoalObject.h"
#include "GM_RespectClock.h"
#include "Engine/World.h"
#include "RespectClock/Characters/Player/PlayerCharacter.h"

//開始時処理
void AGoalObject::BeginPlay()
{
	Super::BeginPlay();

	UWorld* world = GetWorld();
	AGameModeBase* gameModeBase = world->GetAuthGameMode();
	CurrentGameMode = Cast<AGM_RespectClock>(gameModeBase);
}

void AGoalObject::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	//オーバーラップした際に実行したいイベント
	if (OtherActor && (OtherActor != this) && OtherComp && IsPlayer(OtherActor))
	{
		//一回だけ
		if (IsPlayedOnce == false)
		{
			CurrentGameMode->GameClearDispatcer.Broadcast();
			IsPlayedOnce = true;
		}
	}
}

bool AGoalObject::IsPlayer(AActor* OtherActor)
{
	if (OtherActor)
	{
		// 衝突したアクターがAAplayerCharacterクラスのインスタンスであるかをチェックする
		return OtherActor->IsA(APlayerCharacter::StaticClass());
	}
	return false;
}
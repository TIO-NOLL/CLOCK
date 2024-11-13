/*
* FileName	: AIC_RazorKiller.cpp
* Created	: Misora Tanaka
* Summary	: キラーの頭脳を制御する
* Update	: ヘッダファイルに記載
*/

#include "AIC_RazorKiller.h"
#include "AIController.h"
#include "RazorKiller.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "RespectClock/Core/WarpPoint.h"
#include "UObject/ConstructorHelpers.h"
#include "RespectClock/Interactive/Door.h"

AAIC_RazorKiller::AAIC_RazorKiller(const class FObjectInitializer& ObjectInitializer)
{
	BehaviorComp = ObjectInitializer.CreateDefaultSubobject<UBehaviorTreeComponent>(this, TEXT("BehaviorComp"));
	BlackboardComp = ObjectInitializer.CreateDefaultSubobject<UBlackboardComponent>(this, TEXT("BlackboardComp"));

	ConstructorHelpers::FObjectFinder<UBehaviorTree> const BTFinder(TEXT("/Game/RespectClock/Characters/RazorKiller/AI/BT_RazorKiller_Main.BT_RazorKiller_Main"));
	BehaviorTree = BTFinder.Object;
}


void AAIC_RazorKiller::BeginPlay()
{
	Super::BeginPlay();

	//-----2024/09/03 22cu0127 丹代天 記述：ゲーム開始時、タグで検索をかけてすべてのワープポイントを保有する-----
	//-----2024/09/06 22cu0127 丹代天 記述：開始時にマップ全体のドアのポインタと所属するレベルインスタンス名をキーとするMap配列-----

	TArray<AActor*> FoundActors;

	// ワールド内のすべてのアクターを取得
	//WarpPoint
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), FoundActors);
	//キャストし、それぞれ配列へ代入する
	for (AActor* Actor : FoundActors)
	{
		//タグでフィルタリングしてCastする
		if (Actor->ActorHasTag("place"))
		{
			AWarpPoint* TargetActor = Cast<AWarpPoint>(Actor);
			if (TargetActor)
			{
				m_AWarpPoints.Add(TargetActor);
			}
		}
	}

	//// ワールド内のすべてのアクターを取得
	////Door
	//UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADoor::StaticClass(), FoundActors);
	//TMap<FString, int32> RoomInstanceCounter;

	//// 各アクタを処理
	//for (AActor* Actor : FoundActors)
	//{
	//	ADoor* Door = Cast<ADoor>(Actor);
	//	if (Door)
	//	{
	//		ULevel* Level = Door->GetLevel();
	//		if (Level)
	//		{
	//			// レベル名を取得
	//			FString LevelName = Level->GetOutermost()->GetName();

	//			// 「LI_」で始まる部屋名を抽出
	//			FString RoomName;
	//			if (LevelName.Split(TEXT("LI_"), nullptr, &RoomName))
	//			{
	//				FString ExtractedName;
	//				RoomName.Split(TEXT("_"), &ExtractedName, nullptr);

	//				// 「LI_」を含めた部屋名を作成
	//				FString BaseRoomName = TEXT("LI_") + ExtractedName;

	//				// ナンバリング処理
	//				int32 InstanceNumber = 1;
	//				FString FinalRoomName = BaseRoomName;
	//				if (RoomInstanceCounter.Contains(BaseRoomName))
	//				{
	//					InstanceNumber = RoomInstanceCounter[BaseRoomName] + 1;
	//					RoomInstanceCounter[BaseRoomName] = InstanceNumber;
	//					FinalRoomName = FString::Printf(TEXT("%s%d"), *BaseRoomName, InstanceNumber);
	//				}
	//				else
	//				{
	//					RoomInstanceCounter.Add(BaseRoomName, InstanceNumber);
	//				}

	//				// TMapに追加
	//				if (!DoorMap.Contains(FinalRoomName))
	//				{
	//					DoorMap.Add(FinalRoomName, TArray<TObjectPtr<ADoor>>());
	//				}
	//				DoorMap[FinalRoomName].Add(Door);
	//			}
	//		}
	//	}
	//}

	//// デバッグ用: TMapの内容をログに出力
	//for (const auto& Elem : DoorMap)
	//{
	//	UE_LOG(LogTemp, Log, TEXT("Room: %s"), *Elem.Key);
	//	for (const TObjectPtr<ADoor>& Door : Elem.Value)
	//	{
	//		UE_LOG(LogTemp, Log, TEXT("  Door Actor: %s"), *Door->GetName());
	//	}
	//}

	//FName TagName = "Strolling";
	//UGameplayStatics::GetAllActorsWithTag(GetWorld(), TagName, FoundActors);

	//for (AActor* Actor : FoundActors)
	//{
	//	ULevel* Level = Actor->GetLevel();
	//	if (Level)
	//	{
	//		// レベル名を取得
	//		FString LevelName = Level->GetOutermost()->GetName();

	//		// 「LI_」で始まる部屋名を抽出
	//		FString StrollingActorName;
	//		if (LevelName.Split(TEXT("LI_"), nullptr, &StrollingActorName))
	//		{
	//			FString ExtractedName;
	//			StrollingActorName.Split(TEXT("_"), &ExtractedName, nullptr);

	//			// 「LI_」を含めた部屋名を作成
	//			FString BaseStrollingActorName = TEXT("LI_") + ExtractedName;

	//			// TMapに追加
	//			if (!StrollingActorMap.Contains(BaseStrollingActorName))
	//			{
	//				StrollingActorMap.Add(BaseStrollingActorName, TArray<AActor*>());
	//			}
	//			StrollingActorMap[BaseStrollingActorName].Add(Actor);
	//		}
	//	}
	//}

	//// デバッグログ: マップの内容を確認
	//for (const auto& Elem : StrollingActorMap)
	//{
	//	UE_LOG(LogTemp, Log, TEXT("Room: %s has %d actors"), *Elem.Key, Elem.Value.Num());
	//	for (AActor* Actor : Elem.Value)
	//	{
	//		UE_LOG(LogTemp, Log, TEXT("  Actor: %s"), *Actor->GetName());
	//	}
	//}
	//----------------------------------------------------------------------------------------------------
}

void AAIC_RazorKiller::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	RazorKiller = Cast<ARazorKiller>(GetPawn());
	if (RazorKiller == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("RazorKiller Ptr Set Fail"));
	}
	// AIControllerがPawn所持した際にBBとBTを使用する
	BlackboardComp->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
	// 初期化処理(待機座標の設定)
	WaitingPoint = Cast<AWarpPoint>(SearchForNearestObjectWithTag("Wait"));
	if (RazorKiller != nullptr && WaitingPoint != nullptr)
	{
		RazorKiller->SetActorLocation(WaitingPoint->GetActorLocation());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Initialize Set Fail"));
	}

	if (BlackboardComp)
	{
		BlackboardComp->SetValueAsObject(TEXT("SelfActor"), InPawn);
	}
}

void AAIC_RazorKiller::SetTrackingPointKey(const int Num) const
{
	BlackboardComp->SetValueAsInt("TrackingPoint", Num);
}

void AAIC_RazorKiller::SetWarpPointIDKey(const int Num) const
{
	BlackboardComp->SetValueAsInt("WarpPointID", Num);
}

void AAIC_RazorKiller::AddWarpPointIDKey() const
{
	int ID = BlackboardComp->GetValueAsInt("WarpPointID");
	ID++;
	BlackboardComp->SetValueAsInt("WarpPointID", ID);
}

void AAIC_RazorKiller::SetIsSeePlayerKey(const bool bValue) const
{
	BlackboardComp->SetValueAsBool("IsSeePlayer", bValue);
}

/// <summary>
/// イベント開始時に呼ぶ処理
/// </summary>
void AAIC_RazorKiller::SetEvent(float MoveStartTime)
{
	//ツリーを動かす
	//BehaviorComp->StartTree(*BehaviorTree);

   // ツリーの初期化
   // PlayerとKillerのキーを設定(プレイヤーどこから持ってくんねん問題はある)
	BlackboardComp->SetValueAsObject("SelfActor", RazorKiller);
	const TObjectPtr<AActor> Player = SearchForNearestObjectWithTag(TEXT("player"));
	BlackboardComp->SetValueAsObject("PlayerActor", Player);

	//// 出現位置設定(何回よんでもここだよーん)
	//const TObjectPtr<AWarpPoint> PlacementPoint = Cast<AWarpPoint>(SearchForNearestObjectWithTag(TEXT("place")));
	//const FVector Location = PlacementPoint->GetActorLocation();
	//RazorKiller->SetActorLocation(Location);

	//-----2024/09/03 22cu0127 丹代天 記述：プレイヤーの近くのワープポイントに飛ぶ処理-----
	SetLocationClosestToPlayerWarpPoint();
	//--------------------------------------------------------------------------------

	// 今のところ初期化を入れておく(CheckGoalで初期化を通してくれる)
	BlackboardComp->SetValueAsVector("PatrolPos", FVector(0, 0, 0));

	// 座標移動(ワープポイントの位置に移動)
	// SetActorLocationAtWarpPoint()

	// 徘徊エリアの設定(自作クラスに置き換える)
	const TObjectPtr<AActor> MoveArea = SearchForNearestObjectWithTag(TEXT("movearea"));
	BlackboardComp->SetValueAsObject("MoveArea", MoveArea);

	BlackboardComp->SetValueAsBool("IsBTRunning", true);
	BlackboardComp->SetValueAsInt("TrackingPoint", 100);
}

//-----2024/09/03 22cu0127 丹代天 記述：プレイヤーの近くのワープポイントに飛ぶ処理-----
void AAIC_RazorKiller::SetLocationNearestWarpPoint()
{
	BlackboardComp->SetValueAsObject("PlayerActor", UGameplayStatics::GetPlayerPawn(this, 0));
}
//---------------------------------------------------------------------
void AAIC_RazorKiller::SetLocationClosestToPlayerWarpPoint()
{
	// プレイヤーのアクターを取得
	APawn* pPlayerActor = Cast<APawn>(UGameplayStatics::GetPlayerPawn(this, 0));
	if (!pPlayerActor)
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerActor is invalid."));
		return;
	}

	const FVector PlayerPos = pPlayerActor->GetActorLocation();
	UE_LOG(LogTemp, Error, TEXT("Owner Position: %s"), *PlayerPos.ToString());

	float NearestDistance = FLT_MAX; // 最も近い距離を追跡
	FVector NearestLocation;
	FRotator NearestObjectRotator;

	// 一番近いオブジェクトを探す
	for (int Wi = 0; Wi < m_AWarpPoints.Num(); Wi++)
	{
		if (m_AWarpPoints[Wi])
		{
			const FVector NowLocation = m_AWarpPoints[Wi]->GetActorLocation();
			const FRotator NowRotator = m_AWarpPoints[Wi]->GetActorRotation();
			UE_LOG(LogTemp, Error, TEXT("Warp Point %d Location: %s"), Wi, *NowLocation.ToString());

			float CurrentDistance = FVector::Dist(PlayerPos, NowLocation); // ユークリッド距離を計算

			if (CurrentDistance < NearestDistance)
			{
				NearestDistance = CurrentDistance;
				NearestLocation = NowLocation;
				NearestObjectRotator = NowRotator;
			}
		}
	}

	// 有効かどうか調べる
	if (m_AWarpPoints.Num() > 0)
	{
		RazorKiller->SetActorLocation(NearestLocation);
		RazorKiller->SetActorRotation(NearestObjectRotator);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No Warp Points Found."));
	}
}
//--------------------------------------------------------------------------------
//-----2024/09/08 22cu0127 丹代天 記述：プレイヤーが部屋に入った際に部屋にワープする関数-----
void AAIC_RazorKiller::HandleWarp(FString DoorName, TSoftObjectPtr<ADoor> DoorObj)
{
	//所有しているPawnを取得する
	APawn* Killer = this->GetPawn();

	FString AssetPath = DoorObj.ToString();
	UE_LOG(LogTemp, Warning, TEXT("Asset path: %s"), *AssetPath);

	//ADoor* Obj = nullptr; // 初期化

	//// DoorName に対応するエントリがマップに存在するかチェック
	//if (DoorMap.Contains(DoorName))
	//{
	//	// TArray<TObjectPtr<ADoor>>* を取得
	//	TArray<TObjectPtr<ADoor>>* DoorArray = DoorMap.Find(DoorName);

	//	if (DoorArray && DoorArray->Num() > 0)
	//	{
	//		// TArray の最初の要素を取得（適切な要素を選択する方法はアプリケーションに依存）
	//		Obj = (*DoorArray)[0].Get();
	//	}
	//}

	ADoor* Obj = DoorObj.Get();

	if (!Obj)
	{
		Obj = DoorObj.LoadSynchronous();
	}

	if (!Obj) return;
	//前後ろどちらかのフラグ
	bool IsFrontOf;

	//キラーがドアの表裏のどちらにいるのかを調べる
	if (Killer != nullptr)
	{
		//２座標を取得
		FVector KillerPos = Killer->GetActorLocation();
		FVector DoorPos = Obj->GetActorLocation();

		//２座標間の差を求めノーマライズ
		FVector DirectionToActor = DoorPos - KillerPos;
		DirectionToActor.Normalize();

		//
		FVector DoorForwardVector = -(Obj->GetActorRightVector());
		float Dot = FVector::DotProduct(DoorForwardVector, DirectionToActor);

		if (Dot >= 0) IsFrontOf = true;
		else IsFrontOf = false;

		UE_LOG(LogTemp, Error, TEXT("%f"), Dot);
	}

	//逆側のLerppointを取得する
	if (!IsValid(Obj->LerpPointSceneInRoom)) return;

	//Lerppointの座標に移動する
	FVector LerpPointLocation;

	if (IsFrontOf) LerpPointLocation = Obj->LerpPointScene->GetComponentLocation();
	else LerpPointLocation = Obj->LerpPointSceneInRoom->GetComponentLocation();

	if (IsFrontOf)
	{
		FVector setLocation = Obj->LerpPointSceneInRoom->GetComponentLocation();
		Killer->SetActorLocation(FVector(setLocation.X, setLocation.Y, Killer->GetActorLocation().Z));
		Killer->SetActorRotation(FRotator(0.f, 0.f, 0.f));
		BlackboardComp->SetValueAsVector("DoorLocation", setLocation);
	}
	else
	{
		FVector setLocation = Obj->LerpPointScene->GetComponentLocation();
		Killer->SetActorLocation(FVector(setLocation.X, setLocation.Y, Killer->GetActorLocation().Z));
		Killer->SetActorRotation(FRotator(0.f, 0.f, 0.f));
		BlackboardComp->SetValueAsVector("DoorLocation", setLocation);
	}

	
	BlackboardComp->SetValueAsBool("WaitingWarp", false);
}

float AAIC_RazorKiller::WarpRoom(FString DoorName, TSoftObjectPtr<ADoor> DoorObj)
{
	// 既存のタイマーが動作中ならばリセットする
	if (GetWorld()->GetTimerManager().IsTimerActive(WarpTimerHandle))
	{
		// タイマーをクリア（リセット）
		GetWorld()->GetTimerManager().ClearTimer(WarpTimerHandle);
	}

	BlackboardComp->SetValueAsBool("WaitingWarp", true);

	// 新しいタイマーを設定し、DelayTimeの後にWarp処理を呼び出す
	GetWorld()->GetTimerManager().SetTimer(WarpTimerHandle, FTimerDelegate::CreateUObject(this, &AAIC_RazorKiller::HandleWarp, DoorName, DoorObj), DelayTime, false);

	return DelayTime;
}
//-------------------------------------------------------------------------------------------

//-----2024/09/08 22cu0127 丹代天 記述：散策するための基準となるアクターをブラックボードに入力されてる部屋名をもとにして返す関数-----
void AAIC_RazorKiller::SetStrollingActors(TArray<AActor*> StrollingActors)
{
	StrollingActorArray.Empty();

	if(StrollingActorArray.IsEmpty() && !StrollingActors.IsEmpty()) StrollingActorArray = StrollingActors;
}

TArray<AActor*> AAIC_RazorKiller::GetStrollingActors()
{
	return StrollingActorArray;
}
//--------------------------------------------------------------------------------------------------------------------------

/// <summary>
/// ツリーを起動する
/// </summary>
void AAIC_RazorKiller::StartEvent() const
{
	// ツリーを動かす
	BehaviorComp->StartTree(*BehaviorTree);
	//
	BlackboardComp->SetValueAsBool("IsBTRunning", true);
	BlackboardComp->SetValueAsInt("TrackingPoint", 100);
}

/// <summary>
/// 抵抗完了後に呼ぶタイマー付きイベント開始
/// </summary>
/// <param name="Seconds">タイマーの秒数</param>
void AAIC_RazorKiller::ReStartEventForTimer(float Seconds)
{
	// ※直そう
	// これStartEventだと位置戻るから良くないんだよね
	GetWorldTimerManager().SetTimer(ReStartHandle, this, &AAIC_RazorKiller::StartEvent, Seconds, false);
}

void AAIC_RazorKiller::StopEvent() const
{
	// ビヘイビアツリーを停止
	if (BehaviorComp)
	{
		BehaviorComp->StopTree();
	}

	if (BlackboardComp == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("BlackboardComp is null"));
		return;
	}

	const UBlackboardData* BlackboardData = BlackboardComp->GetBlackboardAsset();
	if (BlackboardData == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("BlackboardData is null"));
		return;
	}

	// ブラックボードキーのリストを取得
	const TArray<FBlackboardEntry>& Entries = BlackboardData->Keys;

	for (const FBlackboardEntry& Entry : Entries)
	{
		// 各キーをクリア
		BlackboardComp->ClearValue(Entry.EntryName);
		UE_LOG(LogTemp, Log, TEXT("Cleared Blackboard Key: %s"), *Entry.EntryName.ToString());
	}

	// 待機ポイントへの座標移動
	if (GetPawn() != nullptr && WaitingPoint != nullptr)
	{
		GetPawn()->SetActorLocation(WaitingPoint->GetActorLocation());
		UE_LOG(LogTemp, Log, TEXT("Pawn moved to WaitingPoint location."));
	}
	else
	{
		if (GetPawn() == nullptr)
		{
			UE_LOG(LogTemp, Error, TEXT("Pawn is null, unable to move to WaitingPoint."));
		}
		if (WaitingPoint == nullptr)
		{
			UE_LOG(LogTemp, Error, TEXT("WaitingPoint is null, unable to move pawn."));
		}
	}

	RazorKiller->bLook = false;

	if (!bOnce)
	{
		ItemDispatcer.Broadcast();
	}
}

/// <summary>
/// 最も近いアクターをタグを使って検索
/// </summary>
/// <param name="Tag">検索するタグ</param>
/// <returns>タグで発見した中で一番近いアクター</returns>
/// Template使ってクラスで検索したかった(タグ設定が面倒+事故の元になるので)
TObjectPtr<AActor> AAIC_RazorKiller::SearchForNearestObjectWithTag(const FString& Tag) const
{
	TArray<TObjectPtr<AActor>> FindObjects;
	const FVector OwnerPos = GetPawn()->GetActorLocation();
	float Difference = 0.f;
	float PreviousDifference = 10000000.f;
	int NearestNum = 0;

	// Tagで検索
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName(Tag), FindObjects);
	// 一番近いオブジェクトを探す
	for (int Wi = 0; Wi < FindObjects.Num(); Wi++)
	{
		const FVector NowLocation = FindObjects[Wi]->GetActorLocation();
		Difference += UKismetMathLibrary::Abs(OwnerPos.X - NowLocation.X) + UKismetMathLibrary::Abs(OwnerPos.Y - NowLocation.Y) + UKismetMathLibrary::Abs(OwnerPos.Z - NowLocation.Z);
		if (Difference < PreviousDifference && FindObjects[Wi]->ActorHasTag(FName(Tag)))
		{
			PreviousDifference = Difference;
			NearestNum = Wi;
		}
	}
	// 有効かどうか調べる
	if (FindObjects.IsValidIndex(0))
	{
		return FindObjects[NearestNum];
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Not Found Objects. TagName = %s"), *Tag);
		return nullptr;
	}
}

void AAIC_RazorKiller::SubtractionTrackingPoint(const int Num) const
{
	int i = BlackboardComp->GetValueAsInt("TrackingPoint");

	if (i <= 0)
	{
		int PreviousTrackingPoint = BlackboardComp->GetValueAsInt("TrackingPoint");
		PreviousTrackingPoint -= Num;
		BlackboardComp->SetValueAsInt("TrackingPoint", PreviousTrackingPoint);
	}
}

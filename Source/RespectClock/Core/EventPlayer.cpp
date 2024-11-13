// Fill out your copyright notice in the Description page of Project Settings.

#include "EventPlayer.h"
#include "EventDataAsset_Base.h"
#include "Engine/World.h" 
#include "StaticDataManager.h"
#include "GM_RespectClock.h"
#include "Kismet/GameplayStatics.h"
#include "RespectClock/Characters/RazorKiller/AIC_RazorKiller.h"
#include "RespectClock/Characters/Player/PlayerCharacter.h"

UEventPlayer* UEventPlayer::Instance = nullptr;

UEventPlayer::UEventPlayer()
{
    if (!Instance)
    {
        Instance = this;
    }
}

UEventPlayer* UEventPlayer::GetInstance(UObject* WorldContextObject)
{
    return Instance;
}

void UEventPlayer::CacheEventData(const UWorld* World)
{
    UEventDataAsset_Base* EventDataAsset = UStaticDataManager::GetEventDataAsset(World);

    if (EventDataAsset != nullptr)
    {
        int Num = EventDataAsset->DataMap.Num();

        for (int i = 0; i < Num; ++i)
        {
            FEventTableBase* EventData = &EventDataAsset->DataMap[i];

            if (EventData)
            {
                NameToEventDataMap.Add(EventData->Name, *EventData);
                IDToEventDataMap.Add(EventData->ID, *EventData);
            }
        }

        UE_LOG(LogTemp, Warning, TEXT("Sound data cached: %d items"), Num);
    }
}

void UEventPlayer::SetKillerController(const UWorld* World)
{
    TArray<AActor*> FoundActors;

	// ワールド内のすべてのアクターを取得
	//WarpPoint
	UGameplayStatics::GetAllActorsOfClass(World, AActor::StaticClass(), FoundActors);
	//キャストし、それぞれ配列へ代入する
	for (AActor* Actor : FoundActors)
	{
		KillerController = Cast<AAIC_RazorKiller>(Actor);
		if (KillerController)
		{
            return;
		}
	}
}

void UEventPlayer::PlayEventByName(const UWorld* World, const FString& _name)
{
    AGM_RespectClock* myGameMode = nullptr;
    if (!World) return;
    
    AGameModeBase* gameModeBase = World->GetAuthGameMode();
    if (!gameModeBase) return;
    
    myGameMode = Cast<AGM_RespectClock>(gameModeBase);
    if (!myGameMode) return;

    if (NameToEventDataMap.Contains(_name))
    {
        FEventTableBase* EventDataPtr = NameToEventDataMap.Find(_name);

        if (EventDataPtr)
        {
            switch (EventDataPtr->eType)
            {
            //サウンド
            case EEventType::Sound:
                myGameMode->CallSoundEvent(EventDataPtr->ID, EventDataPtr->Name, EventDataPtr->SoundValue.Location, EventDataPtr->SoundValue.Volume, EventDataPtr->SoundValue.Pitch);
                break;

            //キラー
            case EEventType::Killer:
                myGameMode->CallKillerEvent();
                break;

            //エフェクト
            case EEventType::Effect:
                break;

            //シーケンサ
            case EEventType::Sequence:
                myGameMode->CallSequenceEvent(EventDataPtr->SequenceActor);
                break;

            //部屋通知
            case EEventType::EntryRoom:
                myGameMode->CallEntryRoom(EventDataPtr->RoomName, EventDataPtr->DoorObj);
                ActorArray.Empty();

                // SoftObjectArray の各要素を AActor* に変換して PointerArray に追加
                for (TSoftObjectPtr<AActor> SoftObject : EventDataPtr->StrollingActors)
                {
                    AActor* ActorPtr = SoftObject.Get();  // TSoftObjectPtr から AActor* に変換

                    if (!ActorPtr)
                    {
                        ActorPtr = SoftObject.LoadSynchronous();
                    }

                    if (ActorPtr)  // nullptr チェック
                    {
                        ActorArray.Add(ActorPtr);
                    }
                }

                KillerController->SetStrollingActors(ActorArray);
                break;

            //アイテム
            case EEventType::Item:
                EventDataPtr->Item->ActiveItem(EventDataPtr->IsActivate);
                break;
            //リスポーン
            case EEventType::Respawn:
                myGameMode->SetRespawnPos(EventDataPtr->RespawnPos);
                break;
            // 進行状況テキスト
            case EEventType::ProgressState:
                //myGameMode->CallProgreesUpdate(EventDataPtr->ProgressText);
                break;
            default:
                break;
            }
        }
    }
}

void UEventPlayer::PlayEventByID(const UWorld* World, int32 _iD)
{
    AGM_RespectClock* myGameMode = nullptr;
    if (!World) return;

    AGameModeBase* gameModeBase = World->GetAuthGameMode();
    if (!gameModeBase) return;

    myGameMode = Cast<AGM_RespectClock>(gameModeBase);
    if (!myGameMode) return;

    if (IDToEventDataMap.Contains(_iD))
    {
        FEventTableBase* EventDataPtr = IDToEventDataMap.Find(_iD);

        if (EventDataPtr)
        {
            switch (EventDataPtr->eType)
            {
            //サウンド
            case EEventType::Sound:
                myGameMode->CallSoundEvent(EventDataPtr->ID, EventDataPtr->Name, EventDataPtr->SoundValue.Location, EventDataPtr->SoundValue.Volume, EventDataPtr->SoundValue.Pitch);
                break;

            //キラー
            case EEventType::Killer:
                myGameMode->CallKillerEvent();
                break;

            //エフェクト
            case EEventType::Effect:
                break;

            //シーケンサ
            case EEventType::Sequence:
                myGameMode->CallSequenceEvent(EventDataPtr->SequenceActor);
                break;

            //部屋通知
            case EEventType::EntryRoom:
                myGameMode->CallEntryRoom(EventDataPtr->RoomName, EventDataPtr->DoorObj);

                ActorArray.Empty();

                // SoftObjectArray の各要素を AActor* に変換して PointerArray に追加
                for (TSoftObjectPtr<AActor> SoftObject : EventDataPtr->StrollingActors)
                {
                    AActor* ActorPtr = SoftObject.Get();  // TSoftObjectPtr から AActor* に変換

                    if (!ActorPtr)
                    {
                        ActorPtr = SoftObject.LoadSynchronous();
                    }

                    if (ActorPtr)  // nullptr チェック
                    {
                        ActorArray.Add(ActorPtr);
                    }
                }

                KillerController->SetStrollingActors(ActorArray);
                break;

            //アイテム
            case EEventType::Item:
                EventDataPtr->Item->ActiveItem(EventDataPtr->IsActivate);
                //リスポーン
            case EEventType::Respawn:
                myGameMode->SetRespawnPos(EventDataPtr->RespawnPos);
                break;
                // 進行状況テキスト
            case EEventType::ProgressState:
                myGameMode->CallProgreesUpdate(EventDataPtr->ProgressText);

            default:
                break;
            }
        }
    }
}
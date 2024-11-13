 //Fill out your copyright notice in the Description page of Project Settings.


#include "StaticDataManager.h"
#include "Engine.h"
#include "RespectClockInstance.h"
#include "SoundDataAsset.h"
#include "EventDataAsset_Base.h"
#include "SoundPlayer.h"

// サウンドデータアセットの取得
USoundDataAsset* UStaticDataManager::GetSoundDataAsset(const UObject* WorldContextObject)
{
    //ワールド取得
    UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::ReturnNull);
    //ゲームインスタンスを取得
    URespectClockInstance* GameInstance = Cast<URespectClockInstance>(World->GetGameInstance());
    //スタティックデータマネージャーを取得
    UStaticDataManager* StaticDataManager = GameInstance->StaticDataManager;

    //サウンド
    if (StaticDataManager->SoundDataAsset == nullptr)
    {
        // アセットのパスを指定
        FString DataAssetPath = StaticDataManager->SoundDataAssetPath.ToString();
        //パスをもとにデータアセットを取得
        StaticDataManager->SoundDataAsset = Cast<USoundDataAsset>(StaticLoadObject(USoundDataAsset::StaticClass(), nullptr, *DataAssetPath));
        if (!StaticDataManager->SoundDataAsset)
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to load SoundDataAsset from path: %s"), *DataAssetPath);
        }
    }

    return StaticDataManager->SoundDataAsset;
}

// イベントデータアセットの取得
UEventDataAsset_Base* UStaticDataManager::GetEventDataAsset(const UObject* WorldContextObject)
{
    //ワールド取得
    UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::ReturnNull);
    //ゲームインスタンスを取得
    URespectClockInstance* GameInstance = Cast<URespectClockInstance>(World->GetGameInstance());
    //スタティックデータマネージャーを取得
    UStaticDataManager* StaticDataManager = GameInstance->StaticDataManager;

    //イベント
    if (StaticDataManager->EventDataAsset == nullptr)
    {
        // アセットのパスを指定
        FString DataAssetPath = StaticDataManager->EventDataAssetPath.ToString();
        //パスをもとにデータアセットを取得
        StaticDataManager->EventDataAsset = Cast<UEventDataAsset_Base>(StaticLoadObject(UEventDataAsset_Base::StaticClass(), nullptr, *DataAssetPath));
        if (!StaticDataManager->EventDataAsset)
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to load EventDataAsset from path: %s"), *DataAssetPath);
        }
    }

    return StaticDataManager->EventDataAsset;
}
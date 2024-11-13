//--------------------------------------------------------------------------------------
// ファイル名：SoundPlayer.cpp
// 内容		：SoundDataTableAssetからデータを受け取り、ID又は名前から検索して音を再生するクラス
// 作成者	：22cu0127 丹代天
// 更新日	：2024/05/30		作成
//--------------------------------------------------------------------------------------

#include "SoundPlayer.h"
#include "SoundDataAsset.h"
#include "StaticDataManager.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"
#include "Engine/World.h"

USoundPlayer* USoundPlayer::Instance = nullptr;

// コンストラクタ
USoundPlayer::USoundPlayer()
{
    if (!Instance)
    {
        Instance = this;
    }
}

// シングルトンインスタンスを取得
USoundPlayer* USoundPlayer::GetInstance(UObject* WorldContextObject)
{
    if (!Instance)
    {
        UE_LOG(LogTemp, Warning, TEXT("Creating new USoundPlayer instance"));
        Instance = NewObject<USoundPlayer>();
        Instance->AddToRoot(); // ガベージコレクションされないようにする
    }
    if (!Instance->BGMComponent)
    {
        if (WorldContextObject)
        {
            UE_LOG(LogTemp, Warning, TEXT("Initializing BGMComponent with valid WorldContextObject"));
            Instance->InitializeBGMComponent(WorldContextObject);
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("WorldContextObject is nullptr"));
        }
    }

    return Instance;
}

void USoundPlayer::InitializeBGMComponent(UObject* WorldContextObject)
{
    if (!BGMComponent)
    {
        UWorld* World = WorldContextObject->GetWorld();
        if (World)
        {
            AActor* TempActor = World->SpawnActor<AActor>();
            if (TempActor)
            {
                BGMComponent = NewObject<UAudioComponent>(TempActor);
                BGMComponent->bAutoActivate = false; // 自動再生を無効にする
                BGMComponent->RegisterComponent();
                UE_LOG(LogTemp, Warning, TEXT("BGMComponent initialized"));
            }
            else
            {
                UE_LOG(LogTemp, Error, TEXT("Failed to spawn TempActor"));
            }
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("World is nullptr"));
        }
    }
}

void USoundPlayer::ResetBGMComponent(UObject* WorldContextObject)
{
    UE_LOG(LogTemp, Warning, TEXT("ResetBGMComponent called"));

    if (BGMComponent)
    {
        if (BGMComponent->IsValidLowLevel() && !BGMComponent->IsPendingKill())
        {
            UE_LOG(LogTemp, Warning, TEXT("Destroying BGMComponent"));
            BGMComponent->DestroyComponent();
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("BGMComponent is not valid or is pending kill"));
        }

        BGMComponent = nullptr;
    }

    InitializeBGMComponent(WorldContextObject);
}

//データアセット内のデータをMapにキャッシュ
void USoundPlayer::CacheSoundData(const UWorld* World)
{
    //データアセット取得
    USoundDataAsset* SoundDataAsset = UStaticDataManager::GetSoundDataAsset(World);

    if (SoundDataAsset != nullptr)
    {
        int Num = SoundDataAsset->SoundDataMap.Num();

        //行ごとにキャッシュ
        for (int i = 0; i < Num; ++i)
        {
            FSoundDataTableRow* SoundData = &SoundDataAsset->SoundDataMap[i];

            if (SoundData)
            {
                NameToSoundDataMap.Add(SoundData->Name, *SoundData);
                IDToSoundDataMap.Add(SoundData->ID, *SoundData);
            }
        }

        UE_LOG(LogTemp, Warning, TEXT("Sound data cached: %d items"), Num);
    }
}

void USoundPlayer::PlaySoundByName(const UWorld* World, const FString& _name, FVector _location, float _volume, float _pitch)
{
    //再生
    if (NameToSoundDataMap.Contains(_name))
    {
        FSoundDataTableRow* SoundDataPtr = NameToSoundDataMap.Find(_name);

        if (SoundDataPtr)
        {
            Play(World, SoundDataPtr->SoundObj, _location, _volume, _pitch);
        }
    }
}

void USoundPlayer::PlaySoundByID(const UWorld* World, int32 _iD, FVector _location, float _volume, float _pitch)
{
    //再生
    if (IDToSoundDataMap.Contains(_iD))
    {
        FSoundDataTableRow* SoundDataPtr = IDToSoundDataMap.Find(_iD);

        if (SoundDataPtr)
        {
            Play(World, SoundDataPtr->SoundObj, _location, _volume, _pitch);
        }
    }
}

void USoundPlayer::Play(const UWorld* World, USoundBase* SoundObj, FVector _location, float _volume, float _pitch)
{
    UGameplayStatics::PlaySoundAtLocation(World, SoundObj, _location, _volume, _pitch);
}

void USoundPlayer::PlayBGMByName(UWorld* World, const FString& _name, float _volume, float _pitch)
{
    if (!BGMComponent) InitializeBGMComponent(World);

    if (NameToSoundDataMap.Contains(_name))
    {
        FSoundDataTableRow* SoundDataPtr = NameToSoundDataMap.Find(_name);

        UE_LOG(LogTemp, Warning, TEXT("Found sound data for name: %s"), *_name);

        if (SoundDataPtr && SoundDataPtr->SoundObj)
        {
            BGMComponent->SetSound(SoundDataPtr->SoundObj);
            BGMComponent->SetVolumeMultiplier(_volume);
            BGMComponent->SetPitchMultiplier(_pitch);
            BGMComponent->bIsUISound = false;
            BGMComponent->bAllowSpatialization = false;
            BGMComponent->bAutoActivate = false; // 自動再生を無効にする

            BGMComponent->Play();
            UE_LOG(LogTemp, Warning, TEXT("Playing BGM: %s"), *_name);
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Sound data not found for name: %s"), *_name);
    }
}

void USoundPlayer::StopBGM()
{
    if (BGMComponent && BGMComponent->IsPlaying())
    {
        BGMComponent->Stop();
    }
}
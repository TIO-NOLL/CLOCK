// Fill out your copyright notice in the Description page of Project Settings.


#include "RespectClockInstance.h"
#include "StaticDataManager.h"
#include "SoundPlayer.h"
#include "EventPlayer.h"
#include "Blueprint/UserWidget.h"
#include "Message.h"
#include "Kismet/KismetSystemLibrary.h"

void URespectClockInstance::Init()
{
    Super::Init();

    // StaticDataManagerの生成
    StaticDataManager = NewObject<UStaticDataManager>();
    UE_LOG(LogTemp, Warning, TEXT("StaticDataManager created"));

    UWorld* World = GetWorld();
    if (World)
    {
        // SoundPlayerの生成
        FActorSpawnParameters SoundSpawnParams;
        SoundSpawnParams.Name = TEXT("UniqueSoundPlayer");

        SoundPlayer = USoundPlayer::GetInstance(GetWorld());
        SoundPlayer->AddToRoot(); //ガベージコレクションされないように
        SoundPlayer->CacheSoundData(World); //取得時点でキャッシュする
        
        if (SoundPlayer)
        {
            UE_LOG(LogTemp, Warning, TEXT("SoundPlayer spawned"));
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to spawn SoundPlayer"));
        }

        //EventPlayerの生成
        FActorSpawnParameters EventSpawnParams;
        EventSpawnParams.Name = TEXT("UniqueEventPlayer");

        EventPlayer = UEventPlayer::GetInstance(GetWorld());
        EventPlayer->AddToRoot(); //ガベージコレクションされないように
        EventPlayer->CacheEventData(World); //取得時点でキャッシュする

        if (EventPlayer)
        {
            UE_LOG(LogTemp, Warning, TEXT("EventPlayer spawned"));
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to spawn EventPlayer"));
        }

        // メッセージウィジェットの作成
        CreateMessage();
    }
}

// GetWorld関数の実装
UWorld* URespectClockInstance::GetWorldfromInstance() const
{
    // GameInstance内でのUWorldへのアクセス
    return GetWorld();
}

USoundPlayer* URespectClockInstance::GetSoundPlayer() const
{ 
    return SoundPlayer; 
}

UEventPlayer* URespectClockInstance::GetEventPlayer() const
{
    return EventPlayer;
}

// メッセージウィジェットの作成
void URespectClockInstance::CreateMessage()
{
    // ウィジェットのパス
    WBPPath = TEXT("/Game/RespectClock/Core/Widget/Player/WBP_Message.WBP_Message_C");
    // ウィジェットクラスの設定
    WidgetClass = TSoftClassPtr<UUserWidget>(FSoftObjectPath(*WBPPath)).LoadSynchronous();

    if (WidgetClass)
    {
        UE_LOG(LogTemp, Log, TEXT("パスの取得に成功: %s"), *WidgetClass->GetName());
        // ウィジェットのインスタンスを作成
        WidgetInstance = CreateWidget<UMessage>(GetWorldfromInstance(), WidgetClass);
    }
    else
    {
        // パスが動作してない
        UE_LOG(LogTemp, Error, TEXT("パスの取得に失敗: %s"), *WBPPath);
    }
}
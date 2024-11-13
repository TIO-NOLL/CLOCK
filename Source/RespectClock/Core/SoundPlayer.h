//--------------------------------------------------------------------------------------
// ファイル名：SoundPlayer.cpp
// 内容		：SoundDataTableAssetからデータを受け取り、ID又は名前から検索して音を再生するクラス
// 作成者	：22cu0127 丹代天
// 更新日	：2024/05/30		作成
//--------------------------------------------------------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SoundDataTableRow.h"
#include "SoundPlayer.generated.h"

class USoundDataAsset;
class UAudioComponent;

UCLASS(Blueprintable)
class RESPECTCLOCK_API USoundPlayer : public UObject
{
    GENERATED_BODY()

public:
    // シングルトンインスタンスを取得
    UFUNCTION(BlueprintCallable, Category = "SoundPlayer")
    static USoundPlayer* GetInstance(UObject* WorldContextObject);

    // サウンド再生関数
    //名前
    UFUNCTION(BlueprintCallable, Category = "SoundPlayer")
    void PlaySoundByName(const UWorld* World, const FString& _name, FVector _location, float _volume = 1.f, float _pitch = 1.f);
    //ID
    UFUNCTION(BlueprintCallable, Category = "SoundPlayer")
    void PlaySoundByID(const UWorld* World, int32 _iD, FVector _location, float _volume = 1.f, float _pitch = 1.f);

    //BGM再生
    UFUNCTION(BlueprintCallable, Category = "SoundPlayer")
    void PlayBGMByName(UWorld* World, const FString& Name, float Volume = 1.f, float Pitch = 1.f);
    //BGM停止
    UFUNCTION(BlueprintCallable, Category = "SoundPlayer")
    void StopBGM();

    //データアセットのデータをMap二キャッシュする
    void CacheSoundData(const UWorld* World);

    //BGMComponentの再取得用(サブレベルを使用する際に、必要になるかも)
    void ResetBGMComponent(UObject* WorldContextObject);

    //再生部分をまとめた関数（必要ないかもしれない）
    void Play(const UWorld* World, USoundBase* SoundObj, FVector _location, float _volume, float _pitch);

private:
    // コンストラクタをプライベートにしてシングルトンにする
    USoundPlayer();

    // シングルトンインスタンス
    static USoundPlayer* Instance;

    // サウンドデータマップ
    TMap<FString, FSoundDataTableRow> NameToSoundDataMap;
    TMap<int32, FSoundDataTableRow> IDToSoundDataMap;

    // BGM再生用コンポーネント
    UAudioComponent* BGMComponent;

    //BGMComponentを初期化する用
    void InitializeBGMComponent(UObject* WorldContextObject);
};
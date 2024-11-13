// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "StaticDataManager.generated.h"

UCLASS(config = Game, defaultconfig)
class RESPECTCLOCK_API UStaticDataManager : public UObject
{
	GENERATED_BODY()
	
public:
	// データアセットのファイルパス
	//サウンド
	UPROPERTY(EditAnywhere, Config, meta = (AllowedClasses = "SoundDataAsset"))
	FSoftObjectPath SoundDataAssetPath;
	UPROPERTY(BlueprintReadOnly)
	class USoundDataAsset* SoundDataAsset;

	//イベント
	UPROPERTY(EditAnywhere, Config, meta = (AllowedClasses = "EventDataAsset_Base"))
	FSoftObjectPath EventDataAssetPath;
	UPROPERTY(BlueprintReadOnly)
	class UEventDataAsset_Base* EventDataAsset;

public:
	// データアセットの取得
	//サウンド
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"))
	static class USoundDataAsset* GetSoundDataAsset(const UObject* WorldContextObject);

	//イベント
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"))
	static class UEventDataAsset_Base* GetEventDataAsset(const UObject* WorldContextObject);
};

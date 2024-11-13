// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "Settings.generated.h"

UCLASS(config = "MySettings", defaultconfig)
class RESPECTCLOCK_API USettings : public UDeveloperSettings
{
    GENERATED_BODY()

public:

    USettings();

#if WITH_EDITORONLY_DATA
    // ファイルの出力先フォルダー
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Config, Category = "Data")
    FString ResourceOutputFolder;

    // 自動エクスポート機能の ON/OFF
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Config, Category = "Data")
    bool AutoExportOnEdit;
#endif

    UFUNCTION(BlueprintPure)
    static USettings* GetSettings();

    virtual FName GetCategoryName() const override;

#if WITH_EDITOR
    virtual FText GetSectionText() const override;
    virtual FText GetSectionDescription() const override;
#endif

};

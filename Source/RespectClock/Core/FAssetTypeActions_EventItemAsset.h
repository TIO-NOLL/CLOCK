// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#if WITH_EDITOR
#include "AssetTypeActions_Base.h"

class UEventDataAsset_Base;

class FAssetTypeActions_EventItemAsset : public FAssetTypeActions_Base
{
public:
    virtual FText GetName() const override
    {
        return NSLOCTEXT("AssetTypeActions", "AssetTypeActions_MyDataAsset", "MyDataAsset");
    }
    virtual FColor GetTypeColor() const override { return FColor::Red; }
    virtual uint32 GetCategories() override { return EAssetTypeCategories::Misc; }
    virtual void GetResolvedSourceFilePaths(const TArray<UObject*>& TypeAssets, TArray<FString>& OutSourceFilePaths) const override;
    virtual UClass* GetSupportedClass() const override;
    virtual bool IsImportedAsset() const override { return true; }
    virtual bool HasActions(const TArray<UObject*>& InObjects) const override { return true; }
};
#endif
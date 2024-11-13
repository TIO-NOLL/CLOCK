// Fill out your copyright notice in the Description page of Project Settings.

#if WITH_EDITOR
#include "FAssetTypeActions_EventItemAsset.h"
#include "EventDataAsset_Base.h"
#include "EditorFramework/AssetImportData.h"

#define LOCTEXT_NAMESPACE "AssetTypeActions"

UClass* FAssetTypeActions_EventItemAsset::GetSupportedClass() const
{
    return UEventDataAsset_Base::StaticClass();
}

void FAssetTypeActions_EventItemAsset::GetResolvedSourceFilePaths(const TArray<UObject*>& TypeAssets, TArray<FString>& OutSourceFilePaths) const
{
    for (UObject* TypeAsset : TypeAssets)
    {
        const UEventDataAsset_Base* MyDataAsset = CastChecked<UEventDataAsset_Base>(TypeAsset);
        if (MyDataAsset != nullptr && MyDataAsset->AssetImportData != nullptr)
        {
            MyDataAsset->AssetImportData->ExtractFilenames(OutSourceFilePaths);
        }
    }
}

#undef LOCTEXT_NAMESPACE
#endif
// Copyright Epic Games, Inc. All Rights Reserved.

#include "RespectClock.h"
#include "Modules/ModuleManager.h"

#include "Developer/Settings/Public/ISettingsModule.h"
#include "Core/StaticDataManager.h"
#if WITH_EDITORONLY_DATA
#include "RespectClock/Core/FAssetTypeActions_EventItemAsset.h"
#endif

#define LOCTEXT_NAMESPACE ""

class FRespectClockModule : public FDefaultGameModuleImpl
{
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
#if WITH_EDITORONLY_DATA
	TSharedPtr<FAssetTypeActions_EventItemAsset> DataAssetTypeActions;
#endif
};

IMPLEMENT_PRIMARY_GAME_MODULE(FRespectClockModule, RespectClock, "RespectClock" );

void FRespectClockModule::StartupModule()
{
	ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings");
	if (SettingsModule != nullptr)
	{
		SettingsModule->RegisterSettings(
			"Project", // ※２
			"Project",
			"MySetting",
			LOCTEXT("MySettingName", "StaticData"),
			LOCTEXT("MySettingDescription", "My setting discription"),
			GetMutableDefault<UStaticDataManager>()
		);
	}
#if WITH_EDITORONLY_DATA
	DataAssetTypeActions = MakeShareable(new FAssetTypeActions_EventItemAsset);
	FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get().RegisterAssetTypeActions(DataAssetTypeActions.ToSharedRef());
#endif
}

void FRespectClockModule::ShutdownModule()
{
	ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings");
	if (SettingsModule != nullptr)
	{
		SettingsModule->UnregisterSettings(
			"Project", // ※３
			"MySettingCategory",
			"MySetting"
		);
	}

#if WITH_EDITOR
	if (DataAssetTypeActions.IsValid())
	{
		if (FModuleManager::Get().IsModuleLoaded("AssetTools"))
		{
			FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools").Get().UnregisterAssetTypeActions(DataAssetTypeActions.ToSharedRef());
		}
		DataAssetTypeActions.Reset();
	}
#endif
}

#undef LOCTEXT_NAMESPACE
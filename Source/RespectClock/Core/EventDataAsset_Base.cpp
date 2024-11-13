#include "EventDataAsset_Base.h"

#if WITH_EDITOR
#include "BPFunctionLibrary.h"
#include "Settings.h"
#include "UObject/UnrealType.h"
#include "Windows/WindowsPlatformProcess.h"
#include "EditorFramework/AssetImportData.h"
#include "Engine/DataTable.h"
#include "EventTable.h"
#include "LevelSequence.h"
#endif

#if WITH_EDITOR
FString UEventDataAsset_Base::FileExtension = TEXT("json");

//インポート
void UEventDataAsset_Base::Import()
{
    if (!DataTable) return;

    FString FilePath = UBPFunctionLibrary::GetExportFileName(DataTable, FileExtension);
    UBPFunctionLibrary::ImportJsonToObject(FilePath, this);
    UpdateAssetImportData(FilePath);

    UBPFunctionLibrary::ImportDataTableToJSON(DataTable, FilePath);
    
    LoadData();
}

void UEventDataAsset_Base::OpenSourceLocation() const
{
    if (AssetImportData != nullptr)
    {
        FPlatformProcess::ExploreFolder(*AssetImportData->GetFirstFilename());
    }
}

void UEventDataAsset_Base::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
    if (PropertyChangedEvent.ChangeType == EPropertyChangeType::ArrayAdd)
    {
        // 配列要素を追加した瞬間は自動エクスポートしない（クラッシュ回避）
        return;
    }
}

void UEventDataAsset_Base::UpdateAssetImportData(FString Filename)
{
    if (AssetImportData == nullptr)
    {
        AssetImportData = NewObject<UAssetImportData>(this);
    }
    AssetImportData->Update(Filename);
}

void UEventDataAsset_Base::LoadData()
{
#if WITH_EDITORONLY_DATA

    DataMap.Empty();

    FString FilePath = UBPFunctionLibrary::GetExportFileName(DataTable, FileExtension);
    FString JsonString;
    if (!FFileHelper::LoadFileToString(JsonString, *FilePath))
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to load Json file: %s"), *FilePath);
        return;
    }

    TArray<TSharedPtr<FJsonValue>> JsonParsed;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);
    if (!FJsonSerializer::Deserialize(Reader, JsonParsed))
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to parse Json data"));
        return;
    }

    TArray<FString>RowNames;

    int i = 0;
    TArray EventRowNames = DataTable->GetRowNames();

    for (auto JsonValue : JsonParsed)
    {
        TSharedPtr<FJsonObject> JsonObject = JsonValue->AsObject();
        if (JsonObject.IsValid())
        {
            FEventTableBase EventData;

            EventData.ID = FCString::Atoi(*JsonObject->GetStringField("ID"));
            EventData.Name = JsonObject->GetStringField("Name");
            EventData.eType = FStringToEnum(*JsonObject->GetStringField("eType"), TEXT("EEventType"));

            EventData.RoomName = JsonObject->GetStringField("RoomName");
            EventData.DoorName = JsonObject->GetStringField("DoorName");
            EventData.IsActivate = JsonObject->GetStringField("IsActivate").ToBool();

            auto RowName = EventRowNames[i];
            FEventTableBase* TableRow = DataTable->FindRow< FEventTableBase >(RowName, FString());
            i++;

            if (TableRow)
            {
                EventData.SoundValue = TableRow->SoundValue;
                EventData.SequenceActor = TableRow->SequenceActor;
                EventData.Item = TableRow->Item;
                EventData.DoorObj = TableRow->DoorObj;
                EventData.StrollingActors = TableRow->StrollingActors;
                EventData.RespawnPos = TableRow->RespawnPos;
                EventData.ProgressText = TableRow->ProgressText;
            }


            DataMap.Add(EventData);
        }
    }

    MarkPackageDirty();

#endif
}
#endif
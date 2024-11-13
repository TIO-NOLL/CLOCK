////--------------------------------------------------------------------------------------
//// ファイル名：EventDataAsset.cpp
//// 内容		：Event関係のデータをひとまとめに管理するアセット
//// 作成者	：22cu0127 丹代天
//// 更新日	：2024/05/30		作成
////--------------------------------------------------------------------------------------
//
//#include "EventDataAsset_Item.h"
//
//#if WITH_EDITOR
//#include "UObject/Package.h"
//#include "EditorReimportHandler.h"
//#include "FileHelpers.h"
//#include "EventTable.h"
//#endif
//
//#if WITH_EDITOR
//void UEventDataAsset::LoadData()
//{
//#if WITH_EDITORONLY_DATA
//
//    DataMap.Empty();
//
//    FString FilePath = UBPFunctionLibrary::GetExportFileName(DataTable, FileExtension);
//    FString JsonString;
//    if (!FFileHelper::LoadFileToString(JsonString, *FilePath))
//    {
//        UE_LOG(LogTemp, Error, TEXT("Failed to load Json file: %s"), *FilePath);
//        return;
//    }
//
//    TArray<TSharedPtr<FJsonValue>> JsonParsed;
//    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);
//    if (!FJsonSerializer::Deserialize(Reader, JsonParsed))
//    {
//        UE_LOG(LogTemp, Error, TEXT("Failed to parse Json data"));
//        return;
//    }
//
//    TArray<FString>RowNames;
//
//    for (auto JsonValue : JsonParsed)
//    {
//        TSharedPtr<FJsonObject> JsonObject = JsonValue->AsObject();
//        if (JsonObject.IsValid())
//        {
//            FEventTable_Item EventData;
//
//            EventData.EventObj = StringToObject<AActor>(JsonObject->GetStringField("EventObj"));
//            EventData.ID = FCString::Atoi(*JsonObject->GetStringField("ID"));
//            EventData.Name = JsonObject->GetStringField("Name");
//            EventData.eType = FStringToEnum(*JsonObject->GetStringField("eType"), TEXT("EEventType"));
//            EventData.bOnce = JsonObject->GetStringField("bOnce").ToBool();
//
//            DataMap.Add(EventData);
//        }
//    }
//
//    MarkPackageDirty();
//
//#endif
//}
//#endif
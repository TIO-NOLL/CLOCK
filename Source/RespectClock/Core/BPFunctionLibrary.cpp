 //Fill out your copyright notice in the Description page of Project Settings.


#include "BPFunctionLibrary.h"
#include "Settings.h"
#include "Misc/Paths.h"

#if WITH_EDITOR
#include "Engine/DataTable.h"
#include "Serialization/JsonWriter.h"
#include "Misc/FileHelper.h"
#include "JsonObjectConverter.h"
#include "EditorReimportHandler.h"
#include "SourceControlHelpers.h"
#endif

#if WITH_EDITOR
FString UBPFunctionLibrary::GetExportFileName(const UObject* Object, const FString& Extension, const FName Name)
{
    // 指定の拡張子で、UObject と同じ名前のファイル名を生成する
    // ファイルパスは、プロジェクト設定に依存
    if (Name == (TEXT("")))
    {
        static const FString PathPrefix = TEXT("/Game");
        const USettings* Settings = USettings::GetSettings();
        if (Settings != nullptr) {
            const FString ProjectDir = FPaths::ProjectDir();
            FString FileName = Object->GetPackage()->GetFName().ToString() + TEXT(".") + Extension;
            if (FileName.StartsWith(PathPrefix))
            {
                FileName = FileName.RightChop(PathPrefix.Len());
            }
            const FString OutputPath = FPaths::Combine(ProjectDir, Settings->ResourceOutputFolder, FileName);
            return OutputPath;
        }
    }
    else
    {
        static const FString PathPrefix = TEXT("/Game");
        const USettings* Settings = USettings::GetSettings();
        if (Settings != nullptr)
        {
            const FString ProjectDir = FPaths::ProjectDir();
            FString FileName = Name.ToString() + TEXT(".") + Extension;
            if (FileName.StartsWith(PathPrefix))
            {
                FileName = FileName.RightChop(PathPrefix.Len());
            }
            const FString OutputPath = FPaths::Combine(ProjectDir, Settings->ResourceOutputFolder, FileName);
            return OutputPath;
        }
    }

    return FString();
}

void UBPFunctionLibrary::ImportJsonToObject(const FString& FilePath, UObject* OutObject)
{
    // JSON ファイルから UObject を生成
    if (!OutObject)
    {
        return;
    }

    FString JsonString;
    if (LoadFileToString(FilePath, JsonString))
    {
        TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(JsonString);
        TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
        FJsonSerializer::Deserialize(JsonReader, JsonObject);
        if (!FJsonObjectConverter::JsonObjectToUStruct(JsonObject.ToSharedRef(), OutObject->GetClass(), (void*)(OutObject), 0, 0))
        {
            UE_LOG(LogTemp, Error, TEXT("UMyBlueprintFunctionLibrary::ImportJsonToObject: Failed to Import %s"), *OutObject->GetName());
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("UMyBlueprintFunctionLibrary::ImportJsonToObject: Failed to Load %s"), *FilePath);
    }
}

void UBPFunctionLibrary::SaveStringToFile(const FString& String, const FString& FilePath)
{
    // UTF-8 設定でテキストファイルを出力
    if (USourceControlHelpers::IsEnabled())
    {
        USourceControlHelpers::CheckOutFile(FilePath);
    }
    FFileHelper::SaveStringToFile(String, *FilePath, FFileHelper::EEncodingOptions::ForceUTF8);
    if (USourceControlHelpers::IsEnabled())
    {
        USourceControlHelpers::MarkFileForAdd(FilePath);
    }
}

bool UBPFunctionLibrary::LoadFileToString(const FString& FilePath, FString& OutString)
{
    return FFileHelper::LoadFileToString(OutString, *FilePath, FFileHelper::EHashOptions::None, 0);
}

void UBPFunctionLibrary::ImportDataTableToJSON(UDataTable* DataTable, const FString& FilePath)
{

    FString JsonString;
    if (FFileHelper::LoadFileToString(JsonString, *FilePath))
    {
        DataTable->EmptyTable();
        FReimportManager::Instance()->Reimport(DataTable, false, true);
    }
}

void UBPFunctionLibrary::ExportDataTableAsJSON(const UDataTable* Table)
{
    // JSON 形式でデータテーブルを出力
    if (!Table)
    {
        return;
    }

    const FString JsonString = Table->GetTableAsJSON(EDataTableExportFlags::None);
    const FString OutputPath = GetExportFileName(Table, TEXT("json"));
    SaveStringToFile(JsonString, OutputPath);
}

bool UBPFunctionLibrary::UObjectToJsonString(const UObject* Object, FString& OutJsonString)
{
    //  UObject から Json 文字列に変換
    TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
    FJsonObjectConverter::UStructToJsonObject(Object->GetClass(), (void*)(Object), JsonObject.ToSharedRef(), 0, 0, nullptr);
    TSharedRef<TJsonWriter<TCHAR, TPrettyJsonPrintPolicy<TCHAR>>> JsonWriter = TJsonWriterFactory<TCHAR, TPrettyJsonPrintPolicy<TCHAR>>::Create(&OutJsonString, 0);
    return FJsonSerializer::Serialize(JsonObject.ToSharedRef(), JsonWriter);
    return false;
}

void UBPFunctionLibrary::ExportObjectAsJson(const UObject* Object, const FString& OutputPath)
{
    // JSON 形式で UObject を出力
    if (!Object)
    {
        return;
    }

    FString JsonString;
    if (UObjectToJsonString(Object, JsonString))
    {
        SaveStringToFile(JsonString, OutputPath);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("UMyBlueprintFunctionLibrary::ExportObjectAsJson: Failed to Export %s"), *Object->GetName());
    }
}

bool UBPFunctionLibrary::JsonStringToUObject(FString& JsonString, UStruct* ObjectStruct, const UObject* OutObject)
{
    // Json 文字列から UObject に変換
    if (!OutObject)
    {
        return false;
    }
    TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(JsonString);
    TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
    FJsonSerializer::Deserialize(JsonReader, JsonObject);

    if (FJsonObjectConverter::JsonObjectToUStruct(JsonObject.ToSharedRef(), ObjectStruct, (void*)OutObject, 0, 0))
    {
        OutObject->GetPackage()->MarkPackageDirty();
        return true;
    }
    return false;
}

#endif
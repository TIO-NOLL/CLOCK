// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BPFunctionLibrary.generated.h"

UCLASS()
class RESPECTCLOCK_API UBPFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:

#if WITH_EDITOR

    //ファイル名を取得する
    UFUNCTION()
    static FString GetExportFileName(const UObject* Object, const FString& Extension, const FName Name = TEXT(""));

    //Jsonファイルからオブジェクトを生成する
    UFUNCTION()
    static void ImportJsonToObject(const FString& FilePath, UObject* OutObject);
    
    //Jsonをオブジェクトとして書き出す
    UFUNCTION()
    static void ExportObjectAsJson(const UObject* Object, const FString& OutputPath);
    
    //データテーブルにJsonファイルを読み込む
    UFUNCTION()
    static void ImportDataTableToJSON(UDataTable* Table, const FString& FilePath);

    //データテーブルをJsonファイルに書き出す
    UFUNCTION()
    static void ExportDataTableAsJSON(const UDataTable* Table);
    
protected:

    // 
    UFUNCTION()
    static void SaveStringToFile(const FString& String, const FString& FilePath);

    UFUNCTION()
    static bool LoadFileToString(const FString& FilePath, FString& OutString);

    UFUNCTION()
    static bool UObjectToJsonString(const UObject* Object, FString& OutJsonString);

    UFUNCTION()
    static bool JsonStringToUObject(FString& JsonString, UStruct* ObjectStruct, const UObject* OutObject);
#endif
};

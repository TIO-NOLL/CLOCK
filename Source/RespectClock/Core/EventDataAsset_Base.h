// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EventTable.h"
#include "EventDataAsset_Base.generated.h"

class UAssetImportData;

UCLASS()
class RESPECTCLOCK_API UEventDataAsset_Base : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:

#if WITH_EDITORONLY_DATA
    static FString FileExtension;

    UPROPERTY(VisibleAnywhere, Instanced, Category = "ImportSettings")
    UAssetImportData* AssetImportData;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FileName")
    class UDataTable* DataTable;
#endif

#if WITH_EDITOR
    UFUNCTION(Category = "Import/Export", meta = (CallInEditor = true))
    void Import();

    UFUNCTION(Category = "Import/Export", meta = (CallInEditor = true))
    void OpenSourceLocation() const;

    void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent);

    void UpdateAssetImportData(FString Filename);

	virtual void LoadData();

    //StringからEnumに変換する関数
    EEventType FStringToEnum(const FString& TypeName, const FString& EnumName)
    {
        UEnum* const Enum = FindObject<UEnum>(ANY_PACKAGE, *EnumName);
        int64 Value = Enum->GetValueByName(FName(*TypeName));
        return static_cast<EEventType>(Value);
    }
#endif
public:
    UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
    TArray<FEventTableBase> DataMap;
};

// Fill out your copyright notice in the Description page of Project Settings.


#include "Settings.h"

#define LOCTEXT_NAMESPACE "Settings"

USettings::USettings()
    : Super()
#if WITH_EDITOR
    , ResourceOutputFolder(TEXT("Output"))
    , AutoExportOnEdit(true)
#endif
{}

USettings* USettings::GetSettings()
{
    return GetMutableDefault<USettings>();
}

FName USettings::GetCategoryName() const
{
    return "MySettings";
}

#if WITH_EDITOR
FText USettings::GetSectionText() const
{
    return LOCTEXT("USettings::GetSectionText", "Import/Export");
}

FText USettings::GetSectionDescription() const
{
    return LOCTEXT("USettings::GetSectionDescription", "Configuring the export and import of data assets");
}
#endif

#undef LOCTEXT_NAMESPACE
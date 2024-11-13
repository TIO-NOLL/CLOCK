#include "EventTable.h"

#if WITH_EDITOR
#include "BPFunctionLibrary.h"
#include "Settings.h"
#endif

FSoundValue::FSoundValue()
    : ID(0)
    , Name(TEXT(""))
    , Location(FVector::ZeroVector)
    , Volume(1.f)
    , Pitch(1.f)
{
}

#if WITH_EDITOR
void FEventTableBase::OnDataTableChanged(const UDataTable* InDataTable, const FName InRowName)
{
    const USettings* Settings = USettings::GetSettings();
    if ((Settings != nullptr) && (Settings->AutoExportOnEdit))
    {
        UBPFunctionLibrary::ExportDataTableAsJSON(InDataTable);
    }
}

#endif
//// Fill out your copyright notice in the Description page of Project Settings.
//
//#if WITH_EDITOR
//#include "EventDataAssetFactory.h"
//#include "EventDataAsset_Base.h"
//#include "EditorFramework/AssetImportData.h"
//#include "EditorReimportHandler.h"
//#include "JsonObjectConverter.h"
//#endif
//
//#if WITH_EDITOR
//UEventDataAssetFactory::UEventDataAssetFactory(const FObjectInitializer& ObjectInitializer)
//    : Super(ObjectInitializer)
//{
//    SupportedClass = UEventDataAsset_Base::StaticClass();
//    bCreateNew = false;
//    bEditorImport = true;
//    bText = true;
//    Formats.Add(UEventDataAsset_Base::FileExtension + TEXT(";My Data Asset"));
//}
//
//bool UEventDataAssetFactory::DoesSupportClass(UClass* Class)
//{
//    return Class->IsChildOf(UEventDataAsset_Base::StaticClass());
//}
//
//UClass* UEventDataAssetFactory::ResolveSupportedClass()
//{
//    return UEventDataAsset_Base::StaticClass();
//}
//
//UObject* UEventDataAssetFactory::FactoryCreateText(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, const TCHAR* Type, const TCHAR*& Buffer, const TCHAR* BuferEnd, FFeedbackContext* Warn)
//{
//    UClass* Class = InClass;
//    TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(Buffer);
//    TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
//    FJsonSerializer::Deserialize(JsonReader, JsonObject);
//    TSharedPtr<FJsonValue>* ClassName = (*JsonObject).Values.Find(TEXT("nativeClass"));
//    if (ClassName != nullptr)
//    {
//        UClass* FoundClass = FindObject<UClass>(ANY_PACKAGE, *(*ClassName).Get()->AsString(), false);
//        if (FoundClass != nullptr)
//        {
//            Class = FoundClass;
//        }
//        FStaticConstructObjectParameters Params(Class);
//        Params.Outer = InParent;
//        Params.Name = InName;
//        Params.SetFlags = Flags;
//        UEventDataAsset_Base* NewMyAsset = CastChecked<UEventDataAsset_Base>(StaticConstructObject_Internal(Params));
//        FJsonObjectConverter::JsonObjectToUStruct(JsonObject.ToSharedRef(), Class, (void*)(NewMyAsset), 0, 0);
//
//        return NewMyAsset;
//    }
//
//    UE_LOG(LogTemp, Error, TEXT("UMyDataAssetFactory::FactoryCreateText: nativeClass was not found"));
//    return nullptr;
//}
//
//bool UEventDataAssetFactory::CanReimport(UObject* Obj, TArray<FString>& OutFilenames)
//{
//    UEventDataAsset_Base* MyAsset = Cast<UEventDataAsset_Base>(Obj);
//    if (MyAsset != nullptr && MyAsset->AssetImportData != nullptr)
//    {
//        MyAsset->AssetImportData->ExtractFilenames(OutFilenames);
//        return true;
//    }
//    return false;
//}
//
//void UEventDataAssetFactory::SetReimportPaths(UObject* Obj, const TArray<FString>& NewReimportPaths)
//{
//    UEventDataAsset_Base* MyAsset = Cast<UEventDataAsset_Base>(Obj);
//    if (MyAsset && ensure(NewReimportPaths.Num() == 1))
//    {
//        MyAsset->AssetImportData->UpdateFilenameOnly(NewReimportPaths[0]);
//    }
//}
//
//EReimportResult::Type UEventDataAssetFactory::Reimport(UObject* Obj)
//{
//    UEventDataAsset_Base* MyAsset = Cast<UEventDataAsset_Base>(Obj);
//    if (!MyAsset)
//    {
//        return EReimportResult::Failed;
//    }
//
//    const FString Filename = MyAsset->AssetImportData->GetFirstFilename();
//    if (!Filename.Len() || IFileManager::Get().FileSize(*Filename) == INDEX_NONE)
//    {
//        return EReimportResult::Failed;
//    }
//
//    EReimportResult::Type Result = EReimportResult::Failed;
//    if (UFactory::StaticImportObject(MyAsset->GetClass(), MyAsset->GetOuter(), *MyAsset->GetName(), RF_Public | RF_Standalone, *Filename, NULL, this))
//    {
//        if (MyAsset->GetOuter())
//        {
//            MyAsset->GetOuter()->MarkPackageDirty();
//        }
//        else
//        {
//            MyAsset->MarkPackageDirty();
//        }
//        return EReimportResult::Succeeded;
//    }
//
//    return EReimportResult::Failed;
//};
//#endif
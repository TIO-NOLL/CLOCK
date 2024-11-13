//// Fill out your copyright notice in the Description page of Project Settings.
//
//#pragma once
//
//#include "CoreMinimal.h"
//
//#if WITH_EDITOR
//#include "Factories/Factory.h"
//#include "EditorReimportHandler.h"
//#include "EventDataAssetFactory.generated.h"
//#endif
//
//#if WITH_EDITOR
//UCLASS()
//class RESPECTCLOCK_API UEventDataAssetFactory : public UFactory, public FReimportHandler
//{
//    GENERATED_BODY()
//
//public:
//
//    UEventDataAssetFactory(const FObjectInitializer& ObjectInitializer);
//
//    virtual bool DoesSupportClass(UClass* Class) override;
//    virtual UClass* ResolveSupportedClass() override;
//    virtual UObject* FactoryCreateText(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, const TCHAR* Type, const TCHAR*& Buffer, const TCHAR* BuferEnd, FFeedbackContext* Warn) override;
//
//    virtual bool CanReimport(UObject* Obj, TArray<FString>& OutFilenames) override;
//    virtual void SetReimportPaths(UObject* Obj, const TArray<FString>& NewReimportPaths) override;
//    virtual EReimportResult::Type Reimport(UObject* Obj) override;
//};
//#endif
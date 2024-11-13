// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "RespectClockInstance.generated.h"

/**
 * 
 */
UCLASS()
class RESPECTCLOCK_API URespectClockInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	UPROPERTY()
	class UStaticDataManager* StaticDataManager;

private:
	UPROPERTY()
	class USoundPlayer* SoundPlayer;

	UPROPERTY()
	class UEventPlayer* EventPlayer;

public:
	virtual void Init()override;

	UFUNCTION(BlueprintCallable, Category = "SoundPlayer")
	USoundPlayer* GetSoundPlayer() const;

	UFUNCTION(BlueprintCallable, Category = "EventPlayer")
	UEventPlayer* GetEventPlayer() const;

	UFUNCTION(BlueprintCallable, Category = "SoundPlayer")
	UWorld* GetWorldfromInstance() const;

public:
	// ウィジェットブループリント(BWP)のパス
	UPROPERTY()
	FString WBPPath;

	//ウィジェット
	UPROPERTY()
	TSubclassOf<class UUserWidget> WidgetClass;

	// ウィジェットのインスタンス
	UPROPERTY()
	class UMessage* WidgetInstance;

	UFUNCTION()
	void CreateMessage();

	//タイトル画面遷移用のフラグ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Title")
	bool RemoveLogo = false;

};

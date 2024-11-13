// Message.h
// データテーブルから取得したテキストを１文字ずつ表示する

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RespectClock/Interactive/MsgType.h"
#include "Message.generated.h"

UCLASS()
class RESPECTCLOCK_API UMessage : public UUserWidget
{
	GENERATED_BODY()

protected:
	// ビューポートに追加されたときに実行される関数
	void NativeConstruct() override;	

	// 一度だけデータテーブルのロードをするため
	UPROPERTY()
	bool DataTableLoaded;		

	// データテーブルのパス
	UPROPERTY()
	FString DataPath;				

	// テキストのデータテーブル
	UPROPERTY()
	UDataTable* TextDataTable;		

	// 取得したデータのテキストをString型に変換したもの
	UPROPERTY()
	FString ObtainedText;						

	// セットするテキスト
	UPROPERTY()
	FString SetText;		

	// 取得したテキストの文字数
	UPROPERTY()
	int32 TextLen;						

	// 現在表示している文字数
	UPROPERTY()
	int32 NowTextLen = 0;				

	// １文字ずつ文字を追加する
	UFUNCTION()
	void TextCount();			

	// メッセージの初期化
	UFUNCTION()
	void ResetText();			

	// メッセージ表示中か？
	UPROPERTY()
	bool IsActive;

	// テキスト更新時間
	UPROPERTY()
	int32 TextUpdateTime = 0.05f;

	// テキスト表示時間
	UPROPERTY()
	float TextDisplayTime = 1.f;

public:
	// メッセージテキスト
	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional))
	class UTextBlock* DisplayText;	

	// テキストをセットする関数
	UFUNCTION()
	void DisplayMessage(FString _Text, EMsgType _type);			

	// タイマー取得用
	UPROPERTY()
	UWorld* World;
	UPROPERTY()
	FTimerHandle Handle;

	UFUNCTION()
	bool GetActive() { return IsActive; }
};
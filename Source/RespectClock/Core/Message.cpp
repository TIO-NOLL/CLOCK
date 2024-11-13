// Message.cpp

#include "Message.h"
#include "TextData.h"
#include "Components/TextBlock.h"
#include "RespectClock/Interactive/ItemData.h"
#include "RespectClock/Core/GM_RespectClock.h"
#include "RespectClock/Interactive/MsgType.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void UMessage::NativeConstruct()
{
    Super::NativeConstruct();

    // タイマー用
    if (!World)
        World = GEngine->GameViewport->GetWorld();

    // データテーブルのロード
    // 正常にロードできれば実行されないようになる
    if (!DataTableLoaded)
    {
        DataPath = TEXT("/Game/RespectClock/Interactive/Items/DT_textData");
        TextDataTable = LoadObject<UDataTable>(nullptr, *DataPath);
    }
    if (TextDataTable && !DataTableLoaded)
        DataTableLoaded = true;
}

// テキストをセットして表示する関数
void UMessage::DisplayMessage(FString _text, EMsgType _type)
{
    // 受け取ったテキストが前回と違うものであればリセットする
    if (_text != ObtainedText)
        ResetText();

    // テキストを表示中でなければテキスト表示開始
    if (!IsActive)
    {
        // ビューポートに追加
        AddToViewport();
        // アクティブ化する
        IsActive = true;
        // １文字ずつ追加する関数を指定秒ごとに起動
        if (World)
            World->GetTimerManager().SetTimer(Handle, this, &UMessage::TextCount, 0.05f, true);
        // データテーブルがロードされているなら
        if (DataTableLoaded)
        {
            // 行名を取得する
            TArray<FName> _RowNames = TextDataTable->GetRowNames();
            //アイテムの場合
            if (_type == EMsgType::Item)
                ObtainedText = _text + TextDataTable->FindRow<FTextData>(_RowNames[2], FString())->Info.ToString();
            else    ObtainedText = _text;
            TextLen = ObtainedText.Len();     //　テキストの文字数を取得
        }
        // データテーブルが読み込めていない場合
        else
            UE_LOG(LogTemp, Error, TEXT("DataTable Not Found."));
        // 指定したIDにデータがない
        if (ObtainedText.IsEmpty())
            UE_LOG(LogTemp, Error, TEXT("TextData Not Found."));
    }
}

void UMessage::TextCount()
{
    // 現在のテキストに１文字だけ追加
    SetText.Append(ObtainedText.Mid(NowTextLen, 1));
        // 現在の文字数が渡された文字数と同じになるまで実行
        if (NowTextLen < TextLen)                         
        {
            // テキストをセット(これが実際に表示されるテキスト)
            DisplayText->SetText(FText::FromString(SetText));
            // 現在の文字数を追加
            NowTextLen++;                                                   
        }
        else
            // 指定秒数後にテキストを非表示
            if(World)
            World->GetTimerManager().SetTimer(Handle, this, &UMessage::ResetText, TextDisplayTime, false);
}

void UMessage::ResetText()
{
    // 非表示にする
    RemoveFromParent();
    // テキストを空白にする
    FText reset;
    DisplayText->SetText(reset);
    NowTextLen = 0;
    SetText = "";
    // 非アクティブ化する
    IsActive = false;
}
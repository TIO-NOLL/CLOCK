//--------------------------------------------------------------------------------------
// ファイル名：SoundDataAsset.cpp
// 内容		：SoundDataTableRowのテータテーブルを保持、リインポートができるデータアセット
// 作成者	：22cu0127 丹代天
// 更新日	：2024/05/30		作成、リインポート用の関数追加
//--------------------------------------------------------------------------------------

#include "SoundDataAsset.h"

#if WITH_EDITOR
#include "Misc/MessageDialog.h"
#include "UObject/Package.h"
#include "EditorReimportHandler.h"
#include "FileHelpers.h"
#endif

#define LOCTEXT_NAMESPACE "TEST"

void USoundDataAsset::LoadData()
{
#if WITH_EDITORONLY_DATA

    // データテーブルのNULLチェック
    if (SoundDataTable == nullptr)
    {
        const FText TitleText = LOCTEXT("Title", "WarningMassege");
        FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("Message", "DataTable is Null !!"), &TitleText);
        return;
    }

    // データテーブルの型チェック
    if (!SoundDataTable->GetRowStruct()->IsChildOf(FSoundDataTableRow::StaticStruct()))
    {
        const FText TitleText = LOCTEXT("Title", "WarningMassege");
        FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("Message", "DataTable type does not match !!"), &TitleText);
        return;
    }

    TArray<UPackage*> PackagesToSave;

    // データテーブルをリインポート
    if (FReimportManager::Instance()->Reimport(SoundDataTable, false, true))
    {
        // リインポートに成功したデータテーブルを保存対象に追加
        PackagesToSave.Add(SoundDataTable->GetOutermost());
    }

    SoundDataMap.Empty();

    // データテーブルの行の要素を配列で取得
    TArray RowNames = SoundDataTable->GetRowNames();
    // 行の数だけループ
    for (auto RowName : RowNames)
    {
        // １行分の構造体を取得
        FSoundDataTableRow* TableRow = SoundDataTable->FindRow< FSoundDataTableRow >(RowName, FString());

        if (TableRow)
        {
            //UE_LOG(LogTemp, Log, TEXT("Loaded Row - ID: %d, Name: %s"), TableRow->ID, *TableRow->Name);

            FSoundDataTableRow SoundData;
            SoundData.Name = TableRow->Name;
            SoundData.ID = TableRow->ID;
            SoundData.SoundObj = TableRow->SoundObj;

            SoundDataMap.Add(SoundData);
        }
        else
        {
            //UE_LOG(LogTemp, Error, TEXT("Failed to find row: %s"), *RowName.ToString());
        }
    }

    // データアセットに編集フラグを追加
    MarkPackageDirty();

    // データアセットを保存対象に追加
    PackagesToSave.Add(GetOutermost());

    // 関連アセットを全て保存（SourceControl使用時はチェックアウトするかメッセージウィンドウを出す）
    // ファイル編集フラグ（Dirty)が付いてるもののみを保存対象にしたいので第一引数はtrue
    // 保存する際に確認のメッセージウィンドウを出さない場合は第二引数をfalseにする
    FEditorFileUtils::PromptForCheckoutAndSave(PackagesToSave, true, true);

#endif
}
#undef LOCTEXT_NAMESPACE
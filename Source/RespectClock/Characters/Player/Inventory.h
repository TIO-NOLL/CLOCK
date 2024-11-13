// Inventory.h

#pragma once

#include "CoreMinimal.h"
#include "RespectClock/Interactive/ItemBase.h"
#include "Inventory.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class RESPECTCLOCK_API UInventory : public UActorComponent
{
    GENERATED_BODY()
    
public:
    UInventory();

public:
    // 所持アイテム取得
    UFUNCTION(BlueprintCallable, Category = "Inventory")
    TArray<AItemBase*> GetItem();       
   
    // IDで検索して見つけたらtrueを返す
    UFUNCTION(BlueprintCallable,Category = "Inventory")
    bool MatchItemID(int32 _ID);

    // スロット検索
    UFUNCTION(BlueprintCallable, Category = "Inventory")
    bool MatchSlotNum(int32 _Num);

    // アイテム使用
    UFUNCTION(BlueprintCallable, Category = "Inventory")
    void UseItem(int32 _itemNum);

    // 最後に使用したアイテム
    UPROPERTY(BlueprintReadWrite, Category = "Inventory")
    AItemBase* LastUsedItem;

    // アイテム追加  
    UFUNCTION(Category = "Inventory")
    void AddItem(AItemBase* _item);     

    // アイテム削除
    UFUNCTION(Category = "Inventory")
    void DeleteItem(AItemBase* _item);  

protected:
    // インベントリ
    UPROPERTY()
    TArray<AItemBase*> Inventory;


};
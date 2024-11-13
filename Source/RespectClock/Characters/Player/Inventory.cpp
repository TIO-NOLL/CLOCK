// Inventory.cpp

#include "Inventory.h"

UInventory::UInventory()
	:LastUsedItem(nullptr)
{

}

// インベントリのアイテムを取得する処理
TArray<AItemBase*> UInventory::GetItem()
{
	return Inventory;
}

bool UInventory::MatchItemID(int32 _ID)
{
	for (AItemBase* Item : Inventory)
	{
		if (Item->GetID() == _ID)
			return true;	// 見つけた時点で終了
	}
	return false;
}

bool UInventory::MatchSlotNum(int32 _Num)
{
	if (Inventory.IsValidIndex(_Num))
		return true;
	return false;
}

void UInventory::UseItem(int32 _Num)
{
	LastUsedItem = Inventory[_Num];
	//UE_LOG(LogTemp, Error, TEXT("SlotNum:%d"), _Num);
	//UE_LOG(LogTemp, Error, TEXT("Description:%s"), *Inventory[_Num]->GetDescription().ToString());
}

// アイテムをインベントリに追加する処理
void UInventory::AddItem(AItemBase* _item)
{
	if(_item)
		Inventory.Add(_item);
}

// インベントリからアイテムを検索して、一致したものがあれば削除する
void UInventory::DeleteItem(AItemBase* _item)
{
	if (_item)
    {
        for (int32 i = Inventory.Num() - 1; i >= 0; --i)
        {
            if (Inventory[i] && Inventory[i]->GetID() == _item->GetID())
            {
				Inventory[i] = nullptr;
                Inventory.RemoveAt(i);
            }
        }
    }
}
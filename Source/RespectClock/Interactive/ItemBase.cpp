// ItemBase.cpp

#include "ItemBase.h"
#include "RespectClock/Characters/Player/PlayerCharacter.h"
#include "RespectClock/Characters/Player/Inventory.h"
#include "Kismet/GameplayStatics.h"
#include "RespectClock/Core/Message.h"
#include "RespectClock/Core/RespectClockInstance.h"

// Sets default values
AItemBase::AItemBase()
    :Player(nullptr)
{

}

// Called when the game starts or when spawned
void AItemBase::BeginPlay()
{
    Player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
    // 開始時に非表示にする
    if (IsActiveBeginPlay)
        ActiveItem(false);
}

// インタラクトされたらアイテムをインベントリに追加
void AItemBase::Interactive()
{
    if(!Player)
        Player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

    if (Player != NULL)
    {
        // インベントリに追加
        Player->GetInventory()->AddItem(this);
        // メッセージを表示
        SendMessage(ItemData.Name.ToString(), EMsgType::Item);
        // アイテムが追加された後の処理
        AddedToInventory();
    }
}

EPlayerStatus AItemBase::GetStatus()
{
    return EPlayerStatus::PickUp;
}

// アイテム有無効化処理
void AItemBase::ActiveItem(bool _IsActive)
{
    if (_IsActive)
    {
        SetActorHiddenInGame(false);    // アイテムを表示
        SetActorEnableCollision(true);  // コリジョン有効
    }
    else
    {
        SetActorHiddenInGame(true);     // アイテムを非表示
        SetActorEnableCollision(false); // コリジョン無効
    }
}


// アイテムがインベントリに追加されたときの処理
void AItemBase::AddedToInventory()
{
    if (IsEventCallable)
        BPCallEventItem();
    ActiveItem(false);      // 非表示にする
    Destroy();              // 破棄
}

FRotator AItemBase::GetLerpPointRot()
{
    return Player->GetActorRotation();
}
FVector AItemBase::GetLerpPointPos()
{
    return Player->GetActorLocation();
}

// オブジェクトをエディタで指定できる機能
TArray<FName> AItemBase::GetRowNames() const
{
    TArray<FName> RowNames;
    RowNames.Empty();
#if WITH_EDITOR	
    if (DataTable)
    {
        RowNames = DataTable->GetRowNames();
    }
#endif
    return RowNames;
}
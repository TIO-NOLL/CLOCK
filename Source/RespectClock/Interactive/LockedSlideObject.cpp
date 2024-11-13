// Fill out your copyright notice in the Description page of Project Settings.


#include "LockedSlideObject.h"
#include "Kismet/GameplayStatics.h"
#include "RespectClock/Characters/Player/PlayerCharacter.h"
#include "ItemBase.h"
#include "MsgType.h"
#include "RespectClock/Characters/Player/Inventory.h"
#include "RespectClock/Core/Message.h"

void ALockedSlideObject::Interactive()
{
	if (IsOpenable) Super::Interactive();
}

EPlayerStatus ALockedSlideObject::GetStatus()
{
	// プレイヤーが鍵を使用したか
	if (!IsOpenable)
	{
		if (Player == nullptr) Player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		if (Player != nullptr)
		{
			TObjectPtr<UInventory> Inventory = Player->GetInventory();
			if (Inventory->LastUsedItem)
			{
				if (Inventory->LastUsedItem->GetID() == DoorID)
				{
					IsOpenable = true;
					SendMessage(OpenableMessage, EMsgType::Door);	// 開錠	
				}
				else if (Inventory->LastUsedItem->GetID() != DoorID)
					SendMessage(DifferentMessage, EMsgType::Door);	// アイテムの番号が違う		
			}
			else SendMessage(DisableMessage, EMsgType::Door);	// ドアを開けられない
		}
	}
	return EPlayerStatus::Idle;
}

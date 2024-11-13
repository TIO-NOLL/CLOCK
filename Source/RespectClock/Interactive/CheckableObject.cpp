// Fill out your copyright notice in the Description page of Project Settings.


#include "CheckableObject.h"
#include "Kismet/GameplayStatics.h"
#include "RespectClock/Characters/Player/PlayerCharacter.h"
#include "RespectClock/Core/Message.h"
#include "MsgType.h"
#include "ObjectData.h"

ACheckableObject::ACheckableObject()
{
	
}

void ACheckableObject::BeginPlay()
{
	if (Player == nullptr)
		Player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}

// イベント呼び出し
void ACheckableObject::CallEvent()
{
	IsEventCallable = false;
}

// インタラクト処理
void ACheckableObject::Interactive()
{
	if (Player == nullptr)
		Player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	// テキスト送信
	if (Player != nullptr)
		SendMessage(ObjectData.Description.ToString(), EMsgType::CheckObj);

	// イベントを呼び出せるものなら呼び出し
	if (IsEventCallable)
		BPCallEvent();

	if (Player)
		Player->bInvincible = false;
}

EPlayerStatus ACheckableObject::GetStatus()
{
	if(ObjectData.Anim == EPlayerStatus::None)
		Player->IsMoveable = true;
	return ObjectData.Anim;
}
FRotator ACheckableObject::GetLerpPointRot()
{
	return Player->GetActorRotation();
}
FVector ACheckableObject::GetLerpPointPos()
{
	return Player->GetActorLocation();
}

// オブジェクトをエディタで指定できる機能
TArray<FName> ACheckableObject::GetRowNames() const
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
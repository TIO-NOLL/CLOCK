// Fill out your copyright notice in the Description page of Project Settings.


#include "EventPlayVolume.h"
#include "EventPlayer.h"
#include "RespectClockInstance.h"
#include "Kismet/GameplayStatics.h"
#include "RespectClock/Characters/Player/PlayerCharacter.h"

void AEventPlayVolume::BeginPlay()
{
	Super::BeginPlay();

	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GetWorld());

	if (GameInstance)
	{
		URespectClockInstance* RCGameInstance = Cast<URespectClockInstance>(GameInstance);
		if (RCGameInstance)
		{
			EP = RCGameInstance->GetEventPlayer();
		}
	}
}

void AEventPlayVolume::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (m_bOncePlay) return;

	APlayerCharacter* PlayerChara = Cast<APlayerCharacter>(OtherActor);

	if (!PlayerChara) return;

	if (EP)
	{
		if (EventID != 0) EP->PlayEventByID(GetWorld(), EventID);
		else if (EventName != TEXT("")) EP->PlayEventByName(GetWorld(), EventName);
	}

	if (m_bIsOncePlayEvent == true)
	{
		m_bOncePlay = true;
	}
}
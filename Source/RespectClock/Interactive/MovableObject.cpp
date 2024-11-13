// MovableObject.cpp


#include "MovableObject.h"
#include "RespectClock/Characters/Player/PlayerCharacter.h"
#include "MsgType.h"
#include "Kismet/GameplayStatics.h"

AMovableObject::AMovableObject()
	:MovableDistance(0.0f)
	,CurrentMoveDistance()
	,CanMove(true)
	,Player()
{

}

void AMovableObject::BeginPlay()
{
	Super::BeginPlay();
	SetActorTickEnabled(false);
	DefaultPos = GetActorLocation();
	Player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}

void AMovableObject::Interactive()
{
	if (CanMove)
	{
		SetActorTickEnabled(true);
	}
	else SetPlayerState();
}

void AMovableObject::Tick(float DeltaTime)
{
	//if (CurrentMoveDistance >= MovableDistance)
	//	CanMove = false;

	if (Player->IsInteractable && CanMove)
	{
		FVector location = GetActorLocation();
		FVector playerLoc = Player->GetActorLocation();
		FRotator playerRot = Player->GetActorRotation();
		float DirectionRadian = FMath::DegreesToRadians(playerRot.Yaw);	// プレイヤーの現在角度を変換する
		FVector DirectionVector = FVector(FMath::Cos(DirectionRadian), FMath::Sin(DirectionRadian), 0.0f);	// ベクターに変換

		FVector Movement = DirectionVector * 1.0f;	// 移動距離の設定 プレイヤーの角度 * 移動距離

		location +=  Movement;
		playerLoc += Movement;
		SetActorLocation(location);
		//StaticMesh->AddForce(Movement)
		Player->SetActorLocation(playerLoc);
	}
	else
	{
		SetActorTickEnabled(false);	
		SetPlayerState();
	}
}

void AMovableObject::SetPlayerState()
{
	Player->IsMoveable = true;
	Player->PlayerStatus = EPlayerStatus::Idle;
}

FRotator AMovableObject::GetLerpPointRot()
{
	return Player->GetActorRotation();
}
FVector AMovableObject::GetLerpPointPos()
{
	return Player->GetActorLocation();
}
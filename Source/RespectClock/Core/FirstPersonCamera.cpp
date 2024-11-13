#include "FirstPersonCamera.h"
#include "RespectClock/Characters/Player/PlayerCharacter.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"

AFirstPersonCamera::AFirstPersonCamera()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AFirstPersonCamera::BeginPlay()
{
	Super::BeginPlay();

	// APawn の取得
	APawn* aPlayerPawn = UGameplayStatics::GetPlayerPawn(this->GetWorld(), 0);
	TargetCharacter = (AActor*)aPlayerPawn;

	// 頭の高さに補正
	// this->SetActorLocation(TragetCharacter->GetActorLocation() + HeadOffset);

}

// Called every frame
void AFirstPersonCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector HeadHight = FVector(TargetCharacter->GetActorLocation().X, TargetCharacter->GetActorLocation().Y, TargetCharacter->GetActorLocation().Z + HeadOffset);

	// 位置を設定
	this->SetActorLocation(HeadHight);

}
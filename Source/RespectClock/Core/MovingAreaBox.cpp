/*
* FileName	: MovingAreaBox.h
* Created	: Misora Tanaka
* Summary	: 行動範囲を指定する箱
* Update	: ヘッダファイルに記載
*/


#include "MovingAreaBox.h"

// Sets default values
AMovingAreaBox::AMovingAreaBox()
{
	MovingBoxPtr = CreateDefaultSubobject<UBoxComponent>(TEXT("MovingBox"));
	RootComponent = Cast<USceneComponent>(MovingBoxPtr);

 	// 07/04現在Tick動く必要ないので
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AMovingAreaBox::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMovingAreaBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


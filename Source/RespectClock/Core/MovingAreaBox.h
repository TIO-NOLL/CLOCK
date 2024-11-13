/*
* FileName	: MovingAreaBox.h
* Created	: Misora Tanaka
* Summary	: 行動範囲を指定する箱
* Update	: 24/07/04 作成
*/

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MovingAreaBox.generated.h"

// 前方宣言
class UBoxComponent;

UCLASS()
class RESPECTCLOCK_API AMovingAreaBox : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMovingAreaBox();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	// 動ける範囲
	UPROPERTY(EditAnywhere, Category = "Box")
	TObjectPtr<UBoxComponent> MovingBoxPtr;

};

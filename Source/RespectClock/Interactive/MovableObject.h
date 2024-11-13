// MovableObject.h

#pragma once

#include "CoreMinimal.h"
#include "InteractiveObject.h"
#include "MovableObject.generated.h"

class APlayerCharacter;

/**
 *
 */
UCLASS()
class RESPECTCLOCK_API AMovableObject : public AInteractiveObject
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	AMovableObject();
	virtual void Interactive() override;
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 MovableDistance;	// 移動可能距離
	int32 CurrentMoveDistance;	// 移動した距離
	FVector DefaultPos;		// 最初の位置
	bool CanMove;			// 移動可能か
	APlayerCharacter* Player;	// プレイヤー

	void SetPlayerState();

	virtual FRotator GetLerpPointRot() override;
	virtual FVector GetLerpPointPos() override;
};

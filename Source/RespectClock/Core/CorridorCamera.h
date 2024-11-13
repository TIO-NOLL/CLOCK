///--------------------------------------------------------------------------------------
// ファイル名	：CorridorCamera.h
// 内容			：廊下用のカメラ作成
// 作成者		：22cu0236 八町陸斗
// 更新日		：2024/04/11		作成
//--------------------------------------------------------------------------------------
#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraActor.h"
#include "CorridorCamera.generated.h"

UCLASS()
class RESPECTCLOCK_API ACorridorCamera : public ACameraActor
{
	GENERATED_BODY()

public:
	ACorridorCamera();
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// 
	virtual void BeginPlay() override;

protected:
	// ターゲットキャラクター
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* TargetCharacter;

	// Corridor Camera の初期位置
	FVector startLocation;

	// Corridor Camera の初期位置
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PlayerDistance;
	
	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//APlayerCharacter* PlayerCharacterCheck;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//APlayerController* PlayerCharacterController;

	
protected:

	UFUNCTION()
	FVector AxisChange(bool bAxis);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)	
	bool bAxisXY; 
	
	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//bool bsignPlus;

};

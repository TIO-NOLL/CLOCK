// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractableInterface.h"
#include "InteractiveObject.generated.h"

enum class EMsgType : uint8;
class UBoxComponent;
class USceneComponent;
class URespectClockInstance;

UCLASS()
class RESPECTCLOCK_API AInteractiveObject : public AActor, public IInteractableInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractiveObject();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	/// @brief 線形補間する座標を返す 
	virtual FVector GetLerpPointPos();
	virtual FVector GetSecondLerpPointPos()override;
	/// @brief 線形補間する角度を返す 
	virtual FRotator GetLerpPointRot();
	virtual FRotator GetSecondLerpPointRot()override;
	/// @brief インタラクト可能かどうかを返す 
	bool IsInteractive();
	/// @brief インタラクトされた際の処理
	virtual void Interactive() override; 
	/// @brief インタラクトした際のプレイヤーの状態を返す 
	virtual EPlayerStatus GetStatus() override;
	/// @brief インタラクトした際にカメラを変える
	virtual void ChangeCamera() override;
	/// @brief オーバーラップが始まった際の処理
	UFUNCTION()
 	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	/// @brief オーバーラップが終わった際の処理
	UFUNCTION()
	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USceneComponent> LerpPointScene;		/* インタラクトした際に線形補間するポイント */

protected:
	/// @brief テキスト送信
	UFUNCTION()
	void SendMessage(FString _text, EMsgType _type);


protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBoxComponent> Box;					/* 当たり判定ボックス */
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> StaticMesh;	/* メッシュ */
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> DefaultScene;		/* デフォルトのシーン */
	UPROPERTY(EditAnywhere)
	TObjectPtr<UMaterialInterface> OutLineMaterial; /* アウトラインのマテリアル */
	UPROPERTY()
	bool IsInteractable;							/* インタラクト可能か */
	UPROPERTY(EditAnywhere)
	EPlayerStatus PlayerStatus;						/* インタラクトされた際に返すプレイヤーの状態 */

};

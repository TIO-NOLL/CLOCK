
#pragma once

#include "CoreMinimal.h"
#include "InteractiveObject.h"
#include "Door.generated.h"

class URoomNameWidget;

UENUM(BlueprintType)
enum class EDoorStatus : uint8
{
    None,
    Close,
    Open_Push,
    Open_Pull
};

UCLASS()
class RESPECTCLOCK_API ADoor : public AInteractiveObject
{
	GENERATED_BODY()

public:
	ADoor();

protected:
	void BeginPlay();

public:
    // Called every frame
    void Tick(float DeltaTime) override;

    FVector GetLerpPointPos();
    FRotator GetLerpPointRot();

    UFUNCTION(BlueprintCallable, Category = "Trash")
    void Interactive() override;

    virtual EPlayerStatus GetStatus();

    UFUNCTION(BlueprintImplementableEvent)
    USceneComponent* GetRotationRoot();
    UFUNCTION(BlueprintCallable)
    void ShowRoomName();

protected:
    // プレイヤーが前にいるのか後ろにいるのか判定する関数
    bool IsPlayerInFrontOfActor();
    // プレイヤーの位置を変更
    void SetPlayerLocation();

public:
    // 05/20 追加　間に合わせの音追加
    UFUNCTION(BlueprintImplementableEvent)
    void PlaySound();

    bool bIsOpenable()const;

    // インタラクティブした時に線形補間するポイント
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    TObjectPtr<USceneComponent> LerpPointSceneInRoom;

private:
    // 経過時間
    UPROPERTY()
    float ElapsedTime;
    // ドアの状態
    UPROPERTY()
    EDoorStatus DoorStatus;

protected:
    // プレイヤーがドアの前側にいるか
    UPROPERTY()
    bool IsPlayerInFrontOf;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ChangeLocationTime = 2.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool IsOpenable;

    /// @brief 入る部屋の名前
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="RoomName")
    FString RoomNameEnter;
    /// @brief 出る部屋の名前
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="RoomName")
    FString RoomNameExit;

    /// @brief 部屋名のUI
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<URoomNameWidget> RoomNameWidgetClass;
    UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)
    URoomNameWidget* RoomNameWidget;
};

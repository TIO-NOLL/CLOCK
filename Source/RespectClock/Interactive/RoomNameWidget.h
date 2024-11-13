
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RoomNameWidget.generated.h"

class UTextBlock;

UCLASS()
class RESPECTCLOCK_API URoomNameWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetRoomName(const FString& RoomName);

	UFUNCTION(BlueprintImplementableEvent)
	void ShowRoomName();

protected:
	UPROPERTY(meta=(BindWidget))
	UTextBlock* RoomNameText;
};


#include "RoomNameWidget.h"
#include "Components/TextBlock.h"

void URoomNameWidget::SetRoomName(const FString& RoomName)
{
	// 部屋の名前を設定
	if(RoomNameText)
	{
		RoomNameText->SetText(FText::FromString(RoomName));
	}
}

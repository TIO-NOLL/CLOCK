// MsgType.h

#pragma once

UENUM(BlueprintType)
enum class EMsgType : uint8
{
    Item        UMETA(DisplayName = "Item"),
    CheckObj    UMETA(DisplayName = "CheckObj"),
    MoveObj     UMETA(DisplayName = "MoveObj"),
    Door        UMETA(DisplayName = "Door"),
    Story       UMETA(DisplayName = "Story"),
};
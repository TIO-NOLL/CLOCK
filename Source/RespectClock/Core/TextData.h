// TextData.h

#pragma once

#include "CoreMinimal.h"
#include "TextData.generated.h"

USTRUCT(BlueprintType)
struct FTextData : public FTableRowBase
{
	GENERATED_BODY()

public:
	// 追加した変数
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Text")
	FText Info;	// テキスト情報

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Text")
	int32 ID;	// 識別用ID

	FTextData()
		: Info(FText::FromString(TEXT("")))
		, ID(0)
	{}
};
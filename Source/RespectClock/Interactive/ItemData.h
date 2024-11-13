// ItemData.h
// アイテムのデータの構造体

#pragma once

#include "CoreMinimal.h"
#include "ItemData.generated.h"

USTRUCT(BlueprintType)
struct FItemData : public FTableRowBase
{
    GENERATED_BODY()

public:
	// アイテム名
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "ItemData")
	FName Name;	

	// アイテムID
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "ItemData")
	int ID;	

	// メッシュ
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "ItemData")
	TObjectPtr<UStaticMesh> Mesh;

	// アイテム欄で表示するテクスチャ 
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "ItemData")
	UTexture2D* Texture; 

	// アイテムの説明
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "ItemData")
	FText Description;	

	FItemData()
		: Name(NAME_None)
		, ID(0)
		, Mesh(nullptr)
		, Texture(nullptr)
		, Description(FText::FromString(TEXT("")))
	{}

};
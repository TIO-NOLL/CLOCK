// ObjectData.h

#pragma once

#include "CoreMinimal.h"
#include "ObjectData.generated.h"

USTRUCT(BlueprintType)
struct FObjectData : public FTableRowBase
{
	GENERATED_BODY()

public:
	// オブジェクト名
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "ObjectData", DisplayName = "オブジェクト名")
	FName Name;

	// オブジェクトID
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "ObjectData", DisplayName = "オブジェクトID")
	int ID;

	// メッシュ
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "ObjectData", DisplayName = "メッシュ")
	TObjectPtr<UStaticMesh> Mesh;

	// テキスト
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "ObjectData", DisplayName = "テキスト")
	FText Description;

	// プレイヤーアニメーション
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "ObjectData", DisplayName = "プレイヤーアニメーション")
	EPlayerStatus Anim;

	// メッシュ調整用
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "ObjectData", DisplayName = "メッシュのズレ修正用座標")
	FVector MeshPos;

	// 当たり判定調整
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "ObjectData", DisplayName = "当たり判定スケール修正用")
	FVector CollisionScale;

	FObjectData()
		: Name(NAME_None)
		, ID(0)
		, Mesh(nullptr)
		, Description(FText::FromString(TEXT("")))
		, Anim(EPlayerStatus::None)
		, MeshPos(0, 0, 0)
		, CollisionScale(1,1,1)
	{}
};
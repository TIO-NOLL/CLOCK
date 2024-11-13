
// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractiveObject.h"
#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "MsgType.h"
#include "Kismet/GameplayStatics.h"
#include "RespectClock/Core/Message.h"
#include "RespectClock/Core/RespectClockInstance.h"
#include "RespectClock/Characters/Player/PlayerCharacter.h"


// Sets default values
AInteractiveObject::AInteractiveObject()
	:IsInteractable(true)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DefaultScene = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = DefaultScene;

	LerpPointScene = CreateDefaultSubobject<USceneComponent>(TEXT("LerpPoint"));
	LerpPointScene->SetupAttachment(RootComponent);

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	Box->SetupAttachment(RootComponent);
	Box->OnComponentBeginOverlap.AddDynamic(this, &AInteractiveObject::OnOverlapBegin);
	Box->OnComponentEndOverlap.AddDynamic(this, &AInteractiveObject::OnOverlapEnd);

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(RootComponent);

	//OutLineMaterial = LoadObject<UMaterialInterface>(NULL, TEXT("/RespectClock/Content/RespectClock/Assets/Materials/M_Outline.uasset"), NULL, LOAD_None, NULL);
}

// Called when the game starts or when spawned
void AInteractiveObject::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AInteractiveObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

FVector AInteractiveObject::GetLerpPointPos()
{
	if (!IsValid(LerpPointScene)) return FVector::ZeroVector;
	return LerpPointScene->GetComponentLocation();
}

FRotator AInteractiveObject::GetLerpPointRot()
{
	if (!IsValid(LerpPointScene)) return FRotator::ZeroRotator;
	FRotator rot = LerpPointScene->GetComponentRotation();
	// プレイヤー回ってるから補正
	rot.Yaw -= 90.0f;
	return rot;
}

bool AInteractiveObject::IsInteractive()
{
	return IsInteractable;
}

void AInteractiveObject::Interactive()
{
	// アイテムとのインタラクト処理
}

FVector AInteractiveObject::GetSecondLerpPointPos()
{
	return FVector();
}

FRotator AInteractiveObject::GetSecondLerpPointRot()
{
	return FRotator();
}

EPlayerStatus AInteractiveObject::GetStatus()
{
	return PlayerStatus;
}

void AInteractiveObject::ChangeCamera()
{
}

void AInteractiveObject::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//UE_LOG(LogTemp, Warning, TEXT("OnOverlap"));
	StaticMesh->SetOverlayMaterial(OutLineMaterial);
}

void AInteractiveObject::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//UE_LOG(LogTemp, Warning, TEXT("NonOverlap"));
	StaticMesh->SetOverlayMaterial(nullptr);
}

// テキストを送信して表示
void AInteractiveObject::SendMessage(FString _text, EMsgType _type)
{
	TObjectPtr<URespectClockInstance> GameInstance;
	GameInstance = Cast<URespectClockInstance>(UGameplayStatics::GetGameInstance(this));
	GameInstance->WidgetInstance->DisplayMessage(_text, _type);
}


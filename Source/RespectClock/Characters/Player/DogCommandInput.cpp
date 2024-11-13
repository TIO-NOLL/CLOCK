#include "DogCommandInput.h"
#include "Kismet/GameplayStatics.h"
#include "RespectClock/Characters/Dog/AIC_Dog.h"

// Sets default values for this component's properties
UDogCommandInput::UDogCommandInput()
    :Dog(nullptr)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UDogCommandInput::BeginPlay()
{
	Super::BeginPlay();

    GetDog();
	// ...
	
}


// Called every frame
void UDogCommandInput::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UDogCommandInput::Call()
{
    Dog->SwitchDogState(EDogState::Tracking);
}

void UDogCommandInput::Stay()
{
    Dog->SwitchDogState(EDogState::Stay);
}

void UDogCommandInput::MoveForward()
{
    Dog->SwitchDogState(EDogState::Stroke);
}

void UDogCommandInput::GetDog()
{
    TArray<AActor*> Actors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAIC_Dog::StaticClass(), Actors);

    for (AActor* actor : Actors)
    {
        AAIC_Dog* dog = Cast<AAIC_Dog>(actor);
        if (dog != nullptr)
        {
            Dog = dog;
        }
    }
}


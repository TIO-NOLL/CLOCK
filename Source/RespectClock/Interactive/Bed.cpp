#include "Bed.h"

ABed::ABed()
{

}

void ABed::BeginPlay()
{
	Super::BeginPlay();
	SetActorTickEnabled(false);
}

void ABed::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABed::Interactive()
{
	Super::Interactive();
}

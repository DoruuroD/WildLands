// Fill out your copyright notice in the Description page of Project Settings.


#include "WoodResource.h"

// Sets default values for this component's properties
UWoodResource::UWoodResource()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	ResourceData.ResourceType = EResourceType::Forest;
	ResourceData.ResourceAmount = 65;
	//ResourceMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ResourceMesh"));
}


// Called when the game starts
void UWoodResource::BeginPlay()
{
	Super::BeginPlay();
	//ObjectType* object = ConstructObject< ObjectType >(UClassReference);
	// ...

}


// Called every frame
void UWoodResource::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}
// Fill out your copyright notice in the Description page of Project Settings.


#include "Resource.h"

// Sets default values for this component's properties
UResource::UResource()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	ResourceData.ResourceAmount = 20;
	ResourceData.ResourceType = EResourceType::UnknownResource;
}


// Called when the game starts
void UResource::BeginPlay()
{
	Super::BeginPlay();
	//ObjectType* object = ConstructObject< ObjectType >(UClassReference);
	// ...

}


// Called every frame
void UResource::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}
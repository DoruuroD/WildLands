// Fill out your copyright notice in the Description page of Project Settings.


#include "MeadowResource.h"

// Sets default values for this component's properties
UMeadowResource::UMeadowResource()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	ResourceData.ResourceType = EResourceType::Meadow;
	ResourceData.ResourceAmount = 50;
	
	//ResourceMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ResourceMesh"));
}


// Called when the game starts
void UMeadowResource::BeginPlay()
{
	Super::BeginPlay();
	//ObjectType* object = ConstructObject< ObjectType >(UClassReference);
	// ...

}


// Called every frame
void UMeadowResource::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}
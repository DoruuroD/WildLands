// Fill out your copyright notice in the Description page of Project Settings.

#include "Engine/World.h"
#include "Materials/Material.h"
#include "Components/StaticMeshComponent.h"
#include "CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Building.h"
#include "Citizen.h"
// Sets default values
UBuilding::UBuilding()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
//	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> TempStaticMesh(TEXT("staticmesh'/game/meshes/lumberjackhut.lumberjackhut'"));
	if (TempStaticMesh.Succeeded())
	{
		this->SetStaticMesh(TempStaticMesh.Object);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("failed to load building mesh"));
	}

}

// Called when the game starts or when spawned
void UBuilding::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void UBuilding::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

 void UBuilding::GenerateResource() { UE_LOG(LogTemp, Warning, TEXT("We are in Building")); }

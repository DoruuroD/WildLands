// Fill out your copyright notice in the Description page of Project Settings.

#include "Engine/World.h"
#include "Materials/Material.h"
#include "Components/StaticMeshComponent.h"
#include "CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Building.h"
#include "Citizen.h"
// Sets default values
ABuilding::ABuilding()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BuildingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BuildingMesh"));
	RootComponent = BuildingMesh;
	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMesh(TEXT("staticmesh'/game/meshes/lumberjackhut.lumberjackhut'"));
	if (StaticMesh.Succeeded())
	{
		BuildingMesh->SetStaticMesh(StaticMesh.Object);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("failed to load building mesh"));
	}

}

// Called when the game starts or when spawned
void ABuilding::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABuilding::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABuilding::AddWorkerToBuilding(ACitizen* Worker)
{
	if (WorkersInBuilding.Num() != MaximumWorkerCapacity)
	{
		WorkersInBuilding.Add(Worker);
	}
	// return bool?
}
UFUNCTION()
void ABuilding::RemoveWorkerFromBuilding(class ACitizen* Worker)
{
	if (WorkersInBuilding.Num() != 0 )
	{
		WorkersInBuilding.Remove(Worker);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Could't remove worker from building"));
	}
}
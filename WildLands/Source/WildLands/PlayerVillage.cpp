// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerVillage.h"
#include "WildLandsGameMode.h"
#include "Engine/World.h"
#include "Citizen.h"
#include "Components/SceneComponent.h"
#include "Building.h"
#include "Worker.h"
#include "LumberjackHut.h"

UPlayerVillage::UPlayerVillage()
{
	BuildingType = EBuildingType::Village;
	MaximumCitizenCapacity = 4;


}

void UPlayerVillage::BeginPlay()
{
	Super::BeginPlay();
	MyGamemode = Cast<AWildLandsGameMode>(GetWorld()->GetAuthGameMode());
	if (MyGamemode != nullptr)
	{
		
		FActorSpawnParameters spawnParams;
	
		FTransform tempT = GetOwner()->GetTransform();
		FVector thisLocation = GetOwner()->GetTransform().GetLocation();
		thisLocation.Z += 67;
		//tempT.SetLocation(thisLocation);
		tempT.SetScale3D(FVector(0.04, 0.04, 0.04));
		
		for (int i = 0; i < MaximumCitizenCapacity; i++)
		{
			tempT.SetLocation(thisLocation);
			if (i % 2 == 0)
			{
				thisLocation.X = i*10 + thisLocation.X;
			}
			else
			{
				thisLocation.Y = i * 10 + thisLocation.Y;
			}
			ACitizen* Citizen = GetWorld()->SpawnActor<ACitizen>(MyGamemode->CitizenBP, tempT, spawnParams);
			if (Citizen)
			{
				Citizen->SetActorLocation(thisLocation);
				CitizensInBuilding.Add(Citizen);
				Citizen->SetHouse(this);
			}
		}
	}
}

AWorker* UPlayerVillage::DelegateToWork(UBuilding* Building, ACitizen* Citizen)
{
	//
	if (MyGamemode != nullptr)
	{
		FActorSpawnParameters spawnParams;

		spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		FTransform tempT = Building->GetOwner()->GetTransform();
		FVector thisLocation = Building->GetOwner()->GetTransform().GetLocation();
		thisLocation.Z += 67;
		//tempT.SetLocation(thisLocation);
		tempT.SetScale3D(FVector(0.04, 0.04, 0.04));


		tempT.SetLocation(thisLocation);

		AWorker* Worker = GetWorld()->SpawnActor<AWorker>(MyGamemode->WorkerBP, tempT, spawnParams);
		if (Worker)
		{
			Worker->SetActorLocation(thisLocation);
			CitizensInBuilding.Add(Worker);
			Worker->SetHouse(this);
			Worker->WorkPlace = Building;
			
			Citizen->Destroy();
			this->CitizensInBuilding.Remove(Citizen);
			return Worker;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Ups worker culdn't spawn"));
		}

	}
	return nullptr;
}
// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerVillage.h"
#include "WildLandsGameMode.h"
#include "Engine/World.h"
#include "Citizen.h"
#include "Components/SceneComponent.h"
#include "Building.h"
#include "Worker.h"
#include "LumberjackHut.h"
#include "Porter.h"
#include "Tile.h"
#include "WildLandsPlayerController.h"
UPlayerVillage::UPlayerVillage()
{
	BuildingType = EBuildingType::Village;
	MaximumCitizenCapacity = 4;


}

void UPlayerVillage::BeginPlay()
{
	Super::BeginPlay();
	MyGamemode = Cast<AWildLandsGameMode>(GetWorld()->GetAuthGameMode());
	MyPlayerController = Cast<AWildLandsPlayerController>(GetWorld()->GetFirstPlayerController());
	if (MyGamemode != nullptr)
	{				
		for (int i = 0; i < MaximumCitizenCapacity; i++)
		{
			CreateNewCitizen();
		}
	}
}
void UPlayerVillage::CreateNewCitizen()
{
	int RandomNumber = rand() % 20 + (-9);

	FActorSpawnParameters spawnParams;

	//spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	FTransform tempT = GetOwner()->GetTransform();
	FVector thisLocation = GetOwner()->GetTransform().GetLocation();
	thisLocation.Z += 87;

	tempT.SetScale3D(FVector(0.25, 0.25, 0.25));

	thisLocation.X = RandomNumber * 5 + thisLocation.X;
	thisLocation.Y = RandomNumber * 5 + thisLocation.Y;
	tempT.SetLocation(thisLocation);

	ACitizen* Citizen = GetWorld()->SpawnActor<ACitizen>(MyGamemode->CitizenBP, tempT, spawnParams);
	int TempCounter = 0;
	while (Citizen == nullptr)
	{
		thisLocation.X = thisLocation.X - RandomNumber * 5;
		thisLocation.Y = thisLocation.Y - RandomNumber * 5;
		RandomNumber = rand() % 20 + (-9);
		thisLocation.X = RandomNumber * 5 + thisLocation.X;
		thisLocation.Y = RandomNumber * 5 + thisLocation.Y;

		tempT.SetLocation(thisLocation);
		Citizen = GetWorld()->SpawnActor<ACitizen>(MyGamemode->CitizenBP, tempT, spawnParams);
		TempCounter++;
		if (TempCounter > 300)
		{
			UE_LOG(LogTemp, Fatal, TEXT("Cant find place to spawn worker!"));
		}
	}
	if (Citizen)
	{
		Citizen->CitizenType = ECitizenType::Citizen;
		CitizensInBuilding.Add(Citizen);
		Citizen->SetHouse(this);
	}
}
AWorker* UPlayerVillage::DelegateToWork(UBuilding* Building, ACitizen* Citizen)
{
	int RandomNumber = rand() % 20 + (-9);
	if (MyGamemode != nullptr)
	{
		FActorSpawnParameters spawnParams;

		//spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		FTransform tempT = Building->GetOwner()->GetTransform();
		FVector thisLocation = Building->GetOwner()->GetTransform().GetLocation();
		thisLocation.Z += 87;
		//tempT.SetLocation(thisLocation);
		tempT.SetScale3D(FVector(0.25, 0.25, 0.25));

		thisLocation.X = RandomNumber * 5 + thisLocation.X;
		thisLocation.Y = RandomNumber * 5 + thisLocation.Y;

		tempT.SetLocation(thisLocation);

		
		AWorker* Worker = GetWorld()->SpawnActor<AWorker>(MyGamemode->WorkerBP, tempT, spawnParams);
		int TempCounter = 0;
		while (Worker == nullptr)
		{
			thisLocation.X = thisLocation.X - RandomNumber * 5;
			thisLocation.Y = thisLocation.Y - RandomNumber * 5;
			RandomNumber = rand() % 20 + (-9);
			thisLocation.X = RandomNumber * 5 + thisLocation.X;
			thisLocation.Y = RandomNumber * 5 + thisLocation.Y;

			tempT.SetLocation(thisLocation);
			Worker = GetWorld()->SpawnActor<AWorker>(MyGamemode->WorkerBP, tempT, spawnParams);
			TempCounter++;
			if (TempCounter > 300)
			{
				UE_LOG(LogTemp, Fatal, TEXT("Cant find place to spawn worker!"));
			}
		}
		if (Worker)
		{
			Worker->CitizenType = ECitizenType::Worker;
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

APorter* UPlayerVillage::DelegateToTransport(UBuilding* Building, ACitizen* Citizen)
{
	int RandomNumber = rand() % 20 + (-9);
	if (MyGamemode != nullptr)
	{
		FActorSpawnParameters spawnParams;

		//spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		FTransform tempT = this->GetOwner()->GetTransform();
		FVector thisLocation = this->GetOwner()->GetTransform().GetLocation();
		thisLocation.Z += 67;
		//tempT.SetLocation(thisLocation);
		tempT.SetScale3D(FVector(0.25, 0.25, 0.25));


		thisLocation.X = RandomNumber * 5 + thisLocation.X;
		thisLocation.Y = RandomNumber * 5 + thisLocation.Y;

		tempT.SetLocation(thisLocation);

		APorter* Porter = GetWorld()->SpawnActor<APorter>(MyGamemode->PorterBP, tempT, spawnParams);
		int TempCounter = 0;
		while (Porter == nullptr)
		{
			thisLocation.X = thisLocation.X - RandomNumber * 5;
			thisLocation.Y = thisLocation.Y - RandomNumber * 5;
			RandomNumber = rand() % 20 + (-9);
			thisLocation.X = RandomNumber * 5 + thisLocation.X;
			thisLocation.Y = RandomNumber * 5 + thisLocation.Y;

			tempT.SetLocation(thisLocation);
			Porter = GetWorld()->SpawnActor<APorter>(MyGamemode->PorterBP, tempT, spawnParams);
			TempCounter++;
			if (TempCounter > 300)
			{
				UE_LOG(LogTemp, Fatal, TEXT("Cant find place to spawn worker!"));
			}
		}
		if (Porter)
		{
			Porter->CitizenType = ECitizenType::Porter;
			Porter->GetFrom = Building;
			Porter->TransportTo = this;
			CitizensInBuilding.Add(Porter);
			Porter->SetHouse(this);
			Porter->WorkPlace = Building;
			Porter->CharacterTile = Cast<ATile>(this->GetOwner());
			if (Porter->CharacterTile == nullptr)
			{
				UE_LOG(LogTemp, Fatal , TEXT("Porter->CharacterTile = Cast<ATile>(Building->GetOwner()) failed"))
			}

			Citizen->Destroy();
			this->CitizensInBuilding.Remove(Citizen);
			return Porter;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Ups worker culdn't spawn"));
		}

	}
	return nullptr;
}
void UPlayerVillage::UnloadResourceFromPorter(float &Amount)
{

	MyPlayerController->WoodResource++;
	MyPlayerController->UpdatePlayerWoodAmount.Broadcast();
	Amount--;
}
// Fill out your copyright notice in the Description page of Project Settings.


#include "LumberjackHut.h"
#include "CoreUObject/Public/UObject/UObjectIterator.h"
#include "Worker.h"
#include "WildLandsGameMode.h"
#include "Tile.h"
#include "PlayerVillage.h"
#include "WildLandsPlayerController.h"
#include "Building.h"
#include "Porter.h"

ULumberjackHut::ULumberjackHut()
{
	BuildingType = EBuildingType::Lumberjack;
}

void ULumberjackHut::BeginPlay()
{
	Super::BeginPlay();
	MyGamemode = Cast<AWildLandsGameMode>(GetWorld()->GetAuthGameMode());
	MyPlayerController = Cast<AWildLandsPlayerController>(GetWorld()->GetFirstPlayerController());
	MaximumCitizenCapacity = 0;
}
void ULumberjackHut::AddWorker()
{
	// Checking if we have space for another worker
	if (WorkersInBuilding.Num() < MaximumWorkerCapacity)
	{
		bool Check = false;

		//finding tile with a village in central slot
		for (TObjectIterator<ATile> Itr; Itr; ++Itr)
		{
			if (Check)
			{
				break;
			}
			ATile* VillageRef = *Itr;
			UPlayerVillage* PlayerVillageRef = Cast<UPlayerVillage>(VillageRef->CentralSlot);

			if (PlayerVillageRef != nullptr)
			{
				// Checking if village have unemployed citizens			
				for (auto& Elem : PlayerVillageRef->CitizensInBuilding)
				{
					if (Elem->WorkPlace == nullptr)
					{						
						AWorker* WorkerRef = PlayerVillageRef->DelegateToWork(this, Elem);
						WorkersInBuilding.Add(WorkerRef);
						Check = true;
						WorkerRef->BeginWork();
						
					}
					if (Check)
					{
						break;
					}
				}
				
			}			
		}
		

	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("MaximumWorkerCapacity reached"));
	}
}


void ULumberjackHut::AddPorter()
{
	// Checking if we have space for another worker
	if (PortersInBuilding.Num() < MaximumPorterCapacity)
	{
		bool Check = false;

		//finding tile with a village in central slot
		for (TObjectIterator<ATile> Itr; Itr; ++Itr)
		{
			if (Check)
			{
				break;
			}
			ATile* VillageRef = *Itr;
			UPlayerVillage* PlayerVillageRef = Cast<UPlayerVillage>(VillageRef->CentralSlot);

			if (PlayerVillageRef != nullptr)
			{
				// Checking if village have unemployed citizens			
				for (auto& Elem : PlayerVillageRef->CitizensInBuilding)
				{
					if (Elem->WorkPlace == nullptr)
					{
						APorter* PorterRef = PlayerVillageRef->DelegateToTransport(this, Elem);
						PorterRef->PorterState = EPorterState::Waiting;
						PortersInBuilding.Add(PorterRef);
						Check = true;
						PorterRef->BeginMovingToWork();
					}
					if (Check)
					{
						break;
					}
				}

			}

		}

	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("MaximumWorkerCapacity reached"));
	}
}

void ULumberjackHut::RemoveWorker()
{
	if (WorkersInBuilding.Num() != 0)
	{
		for (auto& Worker : WorkersInBuilding)
		{
			UPlayerVillage* PlayerVillage = Cast<UPlayerVillage>(Worker->House);
			PlayerVillage->CitizensInBuilding.Remove(Worker);
			Worker->Destroy();
			WorkersInBuilding.Remove(Worker);
			PlayerVillage->CreateNewCitizen();
			break;
		}
	}
}

void ULumberjackHut::RemovePorter()
{
	if (PortersInBuilding.Num() != 0)
	{
		for (auto& Porter : PortersInBuilding)
		{
			UPlayerVillage* PlayerVillage = Cast<UPlayerVillage>(Porter->House);
			PlayerVillage->CitizensInBuilding.Remove(Porter);
			Porter->Destroy();
			PortersInBuilding.Remove(Porter);
			PlayerVillage->CreateNewCitizen();
			break;
		}
	}
}
void ULumberjackHut::GenerateResource()
{
	if (ResourceAmountOnTile != 0)
	{
		if (ResourcesInBuilding != MaximumResourceCapacity)
		{
			ResourceAmountOnTile--;
			ResourcesInBuilding++;
			MyPlayerController->UpdatePlayerWoodAmount.Broadcast();
		}
		else
		{
			for (auto& WorkerRef : WorkersInBuilding)
			{
				WorkerRef->StopWork();
			}
		}

	}
	else
	{
		for (auto& WorkerRef: WorkersInBuilding)
		{
			WorkerRef->StopWork();
		}
	}
}
bool ULumberjackHut::LoadResourceOnPorter(float &Amount)
{
	if (ResourcesInBuilding != 0)
	{
		ResourcesInBuilding--;
		SendToWork();
		Amount++;
		MyPlayerController->UpdatePlayerWoodAmount.Broadcast();
		return true;
	}
	else
	{
		return false;
	}
}
void ULumberjackHut::SendToWork()
{
	for (auto& WorkerRef : WorkersInBuilding)
	{
		WorkerRef->WorkerStatus = EWorkerStatus::Working;
	}
}
// Fill out your copyright notice in the Description page of Project Settings.


#include "LumberjackHut.h"
#include "CoreUObject/Public/UObject/UObjectIterator.h"
#include "Worker.h"
#include "WildLandsGameMode.h"
#include "Tile.h"
#include "PlayerVillage.h"
#include "WildLandsPlayerController.h"
#include "Building.h"

ULumberjackHut::ULumberjackHut()
{
	BuildingType = EBuildingType::Lumberjack;
}

void ULumberjackHut::BeginPlay()
{
	Super::BeginPlay();
	MyGamemode = Cast<AWildLandsGameMode>(GetWorld()->GetAuthGameMode());
	MyPlayerController = Cast<AWildLandsPlayerController>(GetWorld()->GetFirstPlayerController());
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

}

void ULumberjackHut::RemoveWorker()
{

}

void ULumberjackHut::RemovePorter()
{

}
// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerVillage.h"
#include "WildLandsGameMode.h"
#include "Engine/World.h"
#include "Citizen.h"
#include "Components/SceneComponent.h"

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
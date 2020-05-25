// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.
#include "Engine/World.h"
#include "WildLandsGameMode.h"
#include "WildLandsPlayerController.h"
#include "WildLandsCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Grid.h"
#include "Tile.h"

AWildLandsGameMode::AWildLandsGameMode()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("StaticMesh'/Game/Meshes/MainVillage.MainVillage'"));
	if (MeshAsset.Succeeded())
	{
		MainBaseMesh = MeshAsset.Object;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Couldn't read flower mesh"));
	}
}

void AWildLandsGameMode::BeginPlay()
{
//	UUserWidget* WidgetInstance = CreateWidget<UUserWidget>(GetWorld(), WidgetBPReference);
//	WidgetInstance->AddToViewport();
	APawn *myPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
	FVector newLocation = FVector(0.f, 0.f, 700.f);
	myPawn->SetActorLocation(newLocation);
}
void AWildLandsGameMode::SetGrid(AGrid* grid)
{
	GridRef = grid;
}
void AWildLandsGameMode::SetMainTile(ATile* tile)
{
	MainTile = tile;
	MainTile->InitializeBase();
	//CentralSlot 
}
UStaticMesh* AWildLandsGameMode::GetVillageMesh()
{
	return MainBaseMesh;
}

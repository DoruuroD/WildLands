// Fill out your copyright notice in the Description page of Project Settings.
#include "Grid.h"
#include "CoreUObject/Public/UObject/UObjectIterator.h"
#include "Engine/World.h"
#include "Tile.h"
#include "WildLandsGameMode.h"
#include "FastNoise.h"
#include "RoadIndicator.h"
#include "Engine/StaticMesh.h"
#include "CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "UObject/UObjectGlobals.h"
#include "Resource.h"
#include "UObject\Object.h"
FastNoise myNoise;
// Sets default values
AGrid::AGrid()
{
	PrimaryActorTick.bCanEverTick = true;
	FVector tempVector = FVector(0.f, 1.f, 0.f);
	myNoise.SetNoiseType(FastNoise::Perlin);
	myNoise.SetFrequency(0.0015);
	myNoise.SetSeed(1000);


	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("StaticMesh'/Game/Meshes/Trees_01.Trees_01'"));
	if (MeshAsset.Succeeded())
	{		
		Tree = MeshAsset.Object;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Couldn't read tree mesh"));
	}
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset1(TEXT("StaticMesh'/Game/Meshes/Rocks_01.Rocks_01'"));
	if (MeshAsset1.Succeeded())
	{
		Rock = MeshAsset1.Object;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Couldn't read rock mesh"));
	}
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset2(TEXT("StaticMesh'/Game/Meshes/Grass_01.Grass_01'"));
	if (MeshAsset2.Succeeded())
	{
		Bush = MeshAsset2.Object;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Couldn't read rock mesh"));
	}
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset3(TEXT("StaticMesh'/Game/NaturePackage/Meshes/RescaledFlora/flowers01.flowers01'"));
	if (MeshAsset3.Succeeded())
	{
		Flower = MeshAsset3.Object;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Couldn't read flower mesh"));
	}
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset4(TEXT("StaticMesh'/Game/Meshes/Rocks_02.Rocks_02'"));
	if (MeshAsset4.Succeeded())
	{
		SmallRock = MeshAsset4.Object;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Couldn't read flower mesh"));
	}
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset5(TEXT("StaticMesh'/Game/Meshes/Trees_02.Trees_02'"));
	if (MeshAsset5.Succeeded())
	{
		SmallTree = MeshAsset5.Object;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Couldn't read flower mesh"));
	}

}

// Called when the game starts or when spawned
void AGrid::BeginPlay()
{
	Super::BeginPlay();
	MyGamemode = Cast<AWildLandsGameMode>(GetWorld()->GetAuthGameMode());
	MyGamemode->SetGrid(this);
//	OnConstruction(this->GetTransform());
	UE_LOG(LogTemp, Warning, TEXT("TimerSet"));
	//GetWorld()->GetTimerManager().SetTimer(LoopTimerHandle, this, &AGrid::CheckAllNeightbours, 2.f, false);
	if (TileActorBP)
	{
		SpawnTiles();
		CheckAllNeightbours();
		ChangeMaterialForAllTiles();
	}
	//ChangeMaterialForAllTiles();	
}

// Called every frame
void AGrid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGrid::OnConstruction(const FTransform& Transform)
{/*
	for (TObjectIterator<ATile> Itr; Itr; ++Itr)
	{
		ATile *Tile = *Itr;
		Tile->Destroy();
	}
	
	if (TileActorBP)
	{	
		SpawnTiles();
		//CheckAllNeightbours();
		ChangeMaterialForAllTiles();
	}
	*/
	
}

void AGrid::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	//CheckAllNeightbours();
}
void AGrid::SpawnTiles()
{
	
	int gridSize = (Size * 2) + 1; //horizontal length
	int gridControl = gridSize - Size; //length to center tile(included)
	int wingsLimit = Size; //vertical wings length

	int layerControl = 0;
	for (int i = 0; i < gridSize; i++)
	{
		FActorSpawnParameters spawnParams;

		ATile* centerHorizontalTile = GetWorld()->SpawnActor<ATile>(TileActorBP, GetTransform(), spawnParams);
		FVector centerHorizontalTilePosition = FVector(centerHorizontalTile->GetActorLocation().X, centerHorizontalTile->GetActorLocation().Y + (centerHorizontalTile->GetHorizontalStep()*i), centerHorizontalTile->GetActorLocation().Z);
		centerHorizontalTile->SetActorLocation(centerHorizontalTilePosition);
		//setting noise
		centerHorizontalTile->SetTileNoise(myNoise.GetNoise(centerHorizontalTile->GetActorLocation().X, centerHorizontalTile->GetActorLocation().Y));	

		// setting center tile for main base
		if (i == Size)
		{
			MyGamemode->SetMainTile(centerHorizontalTile);
		}
		//setting layers
		if (layerControl <= Size)
		{
			centerHorizontalTile->Layer = Size - layerControl;
		}
		else
		{
			centerHorizontalTile->Layer = layerControl - Size;
		}

		if (i >= gridControl)
		{
			wingsLimit--;
		}
		//vertical wings
		for (int j = 1; j <= wingsLimit; j++)
		{
			int layerWingsControl = wingsLimit - layerControl;

			ATile* plusWingTile = GetWorld()->SpawnActor<ATile>(TileActorBP, GetTransform(), spawnParams);
			FVector plusWingTilePosition = FVector(plusWingTile->GetActorLocation().X + (j* plusWingTile->GetVerticalStep()), plusWingTile->GetActorLocation().Y + (plusWingTile->GetHorizontalStep()*i) + (plusWingTile->GetSmallR()*j), plusWingTile->GetActorLocation().Z);
			plusWingTile->SetActorLocation(plusWingTilePosition);

			ATile* minusWingTile = GetWorld()->SpawnActor<ATile>(TileActorBP, GetTransform(), spawnParams);
			FVector minusWingTilePosition = FVector(minusWingTile->GetActorLocation().X + ((-1)* j * minusWingTile->GetVerticalStep()), minusWingTile->GetActorLocation().Y + (minusWingTile->GetHorizontalStep()*i) + (minusWingTile->GetSmallR()*j), minusWingTile->GetActorLocation().Z);
			minusWingTile->SetActorLocation(minusWingTilePosition);

			//setting noise
			plusWingTile->SetTileNoise(myNoise.GetNoise(plusWingTile->GetActorLocation().X, plusWingTile->GetActorLocation().Y));
			minusWingTile->SetTileNoise(myNoise.GetNoise(minusWingTile->GetActorLocation().X, minusWingTile->GetActorLocation().Y));
			//setting layers
			if (layerControl <= Size)
			{
				if (j <= layerWingsControl)
				{
					plusWingTile->Layer = Size - layerControl;
					minusWingTile->Layer = Size - layerControl;
				}
				else
				{
					plusWingTile->Layer = Size - layerControl + (j - layerWingsControl);
					minusWingTile->Layer = Size - layerControl + (j - layerWingsControl);
				}
			}
			else
			{
				plusWingTile->Layer = layerControl - Size + j;
				minusWingTile->Layer = layerControl - Size + j;
			}
		}

		layerControl++;
	}
}
void AGrid::SetLayerForAllTiles()
{
	
}
void AGrid::CheckAllNeightbours()
{
	for (TObjectIterator<ATile> Itr; Itr; ++Itr)
	{
		
		ATile *Tile = *Itr;
		Tile->CheckNeightbours();
	}
}

void AGrid::SpawnMissingNeightbours(TArray<bool> AreColliding, TSet<AActor*> Neightbours, ATile* tile)
{
	TArray<ATile*> tempArray;
	for (int i = 0; i < AreColliding.Num(); i++)
	{
		if (AreColliding[i] == false)
		{
			switch (i)
			{
			case 0:
				tempArray.Add(CreateAdditionalTile(FVector(tile->GetActorLocation().X, tile->GetActorLocation().Y - tile->GetHorizontalStep(), tile->GetActorLocation().Z)));
				break;
			case 1:
				tempArray.Add(CreateAdditionalTile(FVector(tile->GetActorLocation().X + tile->GetVerticalStep(), tile->GetActorLocation().Y - (tile->GetHorizontalStep()/2), tile->GetActorLocation().Z)));
				break;
			case 2:
				tempArray.Add(CreateAdditionalTile(FVector(tile->GetActorLocation().X + tile->GetVerticalStep(), tile->GetActorLocation().Y + (tile->GetHorizontalStep() / 2), tile->GetActorLocation().Z)));
				break;
			case 3:
				tempArray.Add(CreateAdditionalTile(FVector(tile->GetActorLocation().X, tile->GetActorLocation().Y + tile->GetHorizontalStep(), tile->GetActorLocation().Z)));
				break;
			case 4:
				tempArray.Add(CreateAdditionalTile(FVector(tile->GetActorLocation().X - tile->GetVerticalStep(), tile->GetActorLocation().Y + (tile->GetHorizontalStep() / 2), tile->GetActorLocation().Z)));
				break;
			case 5:
				tempArray.Add(CreateAdditionalTile(FVector(tile->GetActorLocation().X - tile->GetVerticalStep(), tile->GetActorLocation().Y - (tile->GetHorizontalStep() / 2), tile->GetActorLocation().Z)));
				break;
			default:
				break;
			}
		}
	}
	CheckNeightbours(Neightbours);
	tile->CheckNeightbours();
	for (int i = 0; i < tempArray.Num(); i++)
	{
		tempArray[i]->CheckNeightbours();
		tempArray[i]->SetTileNoise(myNoise.GetNoise(tempArray[i]->GetActorLocation().X, tempArray[i]->GetActorLocation().Y));
	}
	ChangeMaterialForTiles(tempArray);
}
void AGrid::CheckNeightbours(TSet<AActor*> Neightbours)
{
	for (auto& Elem : Neightbours)
	{
		Cast<ATile>(Elem)->CheckNeightbours();
	}
	
}
ATile* AGrid::CreateAdditionalTile(FVector position)
{
	FActorSpawnParameters spawnParams;
	ATile* newTile = GetWorld()->SpawnActor<ATile>(TileActorBP, GetTransform(), spawnParams);
	FVector tilePosition = FVector(position);
	newTile->SetActorLocation(tilePosition);

	return newTile;
}

void AGrid::ChangeMaterialForAllTiles()
{
	UE_LOG(LogTemp, Warning, TEXT("Changing material by noise"));
	for (TObjectIterator<ATile> Itr; Itr; ++Itr)
	{
		int32 number;
		ATile *Tile = *Itr;
		float TileNoise = Tile->GetTileNoise();
		Tile->ChangeMaterial(ChooseMaterialByNoise(TileNoise, number));
		Tile->SetBIOM(number);
		SetMeshes(Tile);
		
	}
}
void AGrid::ChangeMaterialForTiles(TArray<ATile*> Tiles)
{
	for (int i = 0; i < Tiles.Num(); i++)
	{
		int32 number;
		Tiles[i]->ChangeMaterial(ChooseMaterialByNoise(Tiles[i]->GetTileNoise(), number));
		Tiles[i]->SetBIOM(number);
		SetMeshes(Tiles[i]);
	}
}
UMaterial* AGrid::ChooseMaterialByNoise(float noise, int32& number)
{
	UMaterial* material;
	if (noise <= -0.2f)
	{
		number = 0;
		material = TileMaterials[0];
	}
	else if (noise <= 0.2f)
	{
		number = 1;
		material = TileMaterials[1];
	}
	else if (noise <= 0.4f)
	{
		number = 2;
		material = TileMaterials[2];
	}
	else
	{
		number = 3;
		material = TileMaterials[3];
	}
	return material;
}
void AGrid::SetMeshes(class ATile* tile)
{
	FVector tempRelativeLocation;
	switch (tile->Biom)
	{
		case Grass:
			if (!tile->IsMainBase)
			{ 
				tempRelativeLocation = tile->CentralSlot->GetRelativeLocation();
				tile->CentralSlot->MeshType = EMeshType::Resource;
				tile->CentralSlot = NewObject<UResource>(tile, MyGamemode->ResourceBP);
				tile->CentralSlot->RegisterComponent();
				tile->CentralSlot->SetStaticMesh(Bush);
				tile->CentralSlot->AttachToComponent(tile->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform, NAME_None);
				tile->CentralSlot->SetRelativeLocation(tempRelativeLocation);
			}

			tempRelativeLocation = tile->Slot0->GetRelativeLocation();
			tile->Slot0->MeshType = EMeshType::Resource;
			tile->Slot0 = NewObject<UResource>(tile, MyGamemode->ResourceBP);
			tile->Slot0->RegisterComponent();
			tile->Slot0->SetStaticMesh(Flower);
			tile->Slot0->AttachToComponent(tile->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform, NAME_None);
			tile->Slot0->SetRelativeLocation(tempRelativeLocation);

			tempRelativeLocation = tile->Slot1->GetRelativeLocation();
			tile->Slot1->MeshType = EMeshType::Resource;
			tile->Slot1 = NewObject<UResource>(tile, MyGamemode->ResourceBP);
			tile->Slot1->RegisterComponent();
			tile->Slot1->SetStaticMesh(Flower);
			tile->Slot1->AttachToComponent(tile->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform, NAME_None);
			tile->Slot1->SetRelativeLocation(tempRelativeLocation);

			tempRelativeLocation = tile->Slot2->GetRelativeLocation();
			tile->Slot2->MeshType = EMeshType::Resource;
			tile->Slot2 = NewObject<UResource>(tile, MyGamemode->ResourceBP);
			tile->Slot2->RegisterComponent();
			tile->Slot2->SetStaticMesh(Flower);
			tile->Slot2->AttachToComponent(tile->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform, NAME_None);
			tile->Slot2->SetRelativeLocation(tempRelativeLocation);

			tempRelativeLocation = tile->Slot3->GetRelativeLocation();
			tile->Slot3->MeshType = EMeshType::Resource;
			tile->Slot3 = NewObject<UResource>(tile, MyGamemode->ResourceBP);
			tile->Slot3->RegisterComponent();
			tile->Slot3->SetStaticMesh(Flower);
			tile->Slot3->AttachToComponent(tile->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform, NAME_None);
			tile->Slot3->SetRelativeLocation(tempRelativeLocation);

			tempRelativeLocation = tile->Slot4->GetRelativeLocation();
			tile->Slot4->MeshType = EMeshType::Resource;
			tile->Slot4 = NewObject<UResource>(tile, MyGamemode->ResourceBP);
			tile->Slot4->RegisterComponent();
			tile->Slot4->SetStaticMesh(Flower);
			tile->Slot4->AttachToComponent(tile->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform, NAME_None);
			tile->Slot4->SetRelativeLocation(tempRelativeLocation);

			tempRelativeLocation = tile->Slot5->GetRelativeLocation();
			tile->Slot5->MeshType = EMeshType::Resource;
			tile->Slot5 = NewObject<UResource>(tile, MyGamemode->ResourceBP);
			tile->Slot5->RegisterComponent();
			tile->Slot5->SetStaticMesh(Flower);
			tile->Slot5->AttachToComponent(tile->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform, NAME_None);
			tile->Slot5->SetRelativeLocation(tempRelativeLocation);

			break;
		case Forest:
			if (!tile->IsMainBase)
			{
				tempRelativeLocation = tile->CentralSlot->GetRelativeLocation();
				tile->CentralSlot->MeshType = EMeshType::Resource;
				tile->CentralSlot = NewObject<UResource>(tile, MyGamemode->ResourceBP);
				tile->CentralSlot->RegisterComponent();
				tile->CentralSlot->SetStaticMesh(Tree);
				tile->CentralSlot->AttachToComponent(tile->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform, NAME_None);
				tile->CentralSlot->SetRelativeLocation(tempRelativeLocation);
			}

			tempRelativeLocation = tile->Slot0->GetRelativeLocation();
			tile->Slot0->MeshType = EMeshType::Resource;
			tile->Slot0 = NewObject<UResource>(tile, MyGamemode->ResourceBP);
			tile->Slot0->RegisterComponent();
			tile->Slot0->SetStaticMesh(SmallTree);
			tile->Slot0->AttachToComponent(tile->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform, NAME_None);
			tile->Slot0->SetRelativeLocation(tempRelativeLocation);

			tempRelativeLocation = tile->Slot1->GetRelativeLocation();
			tile->Slot1->MeshType = EMeshType::Resource;
			tile->Slot1 = NewObject<UResource>(tile, MyGamemode->ResourceBP);
			tile->Slot1->RegisterComponent();
			tile->Slot1->SetStaticMesh(SmallTree);
			tile->Slot1->AttachToComponent(tile->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform, NAME_None);
			tile->Slot1->SetRelativeLocation(tempRelativeLocation);

			tempRelativeLocation = tile->Slot2->GetRelativeLocation();
			tile->Slot2->MeshType = EMeshType::Resource;
			tile->Slot2 = NewObject<UResource>(tile, MyGamemode->ResourceBP);
			tile->Slot2->RegisterComponent();
			tile->Slot2->SetStaticMesh(SmallTree);
			tile->Slot2->AttachToComponent(tile->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform, NAME_None);
			tile->Slot2->SetRelativeLocation(tempRelativeLocation);

			tempRelativeLocation = tile->Slot3->GetRelativeLocation();
			tile->Slot3->MeshType = EMeshType::Resource;
			tile->Slot3 = NewObject<UResource>(tile, MyGamemode->ResourceBP);
			tile->Slot3->RegisterComponent();
			tile->Slot3->SetStaticMesh(SmallTree);
			tile->Slot3->AttachToComponent(tile->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform, NAME_None);
			tile->Slot3->SetRelativeLocation(tempRelativeLocation);

			tempRelativeLocation = tile->Slot4->GetRelativeLocation();
			tile->Slot4->MeshType = EMeshType::Resource;
			tile->Slot4 = NewObject<UResource>(tile, MyGamemode->ResourceBP);
			tile->Slot4->RegisterComponent();
			tile->Slot4->SetStaticMesh(SmallTree);
			tile->Slot4->AttachToComponent(tile->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform, NAME_None);
			tile->Slot4->SetRelativeLocation(tempRelativeLocation);

			tempRelativeLocation = tile->Slot5->GetRelativeLocation();
			tile->Slot5->MeshType = EMeshType::Resource;
			tile->Slot5 = NewObject<UResource>(tile, MyGamemode->ResourceBP);
			tile->Slot5->RegisterComponent();
			tile->Slot5->SetStaticMesh(SmallTree);
			tile->Slot5->AttachToComponent(tile->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform, NAME_None);
			tile->Slot5->SetRelativeLocation(tempRelativeLocation);
			break;
		case Hills:
			if (!tile->IsMainBase)
			{
				tempRelativeLocation = tile->CentralSlot->GetRelativeLocation();
				tile->CentralSlot->MeshType = EMeshType::Resource;
				tile->CentralSlot = NewObject<UResource>(tile, MyGamemode->ResourceBP);
				tile->CentralSlot->RegisterComponent();
				tile->CentralSlot->SetStaticMesh(SmallRock);
				tile->CentralSlot->AttachToComponent(tile->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform, NAME_None);
				tile->CentralSlot->SetRelativeLocation(tempRelativeLocation);
			}

			tempRelativeLocation = tile->Slot0->GetRelativeLocation();
			tile->Slot0->MeshType = EMeshType::Resource;
			tile->Slot0 = NewObject<UResource>(tile, MyGamemode->ResourceBP);
			tile->Slot0->RegisterComponent();
			tile->Slot0->SetStaticMesh(SmallRock);
			tile->Slot0->AttachToComponent(tile->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform, NAME_None);
			tile->Slot0->SetRelativeLocation(tempRelativeLocation);

			tempRelativeLocation = tile->Slot1->GetRelativeLocation();
			tile->Slot1->MeshType = EMeshType::Resource;
			tile->Slot1 = NewObject<UResource>(tile, MyGamemode->ResourceBP);
			tile->Slot1->RegisterComponent();
			tile->Slot1->SetStaticMesh(SmallRock);
			tile->Slot1->AttachToComponent(tile->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform, NAME_None);
			tile->Slot1->SetRelativeLocation(tempRelativeLocation);

			tempRelativeLocation = tile->Slot2->GetRelativeLocation();
			tile->Slot2->MeshType = EMeshType::Resource;
			tile->Slot2 = NewObject<UResource>(tile, MyGamemode->ResourceBP);
			tile->Slot2->RegisterComponent();
			tile->Slot2->SetStaticMesh(SmallRock);
			tile->Slot2->AttachToComponent(tile->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform, NAME_None);
			tile->Slot2->SetRelativeLocation(tempRelativeLocation);

			tempRelativeLocation = tile->Slot3->GetRelativeLocation();
			tile->Slot3->MeshType = EMeshType::Resource;
			tile->Slot3 = NewObject<UResource>(tile, MyGamemode->ResourceBP);
			tile->Slot3->RegisterComponent();
			tile->Slot3->SetStaticMesh(SmallRock);
			tile->Slot3->AttachToComponent(tile->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform, NAME_None);
			tile->Slot3->SetRelativeLocation(tempRelativeLocation);

			tempRelativeLocation = tile->Slot4->GetRelativeLocation();
			tile->Slot4->MeshType = EMeshType::Resource;
			tile->Slot4 = NewObject<UResource>(tile, MyGamemode->ResourceBP);
			tile->Slot4->RegisterComponent();
			tile->Slot4->SetStaticMesh(SmallRock);
			tile->Slot4->AttachToComponent(tile->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform, NAME_None);
			tile->Slot4->SetRelativeLocation(tempRelativeLocation);

			tempRelativeLocation = tile->Slot5->GetRelativeLocation();
			tile->Slot5->MeshType = EMeshType::Resource;
			tile->Slot5 = NewObject<UResource>(tile, MyGamemode->ResourceBP);
			tile->Slot5->RegisterComponent();
			tile->Slot5->SetStaticMesh(SmallRock);
			tile->Slot5->AttachToComponent(tile->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform, NAME_None);
			tile->Slot5->SetRelativeLocation(tempRelativeLocation);
			break;
		case Mountains:
			if (!tile->IsMainBase)
			{
				tempRelativeLocation = tile->CentralSlot->GetRelativeLocation();
				tile->CentralSlot->MeshType = EMeshType::Resource;
				tile->CentralSlot = NewObject<UResource>(tile, MyGamemode->ResourceBP);
				tile->CentralSlot->RegisterComponent();
				tile->CentralSlot->SetStaticMesh(Rock);
				tile->CentralSlot->AttachToComponent(tile->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform, NAME_None);
				tile->CentralSlot->SetRelativeLocation(tempRelativeLocation);
			}

			tempRelativeLocation = tile->Slot0->GetRelativeLocation();
			tile->Slot0->MeshType = EMeshType::Resource;
			tile->Slot0 = NewObject<UResource>(tile, MyGamemode->ResourceBP);
			tile->Slot0->RegisterComponent();
			tile->Slot0->SetStaticMesh(SmallRock);
			tile->Slot0->AttachToComponent(tile->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform, NAME_None);
			tile->Slot0->SetRelativeLocation(tempRelativeLocation);

			tempRelativeLocation = tile->Slot1->GetRelativeLocation();
			tile->Slot1->MeshType = EMeshType::Resource;
			tile->Slot1 = NewObject<UResource>(tile, MyGamemode->ResourceBP);
			tile->Slot1->RegisterComponent();
			tile->Slot1->SetStaticMesh(SmallRock);
			tile->Slot1->AttachToComponent(tile->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform, NAME_None);
			tile->Slot1->SetRelativeLocation(tempRelativeLocation);

			tempRelativeLocation = tile->Slot2->GetRelativeLocation();
			tile->Slot2->MeshType = EMeshType::Resource;
			tile->Slot2 = NewObject<UResource>(tile, MyGamemode->ResourceBP);
			tile->Slot2->RegisterComponent();
			tile->Slot2->SetStaticMesh(SmallRock);
			tile->Slot2->AttachToComponent(tile->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform, NAME_None);
			tile->Slot2->SetRelativeLocation(tempRelativeLocation);

			tempRelativeLocation = tile->Slot3->GetRelativeLocation();
			tile->Slot3->MeshType = EMeshType::Resource;
			tile->Slot3 = NewObject<UResource>(tile, MyGamemode->ResourceBP);
			tile->Slot3->RegisterComponent();
			tile->Slot3->SetStaticMesh(SmallRock);
			tile->Slot3->AttachToComponent(tile->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform, NAME_None);
			tile->Slot3->SetRelativeLocation(tempRelativeLocation);

			tempRelativeLocation = tile->Slot4->GetRelativeLocation();
			tile->Slot4->MeshType = EMeshType::Resource;
			tile->Slot4 = NewObject<UResource>(tile, MyGamemode->ResourceBP);
			tile->Slot4->RegisterComponent();
			tile->Slot4->SetStaticMesh(SmallRock);
			tile->Slot4->AttachToComponent(tile->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform, NAME_None);
			tile->Slot4->SetRelativeLocation(tempRelativeLocation);

			tempRelativeLocation = tile->Slot5->GetRelativeLocation();
			tile->Slot5->MeshType = EMeshType::Resource;
			tile->Slot5 = NewObject<UResource>(tile, MyGamemode->ResourceBP);
			tile->Slot5->RegisterComponent();
			tile->Slot5->SetStaticMesh(SmallRock);
			tile->Slot5->AttachToComponent(tile->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform, NAME_None);
			tile->Slot5->SetRelativeLocation(tempRelativeLocation);
			break;
	}
}
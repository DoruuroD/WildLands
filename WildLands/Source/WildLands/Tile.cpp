// Fill out your copyright notice in the Description page of Project Settings.
#include "Tile.h"
#include "WildLandsGameMode.h"
#include "Components/BoxComponent.h"
#include "Engine/World.h"
#include "Materials/Material.h"
#include "Components/StaticMeshComponent.h"
#include "CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Grid.h"
#include "WildLandsCharacter.h"
#include "WildLandsPlayerController.h"
#include "Containers/Set.h"
#include "RoadIndicator.h"
#include "Scout.h"
#include "Builder.h"
#include "TileMesh.h"
#include "PlayerVillage.h"
// Sets default values
ATile::ATile()
{
	//selection material
	//  Material'/Game/StarterContent/Props/Materials/M_Shelf.M_Shelf'
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	AreColliding.SetNum(6, true);

	TileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TileMesh"));
	RootComponent = TileMesh;
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("StaticMesh'/Game/TileMeshes/cylinderTile.cylinderTile'"));
	if (MeshAsset.Succeeded())
	{
		TileMesh->SetStaticMesh(MeshAsset.Object);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Couldn't read TileMesh"));
	}
	SelectionMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SelectionMesh"));
	SelectionMesh->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset2(TEXT("StaticMesh'/Game/TileMeshes/SelectionHesx.SelectionHesx'"));
	if (MeshAsset2.Succeeded())
	{
		SelectionMesh->SetStaticMesh(MeshAsset2.Object);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Couldn't read SelectionMesh"));
	}
	SelectionMesh->SetRelativeLocation(FVector(0.f, 0.f, 65.f));
	SelectionMesh->SetRelativeRotation(FRotator(0.f, 30.f, 0.f));
	SelectionMesh->SetRelativeScale3D(FVector(0.84, 0.84, 1.f));

	static ConstructorHelpers::FObjectFinder<UMaterial> MaterialAsset(TEXT("Material'/Game/StarterContent/Materials/M_Wood_Pine.M_Wood_Pine'"));
	if (MaterialAsset.Succeeded())
	{
		SelectionMeshMaterial = MaterialAsset.Object;
		SelectionMesh->SetMaterial(0, SelectionMeshMaterial);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Couldn't read MaterialAsset"));
	}
	SelectionMesh->SetVisibility(false);
	SetVariables();
	
	BoxCollider0 = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollider0"));
	BoxCollider0->SetWorldScale3D(FVector(0.25, 0.25, 0.25));
	BoxCollider0->SetupAttachment(RootComponent);
	BoxCollider0->SetRelativeLocation(FVector(0.f,(-1)* HorizontalStep * 0.75, 0.f));
	

	BoxCollider1 = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollider1"));
	BoxCollider1->SetWorldScale3D(FVector(0.25, 0.25, 0.25));
	BoxCollider1->SetupAttachment(RootComponent);
	BoxCollider1->SetRelativeLocation(FVector(VerticalStep * 0.75, (-1)* HorizontalStep * 0.5, 0.f));

	BoxCollider2 = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollider2"));
	BoxCollider2->SetWorldScale3D(FVector(0.25, 0.25, 0.25));
	BoxCollider2->SetupAttachment(RootComponent);
	BoxCollider2->SetRelativeLocation(FVector(VerticalStep * 0.75, HorizontalStep * 0.5, 0.f));

	BoxCollider3 = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollider3"));
	BoxCollider3->SetWorldScale3D(FVector(0.25, 0.25, 0.25));
	BoxCollider3->SetupAttachment(RootComponent);
	BoxCollider3->SetRelativeLocation(FVector(0.f, HorizontalStep * 0.75, 0.f));

	BoxCollider4 = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollider4"));
	BoxCollider4->SetWorldScale3D(FVector(0.25, 0.25, 0.25));
	BoxCollider4->SetupAttachment(RootComponent);
	BoxCollider4->SetRelativeLocation(FVector((-1)*VerticalStep * 0.75, HorizontalStep * 0.5, 0.f));

	BoxCollider5 = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollider5"));
	BoxCollider5->SetWorldScale3D(FVector(0.25, 0.25, 0.25));
	BoxCollider5->SetupAttachment(RootComponent);
	BoxCollider5->SetRelativeLocation(FVector((-1)*VerticalStep * 0.75, (-1)* HorizontalStep * 0.5, 0.f));

	StandbyPosition = CreateDefaultSubobject<UBoxComponent>(TEXT("standbyPosition"));
	StandbyPosition->SetupAttachment(RootComponent);
	StandbyPosition->AddRelativeLocation(FVector(0.f, 0.f, 50.f));

	MovingLeftPosition = CreateDefaultSubobject<UBoxComponent>(TEXT("movingLeftPosition"));
	MovingLeftPosition->SetupAttachment(RootComponent);
	MovingLeftPosition->AddRelativeLocation(FVector(0.f, -70.f, 50.f));

	MovingRightPosition = CreateDefaultSubobject<UBoxComponent>(TEXT("movingRightPosition"));
	MovingRightPosition->SetupAttachment(RootComponent);
	MovingRightPosition->AddRelativeLocation(FVector(0.f, 70.f, 50.f));

	CentralSlot = CreateDefaultSubobject<UTileMesh>(TEXT("Central slot"));
	CentralSlot->SetupAttachment(RootComponent);
	CentralSlot->SetRelativeLocation(FVector(0.f, 0.f, 17.f));
	
	Slot0 = CreateDefaultSubobject<UTileMesh>(TEXT("Slot 0"));
	Slot0->SetupAttachment(RootComponent);
	Slot0->SetRelativeLocation(FVector(0.f, 0.f, 50.f));

	Slot1 = CreateDefaultSubobject<UTileMesh>(TEXT("Slot 1"));
	Slot1->SetupAttachment(RootComponent);
	Slot1->SetRelativeLocation(FVector(0.f, 0.f, 50.f));

	Slot2 = CreateDefaultSubobject<UTileMesh>(TEXT("Slot 2"));
	Slot2->SetupAttachment(RootComponent);
	Slot2->SetRelativeLocation(FVector(0.f, 0.f, 50.f));

	Slot3 = CreateDefaultSubobject<UTileMesh>(TEXT("Slot 3"));
	Slot3->SetupAttachment(RootComponent);
	Slot3->SetRelativeLocation(FVector(0.f, 0.f, 50.f));

	Slot4 = CreateDefaultSubobject<UTileMesh>(TEXT("Slot 4"));
	Slot4->SetupAttachment(RootComponent);
	Slot4->SetRelativeLocation(FVector(0.f, 0.f, 50.f));

	Slot5 = CreateDefaultSubobject<UTileMesh>(TEXT("Slot 5"));
	Slot5->SetupAttachment(RootComponent);
	Slot5->SetRelativeLocation(FVector(0.f, 0.f, 50.f));

}


// Setting all necessary tile variables
void ATile::SetVariables()
{
	FVector temp = this->GetTransform().GetScale3D();
	Scale = temp.X;
	SmallR = 86.5 * Scale;
	BigR = (2 * SmallR) / sqrt(3);
	VerticalStep = sqrt(powf(2 * SmallR, 2.f) - powf(SmallR, 2.f));
	HorizontalStep = 2 * SmallR;
}
// Called when the game starts or when spawned
void ATile::BeginPlay()
{
	Super::BeginPlay();


	MyGamemode = Cast<AWildLandsGameMode>(GetWorld()->GetAuthGameMode());
	TileMesh->SetMaterial(0, TileMaterial);
	
	MyPlayerController = Cast<AWildLandsPlayerController>(GetWorld()->GetFirstPlayerController());
}
void ATile::OnConstruction(const FTransform& Transform)
{

}
void ATile::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
}
// Called every frame
void ATile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATile::CheckNeightbours()
{

	TSubclassOf< ATile > ClassFilter;
	TSet<AActor*> tempNeightbours;
	BoxCollider0->GetOverlappingActors(tempNeightbours, ClassFilter);
	

	if (tempNeightbours.Num() != 0)
	{
		for (auto& Tile : tempNeightbours)
		{
			Neightbours.Add(Tile);
		}
		AreColliding[0] = true;
	}
	else
		AreColliding[0] = false;


	BoxCollider1->GetOverlappingActors(tempNeightbours, ClassFilter);
	if (tempNeightbours.Num() != 0)
	{
		for (auto& Tile : tempNeightbours)
		{
			Neightbours.Add(Tile);
		}
		AreColliding[1] = true;
	}
	else
		AreColliding[1] = false;

	BoxCollider2->GetOverlappingActors(tempNeightbours, ClassFilter);
	if (tempNeightbours.Num() != 0)
	{
		for (auto& Tile : tempNeightbours)
		{
			Neightbours.Add(Tile);
		}
		AreColliding[2] = true;
	}
	else
		AreColliding[2] = false;

	BoxCollider3->GetOverlappingActors(tempNeightbours, ClassFilter);
	if (tempNeightbours.Num() != 0)
	{
		for (auto& Tile : tempNeightbours)
		{
			Neightbours.Add(Tile);
		}
		AreColliding[3] = true;
	}
	else
		AreColliding[3] = false;

	BoxCollider4->GetOverlappingActors(tempNeightbours, ClassFilter);
	if (tempNeightbours.Num() != 0)
	{
		for (auto& Tile : tempNeightbours)
		{
			Neightbours.Add(Tile);
		}
		AreColliding[4] = true;
	}
	else
		AreColliding[4] = false;

	BoxCollider5->GetOverlappingActors(tempNeightbours, ClassFilter);
	if (tempNeightbours.Num() != 0)
	{
		for (auto& Tile : tempNeightbours)
		{
			Neightbours.Add(Tile);
		}
		AreColliding[5] = true;
	}
	else
		AreColliding[5] = false;
}
void ATile::ChangeMaterial(UMaterial* material)
{
	TileMaterial = material;
	TileMesh->SetMaterial(0, TileMaterial);
}
void ATile::NotifyActorBeginCursorOver()
{
	//UE_LOG(LogTemp, Warning, TEXT("BeginCursorOver %s"), *this->GetName());
	if (MyPlayerController)
	{
		MyPlayerController->TileNameUnderCursor = this->GetName();
		MyPlayerController->ObjectUnderTheCursor.Broadcast();
	}

}
void ATile::NotifyActorEndCursorOver()
{
	if (MyPlayerController)
	{
		MyPlayerController->TileNameUnderCursor = FString("");
		MyPlayerController->ObjectUnderTheCursor.Broadcast();
	}
}

void ATile::NotifyActorOnClicked(FKey ButtonPressed)
{
	if (ButtonPressed.ToString() == "LeftMouseButton")
	{
		if (MyPlayerController != nullptr)
		{
			switch (MyPlayerController->SelectionState)
			{
			case Null:
			{
				
				if (this->WildLandsCharacters.Num() != 0)
				{
					/*MyPlayerController->SelectionState = CharacterSelected;
					MyPlayerController->TurnOnCharacterWidget();
					MyPlayerController->SelectedTile = this;
					if (MyPlayerController->SelectedTile->GetFirstCharacter())
					{
						MyPlayerController->SelectedCharacter = MyPlayerController->SelectedTile->GetFirstCharacter();
						MyPlayerController->SelectedCharacter->SetSelection(true);
					}
					MyPlayerController->RoadIndicator->ShowRoad(MyPlayerController->SelectedCharacter, MyPlayerController->SelectedCharacter->Road, true);
					MyPlayerController->Select.Broadcast();*/
					NullToCharacter();
				}
				else
				{
					NullToTile();
					/*MyPlayerController->SelectionState = TileSelected;
					MyPlayerController->TurnOnTileWidget();
					MyPlayerController->SelectedTile = this;
					MyPlayerController->Select.Broadcast();
					SelectionMesh->SetVisibility(true);*/
				}
				break;
			}

			case TileSelected:
			{
				if (this->WildLandsCharacters.Num() != 0)
				{
					/*MyPlayerController->SelectedTile->SelectionMesh->SetVisibility(false);
					MyPlayerController->SelectionState = CharacterSelected;
					MyPlayerController->TurnOnCharacterWidget();
					MyPlayerController->SelectedTile = this;
					if (MyPlayerController->SelectedTile->GetFirstCharacter())
					{
						MyPlayerController->SelectedCharacter = MyPlayerController->SelectedTile->GetFirstCharacter();
						MyPlayerController->SelectedCharacter->SetSelection(true);
					}
					MyPlayerController->RoadIndicator->ShowRoad(MyPlayerController->SelectedCharacter, MyPlayerController->SelectedCharacter->Road, true);
					MyPlayerController->Select.Broadcast();*/
					TileToCharacter();
				}
				else if (MyPlayerController->SelectedTile == this)
				{
					break;
				}
				else
				{
					/*MyPlayerController->SelectedTile->SelectionMesh->SetVisibility(false);
					MyPlayerController->SelectedTile = this;
					MyPlayerController->Select.Broadcast();
					SelectionMesh->SetVisibility(true);*/
					TileToOtherTile();
				}
				break;
			}
			case CharacterSelected:
			{
				if (MyPlayerController->SelectedTile == this)
				{
					/*MyPlayerController->SelectionState = TileSelected;
					MyPlayerController->TurnOnTileWidget();
					MyPlayerController->RoadIndicator->ShowRoad(MyPlayerController->SelectedCharacter, MyPlayerController->SelectedCharacter->Road, false);
					MyPlayerController->SelectedCharacter->SetSelection(false);
					MyPlayerController->SelectedCharacter = nullptr;
					MyPlayerController->Select.Broadcast();
					SelectionMesh->SetVisibility(true);*/
					CharactrToCharacterTile();
				}
				else if (this->WildLandsCharacters.Num() != 0)
				{
					/*MyPlayerController->SelectedTile = this;
					if (MyPlayerController->SelectedTile->GetFirstCharacter())
					{
						MyPlayerController->SelectedCharacter->SetSelection(false);
						MyPlayerController->SelectedCharacter = MyPlayerController->SelectedTile->GetFirstCharacter();
						MyPlayerController->SelectedCharacter->SetSelection(true);
					}
					MyPlayerController->RoadIndicator->ShowRoad(MyPlayerController->SelectedCharacter, MyPlayerController->SelectedCharacter->Road, true);
					MyPlayerController->Select.Broadcast();*/
					CharacterToOtherCharacter();
				}
				else
				{
					/*MyPlayerController->SelectionState = TileSelected;
					MyPlayerController->SelectedTile = this;
					MyPlayerController->RoadIndicator->ShowRoad(MyPlayerController->SelectedCharacter, MyPlayerController->SelectedCharacter->Road, false);
					MyPlayerController->SelectedCharacter->SetSelection(false);
					MyPlayerController->SelectedCharacter = nullptr;
					MyPlayerController->TurnOnTileWidget();
					MyPlayerController->Select.Broadcast();
					
					SelectionMesh->SetVisibility(true);*/
					CharactrToTile();
				}
				break;
			}
			default:
				UE_LOG(LogTemp, Warning, TEXT("myPlayerController selectionState error"));
			}
		}
	}
	if (ButtonPressed.ToString() == "MiddleMouseButton")
	{
		MyGamemode->GridRef->SpawnMissingNeightbours(AreColliding, Neightbours, this);
	}
	if (ButtonPressed.ToString() == "RightMouseButton")
	{
		if (MyPlayerController->SelectedCharacter != nullptr)
		{
			MoveCharacter(MyPlayerController->SelectedCharacter);
		}
	}

	
}
void ATile::SetBIOM(int32 number)
{
	Biom = static_cast<EBiom>(number);
	
}

void ATile::SpawnCharacterOnTile(ECharacterType CharacterType)
{
	FActorSpawnParameters spawnParams;

	FTransform tempT = GetTransform();
	FVector thisLocation = this->GetActorLocation();
	thisLocation.Z += 100;
	tempT.SetLocation(thisLocation);
	tempT.SetScale3D(FVector(0.4, 0.4, 0.4));

	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	switch (CharacterType)
	{
		case ECharacterType::Builder:
		{
			ABuilder* Builder = GetWorld()->SpawnActor<ABuilder>(BuilderBP, tempT, spawnParams);
			if (Builder)
			{
				Builder->SetActorLocation(thisLocation);
				WildLandsCharacters.Add(Builder);
				Builder->CharacterTile = this;
			}
			else
				UE_LOG(LogTemp, Warning, TEXT("AWildLandsCharacter spawn error"));
			break;
		}
		case ECharacterType::Scout:
		{
			AScout* Scout = GetWorld()->SpawnActor<AScout>(ScoutBP, tempT, spawnParams);
			if (Scout)
			{
				Scout->SetActorLocation(thisLocation);
				WildLandsCharacters.Add(Scout);
				Scout->CharacterTile = this;
			}
			else
				UE_LOG(LogTemp, Warning, TEXT("AWildLandsCharacter spawn error"));
			break;
		}
		case ECharacterType::Settler:
		{
			break;
		}
		case ECharacterType::Warrior:
		{
			break;
		}
	}
}
AWildLandsCharacter* ATile::GetFirstCharacter()
{
	if (&WildLandsCharacters != nullptr)
	{
		//TArray<AWildLandsCharacter*>  x = WildLandsCharacters.Array();
		for (auto& Elem : WildLandsCharacters)
		{
			return Elem;
			break;
		}
	}
	return nullptr;
}
void ATile::MoveCharacter(AWildLandsCharacter* character)
{
	MyPlayerController->SelectedCharacter->Road = MyPlayerController->FindRoad(MyPlayerController->SelectedTile, this);


	//	FVector thisTileLocation = RootComponent->GetComponentLocation();
//	thisTileLocation.Z += 50.f;

	character->MoveCharacterDelegate.ExecuteIfBound(this);
	
	MyPlayerController->RoadIndicator->ShowRoad(character, character->Road, true);
	/*
	MyPlayerController->SelectedTile->WildLandsCharacters.Remove(MyPlayerController->SelectedCharacter);
	this->WildLandsCharacters.Add(MyPlayerController->SelectedCharacter);
	MyPlayerController->SelectedTile = this;
	
	MyPlayerController->SelectedCharacter->SetActorLocation(thisTileLocation);
	*/
}
TSet<AActor*> ATile::GetNeightbours()
{
	return Neightbours;
}
void ATile::InitializeBase()
{
	IsMainBase = true;

	
	FVector tempRelativeLocation = this->CentralSlot->GetRelativeLocation();
	this->CentralSlot = NewObject<UPlayerVillage>(this, MyGamemode->PlayerVillageBP);
	this->CentralSlot->MeshType = ETileMeshType::Building;
	this->CentralSlot->RegisterComponent();
	this->CentralSlot->SetStaticMesh(MyGamemode->GetVillageMesh());
	this->CentralSlot->AttachToComponent(this->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform, NAME_None);
	this->CentralSlot->SetRelativeLocation(tempRelativeLocation);
}	

void ATile::NullToTile()
{
	MyPlayerController->SelectionState = TileSelected;
	MyPlayerController->SelectedTile = this;
	SelectionMesh->SetVisibility(true);
	MyPlayerController->TurnOnTileWidget();
	MyPlayerController->Select.Broadcast();
}

void ATile::NullToCharacter()
{
	MyPlayerController->SelectionState = CharacterSelected;
	MyPlayerController->SelectedTile = this;
	if (MyPlayerController->SelectedTile->GetFirstCharacter())
	{
		MyPlayerController->SelectedCharacter = MyPlayerController->SelectedTile->GetFirstCharacter();
		MyPlayerController->SelectedCharacter->SetSelection(true);
	}
	MyPlayerController->RoadIndicator->ShowRoad(MyPlayerController->SelectedCharacter, MyPlayerController->SelectedCharacter->Road, true);
	MyPlayerController->TurnOnCharacterWidget();
	MyPlayerController->Select.Broadcast();
}

void ATile::TileToOtherTile()
{
	MyPlayerController->SelectedTile->SelectionMesh->SetVisibility(false);
	MyPlayerController->SelectedTile = this;
	MyPlayerController->Select.Broadcast();
	SelectionMesh->SetVisibility(true);
}

void ATile::TileToCharacter()
{
	MyPlayerController->SelectedTile->SelectionMesh->SetVisibility(false);
	MyPlayerController->SelectionState = CharacterSelected;
	MyPlayerController->SelectedTile = this;
	if (MyPlayerController->SelectedTile->GetFirstCharacter())
	{
		MyPlayerController->SelectedCharacter = MyPlayerController->SelectedTile->GetFirstCharacter();
		MyPlayerController->SelectedCharacter->SetSelection(true);
	}
	MyPlayerController->RoadIndicator->ShowRoad(MyPlayerController->SelectedCharacter, MyPlayerController->SelectedCharacter->Road, true);
	MyPlayerController->TurnOnCharacterWidget();
	MyPlayerController->Select.Broadcast();
}
void ATile::TileToSpecificCharacter(AWildLandsCharacter* WildLandsCharacter)
{
	MyPlayerController->SelectedTile->SelectionMesh->SetVisibility(false);
	MyPlayerController->SelectionState = CharacterSelected;
	MyPlayerController->SelectedTile = WildLandsCharacter->CharacterTile;

	MyPlayerController->SelectedCharacter = WildLandsCharacter;
	MyPlayerController->SelectedCharacter->SetSelection(true);
	
	MyPlayerController->RoadIndicator->ShowRoad(MyPlayerController->SelectedCharacter, MyPlayerController->SelectedCharacter->Road, true);
	MyPlayerController->TurnOnCharacterWidget();
	MyPlayerController->Select.Broadcast();
}
void ATile::TileToNull()
{
	MyPlayerController->SelectionState = Null;
	MyPlayerController->TurnOffTileWidget();
	MyPlayerController->SelectedTile = nullptr;
	SelectionMesh->SetVisibility(false);
}

void ATile::CharactrToTile()
{
	MyPlayerController->SelectionState = TileSelected;
	MyPlayerController->SelectedTile = this;
	MyPlayerController->RoadIndicator->ShowRoad(MyPlayerController->SelectedCharacter, MyPlayerController->SelectedCharacter->Road, false);
	MyPlayerController->SelectedCharacter->SetSelection(false);
	MyPlayerController->SelectedCharacter = nullptr;
	MyPlayerController->TurnOnTileWidget();
	MyPlayerController->Select.Broadcast();

	SelectionMesh->SetVisibility(true);
}
void  ATile::CharactrToCharacterTile()
{
	MyPlayerController->SelectionState = TileSelected;
	MyPlayerController->TurnOnTileWidget();
	MyPlayerController->RoadIndicator->ShowRoad(MyPlayerController->SelectedCharacter, MyPlayerController->SelectedCharacter->Road, false);
	MyPlayerController->SelectedCharacter->SetSelection(false);
	MyPlayerController->SelectedCharacter = nullptr;
	MyPlayerController->Select.Broadcast();
	SelectionMesh->SetVisibility(true);
}
void ATile::CharacterToOtherCharacter()
{
	MyPlayerController->SelectedTile = this;
	if (MyPlayerController->SelectedTile->GetFirstCharacter())
	{
		MyPlayerController->SelectedCharacter->SetSelection(false);
		MyPlayerController->SelectedCharacter = MyPlayerController->SelectedTile->GetFirstCharacter();
		MyPlayerController->SelectedCharacter->SetSelection(true);
	}
	MyPlayerController->RoadIndicator->ShowRoad(MyPlayerController->SelectedCharacter, MyPlayerController->SelectedCharacter->Road, true);
	MyPlayerController->Select.Broadcast();
}

void ATile::CharacterToNull()
{
	MyPlayerController->SelectionState = Null;
	MyPlayerController->TurnOffCharacterWidget();
	MyPlayerController->SelectedTile = nullptr;

	MyPlayerController->SelectedCharacter->SetSelection(false);
	MyPlayerController->SelectedCharacter = nullptr;

}
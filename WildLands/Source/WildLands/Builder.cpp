// Fill out your copyright notice in the Description page of Project Settings.


#include "Builder.h"
#include "Tile.h"
#include "LumberjackHut.h"
#include "WildLandsGameMode.h"
ABuilder::ABuilder()
{
	CharacterType = ECharacterType::Builder;
}

void ABuilder::BeginPlay()
{
	Super::BeginPlay();
}

void ABuilder::BuildLumberjacksHut()
{
	if (CharacterTile->Biom == EBiom::Woodlands)
	{
		FVector tempRelativeLocation = CharacterTile->CentralSlot->GetRelativeLocation();
		CharacterTile->CentralSlot->DestroyComponent();
		
		CharacterTile->CentralSlot = NewObject<ULumberjackHut>(CharacterTile, MyGamemode->LumberjackHutBP);
		CharacterTile->CentralSlot->MeshType = ETileMeshType::Building;
		CharacterTile->CentralSlot->RegisterComponent();
		CharacterTile->CentralSlot->AttachToComponent(CharacterTile->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform, NAME_None);
		CharacterTile->CentralSlot->SetRelativeLocation(tempRelativeLocation);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Wrong BIOM! Maybe forest?"));
	}
}
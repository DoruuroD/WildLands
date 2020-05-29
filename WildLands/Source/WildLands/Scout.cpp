// Fill out your copyright notice in the Description page of Project Settings.


#include "Scout.h"
#include "WildLandsGameMode.h"
#include "Tile.h"
#include "Grid.h"
AScout::AScout()
{
	CharacterType = ECharacterType::Scout;
}
void AScout::BeginPlay()
{
	Super::BeginPlay();
}
void AScout::ScoutSurroundings()
{
	MyGamemode->GridRef->SpawnMissingNeightbours(CharacterTile->AreColliding, CharacterTile->Neightbours, CharacterTile);
}
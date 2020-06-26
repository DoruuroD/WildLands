// Fill out your copyright notice in the Description page of Project Settings.


#include "Scout.h"
#include "WildLandsGameMode.h"
#include "Tile.h"
#include "Grid.h"
#include "TimerManager.h"
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

	GetWorldTimerManager().SetTimer(Timer, this, &AScout::Scouting, 5.f, false, 5.f);
	StopMoving();
	CharacterState = ECharacterState::TakingAction;
	UpdateCharacterProgressBar.Broadcast();

//	MyGamemode->GridRef->SpawnMissingNeightbours(CharacterTile->AreColliding, CharacterTile->Neightbours, CharacterTile);
}

void AScout::Scouting()
{
	MyGamemode->GridRef->SpawnMissingNeightbours(CharacterTile->AreColliding, CharacterTile->Neightbours, CharacterTile);
	CharacterState = ECharacterState::IdleCharacter;
	UpdateCharacterProgressBar.Broadcast();
}
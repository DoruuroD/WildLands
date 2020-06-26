// Fill out your copyright notice in the Description page of Project Settings.


#include "Builder.h"
#include "Tile.h"
#include "LumberjackHut.h"
#include "WildLandsGameMode.h"
#include "Resource.h"
#include "TimerManager.h"

ABuilder::ABuilder()
{
	CharacterType = ECharacterType::Builder;
}

void ABuilder::BeginPlay()
{
	Super::BeginPlay();
}
void ABuilder::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);


}

void ABuilder::BuildLumberjacksHut()
{
	if (CharacterTile->Biom == EBiom::Woodlands && CharacterTile->CentralSlot->MeshType!=ETileMeshType::Building)
	{
		GetWorldTimerManager().SetTimer(Timer, this, &ABuilder::Building, 5.f, false, 5.f);
		StopMoving();
		CharacterState = ECharacterState::TakingAction;
		UpdateCharacterProgressBar.Broadcast();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Wrong BIOM! Maybe forest?"));
	}
}

void ABuilder::Building()
{
	FVector tempRelativeLocation = CharacterTile->CentralSlot->GetRelativeLocation();
	int TempData = Cast<UResource>(CharacterTile->CentralSlot)->ResourceData.ResourceAmount;
	CharacterTile->CentralSlot->DestroyComponent();

	CharacterTile->CentralSlot = NewObject<ULumberjackHut>(CharacterTile, MyGamemode->LumberjackHutBP);
	CharacterTile->CentralSlot->MeshType = ETileMeshType::Building;
	CharacterTile->CentralSlot->RegisterComponent();
	CharacterTile->CentralSlot->AttachToComponent(CharacterTile->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform, NAME_None);
	CharacterTile->CentralSlot->SetRelativeLocation(tempRelativeLocation);
	Cast<ULumberjackHut>(CharacterTile->CentralSlot)->ResourceAmountOnTile = TempData;
	CharacterState = ECharacterState::IdleCharacter;
	UpdateCharacterProgressBar.Broadcast();
}

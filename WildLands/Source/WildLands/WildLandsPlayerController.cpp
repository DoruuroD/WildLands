// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "WildLandsPlayerController.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "WildLandsCharacter.h"
#include "Engine/World.h"
#include "Tile.h"
#include "Blueprint/UserWidget.h"
#include "RoadIndicator.h"
#include "CoreUObject/Public/UObject/UObjectIterator.h"
#include "EngineUtils.h"

AWildLandsPlayerController::AWildLandsPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
	bEnableMouseOverEvents = true;
	bEnableClickEvents = true;
	TileNameUnderCursor = "";
	SelectionState = static_cast<ESelectionState>(0);
}

void AWildLandsPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);


}
void AWildLandsPlayerController::BeginPlay()
{
	Super::BeginPlay();

	for (TActorIterator<ARoadIndicator> Itr(GetWorld()); Itr; ++Itr)
	{
		RoadIndicator = *Itr;
		break;
	}

	if (HUDOverlayAsset)
	{
		HUDOverlay = CreateWidget<UUserWidget>(this, HUDOverlayAsset);
		
	}
	HUDOverlay->AddToViewport();
	if (TileWidgetBP)
	{
		TileWidget = CreateWidget<UUserWidget>(this, TileWidgetBP);
	}
	if (CharacterWidgetBP)
	{
		CharacterWidget = CreateWidget<UUserWidget>(this, CharacterWidgetBP);
	}
}
void AWildLandsPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();
	
}
// Adding TileWidget to Viewport
void AWildLandsPlayerController::TurnOnTileWidget()
{
	if (CharacterWidget->IsInViewport())
	{
		CharacterWidget->RemoveFromViewport();
	}
	if (!TileWidget->IsInViewport())
	{
		TileWidget->AddToViewport();
	}

}
bool AWildLandsPlayerController::IsTileWidget()
{
	if (TileWidget->IsInViewport())
	{
		return true;
	}
	else
	{
		return false;
	}
}
void AWildLandsPlayerController::TurnOffTileWidget()
{
	if (TileWidget->IsInViewport())
	{
		TileWidget->RemoveFromViewport();
	}
}

void AWildLandsPlayerController::TurnOnCharacterWidget()
{
	if (TileWidget->IsInViewport())
	{
		TileWidget->RemoveFromViewport();
	}
	if (!CharacterWidget->IsInViewport())
	{		
		CharacterWidget->AddToViewport();
	}
}
void AWildLandsPlayerController::TurnOffCharacterWidget() 
{
	if (CharacterWidget->IsInViewport())
	{
		CharacterWidget->RemoveFromViewport();
	}
}
void AWildLandsPlayerController::SpawnMyCharacter()
{
	SelectedTile->SpawnCharacterOnTile();
	Select.Broadcast();
}
TArray<ATile*> AWildLandsPlayerController::FindRoad(ATile* begining, ATile* end)
{
	TArray<ATile*> Road;
	ATile* temp;
	FTransform q = begining->GetTransform();
	FTransform w = end->GetTransform();
	FVector start = q.GetLocation();
	FVector finish = w.GetLocation();
	int error = 0;
	//Check if is in neightbours
	for (auto& Elem : begining->GetNeightbours())
	{
		if (Cast<ATile>(Elem) == end)
		{
			Road.Add(Cast<ATile>(Elem));
			return Road;
		}
	}
	temp = begining;

	while (temp != end)
	{

		// Moving right
		if (start.Y < finish.Y && start.X == finish.X)
		{
			for (auto& Elem : temp->GetNeightbours())
			{
				if (Cast<ATile>(Elem) == end)
				{
					Road.Add(Cast<ATile>(Elem));
					return Road;
				}
				FTransform tmpTransform = Elem->GetTransform();
				FVector tmpVector = tmpTransform.GetLocation();
				if (tmpVector.Y == start.Y + temp->GetHorizontalStep())
				{
					Road.Add(Cast<ATile>(Elem));
					temp = Cast<ATile>(Elem);
					q = temp->GetTransform();
					start = q.GetLocation();
					error--;
					break;
				}
			}
		}
		// Moving left
		else if (start.Y > finish.Y && start.X == finish.X)
		{
			for (auto& Elem : temp->GetNeightbours())
			{
				if (Cast<ATile>(Elem) == end)
				{
					Road.Add(Cast<ATile>(Elem));
					return Road;
				}
				FTransform tmpTransform = Elem->GetTransform();
				FVector tmpVector = tmpTransform.GetLocation();
				if (tmpVector.Y == start.Y - temp->GetHorizontalStep())
				{
					Road.Add(Cast<ATile>(Elem));
					temp = Cast<ATile>(Elem);
					q = temp->GetTransform();
					start = q.GetLocation();
					error--;
					break;
				}
			}
		}
		// Moving top left
		else if (start.Y > finish.Y && start.X < finish.X)
		{
			int counter = Road.Num();
			for (auto& Elem : temp->GetNeightbours())
			{
				if (Cast<ATile>(Elem) == end)
				{
					Road.Add(Cast<ATile>(Elem));
					return Road;
				}
				FTransform tmpTransform = Elem->GetTransform();
				FVector tmpVector = tmpTransform.GetLocation();
				if (tmpVector.X == start.X + temp->GetVerticalStep() && tmpVector.Y < start.Y)
				{
					Road.Add(Cast<ATile>(Elem));
					temp = Cast<ATile>(Elem);
					q = temp->GetTransform();
					start = q.GetLocation();
					error--;
					break;
				}
			}
			if (counter == Road.Num()) // if Moving top left is immpossible
			{
				for (auto& Elem : temp->GetNeightbours())
				{
					FTransform tmpTransform = Elem->GetTransform();
					FVector tmpVector = tmpTransform.GetLocation();
					if (tmpVector.Y == start.Y - temp->GetHorizontalStep())
					{
						Road.Add(Cast<ATile>(Elem));
						temp = Cast<ATile>(Elem);
						q = temp->GetTransform();
						start = q.GetLocation();
						error--;
						break;
					}
				}
			}
		}
		// Moving top right
		else if (start.Y < finish.Y && start.X < finish.X)
		{
			int counter = Road.Num();
			for (auto& Elem : temp->GetNeightbours())
			{
				if (Cast<ATile>(Elem) == end)
				{
					Road.Add(Cast<ATile>(Elem));
					return Road;
				}
				FTransform tmpTransform = Elem->GetTransform();
				FVector tmpVector = tmpTransform.GetLocation();
				if (tmpVector.X == start.X + temp->GetVerticalStep() && tmpVector.Y > start.Y)
				{
					Road.Add(Cast<ATile>(Elem));
					temp = Cast<ATile>(Elem);
					q = temp->GetTransform();
					start = q.GetLocation();
					error--;
					break;
				}
			}
			if (counter == Road.Num()) // if Moving top right is immpossible
			{
				for (auto& Elem : temp->GetNeightbours())
				{
					FTransform tmpTransform = Elem->GetTransform();
					FVector tmpVector = tmpTransform.GetLocation();
					if (tmpVector.Y == start.Y + temp->GetHorizontalStep())
					{
						Road.Add(Cast<ATile>(Elem));
						temp = Cast<ATile>(Elem);
						q = temp->GetTransform();
						start = q.GetLocation();
						error--;
						break;
					}
				}
			}
		}
		// Moving bottom left
		else if (start.Y > finish.Y && start.X > finish.X)
		{
			int counter = Road.Num();
			for (auto& Elem : temp->GetNeightbours())
			{
				if (Cast<ATile>(Elem) == end)
				{
					Road.Add(Cast<ATile>(Elem));
					return Road;
				}
				FTransform tmpTransform = Elem->GetTransform();
				FVector tmpVector = tmpTransform.GetLocation();
				if (tmpVector.X == start.X - temp->GetVerticalStep() && tmpVector.Y < start.Y)
				{
					Road.Add(Cast<ATile>(Elem));
					temp = Cast<ATile>(Elem);
					q = temp->GetTransform();
					start = q.GetLocation();
					error--;
					break;
				}
			}
			if (counter == Road.Num()) // if Moving bottom left is immpossible
			{
				for (auto& Elem : temp->GetNeightbours())
				{
					FTransform tmpTransform = Elem->GetTransform();
					FVector tmpVector = tmpTransform.GetLocation();
					if (tmpVector.Y == start.Y - temp->GetHorizontalStep())
					{
						Road.Add(Cast<ATile>(Elem));
						temp = Cast<ATile>(Elem);
						q = temp->GetTransform();
						start = q.GetLocation();
						error--;
						break;
					}
				}
			}
		}
		// Moving bottom right
		else if (start.Y < finish.Y && start.X > finish.X)
		{
			int counter = Road.Num();
			for (auto& Elem : temp->GetNeightbours())
			{
				if (Cast<ATile>(Elem) == end)
				{
					Road.Add(Cast<ATile>(Elem));
					return Road;
				}
				FTransform tmpTransform = Elem->GetTransform();
				FVector tmpVector = tmpTransform.GetLocation();
				if (tmpVector.X == start.X - temp->GetVerticalStep() && tmpVector.Y > start.Y)
				{
					Road.Add(Cast<ATile>(Elem));
					temp = Cast<ATile>(Elem);
					q = temp->GetTransform();
					start = q.GetLocation();
					error--;
					break;
					
				}				
			}
			if (counter == Road.Num()) // if Moving bottom right is immpossible
			{
				for (auto& Elem : temp->GetNeightbours())
				{
					FTransform tmpTransform = Elem->GetTransform();
					FVector tmpVector = tmpTransform.GetLocation();
					if (tmpVector.Y == start.Y + temp->GetHorizontalStep())
					{
						Road.Add(Cast<ATile>(Elem));
						temp = Cast<ATile>(Elem);
						q = temp->GetTransform();
						start = q.GetLocation();
						error--;
						break;
					}
				}
			}
		}
		else // special occasion when target tile is on X axis
		{
			if (start.X > finish.X)
			{
				for (auto& Elem : temp->GetNeightbours())
				{
					if (Cast<ATile>(Elem) == end)
					{
						Road.Add(Cast<ATile>(Elem));
						return Road;
					}
					FTransform tmpTransform = Elem->GetTransform();
					FVector tmpVector = tmpTransform.GetLocation();
					if (tmpVector.X < start.X)
					{
						Road.Add(Cast<ATile>(Elem));
						temp = Cast<ATile>(Elem);
						q = temp->GetTransform();
						start = q.GetLocation();
						error--;
						break;
					}
				}
			}
			else
			{
				for (auto& Elem : temp->GetNeightbours())
				{
					if (Cast<ATile>(Elem) == end)
					{
						Road.Add(Cast<ATile>(Elem));
						return Road;
					}
					FTransform tmpTransform = Elem->GetTransform();
					FVector tmpVector = tmpTransform.GetLocation();
					if (tmpVector.X > start.X)
					{
						Road.Add(Cast<ATile>(Elem));
						temp = Cast<ATile>(Elem);
						q = temp->GetTransform();
						start = q.GetLocation();
						error--;
						break;
					}
				}
			}
		}

		error++;
		if (error >= 2)
		{
			UE_LOG(LogTemp, Warning, TEXT("There is a problem with finding a road"));
			break;
		}
	}

	/*
	for (auto& Elem : begining->GetNeightbours())
	{
		if (Cast<ATile>(Elem)->Layer < begining->Layer)
		{
			Road.Add(Cast<ATile>(Elem));				
		}
		break;
	}*/
	
	return Road;
}




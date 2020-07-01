// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "WildLandsCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Materials/Material.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SkeletalMeshComponent.h"
#include "WildLandsPlayerController.h"
#include "GameFramework/Actor.h"
#include "TimerManager.h"
#include "Tile.h"
#include "Animation/AnimInstance.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "RoadIndicator.h"
#include "EngineUtils.h"
#include "WildLandsGameMode.h"
#include "Grid.h"

AWildLandsCharacter::AWildLandsCharacter()
{
	CharacterType = ECharacterType::UnknownCharacter;
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to Moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);


	// Create a decal in the world to show the cursor's location
	CursorToWorld = CreateDefaultSubobject<UDecalComponent>("CursorToWorld");
	CursorToWorld->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UMaterial> DecalMaterialAsset(TEXT("Material'/Game/TopDownCPP/Blueprints/M_Cursor_Decal.M_Cursor_Decal'"));
	if (DecalMaterialAsset.Succeeded())
	{
		CursorToWorld->SetDecalMaterial(DecalMaterialAsset.Object);
	}
	CursorToWorld->DecalSize = FVector(32.0f, 64.0f, 64.0f);
	CursorToWorld->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f).Quaternion());
	CursorToWorld->SetRelativeLocation(FVector(0.f, 0.f, -80.f));
	// Activate ticking in order to update the cursor every frame.
	CursorToWorld->SetVisibility(false);

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bTickEvenWhenPaused = true;
	PrimaryActorTick.TickGroup = TG_PrePhysics;
	WalkingTime = 5.f;
	
}
void AWildLandsCharacter::BeginPlay()
{
	Super::BeginPlay();
	MyGamemode = Cast<AWildLandsGameMode>(GetWorld()->GetAuthGameMode());
	UE_LOG(LogTemp, Warning, TEXT("BeginPlay"));
	MyPlayerController = Cast<AWildLandsPlayerController>(GetWorld()->GetFirstPlayerController());
	if (MyPlayerController != nullptr)
	{
		MoveCharacterDelegate.BindUObject(this, &AWildLandsCharacter::BeginJourney);
	}
	/*
	for (TActorIterator<ARoadIndicator> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		RoadIndicator = Cast<ARoadIndicator>(*ActorItr);
		if (RoadIndicator)
		{
			RoadIndicator->LetsTestIt();
			break;
		}
	}*/

}
void AWildLandsCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);



	if (RoadToTake == true && CharacterState == ECharacterState::MovingCharacter)
	{
		if (Road.Num() != 0)
		{
			if (NextTile != Road[0])
			{
				MoveToTile(Road[0]);
			}
			else
			{
				if (IsMoving == true)
				{
					FVector step = NormalizeDirection * GetCharacterMovement()->MaxWalkSpeed * DeltaSeconds;
					SetActorLocation(this->GetActorLocation() + step);
					

					FVector temp = this->GetActorLocation();
					FVector temp2 = NextTile->GetActorLocation();
					temp2.Z += 50;

					FVector tempHeading = temp2 - temp;
					FVector temp3 = tempHeading;
					float tempDistance = tempHeading.Size2D();

					if (tempDistance <= 10.f)
					{
						CharacterTile->WildLandsCharacters.Remove(this);
						if (MyPlayerController->IsTileWidget())
						{
							MyPlayerController->UpdateCharacterListView.Broadcast();
						}
						CharacterTile = NextTile;
						NextTile->WildLandsCharacters.Add(this);
						Road.RemoveAt(0);
						if (MyPlayerController->SelectedCharacter == this && MyPlayerController->SelectedTile != CharacterTile)
						{
							MyPlayerController->SelectedTile = CharacterTile;
						}
						
						//scouting
						//MyGamemode->GridRef->SpawnMissingNeightbours(NextTile->AreColliding, NextTile->Neightbours, NextTile);
					}
				}
			}
		}
		else
		{
			IsMoving = false;
			RoadToTake = false;
			FVector temp = CharacterTile->GetActorLocation();
			temp.Z += 100.f;
			this->SetActorLocation(temp);
			Road.Empty();
		}
	}

	if (RoadToTake == true && CharacterState != ECharacterState::MovingCharacter)
	{
		CharacterState = ECharacterState::MovingCharacter;
	}

	switch (CharacterState)
	{
		case ECharacterState::TakingAction:
		{
			ElapsedTimeInLoop = GetWorldTimerManager().GetTimerElapsed(Timer);

			BarPercent = ElapsedTimeInLoop / 5.f;
			UpdateCharacterProgressBar.Broadcast();

			break;
		}
	}
}
void AWildLandsCharacter::SetSelection(bool selection)
{
	CursorToWorld->SetVisibility(selection);
	//MyPlayerController->RoadIndicator->ShowRoad(this->GetActorLocation(), Road, selection);
}
void AWildLandsCharacter::SelectFromList()
{
	MyPlayerController->SelectedTile->TileToSpecificCharacter(this);
}
void AWildLandsCharacter::BeginJourney(ATile* end)
{	
	//MyPlayerController->RoadIndicator->ShowRoad(this->GetActorLocation(), Road, true);
	RoadToTake = true;
}

void AWildLandsCharacter::MoveToTile(ATile* destination)
{
	FVector thisCharacterLocation = RootComponent->GetComponentLocation();

	if (CharacterTile->GetActorLocation().Y < destination->GetActorLocation().Y)
	{
		thisCharacterLocation = CharacterTile->MovingRightPosition->GetComponentLocation();
	}
	else
	{
		thisCharacterLocation = CharacterTile->MovingLeftPosition->GetComponentLocation();
	}
	thisCharacterLocation.Z += 50.f;
	this->SetActorLocation(thisCharacterLocation);

	


	FVector temp = this->GetActorLocation();

	FVector temp2 = destination->GetActorLocation();
	
	temp2.Z += 100;

	Heading = temp2 - temp;
	FVector temp3 = Heading;
	Distance = Heading.Size2D();
	NormalizeDirection = Heading / Distance;

	//FVector forward = this->GetActorForwardVector();
	FVector x = NormalizeDirection; 
	Rotation = FRotationMatrix::MakeFromX(x).Rotator();
	//rotation.FindLookAtRotation(forward, NormalizeDirection);
	this->SetActorRotation(Rotation);
	UE_LOG(LogTemp, Warning, TEXT("this->GetActorLocation() %s"), *temp.ToString());
	UE_LOG(LogTemp, Warning, TEXT("destination->GetActorLocation() %s"), *temp2.ToString());
	UE_LOG(LogTemp, Warning, TEXT("normalizeDirection %s"), *NormalizeDirection.ToString());
	UE_LOG(LogTemp, Warning, TEXT("distance %f"), Distance);


	GetCharacterMovement()->MaxWalkSpeed = Distance / WalkingTime;
	NextTile = destination;
	IsMoving = true;
}
void AWildLandsCharacter::Moving()
{
	IsMoving = false;
	UE_LOG(LogTemp, Warning, TEXT("Moving - done"));
}
void AWildLandsCharacter::StopMoving()
{
	IsMoving = false;
	RoadToTake = false;
	FVector temp = CharacterTile->GetActorLocation();
	temp.Z += 100.f;
	this->SetActorLocation(temp);
	Road.Empty();
}

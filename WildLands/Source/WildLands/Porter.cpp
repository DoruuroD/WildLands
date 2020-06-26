// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "Components/SkeletalMeshComponent.h"
#include "WildLandsPlayerController.h"
#include "GameFramework/Actor.h"
#include "TimerManager.h"
#include "Animation/AnimInstance.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "WildLandsGameMode.h"
#include "Porter.h"
#include "Building.h"
#include "GameFramework/Actor.h"
#include "Tile.h"
#include "LumberjackHut.h"
#include "TimerManager.h"
#include "PlayerVillage.h"
#include <cmath>

APorter::APorter()
{
	PorterState = EPorterState::Unknown;
	this->CitizenType = ECitizenType::Porter;
	// Set size for player capsule
	
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to Moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);



	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bTickEvenWhenPaused = true;
	PrimaryActorTick.TickGroup = TG_PrePhysics;
	WalkingTime = 10.f;
	NextTile = nullptr;
}

void APorter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	switch (PorterState)
	{
		case EPorterState::Moving:
		{
			if (PorterMovingType == EPorterMovingTypes::MovingToProductionBuilding)
			{
				if (Road.Num() != 0 && NextTileIndex < Road.Num())
				{

					FVector temp = this->GetActorLocation();
					FVector temp2 = NextTile->GetActorLocation();
					temp2.Z += 100;

					Heading = temp2 - temp;
					Distance = Heading.Size2D();
					NormalizeDirection = Heading / Distance;

					FVector x = NormalizeDirection;
					Rotation = FRotationMatrix::MakeFromX(x).Rotator();

					this->SetActorRotation(Rotation);

					FVector step = NormalizeDirection * GetCharacterMovement()->MaxWalkSpeed * DeltaSeconds;
					SetActorLocation(this->GetActorLocation() + step);

					if (Distance <= 10.f)
					{
						NextTileIndex++;
						if (NextTileIndex != Road.Num())
						{
							CharacterTile = NextTile;
							NextTile = Road[NextTileIndex];
	
						}
						else
						{
							CharacterTile = NextTile;
							FVector TempActorLocation = CharacterTile->GetActorLocation();
							TempActorLocation.Z += 100.f;
							SetActorLocation(TempActorLocation);
							PorterState = EPorterState::Loading;
							FromMovingToLoadingCargo();
						}
					}									
				}
			}
			else if (PorterMovingType = EPorterMovingTypes::MovingToStorage)
			{
				if (Road.Num() != 0 && NextTileIndex <= Road.Num()+1)
				{

					FVector temp = this->GetActorLocation();
					FVector temp2 = NextTile->GetActorLocation();
					temp2.Z += 100;

					Heading = temp2 - temp;
					Distance = Heading.Size2D();
					NormalizeDirection = Heading / Distance;

					FVector x = NormalizeDirection;
					Rotation = FRotationMatrix::MakeFromX(x).Rotator();

					this->SetActorRotation(Rotation);

					FVector step = NormalizeDirection * GetCharacterMovement()->MaxWalkSpeed * DeltaSeconds;
					SetActorLocation(this->GetActorLocation() + step);

					if (Distance <= 10.f)
					{
						NextTileIndex++;
						if (NextTileIndex <= Road.Num()+1)
						{
							CharacterTile = NextTile;
							if (Road.Num() - 2 - NextTileIndex >= 0)
							{
								NextTile = Road[Road.Num() - 2 - NextTileIndex];
							}
							else if (Road.Num() - 2 - NextTileIndex == -1)
							{
								ATile* TileRef = Cast<ATile>(TransportTo->GetOwner());
								if (TileRef != nullptr)
								{
									NextTile = TileRef;
								}
								else
								{
									UE_LOG(LogTemp, Fatal, TEXT("EPorterMovingTypes::MovingToStorage cast TileRef to ATile failed"));
								}
							}
							else
							{
								ATile* TileRef = Cast<ATile>(TransportTo->GetOwner());
								if (TileRef != nullptr)
								{
									NextTile = TileRef;
								}
								else
								{
									UE_LOG(LogTemp, Fatal, TEXT("EPorterMovingTypes::MovingToStorage cast TileRef to ATile failed"));
								}
							}

						}
						else
						{
							CharacterTile = NextTile;
							FVector TempActorLocation = CharacterTile->GetActorLocation();
							TempActorLocation.Z += 100.f;
							SetActorLocation(TempActorLocation);
							PorterState = EPorterState::Unloading;
							FromMovingToUnloadingCargo();
						}
					}
				}
			}
			else
			{
				UE_LOG(LogTemp, Fatal, TEXT("PorterMovingType == UnknownPorterMovingType"));
			}		
		}
		case EPorterState::Waiting:
		{
			break;
		}
		case EPorterState::Loading:
		{
			ElapsedTimeInLoop = GetWorldTimerManager().GetTimerElapsed(Timer);

			BarPercent = ElapsedTimeInLoop / LoadingSpeed;
			UpdateProgressBar.Broadcast();


			break;			
		}
		case EPorterState::Unloading:
		{
			ElapsedTimeInLoop = GetWorldTimerManager().GetTimerElapsed(Timer);

			BarPercent = ElapsedTimeInLoop / UnloadingSpeed;
			UpdateProgressBar.Broadcast();
			break;
		}
		default:
			break;
	}
}

void APorter::BeginPlay()
{
	Super::BeginPlay();
	MyGamemode = Cast<AWildLandsGameMode>(GetWorld()->GetAuthGameMode());
	MyPlayerController = Cast<AWildLandsPlayerController>(GetWorld()->GetFirstPlayerController());

	// turnig off visibility
	
	
}
void APorter::BeginMovingToWork()
{
	PorterState = EPorterState::Moving;

	Road = MyPlayerController->FindRoad(CharacterTile, Cast<ATile>(this->GetFrom->GetOwner()));

	NextTile = Road[0];
	NextTileIndex = 0;
	FVector temp = this->GetActorLocation();
	FVector temp2 = NextTile->GetActorLocation();
	temp2.Z += 100;

	Heading = temp2 - temp;
	Distance = Heading.Size2D();
	NormalizeDirection = Heading / Distance;

	GetCharacterMovement()->MaxWalkSpeed = Distance / WalkingTime;

	PorterMovingType = EPorterMovingTypes::MovingToProductionBuilding;
	UpdateProgressBar.Broadcast();
}



void APorter::LoadResource()
{
	ULumberjackHut* HutRef = Cast<ULumberjackHut>(GetFrom);
	if (HutRef == nullptr)
	{
		UE_LOG(LogTemp, Fatal, TEXT("FromMovingToLoadingCargo cast GetFrom to HutRef failed"));
	}
	if ( CargoAmount != MaxCargo)
	{
		bool Check = HutRef->LoadResourceOnPorter(CargoAmount);
		if (!Check)
		{
			GetWorldTimerManager().ClearTimer(Timer);
			FromLoadingCargoToWaitingForWork();
		}
		else
		{
			if (CargoAmount == MaxCargo)
			{
				GetWorldTimerManager().ClearTimer(Timer);
				FromLoadingCargoToMoving();
			}
		}
	}
	
}
void APorter::UnloadResource()
{
	UPlayerVillage* VillageRef = Cast<UPlayerVillage>(TransportTo);
	if (VillageRef == nullptr)
	{
		UE_LOG(LogTemp, Fatal, TEXT("UnloadResource cast TransportTo to VillageRef failed"));
	}



	if (CargoAmount != 0)
	{
		VillageRef->UnloadResourceFromPorter(this->CargoAmount);
	}
	if (CargoAmount == 0)
	{
		GetWorldTimerManager().ClearTimer(Timer);
		FromUnloadingCargoToMoving();
	}
}
// Transitions
void APorter::FromMovingToLoadingCargo()
{
	switch (GetFrom->BuildingType)
	{
		case EBuildingType::Lumberjack:
		{
			ULumberjackHut* HutRef = Cast<ULumberjackHut>(GetFrom);
			if (HutRef == nullptr)
			{
				UE_LOG(LogTemp, Fatal, TEXT("FromMovingToLoadingCargo cast GetFrom to HutRef failed"));
			}
			if (HutRef->ResourcesInBuilding != 0)
			{
				GetWorldTimerManager().SetTimer(Timer, this, &APorter::LoadResource, this->LoadingSpeed, this->LoadingSpeed);
			}
			else
			{
				GetWorldTimerManager().ClearTimer(Timer);
				FromMovingToWaitingForWork();
			}
			break;
		}
		case EBuildingType::Field:
		{
			break;
		}
		case EBuildingType::Mine:
		{
			break;
		}
		default:
		{
			break;
		}
	}
}
void APorter::WaitForResource()
{
	ULumberjackHut* HutRef = Cast<ULumberjackHut>(GetFrom);
	if (HutRef == nullptr)
	{
		UE_LOG(LogTemp, Fatal, TEXT("FromMovingToLoadingCargo cast GetFrom to HutRef failed"));
	}
	if (HutRef->ResourcesInBuilding != 0)
	{
		GetWorldTimerManager().ClearTimer(Timer);
		FromWaitingForWorkToLoadingCargo();
	}
}
void APorter::FromMovingToWaitingForWork() 
{
	PorterState = EPorterState::Waiting;
	GetWorldTimerManager().SetTimer(Timer, this, &APorter::WaitForResource, 1.f, 1.f);
}

void APorter::FromMovingToUnloadingCargo() 
{
	switch (TransportTo->BuildingType)
	{
	case EBuildingType::Village:
	{
		UPlayerVillage* HutRef = Cast<UPlayerVillage>(TransportTo);
		if (HutRef == nullptr)
		{
			UE_LOG(LogTemp, Fatal, TEXT("FromMovingToLoadingCargo cast GetFrom to HutRef failed"));
		}

		GetWorldTimerManager().SetTimer(Timer, this, &APorter::UnloadResource, this->UnloadingSpeed, this->UnloadingSpeed);
		
		break;
	}
	case EBuildingType::Field:
	{
		break;
	}
	case EBuildingType::Mine:
	{
		break;
	}
	default:
	{
		break;
	}
	}
}


void APorter::FromLoadingCargoToMoving() 
{
	PorterMovingType = EPorterMovingTypes::MovingToStorage;
	if (Road.Num() - 2 >= 0)
	{
		NextTile = Road[Road.Num() - 2];
	}
	else
	{
		NextTile = Cast<ATile>(TransportTo->GetOwner());
	}
	PorterState = EPorterState::Moving;
	UpdateProgressBar.Broadcast();
}

void APorter::FromLoadingCargoToWaitingForWork() 
{
	PorterState = EPorterState::Waiting;
	GetWorldTimerManager().SetTimer(Timer, this, &APorter::WaitForResource, 1.f, 1.f);
	UpdateProgressBar.Broadcast();
}

void APorter::FromWaitingForWorkToMoving() 
{

}

void APorter::FromWaitingForWorkToLoadingCargo()
{
	PorterState = EPorterState::Loading;
	ULumberjackHut* HutRef = Cast<ULumberjackHut>(GetFrom);
	if (HutRef == nullptr)
	{
		UE_LOG(LogTemp, Fatal, TEXT("FromWaitingForWorkToLoadingCargo cast GetFrom to HutRef failed"));
	}
	if (HutRef->ResourcesInBuilding != 0)
	{
		GetWorldTimerManager().ClearTimer(Timer);
		GetWorldTimerManager().SetTimer(Timer, this, &APorter::LoadResource, this->LoadingSpeed, this->LoadingSpeed);
	}
}

void APorter::FromWaitingForWorkToUnloadingCargo() 
{

}

void APorter::FromUnloadingCargoToWaitingForWork()
{


}

void APorter::FromUnloadingCargoToMoving() 
{
	PorterMovingType = EPorterMovingTypes::MovingToProductionBuilding;
	NextTile = Road[0];
	NextTileIndex = 0;
	PorterState = EPorterState::Moving;
	UpdateProgressBar.Broadcast();
}
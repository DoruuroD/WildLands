// Fill out your copyright notice in the Description page of Project Settings.


#include "RoadIndicator.h"
#include "Tile.h"
#include "Components/DecalComponent.h"
#include "Engine/World.h"
#include "UObject/ConstructorHelpers.h"

#include "WildLandsPlayerController.h"
#include "Materials/Material.h"
#include "Containers/Array.h"
#include "UObject/UObjectGlobals.h"
#include "Engine/DecalActor.h"
#include "CoreUObject/Public/UObject/UObjectIterator.h"
#include "WildLandsCharacter.h"


// Sets default values
ARoadIndicator::ARoadIndicator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DecalPitchRotation = -90.f;
	DecalRollRotation = 0.f;

	Scale = FVector(1.f, 0.1f, 0.2f);
	Size = FVector(256.f, 512.f, 512.f);
	static ConstructorHelpers::FObjectFinder<UMaterial> DecalMaterialAsset(TEXT("Material'/Game/arrow_Mat.arrow_Mat'"));
	if (DecalMaterialAsset.Succeeded())
	{
		DotsDecalMaterial = DecalMaterialAsset.Object;
	}
	static ConstructorHelpers::FObjectFinder<UMaterial> DecalMaterialAsset2(TEXT("Material'/Game/Road_Indicators/roadEnd_Mat.roadEnd_Mat'"));
	if (DecalMaterialAsset2.Succeeded())
	{
		EndRoadDecalMaterial = DecalMaterialAsset2.Object;
	}
	static ConstructorHelpers::FObjectFinder<UMaterial> DecalMaterialAsset3(TEXT("Material'/Game/Road_Indicators/turnLeft_Mat.turnLeft_Mat'"));
	if (DecalMaterialAsset2.Succeeded())
	{
		TurnLeftDecalMaterial = DecalMaterialAsset3.Object;
	}
	static ConstructorHelpers::FObjectFinder<UMaterial> DecalMaterialAsset4(TEXT("Material'/Game/Road_Indicators/turnRight_Mat.turnRight_Mat'"));
	if (DecalMaterialAsset2.Succeeded())
	{
		TurnRightDecalMaterial = DecalMaterialAsset4.Object;
	}
}

// Called when the game starts or when spawned
void ARoadIndicator::BeginPlay()
{
	Super::BeginPlay();
}
void ARoadIndicator::LetsTestIt()
{
	int i = 0;
	for (TObjectIterator<ATile> Itr; Itr; ++Itr)
	{
		i++;
		TestRoad.Add(*Itr);
		if (i == 3)
		{
			break;
		}
	}
	FVector temp = TestRoad[0]->GetActorLocation();
	TestRoad.RemoveAt(0);
	//ShowRoad(rollDiff, TestRoad, true);
}
// Called every frame
void ARoadIndicator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
void ARoadIndicator::ShowRoad(AWildLandsCharacter* character, TArray<ATile*> Road, bool showHide)
{
	for (int i = 0; i < RoadIndicatorDecals.Num(); i++)
	{
		RoadIndicatorDecals[i]->Destroy();
	}

	RoadIndicatorDecals.Empty();

	if (showHide && Road.Num() != 0)
	{
		for (int j = 0; j <= Road.Num(); j++)
		{		
			CreateIndicator();
		}
		SetupDecals(character->CharacterTile->GetActorLocation(), Road);
		CheckMaterials();
	}
}
void ARoadIndicator::SetupDecals(FVector characterTilePosition, TArray<ATile*> Road)
{
	if (RoadIndicatorDecals.Num() != 0)
	{
		RoadIndicatorDecals[0]->SetActorLocation(characterTilePosition);
		UE_LOG(LogTemp, Warning, TEXT("this->GetActorLocation() %s"), *RoadIndicatorDecals[0]->GetActorLocation().ToString());
		RoadIndicatorDecals[0]->GetDecal()->SetWorldScale3D(Scale);
		RoadIndicatorDecals[0]->GetDecal()->DecalSize = Size;
		RoadIndicatorDecals[0]->GetDecal()->SetVisibility(true);
		

		FVector temp = Road[0]->GetActorLocation();
		FVector direction = temp - characterTilePosition;
		float distance = direction.Size();

		FRotator decalRotation = FRotationMatrix::MakeFromX(direction).Rotator();

		decalRotation.Pitch = -90.f;
		decalRotation.Roll = 0.f;

		RoadIndicatorDecals[0]->SetActorRotation(decalRotation);

		for (int i = 0; i < RoadIndicatorDecals.Num()-1; i++)
		{
			RoadIndicatorDecals[i + 1]->SetActorLocation(Road[i]->GetActorLocation());
			
			RoadIndicatorDecals[i + 1]->GetDecal()->SetWorldScale3D(Scale);
			RoadIndicatorDecals[i + 1]->GetDecal()->DecalSize = Size;
			RoadIndicatorDecals[i + 1]->GetDecal()->SetVisibility(true);

			temp = Road[i]->GetActorLocation();
			direction = temp - RoadIndicatorDecals[i]->GetActorLocation();
			distance = direction.Size();
			direction = direction / distance; //normalize direction
			FRotator decalRotation2 = FRotationMatrix::MakeFromX(direction).Rotator();
	
			decalRotation2.Pitch = -90.f;
		    decalRotation2.Roll = 0.f;

			RoadIndicatorDecals[i + 1]->SetActorRotation(decalRotation2);
			UE_LOG(LogTemp, Warning, TEXT("MakeFromX %f"), *decalRotation2.ToString());
			if (i == RoadIndicatorDecals.Num() - 2)
			{
				RoadIndicatorDecals[i + 1]->GetDecal()->SetMaterial(0, EndRoadDecalMaterial);
			}
		}
	}
}
void ARoadIndicator::CheckMaterials()
{
	FRotator decalRotation;
	FRotator nextDecalRotation;
	if (RoadIndicatorDecals.Num() > 1)
	{
		for (int i = 1; i < RoadIndicatorDecals.Num(); i++)
		{
			decalRotation = RoadIndicatorDecals[i - 1]->GetActorRotation();
			nextDecalRotation = RoadIndicatorDecals[i]->GetActorRotation();
			float rollDiff = nextDecalRotation.Roll - decalRotation.Roll;

			UE_LOG(LogTemp, Warning, TEXT("Rotation diff %f"), rollDiff);

			// Yeah i know... But it's working :D
			if ((rollDiff > 20.f && rollDiff < 133 ) || (rollDiff < 0 && rollDiff > -61))
			{
				if (rollDiff >= 59 && rollDiff < 60)
				{
					RoadIndicatorDecals[i - 1]->GetDecal()->SetMaterial(0, TurnRightDecalMaterial);
				}
				else
				{
					RoadIndicatorDecals[i - 1]->GetDecal()->SetMaterial(0, TurnLeftDecalMaterial);
				}
			}
			else if (rollDiff >= 133 || rollDiff < -130)
			{
				if ((rollDiff >= 183 && rollDiff < 184) || (rollDiff >= -136 && rollDiff < -135))
				{
					RoadIndicatorDecals[i - 1]->GetDecal()->SetMaterial(0, TurnLeftDecalMaterial);
				}
				else
				{
					RoadIndicatorDecals[i - 1]->GetDecal()->SetMaterial(0, TurnRightDecalMaterial);
				}
			}
		}
	}
}
void  ARoadIndicator::SetDecalRotation(FVector start, FVector end, FRotator &rotation)
{
	FVector temp = end;
	FVector direction = temp - start;
	float distance = direction.Size();

	rotation = FRotationMatrix::MakeFromX(direction).Rotator();

	rotation.Pitch = DecalPitchRotation;
	rotation.Roll = 0.f;
	
}
void ARoadIndicator::CreateIndicator()
{
	if (DotsDecalBP)
	{
		FActorSpawnParameters spawnParams;
		ADecalActor* tempDecal = GetWorld()->SpawnActor<ADecalActor>(DotsDecalBP, GetTransform(), spawnParams);
		
		RoadIndicatorDecals.Add(tempDecal);	
		int index = RoadIndicatorDecals.Num() - 1;
		RoadIndicatorDecals[index]->SetDecalMaterial(DotsDecalMaterial);
		RoadIndicatorDecals[index]->GetDecal()->SetVisibility(false);
	}
}
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TileMesh.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Building.generated.h"

UENUM(BlueprintType)
enum EBuildingType {
	Lumberjack = 0,
	Mine,
	Field,
	Village,
	UnknownBuilding
};

UCLASS()
class WILDLANDS_API UBuilding : public UTileMesh
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	UBuilding();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	 
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Variable)
	TEnumAsByte<EBuildingType> BuildingType = EBuildingType::UnknownBuilding;



	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Variable)
	TSet<class ACitizen*> CitizensInBuilding;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Variable)
	int MaximumCitizenCapacity = 2;


	virtual void GenerateResource();
};

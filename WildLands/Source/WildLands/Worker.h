// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Citizen.h"
#include "Worker.generated.h"

/**
 * 
 */
UCLASS()
class WILDLANDS_API AWorker : public ACitizen
{
	GENERATED_BODY()
public:
	// Sets default values for this character's properties
	AWorker();


	UFUNCTION()
	void BecomeWorker(class UBuilding* Building);
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TileMesh.h"
#include "Resource.generated.h"

/**
 * 
 */
UCLASS()
class WILDLANDS_API UResource : public UTileMesh
{

	GENERATED_BODY()
public:
	// Sets default values for this component's properties
	UResource();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Variable)
	int ResourceAmount = 1;
};

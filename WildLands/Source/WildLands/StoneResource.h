// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Resource.h"
#include "StoneResource.generated.h"

/**
 * 
 */
UCLASS()
class WILDLANDS_API UStoneResource : public UResource
{
	GENERATED_BODY()
public:
	// Sets default values for this component's properties
	UStoneResource();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};

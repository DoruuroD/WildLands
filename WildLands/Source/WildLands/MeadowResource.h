// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Resource.h"
#include "MeadowResource.generated.h"

/**
 * 
 */
UCLASS()
class WILDLANDS_API UMeadowResource : public UResource
{
	GENERATED_BODY()
public:
	// Sets default values for this component's properties
	UMeadowResource();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
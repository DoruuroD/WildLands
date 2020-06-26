// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WildLandsCharacter.h"
#include "Scout.generated.h"

/**
 * 
 */
UCLASS()
class WILDLANDS_API AScout : public AWildLandsCharacter
{
	GENERATED_BODY()
public:
	AScout();
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void ScoutSurroundings();

	UFUNCTION()
	void Scouting();

};

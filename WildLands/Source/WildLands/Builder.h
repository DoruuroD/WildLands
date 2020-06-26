// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WildLandsCharacter.h"
#include "Builder.generated.h"


UCLASS()
class WILDLANDS_API ABuilder : public AWildLandsCharacter
{
	GENERATED_BODY()
	ABuilder();

	virtual void Tick(float DeltaSeconds) override;
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable)
	void BuildLumberjacksHut();

	UFUNCTION()
	void Building();
};

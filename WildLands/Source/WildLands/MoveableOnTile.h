// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MoveableOnTile.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UMoveableOnTile : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class WILDLANDS_API IMoveableOnTile
{
	GENERATED_BODY()

public:
	virtual void MoveToTile(class AActor* ActorToMove);
};

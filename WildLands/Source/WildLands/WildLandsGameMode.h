// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "WildLandsGameMode.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTestMultiDelegate);

UCLASS(minimalapi)
class AWildLandsGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AWildLandsGameMode();
	void BeginPlay();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Variables)
	class AGrid* GridRef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Variables)
	class ATile* MainTile;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BP refs")
	TSubclassOf<class UPlayerVillage> PlayerVillageBP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BP refs")
	TSubclassOf<class ULumberjackHut> LumberjackHutBP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BP refs")
	TSubclassOf<class UStoneResource> StoneResourceBP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BP refs")
	TSubclassOf<class UWoodResource> WoodResourceBP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BP refs")
	TSubclassOf<class UMeadowResource> MeadowResourceBP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BP refs")
	TSubclassOf<class ACitizen> CitizenBP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BP refs")
	TSubclassOf<class APorter> PorterBP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BP refs")
	TSubclassOf<class AWorker> WorkerBP;

	UFUNCTION()
	void SetGrid(class AGrid* grid);

	UFUNCTION()
	void SetMainTile(class ATile* tile);

	UFUNCTION()
	class UStaticMesh* GetVillageMesh();

	UPROPERTY(BlueprintAssignable, Category = "event")
	FTestMultiDelegate TestMultiDelegate;

private:
	UPROPERTY()
	class UStaticMesh* MainBaseMesh;
};




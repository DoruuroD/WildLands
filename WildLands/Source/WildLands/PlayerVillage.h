// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Building.h"
#include "PlayerVillage.generated.h"

/**
 * 
 */
UCLASS()
class WILDLANDS_API UPlayerVillage : public UBuilding
{
	GENERATED_BODY()
		UPlayerVillage();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:
	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "BuildingVariable")
	//int MaximumNumberOfInhabitants = 4;

	UFUNCTION()
	class AWorker* DelegateToWork(class UBuilding* Building, class ACitizen* Citizen);

	UFUNCTION()
	class APorter* DelegateToTransport(class UBuilding* Building, class ACitizen* Citizen);

	UFUNCTION()
	void CreateNewCitizen();
	UFUNCTION()
	void UnloadResourceFromPorter(float &Amount);

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Refs, meta = (AllowPrivateAccess = "true"))
		class AWildLandsGameMode* MyGamemode;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Refs, meta = (AllowPrivateAccess = "true"))
		class AWildLandsPlayerController* MyPlayerController;
};

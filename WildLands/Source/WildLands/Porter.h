// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Citizen.h"
#include "Porter.generated.h"

UENUM(BlueprintType)
enum EPorterState {
	Moving = 0,
	Waiting,
	Loading,
	Unloading,
	Unknown
};
UENUM()
enum EPorterMovingTypes {
	MovingToProductionBuilding = 0,
	MovingToStorage,
	UnknownPorterMovingType
};
UCLASS()
class WILDLANDS_API APorter : public ACitizen
{
	GENERATED_BODY()
	APorter();

	virtual void Tick(float DeltaSeconds) override;

	virtual void BeginPlay() override;
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = PorterVariable)
	class UBuilding* GetFrom;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = PorterVariable)
	class UBuilding* TransportTo;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = PorterVariable)
	TEnumAsByte<EPorterState> PorterState;

	UPROPERTY(VisibleAnywhere, Category = PorterVariable)
	TEnumAsByte<EPorterMovingTypes> PorterMovingType = EPorterMovingTypes::UnknownPorterMovingType;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = PorterVariable)
	float MovingSpeed = 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = PorterVariable)
	float CargoAmount = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = PorterVariable)
	float MaxCargo = 2;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = PorterVariable)
	float LoadingSpeed = 4;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = PorterVariable)
	float UnloadingSpeed = 2;

	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = PorterVariable)
	//FTransform PorterDestination;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PorterVariable)
	TArray<class ATile*> Road;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class ATile* CharacterTile;

	FTimerHandle Timer;

	UFUNCTION()
	void BeginMovingToWork();




	UFUNCTION()
	void LoadResource();
	UFUNCTION()
	void UnloadResource();

	UFUNCTION()
	void WaitForResource();

	// Transitions
	UFUNCTION()
	void FromMovingToLoadingCargo();
	UFUNCTION()
	void FromMovingToWaitingForWork();
	UFUNCTION()
	void FromMovingToUnloadingCargo();

	UFUNCTION()
		void FromLoadingCargoToMoving();
	UFUNCTION()
		void FromLoadingCargoToWaitingForWork();
	UFUNCTION()
		void FromWaitingForWorkToMoving();
	UFUNCTION()
		void FromWaitingForWorkToLoadingCargo();
	UFUNCTION()
		void FromWaitingForWorkToUnloadingCargo();
	UFUNCTION()
		void FromUnloadingCargoToWaitingForWork();
	UFUNCTION()
		void FromUnloadingCargoToMoving();
private:

	UPROPERTY(VisibleAnywhere)
	bool GoToWork;

	UPROPERTY(VisibleAnywhere)
	float WalkingTime;

	UPROPERTY(VisibleAnywhere)
	FVector DirectionVector;

	UPROPERTY(VisibleAnywhere)
	int NextTileIndex = 0;

	UPROPERTY(VisibleAnywhere)
	class ATile* NextTile;

	UPROPERTY(VisibleAnywhere)
	FVector NormalizeDirection;

	UPROPERTY(VisibleAnywhere)
	FVector Heading;

	UPROPERTY(VisibleAnywhere)
	FRotator Rotation;

	UPROPERTY(VisibleAnywhere)
	float Distance;

	UPROPERTY(VisibleAnywhere)
	bool RoadToTake;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Citizen.generated.h"

UENUM(BlueprintType)
enum ECitizenType {
	Citizen=0,
	Porter,
	Worker,
	UnknownCitizenType
};
UENUM(BlueprintType)
enum ECitizenAnimationState {
	Idle,
	Walk
};
UCLASS()
class WILDLANDS_API ACitizen : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACitizen();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = CitizenVariable)
	TEnumAsByte<ECitizenType> CitizenType;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = CitizenVariable)
	TEnumAsByte<ECitizenAnimationState> CitizenAnimationState = ECitizenAnimationState::Idle;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = CitizenVariable)
	class UBuilding* WorkPlace;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = CitizenVariable)
	class UBuilding* House;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Refs)
	class AWildLandsPlayerController* MyPlayerController;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Refs)
	class AWildLandsGameMode* MyGamemode;

	UFUNCTION()
	void SetHouse(class UBuilding* Building);


};

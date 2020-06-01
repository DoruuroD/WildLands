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
UCLASS()
class WILDLANDS_API APorter : public ACitizen
{
	GENERATED_BODY()
	APorter();

	virtual void Tick(float DeltaSeconds) override;

	virtual void BeginPlay() override;
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Variable)
	class UBuilding* GetFrom;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Variable)
	class UBuilding* TransportTo;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Variable)
	TEnumAsByte<EPorterState> PorterState;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Variable)
	float MovingSpeed = 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = variable)
	TArray<class ATile*> Road;

};

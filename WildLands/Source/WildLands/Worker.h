// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Citizen.h"
#include "Worker.generated.h"

UENUM(BlueprintType)
enum EWorkerStatus {
	WaitingForWork,
	Working
};
UCLASS()
class WILDLANDS_API AWorker : public ACitizen
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:
	// Sets default values for this character's properties
	AWorker();

	FTimerHandle Timer;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Worker")
	TEnumAsByte<EWorkerStatus> WorkerStatus = EWorkerStatus::WaitingForWork;

	UFUNCTION()
	void BecomeWorker(class UBuilding* Building);

	UFUNCTION()
	void BeginWork();

	UFUNCTION()
	void StopWork();

	UFUNCTION()
	void Working();
};

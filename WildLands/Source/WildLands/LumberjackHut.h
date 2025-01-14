// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Building.h"
#include "LumberjackHut.generated.h"

/**
 * 
 */
UCLASS()
class WILDLANDS_API ULumberjackHut : public UBuilding
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:
	// Sets default values for this actor's properties
	ULumberjackHut();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Variable)
		TSet<class AWorker*> WorkersInBuilding;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Variable)
		float WorkingSpeed = 5;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Variable)
		int MaximumWorkerCapacity = 2;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Variable)
		TSet<class APorter*> PortersInBuilding;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Variable)
	int MaximumPorterCapacity = 2;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Variable)
	int MaximumResourceCapacity = 5;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Variable)
	int ResourcesInBuilding = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Variable)
	int ResourceAmountOnTile;

	UFUNCTION()
	void SendToWork();
	UFUNCTION()
	bool LoadResourceOnPorter(float &amount);


	UFUNCTION(BlueprintCallable)
		void AddWorker();

	UFUNCTION(BlueprintCallable)
		void AddPorter();

	UFUNCTION(BlueprintCallable)
		void RemoveWorker();

	UFUNCTION(BlueprintCallable)
		void RemovePorter();

	UFUNCTION(BlueprintCallable)
	void GenerateResource() override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Refs, meta = (AllowPrivateAccess = "true"))
	class AWildLandsGameMode* MyGamemode;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Refs, meta = (AllowPrivateAccess = "true"))
	class AWildLandsPlayerController* MyPlayerController;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RoadIndicator.generated.h"

UCLASS()
class WILDLANDS_API ARoadIndicator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARoadIndicator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<class ADecalActor*> RoadIndicatorDecals;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = variable)
	TArray<class ATile*> TestRoad;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class ADecalActor> DotsDecalBP;


	UFUNCTION()
	void ShowRoad(class AWildLandsCharacter* character, TArray<class ATile*> Road, bool showHide);

	UFUNCTION()
	void LetsTestIt();

	UFUNCTION()
	void CheckMaterials();

	UFUNCTION()
	void SetupDecals(FVector characterTilePosition, TArray<class ATile*> Road);
	UFUNCTION()
	void SetDecalRotation(FVector start, FVector end, FRotator &rotation);

	UFUNCTION()
	void CreateIndicator();

private:
	UPROPERTY(VisibleAnywhere)
	float DecalPitchRotation;

	UPROPERTY(VisibleAnywhere)
	float DecalRollRotation;

	UPROPERTY(VisibleAnywhere)
	FVector Scale;

	UPROPERTY(VisibleAnywhere)
	class UMaterial* DotsDecalMaterial;

	UPROPERTY(VisibleAnywhere)
	class UMaterial* EndRoadDecalMaterial;

	UPROPERTY(VisibleAnywhere)
	class UMaterial* TurnLeftDecalMaterial;

	UPROPERTY(VisibleAnywhere)
	class UMaterial* TurnRightDecalMaterial;

	UPROPERTY(VisibleAnywhere)
	FVector Size;
};

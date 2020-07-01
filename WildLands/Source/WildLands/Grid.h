// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Grid.generated.h"

UCLASS()
class WILDLANDS_API AGrid : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGrid();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void PostInitializeComponents() override;

	UFUNCTION()
	void SetLayerForAllTiles();

	UFUNCTION()
	void CheckAllNeightbours();
	UFUNCTION()
	void SpawnMissingNeightbours(TArray<bool> AreColliding, TSet<AActor*> Neightbours, class ATile* tile);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = Variable)
	FTimerHandle LoopTimerHandle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = Variable)
	int Size;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = Variable)
	TSubclassOf<class ATile> TileActorBP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = Variable)
	TArray<class ATile*> TileArray;



	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = Variable)
	TArray<class UMaterial*> TileMaterials;

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Variable, meta = (AllowPrivateAccess = "true"))
		class AWildLandsGameMode* MyGamemode;

	UFUNCTION()
		class UMaterial* ChooseMaterialByNoise(float noise, int32& number);
	UFUNCTION()
		class ATile* CreateAdditionalTile(FVector position);
	UFUNCTION()
		void ChangeMaterialForTiles(TArray<ATile*> Tiles);
	UFUNCTION()
		void CheckNeightbours(TSet<AActor*> Neightbours);
	UFUNCTION()
		void SpawnTiles();
	UFUNCTION()
		void ChangeMaterialForAllTiles();


	UPROPERTY()
		class UStaticMesh* Tree;

	UPROPERTY()
		class UStaticMesh* Rock;

	UPROPERTY()
		class UStaticMesh* Bush;

	UPROPERTY()
		class UStaticMesh* Flower;

	UPROPERTY()
		class UStaticMesh* SmallRock;

	UPROPERTY()
		class UStaticMesh* SmallTree;

	UFUNCTION()
		void SetMeshes(class ATile* tile);
};

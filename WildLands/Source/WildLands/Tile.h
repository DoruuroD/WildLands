// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Math/Vector.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tile.generated.h"

UENUM()
enum EBiom {
	Grass=0,
	Forest,
	Hills,
	Mountains
};

UCLASS()
class WILDLANDS_API ATile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void PostInitializeComponents() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:	


	UFUNCTION()
	virtual void NotifyActorEndCursorOver() override;
	UFUNCTION()
	virtual void NotifyActorBeginCursorOver() override;
	UFUNCTION()
	virtual void NotifyActorOnClicked(FKey ButtonPressed = EKeys::LeftMouseButton);


	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = Variable)
	class UMaterial* SelectionMeshMaterial;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Variable)
	class UStaticMeshComponent* SelectionMesh;


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Variable)
	class UStaticMeshComponent* TileMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Variable)
	class UTileMesh* CentralSlot;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Variable)
	class UTileMesh* Slot0;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Variable)
	class UTileMesh * Slot1;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Variable)
	class UTileMesh * Slot2;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Variable)
	class UTileMesh * Slot3;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Variable)
	class UTileMesh * Slot4;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Variable)
	class UTileMesh * Slot5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = Variable)
	class UMaterial* TileMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = Variable)
	TSubclassOf<class AScout> ScoutBP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = Variable)
	TSubclassOf<class ABuilder> BuilderBP;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Variable)
	TSet<class AWildLandsCharacter*> WildLandsCharacters;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Variable)
	TEnumAsByte<EBiom> Biom;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Variable)
	int Layer;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Variable)
	bool IsMainBase;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Variable)
	class UBoxComponent* StandbyPosition;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Variable)
	class UBoxComponent* MovingLeftPosition;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Variable)
	class UBoxComponent* MovingRightPosition;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Neightbours)
	TSet<AActor*> Neightbours;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Neightbours)
	TArray<bool> AreColliding;


	UFUNCTION()
		void TileToSpecificCharacter(class AWildLandsCharacter* WildLandsCharacter);

	UFUNCTION()
		TSet<AActor*> GetNeightbours();
	UFUNCTION()
		void MoveCharacter(AWildLandsCharacter* character);
	UFUNCTION()
		class AWildLandsCharacter* GetFirstCharacter();
	UFUNCTION()
		void SetBIOM(int32 number);
	UFUNCTION()
		void InitializeBase();
	UFUNCTION()
		void CheckNeightbours();
	UFUNCTION()
		void ChangeMaterial(class UMaterial* material);
	UFUNCTION()
		void SpawnCharacterOnTile(enum ECharacterType CharacterType);

	FORCEINLINE float GetTileNoise() { return TileNoise; }
	FORCEINLINE void SetTileNoise(float noise) { TileNoise = noise; }
	FORCEINLINE float GetSmallR() { return SmallR; }
	FORCEINLINE float GetBigR() { return BigR; }
	FORCEINLINE float GetScale() { return Scale; }
	FORCEINLINE float GetVerticalStep() { return VerticalStep; }
	FORCEINLINE float GetHorizontalStep() { return HorizontalStep; }

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Variable, meta = (AllowPrivateAccess = "true"))
	class AWildLandsGameMode* MyGamemode;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Variable, meta = (AllowPrivateAccess = "true"))
	class AWildLandsPlayerController* MyPlayerController;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Variable, meta = (AllowPrivateAccess = "true"))
	float SmallR;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Variable, meta = (AllowPrivateAccess = "true"))
	float BigR;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Variable, meta = (AllowPrivateAccess = "true"))
	float Scale;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Variable, meta = (AllowPrivateAccess = "true"))
	float VerticalStep;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Variable, meta = (AllowPrivateAccess = "true"))
	float HorizontalStep;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Variable, meta = (AllowPrivateAccess = "true"))
	float TileNoise;
	
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Neightbours, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* BoxCollider0;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Neightbours, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* BoxCollider1;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Neightbours, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* BoxCollider2;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Neightbours, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* BoxCollider3;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Neightbours, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* BoxCollider4;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Neightbours, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* BoxCollider5;



	//Selection transitions
	UFUNCTION()
	void NullToTile();
	UFUNCTION()
	void NullToCharacter();
	UFUNCTION()
	void TileToOtherTile();
	UFUNCTION()
	void TileToCharacter();

	UFUNCTION()
	void TileToNull();
	UFUNCTION()
	void CharactrToTile();
	UFUNCTION()
	void CharactrToCharacterTile();
	UFUNCTION()
	void CharacterToOtherCharacter();
	UFUNCTION()
	void CharacterToNull();

	UFUNCTION()
	void SetVariables();

};

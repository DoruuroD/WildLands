// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once
#include "Math/Vector.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "WildLandsCharacter.generated.h"

DECLARE_DELEGATE_OneParam(FMoveCharacter, class ATile*);

UENUM(BlueprintType)
enum ECharacterType {
	Scout = 0,
	Builder,
	Settler,
	Warrior,
	UnknownCharacter
};
UCLASS(Blueprintable)
class AWildLandsCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AWildLandsCharacter();

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

	virtual void BeginPlay() override;

	FMoveCharacter MoveCharacterDelegate;

	FTimerHandle MovingTimeHandle;

	/** Returns CursorToWorld subobject **/
	FORCEINLINE class UDecalComponent* GetCursorToWorld() { return CursorToWorld; }

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Variable)
	TEnumAsByte<ECharacterType> CharacterType;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = variable)
	TArray<class ATile*> Road;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class ARoadIndicator* RoadIndicator;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class ATile* CharacterTile;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class AWildLandsPlayerController* MyPlayerController;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Variable)
	class AWildLandsGameMode* MyGamemode;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool IsMoving;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float WalkingTime;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FVector DirectionVector;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UDecalComponent* CursorToWorld;
	
	UFUNCTION(BlueprintCallable)
	void SelectFromList();
	UFUNCTION()
	void SetSelection(bool selection);

	UFUNCTION()
	void BeginJourney(ATile* end);

	UFUNCTION()
	void MoveToTile(ATile* destination);
	UFUNCTION()
	void Moving();

private:



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

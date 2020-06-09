// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "WildLandsPlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSelect);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FUpdateCharacterListView);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FObjectUnderTheCursor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSpawnCharacter);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FUpdatePlayerWoodAmount);

UENUM()
enum ESelectionState {
	Null=0,
	TileSelected,
	CharacterSelected
};

UCLASS()
class AWildLandsPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AWildLandsPlayerController();


	
	// Events
	UPROPERTY(BlueprintAssignable, Category = "event")
	FSelect Select;

	UPROPERTY(BlueprintAssignable, Category = "event")
	FObjectUnderTheCursor ObjectUnderTheCursor;

	UPROPERTY(BlueprintAssignable, Category = "event")
	FSpawnCharacter SpawnCharacter;

	UPROPERTY(BlueprintAssignable, Category = "event")
	FUpdateCharacterListView UpdateCharacterListView;

	UPROPERTY(BlueprintAssignable, Category = "event")
	FUpdatePlayerWoodAmount UpdatePlayerWoodAmount;

	// UI
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class UUserWidget> HUDOverlayAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	UUserWidget* HUDOverlay;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class UUserWidget> TileWidgetBP;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	UUserWidget* TileWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class UUserWidget> CharacterWidgetBP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	UUserWidget* CharacterWidget;

	// Enums
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Variable)
	TEnumAsByte<ESelectionState> SelectionState;
	
	// Resources

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = PlayerResources)
	int WoodResource = 0;
	// Others
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FString TileNameUnderCursor;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class ARoadIndicator* RoadIndicator;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class ATile* SelectedTile;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class AWildLandsCharacter* SelectedCharacter;

	UFUNCTION()
	void TurnOnTileWidget();
	UFUNCTION()
	void TurnOffTileWidget();
	UFUNCTION()
	bool IsTileWidget();
	UFUNCTION()
	void TurnOnCharacterWidget();
	UFUNCTION()
	void TurnOffCharacterWidget();

	UFUNCTION(BlueprintCallable)
	void SpawnMyCharacter(enum ECharacterType CharacterType);
	UFUNCTION()
	TArray<ATile*> FindRoad(ATile* begining, ATile* end);
protected:

	// Begin PlayerController interface
	virtual void BeginPlay() override;
	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;
	// End PlayerController interface

};



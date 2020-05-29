// Fill out your copyright notice in the Description page of Project Settings.

#include "Engine/World.h"
#include "WildLandsGameMode.h"
#include "WildLandsPlayerController.h"
#include "Citizen.h"

// Sets default values
ACitizen::ACitizen()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACitizen::BeginPlay()
{
	Super::BeginPlay();
	MyGamemode = Cast<AWildLandsGameMode>(GetWorld()->GetAuthGameMode());
	UE_LOG(LogTemp, Warning, TEXT("BeginPlay"));
	MyPlayerController = Cast<AWildLandsPlayerController>(GetWorld()->GetFirstPlayerController());
	if (MyPlayerController != nullptr)
	{
		//MoveCharacterDelegate.BindUObject(this, &AWildLandsCharacter::BeginJourney);
	}
	
}

// Called every frame
void ACitizen::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/*/ Get current location
	auto startPos = GetActorLocation();

	// Get player's location
	FVector playerPos = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();

	// Get the direction to move in
	FVector direction = playerPos - startPos;
	direction.Normalize();

	// Move the player in that direction
	SetActorLocation(startPos + direction);*/
}

// Called to bind functionality to input
void ACitizen::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


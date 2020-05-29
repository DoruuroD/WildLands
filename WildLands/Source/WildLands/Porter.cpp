// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "Components/SkeletalMeshComponent.h"
#include "WildLandsPlayerController.h"
#include "GameFramework/Actor.h"
#include "TimerManager.h"
#include "Animation/AnimInstance.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "WildLandsGameMode.h"
#include "Porter.h"

APorter::APorter()
{
	PorterState = EPorterState::Unknown;
	// Set size for player capsule
	
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to Moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);



	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bTickEvenWhenPaused = true;
	PrimaryActorTick.TickGroup = TG_PrePhysics;
}

void APorter::Tick(float DeltaSeconds)
{

}

void APorter::BeginPlay()
{
	Super::BeginPlay();
	MyGamemode = Cast<AWildLandsGameMode>(GetWorld()->GetAuthGameMode());
	MyPlayerController = Cast<AWildLandsPlayerController>(GetWorld()->GetFirstPlayerController());
}
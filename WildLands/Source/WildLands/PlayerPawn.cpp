// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerPawn.h"
#include "Components/InputComponent.h"
#include "WildLandsGameMode.h"
#include "Engine/World.h"
#include "Grid.h"
// Sets default values
APlayerPawn::APlayerPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MovementSpeed = 20;

	//this->SetActorRot
}

// Called when the game starts or when spawned
void APlayerPawn::BeginPlay()
{
	Super::BeginPlay();
	AWildLandsGameMode* MyGamemode = Cast<AWildLandsGameMode>(GetWorld()->GetAuthGameMode());
	FVector temp = MyGamemode->GridRef->GetActorLocation();
	FVector temp2 = this->GetActorLocation();
	temp2.Y = temp.Y;
	temp2.X = temp.Y - 300.f;
	this->SetActorLocation(temp2);
	this->SetActorRotation(FQuat(0.f,0.f,0.f,0.f));
}

// Called every frame
void APlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!CurrentVelocity.IsZero())
	{
		FVector NewLocation = GetActorLocation() + (CurrentVelocity * DeltaTime);
		SetActorLocation(NewLocation);
	}
}

// Called to bind functionality to input
void APlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerPawn::Forward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerPawn::Right);
}
void APlayerPawn::Forward(float amount)
{
	// Move at 100 units per second forward or backward
	CurrentVelocity.X = (FMath::Clamp(amount, -1.0f, 1.0f) * 100.0f) * MovementSpeed;
}
void APlayerPawn::Right(float amount)
{
	// Move at 100 units per second right or left
	CurrentVelocity.Y = (FMath::Clamp(amount, -1.0f, 1.0f) * 100.0f) * MovementSpeed;
}
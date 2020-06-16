// Fill out your copyright notice in the Description page of Project Settings.


#include "MoveableOnTile.h"
#include "GameFramework/Actor.h"

// Add default functionality here for any IMoveableOnTile functions that are not pure virtual.
void IMoveableOnTile::MoveToTile(AActor* ActorToMove)
{
	/*
	FVector thisCharacterLocation = ActorToMove->GetRootComponent()RootComponent->GetComponentLocation();

	if (CharacterTile->GetActorLocation().Y < destination->GetActorLocation().Y)
	{
		thisCharacterLocation = CharacterTile->MovingRightPosition->GetComponentLocation();
	}
	else
	{
		thisCharacterLocation = CharacterTile->MovingLeftPosition->GetComponentLocation();
	}
	thisCharacterLocation.Z += 50.f;
	this->SetActorLocation(thisCharacterLocation);




	FVector temp = this->GetActorLocation();

	FVector temp2 = destination->GetActorLocation();

	temp2.Z += 100;

	FVector Heading = temp2 - temp;
	FVector temp3 = Heading;
	float Distance = Heading.Size2D();
	FVector NormalizeDirection = Heading / Distance;

	//FVector forward = this->GetActorForwardVector();
	FVector x = NormalizeDirection;
	FRotator Rotation = FRotationMatrix::MakeFromX(x).Rotator();
	//rotation.FindLookAtRotation(forward, NormalizeDirection);
	this->SetActorRotation(Rotation);
	UE_LOG(LogTemp, Warning, TEXT("this->GetActorLocation() %s"), *temp.ToString());
	UE_LOG(LogTemp, Warning, TEXT("destination->GetActorLocation() %s"), *temp2.ToString());
	UE_LOG(LogTemp, Warning, TEXT("normalizeDirection %s"), *NormalizeDirection.ToString());
	UE_LOG(LogTemp, Warning, TEXT("distance %f"), Distance);


	GetCharacterMovement()->MaxWalkSpeed = Distance / WalkingTime;
	NextTile = destination;
	IsMoving = true;*/
}
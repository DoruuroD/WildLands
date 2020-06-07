// Fill out your copyright notice in the Description page of Project Settings.


#include "Worker.h"
#include "Building.h"
#include "GameFramework/Actor.h"

void AWorker::BecomeWorker(UBuilding* Building)
{
	this->CitizenType = ECitizenType::Worker;
	this->WorkPlace = Building;
	FVector TempVector = Building->GetOwner()->GetActorLocation();
	TempVector.Z += 63.f;

	this->SetActorLocation(TempVector);
}
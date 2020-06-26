// Fill out your copyright notice in the Description page of Project Settings.


#include "Worker.h"
#include "Building.h"
#include "GameFramework/Actor.h"
#include "TimerManager.h"
#include "LumberjackHut.h"

AWorker::AWorker()
{
	this->CitizenType = ECitizenType::Worker;
}
void AWorker::BeginPlay()
{
	Super::BeginPlay();
	UpdateProgressBar.Broadcast();
}
void AWorker::BecomeWorker(UBuilding* Building)
{
	this->CitizenType = ECitizenType::Worker;
	this->WorkPlace = Building;
	FVector TempVector = Building->GetOwner()->GetActorLocation();
	TempVector.Z += 63.f;

	this->SetActorLocation(TempVector);
}

void AWorker::BeginWork()
{
	WorkerStatus = EWorkerStatus::Working;
	switch (WorkPlace->BuildingType)
	{
	case EBuildingType::Lumberjack:
	{
		ULumberjackHut* BuildingRef = Cast<ULumberjackHut>(WorkPlace);
		if (BuildingRef != nullptr)
		{
			timerRate = BuildingRef->WorkingSpeed;
			GetWorldTimerManager().SetTimer(Timer, this, &AWorker::Working, BuildingRef->WorkingSpeed, true, BuildingRef->WorkingSpeed);
		}
		else
		{
			UE_LOG(LogTemp, Fatal, TEXT("Worker BeginWork() cast to ULumberjackHut failed!"));
		}
		break;
	}
	//TODO Other states
	default:
		break;
	}
}
void AWorker::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	switch (WorkerStatus)
	{
		case EWorkerStatus::Working:
		{
			ElapsedTimeInLoop = GetWorldTimerManager().GetTimerElapsed(Timer);

			BarPercent = ElapsedTimeInLoop / timerRate;
			UpdateProgressBar.Broadcast();
				
			
			break;
		}
	}
}
void AWorker::StopWork()
{
	WorkerStatus = EWorkerStatus::WaitingForWork;
	BarPercent = 0;
	UpdateProgressBar.Broadcast();
}
void AWorker::Working()
{
	WorkPlace->GenerateResource();
}
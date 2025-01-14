// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TileMesh.h"
#include "Resource.generated.h"


UENUM(BlueprintType)
enum EResourceType {
	Forest= 0,
	StoneDeposit,
	Meadow,
	UnknownResource
};
USTRUCT(BlueprintType)
struct FResourceData 
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ResourceData")
	TEnumAsByte<EResourceType> ResourceType;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ResourceData")
	int ResourceAmount;
};
UCLASS()
class WILDLANDS_API UResource : public UTileMesh
{

	GENERATED_BODY()
public:
	// Sets default values for this component's properties
	UResource();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ResourceData")
	FResourceData ResourceData;
	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Variable)
//	TEnumAsByte<EResourceType> ResourceType = EResourceType::UnknownResource;
	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Variable)
	//int ResourceAmount = 20;
};

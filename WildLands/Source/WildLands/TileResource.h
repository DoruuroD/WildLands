// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TileResource.generated.h"

UENUM(BlueprintType)
enum EResourceType {
	Woods = 0,
	Stone,
	Unknown
};
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WILDLANDS_API UTileResource : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTileResource();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Variable)
	TEnumAsByte<EResourceType> ResourceType = Unknown;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Variable)
	class UStaticMeshComponent* ResourceMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Variable)
	int ResourceAmount = 1;
};

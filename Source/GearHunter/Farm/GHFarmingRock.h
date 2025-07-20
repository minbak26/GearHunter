// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Farm/GHFarmingBase.h"
#include "GHFarmingRock.generated.h"

/**
 * 
 */
UCLASS()
class GEARHUNTER_API AGHFarmingRock : public AGHFarmingBase
{
	GENERATED_BODY()
public:
	AGHFarmingRock();
	
	UPROPERTY(BlueprintReadWrite, Category = "Farming")
	UStaticMeshComponent* RockMesh;
	
	
};

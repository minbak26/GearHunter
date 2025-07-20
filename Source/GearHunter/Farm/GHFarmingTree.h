// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Farm/GHFarmingBase.h"
#include "GHFarmingTree.generated.h"

/**
 * 
 */
UCLASS()
class GEARHUNTER_API AGHFarmingTree : public AGHFarmingBase
{
	GENERATED_BODY()
public:
	AGHFarmingTree();

	UPROPERTY(BlueprintReadWrite, Category = "Farming")
	UStaticMeshComponent* TreeMesh;
};

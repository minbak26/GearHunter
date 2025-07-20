// Fill out your copyright notice in the Description page of Project Settings.


#include "GHFarmingRock.h"

#include "MovieSceneTracksComponentTypes.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"

AGHFarmingRock::AGHFarmingRock()
{
	 // RockMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RockMesh"));
	 // RockMesh->SetupAttachment(RootComponent);
	// PickupCollision->SetupAttachment(RockMesh);

	ItemID = "Stone";
	ItemCount = 1;
}

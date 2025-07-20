// Fill out your copyright notice in the Description page of Project Settings.


#include "Farm/GHFarmingTree.h"

#include "Components/SphereComponent.h"

AGHFarmingTree::AGHFarmingTree()
{
	// TreeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RockMesh"));
	// TreeMesh->SetupAttachment(RootComponent);
	// PickupCollision->SetupAttachment(TreeMesh);
	ItemID = "Wood";
	ItemCount = 1;
}

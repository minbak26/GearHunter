// Fill out your copyright notice in the Description page of Project Settings.


#include "Gimmick/GHPotal.h"

#include "Character/GHCharacterPlayer.h"

// Sets default values
AGHPotal::AGHPotal()
{
	PotalStaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	RootComponent = PotalStaticMeshComponent;

	PotalEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("PotalEffect"));
	PotalEffect->SetupAttachment(PotalStaticMeshComponent);

	
	
	PotalMesh = nullptr;
	
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGHPotal::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGHPotal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


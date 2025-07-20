// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "Components/StaticMeshComponent.h"
#include "GHPotal.generated.h"

UCLASS()
class GEARHUNTER_API AGHPotal : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGHPotal();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Gimmick")
	TObjectPtr<UStaticMeshComponent> PotalStaticMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Gimmick")
	TObjectPtr<UStaticMesh> PotalMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Gimmick")
	TObjectPtr<UNiagaraComponent> PotalEffect;
};

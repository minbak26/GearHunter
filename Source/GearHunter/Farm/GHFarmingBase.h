// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GHFarmingBase.generated.h"


class UWidgetComponent;

UCLASS()
class GEARHUNTER_API AGHFarmingBase : public AActor
{
	GENERATED_BODY()
	
public:	
	
	// 여기에 매시 가져오고
	AGHFarmingBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "prop")
	UStaticMeshComponent* PropComponent;

	// pick up section.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Collision")
	TObjectPtr<class USphereComponent> PickupCollision;

	// 오버랩 조건 확인 함수.
	UFUNCTION()
	void IsOverlapPropStart(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
				  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
				  bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void IsOverlapPropEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:


	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	TObjectPtr<UWidgetComponent> PickupWidget;
	
public:
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	FName ItemID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	int32 ItemCount;

	

};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GHAIController.generated.h"


/**
 * 
 */
UCLASS()
class GEARHUNTER_API AGHAIController : public AAIController
{
	GENERATED_BODY()
public:
	AGHAIController();

	void RunAI();
	void StopAI();

	protected:
	virtual void OnPossess(APawn* InPawn) override ;

private:
	UPROPERTY()
	TObjectPtr<UBlackboardData> BBAsset;

	UPROPERTY()
	TObjectPtr<UBehaviorTree> BTAsset; 
};

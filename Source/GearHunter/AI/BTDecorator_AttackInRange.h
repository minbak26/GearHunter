// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_AttackInRange.generated.h"

/**
 * 
 */
UCLASS()
class GEARHUNTER_API UBTDecorator_AttackInRange : public UBTDecorator
{
	GENERATED_BODY()
	UBTDecorator_AttackInRange();
protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};

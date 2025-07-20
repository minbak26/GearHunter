// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/GHCharacterBase.h"
#include "Engine/StreamableManager.h"
#include "Interface/GHCharacterAIInterface.h"
#include "GHCharacterNonPlayer.generated.h"

/**
 * 
 */
UCLASS()
class GEARHUNTER_API AGHCharacterNonPlayer : public AGHCharacterBase, public IGHCharacterAIInterface
{
	GENERATED_BODY()
public:
	AGHCharacterNonPlayer();

protected:
	virtual void SetDead() override;
	void NPCMeshLoadCompleted();

	UPROPERTY(config)
	TArray<FSoftObjectPath> NPCMeshes;

	TSharedPtr<FStreamableHandle> NPCMeshHandle;


	// AI Section
protected:
	virtual float GetAIPatrolRadius() override;
	virtual float GetAIDetectRange() override;
	virtual float GetAIAttackRange() override;
	virtual float GetAITurnSpeed() override;

	virtual void SetAIAttackDelegate(const FAICharacterAttackFinished& InOnAttackFinished) override;
	virtual void AttackByAI() override;

	FAICharacterAttackFinished OnAttackFinished;

	virtual void NotifyComboActionEnd();
	
};

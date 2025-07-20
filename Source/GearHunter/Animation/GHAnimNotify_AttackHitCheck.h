// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "GHAnimNotify_AttackHitCheck.generated.h"

/**
 * 
 */
UCLASS()
class GEARHUNTER_API UGHAnimNotify_AttackHitCheck : public UAnimNotify
{
	GENERATED_BODY()
public:
	UGHAnimNotify_AttackHitCheck();

protected:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};

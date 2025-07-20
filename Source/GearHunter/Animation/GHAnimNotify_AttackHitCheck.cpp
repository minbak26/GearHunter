// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/GHAnimNotify_AttackHitCheck.h"
#include "Character/GHCharacterBase.h"

UGHAnimNotify_AttackHitCheck::UGHAnimNotify_AttackHitCheck()
{
	
	
}

void UGHAnimNotify_AttackHitCheck::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{

	
	
	Super::Notify(MeshComp, Animation, EventReference);
	if (MeshComp)
	{
		IGHAnimationAttackInterface* AttackPawn = Cast<IGHAnimationAttackInterface> (MeshComp->GetOwner());
		if (AttackPawn)
		{
			AttackPawn->AttackHitCheck();
		}
		
	}
}

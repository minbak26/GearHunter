// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStat/GHCharacterStatComponent.h"

// Sets default values for this component's properties
UGHCharacterStatComponent::UGHCharacterStatComponent()
{
	MaxHp = 200.0f;
	CurrentHp = MaxHp;
}


// Called when the game starts
void UGHCharacterStatComponent::BeginPlay()
{
	Super::BeginPlay();

	SetHp(MaxHp);
	
}

void UGHCharacterStatComponent::SetHp(float NewHp)
{
	CurrentHp = FMath::Clamp<float>(NewHp, 0.0f, MaxHp);
	OnHpChanged.Broadcast(CurrentHp);
}


float UGHCharacterStatComponent::ApplyDamage(float InDamage)
{
	const float PrevHp = CurrentHp;
	const float ActualDamage = FMath::Clamp(InDamage, 0, InDamage);
	SetHp(FMath::Clamp<float>(PrevHp - ActualDamage, 0.0f, MaxHp));
	if (CurrentHp <= KINDA_SMALL_NUMBER)
	{
		OnHpZero.Broadcast();
	}
	return ActualDamage;
}







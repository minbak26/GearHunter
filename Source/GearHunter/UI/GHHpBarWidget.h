// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GHUserWidget.h"
#include "Blueprint/UserWidget.h"
#include "GHUserWidget.h"
#include "GHHpBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class GEARHUNTER_API UGHHpBarWidget : public UGHUserWidget 
{
	GENERATED_BODY()
public:
	UGHHpBarWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;
	
public:
	FORCEINLINE float SetMaxHp(float NewMaxHp) { return NewMaxHp; }
	void UpgradeHpBar(float NewCurrentHp);
	
protected:
	UPROPERTY(EditAnywhere)
	TObjectPtr<class UProgressBar> HpProgressBar;
	
	UPROPERTY(EditAnywhere)
	float MaxHp;
};

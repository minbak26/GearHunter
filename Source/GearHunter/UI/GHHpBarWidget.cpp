// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GHHpBarWidget.h"

#include "Components/ProgressBar.h"
#include "Interface/GHCharacterWidgetInterface.h"


UGHHpBarWidget::UGHHpBarWidget(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{
	
	MaxHp = 200.0f;
}

void UGHHpBarWidget::NativeConstruct()
{
	Super::NativeConstruct();
	HpProgressBar = Cast<UProgressBar>(GetWidgetFromName("PbHpBar"));
	if (HpProgressBar)
	{
		// 초기화 시점에 Visible로 설정
		HpProgressBar->SetVisibility(ESlateVisibility::Visible);
	}

	ensure(HpProgressBar);

	IGHCharacterWidgetInterface* CharacterWidget = Cast<IGHCharacterWidgetInterface>(OwingActor);
	if (CharacterWidget)
	{
		CharacterWidget->SetupCharacterWidget(this);
	}

	
}

void UGHHpBarWidget::UpgradeHpBar(float NewCurrentHp)
{
	ensure(MaxHp > 0.0f);
	if (HpProgressBar)
	{
		// 강제로 바인딩 해제 후 설정
		HpProgressBar->SetPercent(0.0f);
		
		float Percent = NewCurrentHp / MaxHp;
		HpProgressBar->SetPercent(Percent);

		// 강제 새로고침
		HpProgressBar->SynchronizeProperties();
        
		UE_LOG(LogTemp, Warning, TEXT("HP Updated: Current=%f, Max=%f, Percent=%f"), 
			NewCurrentHp, MaxHp, Percent);
            
		// 추가 디버깅
		UE_LOG(LogTemp, Warning, TEXT("ProgressBar IsValid: %s, Visibility: %d"), 
			IsValid(HpProgressBar) ? TEXT("True") : TEXT("False"),
			(int32)HpProgressBar->GetVisibility());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("HpProgressBar is NULL!"));
	}
}

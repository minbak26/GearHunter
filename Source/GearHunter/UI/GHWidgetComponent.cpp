// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GHWidgetComponent.h"
#include "GHUserWidget.h"

void UGHWidgetComponent::InitWidget()
{
	Super::InitWidget();

	UGHUserWidget* GHUserWidget = Cast<UGHUserWidget>(GetWidget());
	if (GHUserWidget)
	{
		GHUserWidget->SetOwingActor(GetOwner());
	}
}

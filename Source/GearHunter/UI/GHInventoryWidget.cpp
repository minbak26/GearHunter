// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GHInventoryWidget.h"

void UGHInventoryWidget::UpdateSlot(FName ItemID, int32 ItemCount)
{
	if (ItemID == FName(TEXT("Wood")) && Slot1Count)
	{
		Slot1Count->SetText(FText::AsNumber(ItemCount));
	}
	else if (ItemID == FName(TEXT("Stone")) && Slot2Count)
	{
		Slot2Count->SetText(FText::AsNumber(ItemCount));
	}
}

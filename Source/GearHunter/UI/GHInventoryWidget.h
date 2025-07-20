// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

// 텍스트 박스 접근.
#include "Components/TextBlock.h"
#include "GHInventoryWidget.generated.h"

struct FInventoryItem;
/**
 * 
 */
UCLASS()
class GEARHUNTER_API UGHInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// 블루프린트에서 변수로 바인딩한 텍스트박스
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Slot1Count;

	

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Slot2Count;

	// 인벤토리 전체 갱신 함수 (블루프린트에서 구현 가능)
	UFUNCTION(BlueprintImplementableEvent, Category="Inventory")
	void UpdateInventory(const TArray<FInventoryItem>& InventoryItems);

	// C++에서 직접 텍스트 업데이트하는 함수 예시
	UFUNCTION(BlueprintCallable, Category="Inventory")
	void UpdateSlot(FName ItemID, int32 ItemCount);
	
};

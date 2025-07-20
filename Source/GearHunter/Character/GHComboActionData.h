// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GHComboActionData.generated.h"

/**
 * 
 */
UCLASS()
class GEARHUNTER_API UGHComboActionData : public UDataAsset
{
	GENERATED_BODY()
public:
	UGHComboActionData();

	// 4가지 속성.
	UPROPERTY(EditAnywhere, Category="Name")
	FString MontageSectionName;

	UPROPERTY(EditAnywhere, Category="Name")
	uint8 MaxComboCount;

	// 기준 프레임 속도 지정
	UPROPERTY(EditAnywhere, Category="Name")
	float FrameRate;

	// 사전에 입력된 프레임을 지정
	UPROPERTY(EditAnywhere, Category="Name")
	TArray<float> EffectiveFrameCount;
};

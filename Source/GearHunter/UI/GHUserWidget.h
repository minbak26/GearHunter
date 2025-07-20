// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GHUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class GEARHUNTER_API UGHUserWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	FORCEINLINE void SetOwingActor(AActor* NewOwner) { OwingActor = NewOwner; }
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Actor")
	TObjectPtr<AActor> OwingActor;
	
	
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "GHAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class GEARHUNTER_API UGHAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	UGHAnimInstance();

protected:
	virtual void NativeInitializeAnimation() override;
	
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	TObjectPtr<class ACharacter> Owner;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	TObjectPtr<class UCharacterMovementComponent> Movement;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	FVector Velocity;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	float GroundSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsIdle : 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	float MovingThreshould;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsFalling : 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsJumping : 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	float JumpingThreshould;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	uint8 IsRolling : 1;


	
	
	
};

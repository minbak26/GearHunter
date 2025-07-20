// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameData/GHDataAsset.h"
#include "GameFramework/Character.h"
#include "Interface/GHAnimationAttackInterface.h"
#include "Interface/GHCharacterWidgetInterface.h"
#include "GHCharacterBase.generated.h"

class AGHFarmingBase;
// 2. WeaponType 만들기. (컨트롤러 -> 캐릭터베이스) 캐릭터가
// 무기를 attach 할때 속성값을 가지면 캐릭터를 불러와서 매쉬에서
// 접근하면 알수 있지 않을까

UCLASS()
class GEARHUNTER_API AGHCharacterBase : public ACharacter, public IGHAnimationAttackInterface, public IGHCharacterWidgetInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AGHCharacterBase();

	virtual void PostInitializeComponents() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void AttachWeapon();


	// 공격 애니메이션 몽타주.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* AttackMontage;



// Attack Hit Section
protected:
	UFUNCTION()
	virtual void AttackHitCheck() override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCursor ) override;
	

	//Death Section.
protected:
	UPROPERTY(EditAnywhere, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> DeadActionMontage;

	virtual void SetDead();
	void PlayDeadAnimation();

	
	
	

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat ,meta=(AllowPrivateAccess=true))
	TObjectPtr<class UGHCharacterStatComponent> Stat;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Widget, meta=(AllowPrivateAccess=true))
	TObjectPtr<class UGHWidgetComponent> HpBar;

	virtual void SetupCharacterWidget(UGHUserWidget* InUserWidget) override;
	
};

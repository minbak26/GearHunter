// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/GHCharacterBase.h"
#include "Game/GHPlayerController.h"
#include "GameFramework/Actor.h"
#include "GHWeaponBase.generated.h"

// 무기 스탯 관리를 위한 구조체
USTRUCT()
struct FWeaponStat
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "WeaponStat")
	float WeaponDamage = 10.f;
	UPROPERTY(EditAnywhere, Category = "WeaponStat")
	float WeaponRange = 10.f;
	UPROPERTY(EditAnywhere, Category = "WeaponStat")
	float WeaponSpeed = 1.f;

	//쿨타임 감소량
	UPROPERTY(EditAnywhere, Category = "WeaponStat")
	float QSKillReduction = 1.0f;
	UPROPERTY(EditAnywhere, Category = "WeaponStat")
	float WSKillReduction = 1.0f;

	// 강화 레벨.
	int32 EnforceLevel = 1;
};


UCLASS()
class GEARHUNTER_API AGHWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// 생성자.
	AGHWeaponBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<UStaticMeshComponent> MeshComp;
	
	// 메시
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	TObjectPtr<class USkeletalMeshComponent> WeaponSkeletal;

	// 소켓 위치를 알아야함.
	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	FName AttachSocketName;
	
	UPROPERTY(VisibleDefaultsOnly, Category = "Animation")
	TObjectPtr<class UAnimMontage> ComboAttack;
public:
	// 무기 종류
	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	TArray<AGHWeaponBase* > Weapons;

	// 기본스탯.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float Damage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float AttackSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float Range;

	// 컨트롤러의 DT데이터 가져오기.
	UFUNCTION()
	void SetWeaponStat(const FWeaponStatsData& Stats);

	void UpdateWeaponStats();
};

  


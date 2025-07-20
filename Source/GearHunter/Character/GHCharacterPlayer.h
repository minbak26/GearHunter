// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/GHCharacterBase.h"
#include "Weapon/GHWeaponBase.h"
#include "GHCharacterPlayer.generated.h"

/**
 * 
 */
UCLASS()
class GEARHUNTER_API AGHCharacterPlayer : public AGHCharacterBase
{
	GENERATED_BODY()
public:
	AGHCharacterPlayer();

	virtual void Tick(float DeltaTime) override;

	virtual void BeginPlay() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	
	
protected:
	// Camera Section.
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> Camera;

	
public:
	// 무기 부착 함수.
	UFUNCTION()
	virtual void AttachWeapon() override;


	UPROPERTY(EditAnywhere, Category = "prop")
	AGHWeaponBase* WeaponBase;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* ComboActionMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* RollingActionMontage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* QSkillActionMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* WSkillActionMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* CraftingActionMontage;
	// ComboAction Section.
public:
	
	// 콤보 처리시 사용할 데이터 애셋.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attack, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UGHComboActionData> ComboActionData;
	
	// 공격 몽타주 재생 함수.
	void ProcessComboCommand();
	void ComboActionBegin();
	void ComboActionEnd(class UAnimMontage* Montage, bool IsProperlyEnded);
	// 다음섹션으로 넘어가기.
	void SetComboCheckTimer();
	void ComboCheck();
	// 구르기 
	void RollingCommand();
	//Q 스킬
	void TryCastQSkill();
	// W 스킬
	void TryCastWSkill();
	// 크래프팅
	void TryCrafting();
	// 콤보 확인을 위한 변수.
	int32 CurrentCombo = 0;

	FTimerHandle ComboTimerHandle;
	bool HasNextCombo = false ;

public:
	UPROPERTY(EditAnywhere, Category = "Skill")
	TSubclassOf<class AGHWeaponSwordFire> WeaponShoot;

public:
	//Rolling Section.
	UPROPERTY(EditAnywhere, Category = "Stat")
	bool bIsRolling = false;

	UPROPERTY(EditAnywhere, Category = "Stat")
	bool bIsQSkill = false;
	
	UPROPERTY(EditAnywhere, Category = "Stat")
	bool bIsWSkill = false;
	// 몽타주 종료 콜백 함수.
	void OnRollingMontageEnded(UAnimMontage* Montage, bool bInterrupted);

private:
	// 입력차단
	UPROPERTY(EditAnywhere, Category = "Stat")
	bool bCanReceiveinput = true;

	// CoolTime Section.
private:
	UPROPERTY(BlueprintReadWrite, Category = "Stat", meta = (AllowPrivateAccess = "true"))
	bool bIsActiving = false;

	UPROPERTY(BlueprintReadWrite, Category = "Stat", meta = (AllowPrivateAccess = "true"))
	bool bCanActive = true;


	// 어떤액션인지 확인하는 불변수.
	bool bIsPerformingAction = false;
public:
	UPROPERTY(BlueprintReadWrite, Category = "Stat", meta = (AllowPrivateAccess = "true"))
	float RollingCoolTime = 3.0f;

	UPROPERTY(BlueprintReadWrite, Category = "Stat", meta = (AllowPrivateAccess = "true"))
	float QSkillCoolTime = 5.0f;

	UPROPERTY(BlueprintReadWrite, Category = "Stat", meta = (AllowPrivateAccess = "true"))
	float WSkillCoolTime = 10.0f;

	FTimerHandle RoolingCoolTimerHandle;
	FTimerHandle QSkillCoolTimerHandle;
	FTimerHandle WSkillCoolTimerHandle;

public:
	
	UFUNCTION()
	void ResetSkillCoolTimeDown(const FString& SkillName);

	//Getter 함수.
	FORCEINLINE bool GetCanActive() const { return bCanActive; }
	FORCEINLINE bool GetIsActiving() const { return bIsActiving; }
	FORCEINLINE bool GetCanReceiveInput() const { return bCanReceiveinput; }
	
	// inventory 플레이어가 포착한 아이템 확인 변수
	// 현재 포착된 재료 아이템
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	TObjectPtr<AGHFarmingBase> CurrentOverlapItem;

	
	
	// DT로부터 받은 새로운 스탯 부여(Q스킬, W스킬 쿨타임)
	UFUNCTION()
	void NewQSkillCoolTimeSet(const FWeaponStatsData& CharacterStat);
	UFUNCTION()
	void NewWSkillCoolTimeSet(const FWeaponStatsData& CharacterStat);
	
};

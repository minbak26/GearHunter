#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "WeaponData.generated.h"

// 무기 강화 테이블.
USTRUCT(BlueprintType)
struct GEARHUNTER_API FWeaponUpgradeData : public FTableRowBase
{
	GENERATED_BODY()

	// 필요한 목재수.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 RequiredWood=0;
	// 필요한 돌 개수.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 RequiredStone=0;

	// 성공확률.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SuccessRate =0.0f;
	
};

// 무기 강화시 부여되는 스테이트 테이블.
USTRUCT(BlueprintType)
struct GEARHUNTER_API FWeaponStatsData : public FTableRowBase
{
	GENERATED_BODY()
	//강화단계
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 EnforceLevel =0;
	
	// 대미지.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Damage =0.0f;

	//공격 속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackSpeed=0.0f;

	// 공격 범위.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Range=0.0f;

	// Q스킬 쿨다운.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float QSkillCooldown=0.0f;

	// W스킬 쿨다운.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WSKillCooldown=0.0f;
};

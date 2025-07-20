// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/GHWeaponBase.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "GHWeaponSwordFire.generated.h"

/**
 * 
 */
UCLASS()
class GEARHUNTER_API AGHWeaponSwordFire : public AGHWeaponBase
{
	GENERATED_BODY()

	AGHWeaponSwordFire();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Projectile)
	TObjectPtr<UStaticMeshComponent> ProjectileMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USphereComponent> SphereComp;

	
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Niagara", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UNiagaraComponent> SwordEffect;
	
	// 스킬 대미지
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat", meta = (AllowPrivateAccess = "true"))
	float QSKillDamage = 10.f;

	
	virtual void BeginPlay() override;
public:
	void FireInDirection(const FVector& ShootDirection);

	UFUNCTION()
	void OnOverlapBegin( UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult);

	
	// 충돌 처리
	void HitQSkill(AActor* OtherActor);
	
};

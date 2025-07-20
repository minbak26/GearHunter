// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/GHWeaponBase.h"

#include "Character/GHCharacterBase.h"
#include "Database/WeaponData.h"
#include "DataWrappers/ChaosVDParticleDataWrapper.h"
#include "Game/GHPlayerController.h"
#include "Misc/MapErrors.h"
#include "Rendering/RenderCommandPipes.h"



// Sets default values
AGHWeaponBase::AGHWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));

	RootComponent = MeshComp;
	
	// 콜리전 세팅값 가져오기.
	MeshComp->SetCollisionProfileName(TEXT("Weapon"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> WeaponBase (TEXT("/Game/Weapons_1/Meshes/SM_katana_001.SM_katana_001"));

	if (WeaponBase.Object)
	{
		MeshComp->SetStaticMesh(WeaponBase.Object);
	}


	
}

// Called when the game starts or when spawned
void AGHWeaponBase::BeginPlay()
{
	Super::BeginPlay();

	
	
	
}

// Called every frame
void AGHWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
}


void AGHWeaponBase::SetWeaponStat(const FWeaponStatsData& WeaponStat)
{
	Damage = WeaponStat.Damage;	
	AttackSpeed = WeaponStat.AttackSpeed;
	Range = WeaponStat.Range;
	
}

void AGHWeaponBase::UpdateWeaponStats()
{
	// 무기의 소유자(캐릭터)를 통해 컨트롤러 접근
	if (AActor* MyOwner = GetOwner())
	{
		if (APawn* OwnerPawn = Cast<APawn>(MyOwner))
		{
			if (AGHPlayerController* PC = Cast<AGHPlayerController>(OwnerPawn->GetController()))
			{
				// 컨트롤러에서 스탯 가져오기
				FWeaponStatsData WeaponStat = PC->GetState();
                
				// 자신의 SetWeaponStat 호출
				SetWeaponStat(WeaponStat);
                
				UE_LOG(LogTemp, Log, TEXT("Weapon stats updated: Damage=%f, AttackSpeed=%f"), 
					   Damage, AttackSpeed);
			}
		}
	}
}








// Fill out your copyright notice in the Description page of Project Settings.


#include "GHWeaponSwordFire.h"

#include "Character/GHCharacterPlayer.h"
#include "Kismet/GameplayStatics.h"

AGHWeaponSwordFire::AGHWeaponSwordFire()
{
	// 이 액터가 프레임마다 Tick()을 호출하도록 설정합니다.
	PrimaryActorTick.bCanEverTick = true;
 
	// 1. 먼저 콜리전 컴포넌트 생성 및 루트로 설정
	if (!SphereComp)
	{
		SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
		SphereComp->InitSphereRadius(50.0f);
		SphereComp->SetCollisionProfileName(TEXT("Weapon"));
		RootComponent = SphereComp;
	}

	// 2. 메시 컴포넌트 생성 (콜리전 컴포넌트 생성 후)
	if (!ProjectileMeshComponent)
	{
		ProjectileMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMeshComponent"));
        
		// 메시 설정
		static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMesh(TEXT("/Script/Engine.StaticMesh'/Game/Weapons_1/Meshes/SM_boomerang_001.SM_boomerang_001'"));
		if (SphereMesh.Succeeded())
		{
			ProjectileMeshComponent->SetStaticMesh(SphereMesh.Object);
		}
  
		// 루트 컴포넌트(SphereComp)에 부착
		ProjectileMeshComponent->SetupAttachment(RootComponent);
	}

	// 3. 프로젝타일 무브먼트 컴포넌트 생성
	if (!ProjectileMovement)
	{
		ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
		ProjectileMovement->SetUpdatedComponent(SphereComp);
		ProjectileMovement->InitialSpeed = 1500.0f;
		ProjectileMovement->MaxSpeed = 3000.0f;
		ProjectileMovement->bRotationFollowsVelocity = true;
		ProjectileMovement->bShouldBounce = true;
		ProjectileMovement->Bounciness = 0.3f;
		ProjectileMovement->ProjectileGravityScale = 0.0f;
	}

	// 나이아가라
	SwordEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("SwordEffect"));
	SwordEffect->SetupAttachment(RootComponent);
}


void AGHWeaponSwordFire::BeginPlay()
{
	Super::BeginPlay();
	if (SphereComp)
	{
		SphereComp->OnComponentBeginOverlap.AddDynamic(this, &AGHWeaponSwordFire::OnOverlapBegin);
	}
}

void AGHWeaponSwordFire::FireInDirection(const FVector& ShootDirection)
{
	ProjectileMovement->Velocity = ShootDirection * ProjectileMovement->InitialSpeed;
	
}

void AGHWeaponSwordFire::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	HitQSkill(OtherActor);
}



void AGHWeaponSwordFire::HitQSkill(AActor* OtherActor)
{
	if (OtherActor && OtherActor != this)
	{
		// 플레이어 캐릭터(혹은 데미지를 받을 수 있는 캐릭터)인지 확인
		AGHCharacterBase* Player = Cast<AGHCharacterBase>(OtherActor);
		if (Player)
		{
			float DamageAmount = 30.0f; // 원하는 데미지 값
			UGameplayStatics::ApplyDamage(
				Player,                      // 데미지 받을 액터
				DamageAmount,                // 데미지 양
				GetInstigatorController(),   // 공격자 컨트롤러
				this,                        // 데미지 원인(발사체 자신)
				nullptr                      // 데미지 타입 (기본)
			);

			Destroy(); // 발사체 삭제
		}
	}
	
}

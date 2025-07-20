// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/GHCharacterBase.h"

#include "GHComboActionData.h"
#include "CharacterStat/GHCharacterStatComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "Database/WeaponData.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Weapon/GHWeaponBase.h"
#include "Physics/GHCollision.h"
#include "UI/GHWidgetComponent.h"
#include "Weapon/GHWeaponSwordFire.h"
#include "UI/GHHpBarWidget.h"


// Sets default values
AGHCharacterBase::AGHCharacterBase()
{
	
	


	// 컨트롤러 회전을 받는 설정 해제.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// 캡슐 컴포넌트 설정.
	GetCapsuleComponent()->SetCapsuleHalfHeight(88.0f);

	GetMesh()->SetRelativeLocationAndRotation(
		FVector(0.0f, 0.0f, -88.0f),
		FRotator(0.0f, -90.0f, 0.0f)
	);

	
	// 리소스 설정.
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Stylized_Scifi_Girl/Meshes/SK_Scifi_Girl.SK_Scifi_Girl'"));
	if (CharacterMesh.Object)
	{
		GetMesh()->SetSkeletalMesh(CharacterMesh.Object);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> CharacterAnimInstance(TEXT("/Game/Stylized_Scifi_Girl/Animations/NewAnimBlueprint.NewAnimBlueprint_C"));
	if (CharacterAnimInstance.Class)
	{
		GetMesh()->SetAnimInstanceClass(CharacterAnimInstance.Class);
	}

	
	// 캐릭터 무브먼트 컴포넌트 설정 추가
	GetCharacterMovement()->bOrientRotationToMovement = true; // 이동 방향으로 회전
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 600.0f, 0.0f); // 회전 속도
	GetCharacterMovement()->MaxWalkSpeed = 500.f; // 이동 속도
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->SetPlaneConstraintNormal(FVector(0.0f, 0.0f, 1.0f)); 

 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	Stat = CreateDefaultSubobject<UGHCharacterStatComponent>(TEXT("Stat"));
	HpBar = CreateDefaultSubobject<UGHWidgetComponent>(TEXT("HpBar"));
	HpBar->SetupAttachment(GetMesh());
	HpBar->SetRelativeLocation(FVector(0.0f, 0.0f, 180.0f));
	static ConstructorHelpers::FClassFinder<UUserWidget> HpBarWidget(TEXT("/Game/UI/WBP_Hpbar.WBP_Hpbar_C"));
	if (HpBarWidget.Class)
	{
		HpBar->SetWidgetClass(HpBarWidget.Class);
		HpBar->SetWidgetSpace(EWidgetSpace::Screen);
		HpBar->SetDrawSize(FVector2D(150.0f, 15.0f));
		HpBar->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	

	
}

void AGHCharacterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	Stat->OnHpZero.AddUObject(this, &AGHCharacterBase::SetDead);
}

// Called when the game starts or when spawned
void AGHCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	AttachWeapon();

	
}

// Called every frame
void AGHCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGHCharacterBase::AttachWeapon()
{
	

	
}






void AGHCharacterBase::AttackHitCheck()
{
	UE_LOG(LogTemp, Log, TEXT("AttackHitcheckCall"));
	FHitResult OutHitResult;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(Attack), false, this);

	const float AttackRange = 40.0f;
	const float AttackRadius = 50.0f;
	const float AttackDamage = 30.0f;
	const FVector Start = GetActorLocation() + GetActorForwardVector() * GetCapsuleComponent()->GetScaledCapsuleRadius();
	const FVector End = Start + GetActorForwardVector() * AttackRange;

	bool HitDected = GetWorld()->SweepSingleByChannel(OutHitResult, Start, End, FQuat::Identity, CCHANNEL_GHACTION, FCollisionShape::MakeSphere(AttackRadius), Params);
	if (HitDected)
	{
		FDamageEvent DamageEvent;
		OutHitResult.GetActor()->TakeDamage(AttackDamage, DamageEvent, GetController(), this);
		
		
	}

#if ENABLE_DRAW_DEBUG

	FVector CapsuleOrigin = Start + (End - Start) * 0.5f;
	float CapsuleHalfHeight = AttackRange * 0.5f;
	FColor DrawColor = HitDected ? FColor::Green : FColor::Yellow;

	DrawDebugCapsule(GetWorld(), CapsuleOrigin, CapsuleHalfHeight, AttackRadius, FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat(), DrawColor, false, 5.0f);


#endif
	
}

float AGHCharacterBase::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, AActor* DamageCursor)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCursor);

	Stat->ApplyDamage(DamageAmount); 
	
	return DamageAmount;
}

void AGHCharacterBase::SetDead()
{
	UE_LOG(LogTemp, Log, TEXT("AGHCharacterBase::SetDead"));
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	PlayDeadAnimation();
	SetActorEnableCollision(false);
	HpBar->SetHiddenInGame(true);
	
}

void AGHCharacterBase::PlayDeadAnimation()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->StopAllMontages(0.0f);
	AnimInstance->Montage_Play(DeadActionMontage, 1.0f);
	Destroy();
}

void AGHCharacterBase::SetupCharacterWidget(UGHUserWidget* InUserWidget)
{
	UGHHpBarWidget*	HpBarWidget = Cast<UGHHpBarWidget>(InUserWidget);
	if (HpBarWidget)
	{
		HpBarWidget->SetMaxHp(Stat->GetMaxHp());
		HpBarWidget->UpgradeHpBar(Stat->GetCurrentHp());
		Stat->OnHpChanged.AddUObject(HpBarWidget, &UGHHpBarWidget::UpgradeHpBar);
	}
}



















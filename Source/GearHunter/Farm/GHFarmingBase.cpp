// Fill out your copyright notice in the Description page of Project Settings.


#include "Farm/GHFarmingBase.h"

#include "Character/GHCharacterBase.h"
#include "Character/GHCharacterPlayer.h"
#include "CharacterStat/GHCharacterStatComponent.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Physics/GHCollision.h"


AGHFarmingBase::AGHFarmingBase()
{
	// 1. RootComponent 초기화
	PropComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PropComponent"));
	RootComponent = PropComponent;

	// 2. PickupCollision 설정
	PickupCollision = CreateDefaultSubobject<USphereComponent>(TEXT("PickupCollision"));
	PickupCollision->SetupAttachment(RootComponent);
	PickupCollision->SetSphereRadius(200.f);
	PickupCollision->SetCollisionProfileName(TEXT("PropType"));
	PickupCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly); // 명시적 활성화

	

	// 4. UI 컴포넌트 설정
	PickupWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("PickupWidget"));
	PickupWidget->SetupAttachment(RootComponent);
	PickupWidget->SetRelativeLocation(FVector(0, 0, 100)); // 위치 조정

	static ConstructorHelpers::FClassFinder<UUserWidget> PickupWidgetRef(TEXT("/Game/Prop/BP_PropOverLapUI.BP_PropOverLapUI_C"));
	if (PickupWidgetRef.Succeeded())
	{
		PickupWidget->SetWidgetClass(PickupWidgetRef.Class);
		PickupWidget->SetVisibility(false);
		PickupWidget->SetDrawSize(FVector2D(300, 100));
		PickupWidget->SetWidgetSpace(EWidgetSpace::Screen); // Screen으로 변경
	}

	PrimaryActorTick.bCanEverTick = true;








	
}

// Called when the game starts or when spawned
void AGHFarmingBase::BeginPlay()
{
	Super::BeginPlay();
	
	PickupCollision->OnComponentBeginOverlap.RemoveDynamic(this, &AGHFarmingBase::IsOverlapPropStart);
	PickupCollision->OnComponentEndOverlap.RemoveDynamic(this, &AGHFarmingBase::IsOverlapPropEnd);
	// 3. 이벤트 바인딩
	PickupCollision->OnComponentBeginOverlap.AddDynamic(this, &AGHFarmingBase::IsOverlapPropStart);
	PickupCollision->OnComponentEndOverlap.AddDynamic(this, &AGHFarmingBase::IsOverlapPropEnd);
}

// Called every frame
void AGHFarmingBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGHFarmingBase::IsOverlapPropStart(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->IsA(AGHCharacterPlayer::StaticClass()))
	{
		UE_LOG(LogTemp, Log, TEXT("overlapStart!!"));
		PickupWidget->SetVisibility(true);

		AGHCharacterPlayer* Player = Cast<AGHCharacterPlayer>(OtherActor);
		if (Player)
		{
			Player->CurrentOverlapItem = this;// this는 현재 아이템 액터
		}
	}
	
}

void AGHFarmingBase::IsOverlapPropEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && OtherActor->IsA(AGHCharacterPlayer::StaticClass()))
	{
		UE_LOG(LogTemp, Log, TEXT("overlapend"));
		PickupWidget->SetVisibility(false); // 오버랩 끝나면 UI 숨김
	}

	ItemCount = 1;
}







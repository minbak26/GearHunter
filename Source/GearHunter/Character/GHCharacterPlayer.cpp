// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/GHCharacterPlayer.h"

#include "GHComboActionData.h"
#include "MovieSceneTracksComponentTypes.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Weapon/GHWeaponBase.h"
#include "Weapon/GHWeaponSwordFire.h"
#include "Physics/GHCollision.h"

class AGHWeaponBullet;

AGHCharacterPlayer::AGHCharacterPlayer()
{


	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 800.0f;
	SpringArm->bUsePawnControlRotation = false;
	// 상속받은 피치 요 롤 회전을 상속을 막아줘야 한다. 플레이어이동시 카메라 회전하는 부분 해결.
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritRoll = false;
	SpringArm->bInheritYaw = false;
	// 충돌테스트 비활성화.
	SpringArm->bDoCollisionTest = false;
	
	SpringArm->SetRelativeRotation(FRotator(-60.0f, 0.0f, 0.0f));
	
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	
}

void AGHCharacterPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGHCharacterPlayer::BeginPlay()
{
	Super::BeginPlay();
	

}

void AGHCharacterPlayer::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AGHCharacterPlayer::AttachWeapon()
{
	Super::AttachWeapon();

	FName WeaponSoket(TEXT("WeaponSocket"));
	// 무기 장착할 폰 찾기.
	WeaponBase = GetWorld()->SpawnActor<AGHWeaponBase>();
	
	
	if (WeaponBase)
	{
		//AttachToComponent 연결하고픈 부모 컴포넌트(스켈레탈 메시), 연결할때 위치 설정값, 부모에서 연결할 소켓
		// 자식될 컴포넌트에서 함수 호출 부모(플레이어베이스) 컴포넌트 지정 (했음)
		//WeaponBase->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSoket);
		
	}
}




void AGHCharacterPlayer::OnRollingMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (Montage == RollingActionMontage)
	{
		bIsRolling = false;
		// 끝나면 다시 입력 허용
		bCanReceiveinput = true;

		UE_LOG(LogTemp, Log, TEXT("Rolling animation ended"));
	}
	
}
// FStiren-> FName
void AGHCharacterPlayer::ResetSkillCoolTimeDown(const FString& SkillName)
{
	if (SkillName== "Rolling")
	{
		//bCanRoll = true;
		bIsRolling = false;
	}
	else if (SkillName== "QSkill")
	{
		bIsQSkill = false;
		
	}
	else if (SkillName == "WSkill")
	{
		bIsWSkill = false;
		
	}

	UE_LOG(LogTemp, Log, TEXT("%s cooldown reset"), *SkillName);
}



void AGHCharacterPlayer::NewQSkillCoolTimeSet(const FWeaponStatsData& CharacterStat)
{
	QSkillCoolTime = CharacterStat.QSkillCooldown;
}

void AGHCharacterPlayer::NewWSkillCoolTimeSet(const FWeaponStatsData& CharacterStat)
{
	WSkillCoolTime = CharacterStat.WSKillCooldown;
}

void AGHCharacterPlayer::ProcessComboCommand()
{
	// if (ComboActionMontage)
	// {
	// 	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	// 	AnimInstance->Montage_Play(ComboActionMontage);
	// }


	if (CurrentCombo == 0)
	{
		ComboActionBegin();
		return;
	}

	if (!ComboTimerHandle.IsValid())
	{
		HasNextCombo = false;
	}
	else
	{
		HasNextCombo = true;
	}
	
}




void AGHCharacterPlayer::ComboActionBegin()
{
	CurrentCombo = 1;

	// 캐릭터 무빙 세팅. 콤보시작시 움직이지 않게.
	GetCharacterMovement()->SetMovementMode(MOVE_None);

	// Animation Setting.
	const float AttackSpeedRate = 1.0f;
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(ComboActionMontage, AttackSpeedRate);

	// 몽타주 종료시 ComboActionEnd 호출.
	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &AGHCharacterPlayer::ComboActionEnd);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, ComboActionMontage);

	ComboTimerHandle.Invalidate();
	SetComboCheckTimer();
}

void AGHCharacterPlayer::ComboActionEnd(class UAnimMontage* Montage, bool IsProperlyEnded)
{
	// 호출시 콤보를 0으로 초기화.
	ensure(CurrentCombo != 0);
	CurrentCombo = 0;
	
	// 다시 움직이되, 좌표점은 본인 위치에서 새로운 좌표를 받게 하고싶음
	// FVector EndLocation = GetActorLocation();
	// SetActorLocation(EndLocation);

	// 다음 함수추가로구현
	GetCharacterMovement()->StopMovementImmediately();
	
	//다시 움직이도록 설정.
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);

}

void AGHCharacterPlayer::SetComboCheckTimer()
{
	int32 ComboIndex = CurrentCombo - 1;
	ensure(ComboIndex >= 0);

	const float AttackSpeedRate = 1.0f;
	float ComboEffectiveTime = (ComboActionData->EffectiveFrameCount[ComboIndex] / (ComboActionData->FrameRate) / AttackSpeedRate);
	if (ComboEffectiveTime > 0.0f)
	{
		GetWorld()->GetTimerManager().SetTimer(ComboTimerHandle, this, &AGHCharacterPlayer::ComboCheck, ComboEffectiveTime,false);
	}
}

void AGHCharacterPlayer::ComboCheck()
{
	ComboTimerHandle.Invalidate();
	if (HasNextCombo)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

		CurrentCombo = FMath::Clamp(CurrentCombo + 1, 1, ComboActionData->MaxComboCount);
		FName NextComboName = *FString::Printf(TEXT("%s%d"), *ComboActionData->MontageSectionName, CurrentCombo);
		AnimInstance->Montage_JumpToSection(NextComboName, ComboActionMontage);
		SetComboCheckTimer();
		HasNextCombo = false;
	}
	
}

void AGHCharacterPlayer::RollingCommand()
{

	if (RollingActionMontage && !RoolingCoolTimerHandle.IsValid())
	{
		// 몽타주 실행 플래그.
		bIsRolling = true;
		bCanReceiveinput = false;
		
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		AnimInstance->Montage_Play(RollingActionMontage);
		GetCharacterMovement()->StopMovementImmediately();

		// 몽타주 종료후 플래그 해제
		// FOnMontageEnded EndDelegate;
		// EndDelegate.BindUObject(this, &AGHCharacterBase::OnRollingMontageEnded);
		// AnimInstance->Montage_SetEndDelegate(EndDelegate, RollingActionMontage);
		AnimInstance->Montage_GetEndedDelegate(RollingActionMontage)->BindLambda(
[&bCanReceiveinput = bCanReceiveinput](UAnimMontage* Montage, bool bInterrupted)
		{ bCanReceiveinput = true;}
		);

		// 쿨타임 타이머.
		GetWorldTimerManager().SetTimer
		(
			RoolingCoolTimerHandle,
			[this]()
			{
				RoolingCoolTimerHandle.Invalidate();
				//ResetSkillCoolTimeDown("Rolling");
			},
			RollingCoolTime,
			false
		);

		
	}
	UE_LOG(LogTemp, Log, TEXT("Rolling started - Cooldown: %.1f seconds"), RollingCoolTime);
}

void AGHCharacterPlayer::TryCastQSkill()
{
    if (!bCanReceiveinput)
    {
       return;
    }

    // 현재 값 저장.
    float CurrentCoolTime = QSkillCoolTime;  // WSkillCoolTime -> QSkillCoolTime으로 수정
    
    // 쿨타임 변경 전 로그
    UE_LOG(LogTemp, Warning, TEXT("Q Skill - 현재 쿨타임: %f"), CurrentCoolTime);
       
    // 컨트롤러에서 해당 불변수라 true이면 새로운 스킬 값 가짐
    AGHPlayerController* PlayerController = Cast<AGHPlayerController>(GetController());
    if (PlayerController && PlayerController->GetbCanEnforce())
    {
       FWeaponStatsData StatsData = PlayerController->GetState();
       NewQSkillCoolTimeSet(StatsData);
       
       // 쿨타임 변경 후 로그
       UE_LOG(LogTemp, Warning, TEXT("Q Skill - 강화 적용 후 쿨타임: %f (변경량: %f)"), 
              QSkillCoolTime, QSkillCoolTime - CurrentCoolTime);
    }
    else
    {
       // 강화가 적용되지 않은 경우 로그
       UE_LOG(LogTemp, Log, TEXT("Q Skill - 강화 미적용, 기본 쿨타임 유지: %f"), QSkillCoolTime);
    }

    
    if (QSkillActionMontage && !QSkillCoolTimerHandle.IsValid())
    {
       bIsQSkill = true;
       bCanReceiveinput = false;
       
       // 스킬 시전 시작 로그
       UE_LOG(LogTemp, Warning, TEXT("Q Skill 시전 시작 - 적용된 쿨타임: %f초"), QSkillCoolTime);
       
       UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
       AnimInstance->Montage_Play(QSkillActionMontage);

       // 소켓 위치와 회전 가져오기.
       FVector SocketLocation = GetMesh()->GetSocketLocation(TEXT("WeaponSocket"));
       FRotator SocketRotation = GetControlRotation();
       

       // 발사방향.
       FVector LaunchDirection = GetActorForwardVector();
       
       AGHWeaponSwordFire* SpawnedProjectile = GetWorld()->SpawnActor<AGHWeaponSwordFire>(WeaponShoot, SocketLocation, SocketRotation);
       if (SpawnedProjectile)
       {
          SpawnedProjectile->FireInDirection(LaunchDirection);
          
       }

       // FOnMontageEnded EndDelegate;
       // EndDelegate.BindUObject(this, &AGHCharacterBase::OnRollingMontageEnded);
       // AnimInstance->Montage_SetEndDelegate(EndDelegate, RollingActionMontage);
       AnimInstance->Montage_GetEndedDelegate(QSkillActionMontage)->BindLambda(
    [&bCanReceiveinput = bCanReceiveinput](UAnimMontage* Montage, bool bInterrupted)
             { 
                bCanReceiveinput = true;
                UE_LOG(LogTemp, Log, TEXT("Q Skill 몽타주 종료 - 입력 재활성화"));
             }
    );

       // 타이머
       GetWorldTimerManager().SetTimer
       (
          QSkillCoolTimerHandle,
          [this]()
          {
             QSkillCoolTimerHandle.Invalidate();
             UE_LOG(LogTemp, Warning, TEXT("Q Skill 쿨타임 완료 - 다시 사용 가능"));
          },
          QSkillCoolTime,
          false
       );
       
       GetCharacterMovement()->StopMovementImmediately();
       
    }
    else
    {
       // 스킬 사용 불가 상황 로그
       if (!QSkillActionMontage)
       {
          UE_LOG(LogTemp, Error, TEXT("Q Skill - ActionMontage가 설정되지 않음"));
       }
       if (QSkillCoolTimerHandle.IsValid())
       {
          UE_LOG(LogTemp, Log, TEXT("Q Skill - 아직 쿨타임 중 (남은 시간: %f초)"), 
                 GetWorldTimerManager().GetTimerRemaining(QSkillCoolTimerHandle));
       }
    }
}


void AGHCharacterPlayer::TryCastWSkill()
{
	if (!bCanReceiveinput)
	{
		return;
	}
	
	if (WSkillActionMontage && !WSkillCoolTimerHandle.IsValid())
	{
		bIsWSkill = true;
		bCanReceiveinput = false;

		// 현재 쿨타임 값 저장 (변경 전)
		float CurrentCoolTime = WSkillCoolTime;
		UE_LOG(LogTemp, Warning, TEXT("W Skill - 현재 쿨타임: %f"), CurrentCoolTime);

		AGHPlayerController* PlayerController = Cast<AGHPlayerController>(GetController());
		if (PlayerController && PlayerController->GetbCanEnforce())
		{
			FWeaponStatsData StatsData = PlayerController->GetState();
			NewWSkillCoolTimeSet(StatsData);
			
			// 쿨타임 변경 후 로그
			UE_LOG(LogTemp, Warning, TEXT("W Skill - 강화 적용 후 쿨타임: %f (변경량: %f)"), 
				   WSkillCoolTime, WSkillCoolTime - CurrentCoolTime);
		}
		else
		{
			// 강화가 적용되지 않은 경우 로그
			UE_LOG(LogTemp, Log, TEXT("W Skill - 강화 미적용, 기본 쿨타임 유지: %f"), WSkillCoolTime);
		}

		// 스킬 시전 시작 로그
		UE_LOG(LogTemp, Warning, TEXT("W Skill 시전 시작 - 적용된 쿨타임: %f초"), WSkillCoolTime);
		
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		AnimInstance->Montage_Play(WSkillActionMontage);
		AnimInstance->Montage_GetEndedDelegate(WSkillActionMontage)->BindLambda(
			[&bCanReceiveinput = bCanReceiveinput](UAnimMontage* Montage, bool bInterrupted)
			{ 
				bCanReceiveinput = true;
				UE_LOG(LogTemp, Log, TEXT("W Skill 몽타주 종료 - 입력 재활성화"));
			}
		);
		GetCharacterMovement()->StopMovementImmediately();

		
		GetWorldTimerManager().SetTimer(
		WSkillCoolTimerHandle,
		[this]()
			{
				WSkillCoolTimerHandle.Invalidate();
				ResetSkillCoolTimeDown("WSKill");
				UE_LOG(LogTemp, Warning, TEXT("W Skill 쿨타임 완료 - 다시 사용 가능"));
			},
		WSkillCoolTime,
		false);
	}
	else
	{
		// 스킬 사용 불가 상황 로그
		if (!WSkillActionMontage)
		{
			UE_LOG(LogTemp, Error, TEXT("W Skill - ActionMontage가 설정되지 않음"));
		}
		if (WSkillCoolTimerHandle.IsValid())
		{
			UE_LOG(LogTemp, Log, TEXT("W Skill - 아직 쿨타임 중 (남은 시간: %f초)"), 
				   GetWorldTimerManager().GetTimerRemaining(WSkillCoolTimerHandle));
		}
	}
}

void AGHCharacterPlayer::TryCrafting()
{

	if (!bCanReceiveinput)
	{
		return;
	}

	
	if (CraftingActionMontage)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		AnimInstance->Montage_Play(CraftingActionMontage);
	}
	
	
}





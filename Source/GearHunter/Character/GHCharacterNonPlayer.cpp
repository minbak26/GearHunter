// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/GHCharacterNonPlayer.h"

#include "CharacterStat/GHCharacterStatComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Physics/GHCollision.h"

AGHCharacterNonPlayer::AGHCharacterNonPlayer()
{
	// pawn
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	//Capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	GetCapsuleComponent()->SetCollisionProfileName(CPROFILE_GHCAPSULE);

	// Movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 700.0f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.0f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.0f;
	GetCharacterMovement()->BrakingDecelerationWalking = 20000.0f;

	//Mesh
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -100.0f), FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));

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

	
	
}

void AGHCharacterNonPlayer::SetDead()
{

	Super::SetDead();
}

void AGHCharacterNonPlayer::NPCMeshLoadCompleted()
{
}


float AGHCharacterNonPlayer::GetAIPatrolRadius()
{
	return 800.0f;
}

float AGHCharacterNonPlayer::GetAIDetectRange()
{
	return 400.0f;
}

float AGHCharacterNonPlayer::GetAIAttackRange()
{
	return 0.0f;
}

float AGHCharacterNonPlayer::GetAITurnSpeed()
{
	return 0.0f;
}

void AGHCharacterNonPlayer::SetAIAttackDelegate(const FAICharacterAttackFinished& InOnAttackFinished)
{
	OnAttackFinished = InOnAttackFinished;
}

void AGHCharacterNonPlayer::AttackByAI()
{
	
}

void AGHCharacterNonPlayer::NotifyComboActionEnd()
{
}

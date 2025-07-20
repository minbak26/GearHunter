// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "NiagaraSystem.h"
#include "Database/WeaponData.h"
#include "GHPlayerController.generated.h"

/**
 * 
 */

// 아이템 구조체
USTRUCT(BlueprintType)
struct FInventoryItem
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FName ItemID;
	
	UPROPERTY(BlueprintReadWrite)
	int ItemCount = 0;
};


UCLASS()
class GEARHUNTER_API AGHPlayerController : public APlayerController
{
	GENERATED_BODY()
	AGHPlayerController();

	protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY()
	bool bClickRightMouse = false;

	// 공격키 : 좌클
	UPROPERTY()
	bool bClickLeftMouse = false;

	// 시프트 : 구르기
	UPROPERTY()
	bool bShiftKey = false;

	// 스페이스바 : 누르고있으면 대시
	UPROPERTY()
	bool bSpaceKey = false;
	
	UPROPERTY()
	bool bOneKey = false;

	UPROPERTY()
	bool bTwoKey = false;
	

	// 마우스 버튼 누를때.	
	void InputRightMouseButtonPressed();

	// 마우스 버튼 입력정보
	FVector LastClickPosition;
	
	// 마우스 버튼 땔때.
	void InputRightMouseButtonReleased();

	// 공격 버튼
	void InputLeftMouseButtonPressed();
	
	// 구르기 버튼
	void InputSpacekeyPressed();

	// Q : 스킬 1
	void InputQButtonPressed();
	// W : 스킬 2
	void InputWButtonPressed();
	// Z : 무기강화 크래프팅.
	void InputZButtonPressed();

	// I : 인벤토리
	void InputIButtonPressed();

	// E : 아이템 획득.
	void InputEButtonPressed();
	
	

	// 입력 컴포넌트.
	virtual void SetupInputComponent() override;

	// 새로운 위치를 받아서 컨트롤러가 소유한 폰을 입력받은 위치로 보내는 함수.
	void SetNewDestination(const FVector& Destination);
	
	void MovetoMouseCursor();

	


	// MovetoMouseCursor를 실시간 호출하기 위한 함수.
	virtual void PlayerTick(float DeltaTime) override;

	// 나이아 가라 실행 함수.
	virtual void PlayHitFX(FVector Destination);

	// input section.
	// 향상된 입력
public:
	UPROPERTY(EditAnywhere)
	UEnhancedInputComponent* PlayerEnhancedInputComponent;

	UPROPERTY(EditAnywhere, Category = "input")
	TObjectPtr<class UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditAnywhere, Category = "input")
	TObjectPtr<class UInputAction> RightClickMove;

	
	// 구르기 애님 몽타주.
	UPROPERTY(EditAnywhere, Category = "Animation")
	UAnimMontage* Rolling;

	// 구르고 있음을 확인하는 bool변수.
	uint8 bIsRolling : 1;

	

	// Cursor Section.
protected:
	UPROPERTY(EditAnywhere, Category = "FX")
	TArray<UNiagaraSystem*> HitFXArray;

	// FX 초기화 함수.
	void InitalizeHitFX();
	UPROPERTY(EditAnywhere, Category = "Effects")
	float EffectZOffset = 5.0f; // 지면에서의 높이 오프셋

public:
	// UI Section.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> HUDUserWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> InventoryWidget;

	// 인벤토리 UI 업데이트.
	UFUNCTION()
	void UpdateInventoryUI();


	
	bool IsInventoryWidgetVisible = false;
	
	UPROPERTY()
	TObjectPtr<UUserWidget> CurrentHUDWidget;
	UPROPERTY()
	TObjectPtr<UUserWidget> CurrentInventoryWidget;
	
	// Invectory Section.
	UPROPERTY(BlueprintReadWrite)
	TArray<FInventoryItem> Inventory;
	// 아이템 획득 함수.
	void AddItemToInventory(FName ItemID, int Count);
	

	// Enforce Section.

	
	
	// 1.강화 가능 여부 확인 함수.
	UFUNCTION()
	bool IsCanEnforce(FName UpgradeLevel);

protected:
	// 강화 가능시 불변수 표시.
	// 전역으로 표시해서 캐릭터, 무기에도 접근가능하게
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enforce")
	bool bCanEnforce = false;
public:
	bool GetbCanEnforce() {return bCanEnforce;};
	
	
	
	// 1-1. 강화 가능 여부 확인 함수의 헬퍼 함수.
	UFUNCTION()
	int GetItemCount(FName ItemID);
	
	
	
	// 강화 가능시 스테이트 부여 테이블.
	UPROPERTY()
	TObjectPtr<UDataTable> WeaponState;
	
	// 0. 강화 테이블.
	UPROPERTY()
	TObjectPtr<UDataTable> WeaponUpgradeDataTable;
	
public:	
	// IsCanEnforce 함수 인자 배열.
	TArray<FName> WeaponLevels =
		{
			TEXT("UPTO2"),
			TEXT("UPTO3"),
			TEXT("UPTO4"),
			TEXT("UPTO5"),
			TEXT("UPTO6"),
			TEXT("UPTO7")
		};
	// WeaponLevels 인덱스.
	int CurrentWeaponLevel = 0;

	// 무기 스테이트 부여는 WeaponBase에서 	
public:
	// 무기강화 테이블
	TArray<FName> EnforceStatLevels =
		{
			TEXT("Enforce2"),
			TEXT("Enforce3"),
			TEXT("Enforce4"),
			TEXT("Enforce5"),
			TEXT("Enforce6"),
			TEXT("Enforce7")
		};
	int CurrentStateLevel = 0;
	
	FWeaponStatsData GetState();


	

};

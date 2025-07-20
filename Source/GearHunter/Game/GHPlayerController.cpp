// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/GHPlayerController.h"

#include "Animation/AnimInstanceProxy.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/Character.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Navigation/PathFollowingComponent.h"
#include "Character/GHCharacterBase.h"
#include "Character/GHCharacterPlayer.h"
#include "Farm/GHFarmingBase.h"
#include "UI/GHInventoryWidget.h"
#include "Database/WeaponData.h"


AGHPlayerController::AGHPlayerController()
{
	bShowMouseCursor = true;
	// 나이아가라 FX추가.
	InitalizeHitFX();

	static ConstructorHelpers::FClassFinder<UUserWidget> HUDWidgetRef(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/WB_SkillCoolTime.WB_SkillCoolTime_C'"));
	if (HUDWidgetRef.Succeeded())
	{
		HUDUserWidgetClass = HUDWidgetRef.Class;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> InventoryRef(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/Iventory/WB_Inventory.WB_Inventory_C'"));
	if (InventoryRef.Succeeded())
	{
		InventoryWidget = InventoryRef.Class;
	}

	static ConstructorHelpers::FObjectFinder<UDataTable> WeaponUpgradeTableRef(TEXT("/Script/Engine.DataTable'/Game/WeaponEnforce/DT_WeaponUpgrade.DT_WeaponUpgrade'"));
	if (WeaponUpgradeTableRef.Succeeded())
	{
		WeaponUpgradeDataTable = WeaponUpgradeTableRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UDataTable> WeaponStateRef(TEXT("/Script/Engine.DataTable'/Game/WeaponEnforce/DT_WeaponState.DT_WeaponState'"));
	if (WeaponStateRef.Succeeded())
	{
		WeaponState = WeaponStateRef.Object;
	}

	// 생성자에 DT두개 미리 선언.
	FWeaponStatsData WeaponStats;
	FWeaponUpgradeData UpgradeData;

	
}


void AGHPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	// FInputModeGameOnly GameOnlyInputMode;	//이걸 뻇는데 왜 되지.
	// SetInputMode(GameOnlyInputMode);

	// 마우스 커서 보이게
	SetShowMouseCursor(true);
	
	// 위젯 생성 및 화면에 추가
	if (HUDUserWidgetClass)
	{
		CurrentHUDWidget = CreateWidget<UUserWidget>(this, HUDUserWidgetClass);
		if (CurrentHUDWidget)
		{
			CurrentHUDWidget->AddToViewport();
		}
	}
	
	
}

void AGHPlayerController::InputRightMouseButtonPressed()
{
	UE_LOG(LogTemp ,Log, TEXT("pressed"));
	bClickRightMouse = true;// bClickRightMouse 설정 추가로.

	// 마우스 위치를 월드 좌표로 변환하여 저장
	FVector WorldLocation, WorldDirection;
	DeprojectMousePositionToWorld(WorldLocation, WorldDirection);
    
	FHitResult HitResult;
	if (GetWorld()->LineTraceSingleByChannel(HitResult, WorldLocation, 
		WorldLocation + WorldDirection * 10000.0f, ECC_Visibility))
	{
		LastClickPosition = HitResult.Location;
	}
	// 나이아가라 재생.
	PlayHitFX(LastClickPosition);

	
}

void AGHPlayerController::InputRightMouseButtonReleased()
{
	UE_LOG(LogTemp ,Log, TEXT("Released"));
	bClickRightMouse = false;


}

void AGHPlayerController::InputLeftMouseButtonPressed()
{
	bClickLeftMouse = true;

	// 여기에 공격 구현.
	UE_LOG(LogTemp ,Log, TEXT("Attack Call"));
	
	AGHCharacterPlayer* MyCharacterBase = Cast<AGHCharacterPlayer>(GetCharacter());
	if (MyCharacterBase)
	{
		MyCharacterBase->ProcessComboCommand();
	}



	

}

void AGHPlayerController::InputSpacekeyPressed()
{
	UE_LOG(LogTemp, Log, TEXT("Space pressed"));
	bSpaceKey = true;

	AGHCharacterPlayer* MyCharacterBase = Cast<AGHCharacterPlayer>(GetCharacter());
	// 다른 입력 차단을 위한 bool 변수 체크.
	if (MyCharacterBase)
	{
		if (MyCharacterBase->GetCanReceiveInput() && MyCharacterBase->GetCanActive())
		{
			MyCharacterBase->RollingCommand();
		}
		else if (!MyCharacterBase->GetCanActive())
		{
			UE_LOG(LogTemp, Log, TEXT("Rolling CoolTime "));
		}
		else if (MyCharacterBase->GetIsActiving())
		{
			UE_LOG(LogTemp, Log, TEXT("is Rolling!! "));
		}
		
	}


}

void AGHPlayerController::InputQButtonPressed()
{
	UE_LOG(LogTemp, Log, TEXT("Q Skill!!"));
	bOneKey = true;

	AGHCharacterPlayer* MyCharacterBase = Cast<AGHCharacterPlayer>(GetCharacter());
	// if (MyCharacterBase&& MyCharacterBase->bCanReceiveinput)
	// {
		MyCharacterBase->TryCastQSkill();
	// }
	
	
}

void AGHPlayerController::InputWButtonPressed()
{
	UE_LOG(LogTemp, Log, TEXT("W Skill!! "));
	bTwoKey = true;

	AGHCharacterPlayer* MyCharacterBase = Cast<AGHCharacterPlayer>(GetCharacter());
	// if (MyCharacterBase && MyCharacterBase->bCanReceiveinput)
	// {
		MyCharacterBase->TryCastWSkill();
	//}
}

void AGHPlayerController::InputZButtonPressed()
{
	UE_LOG(LogTemp, Log, TEXT("Z Crafting"));

	// IsCanEnforce가 true일때만 몽타주 실행하게.
	if (IsCanEnforce(WeaponLevels[CurrentWeaponLevel]))
	{
		AGHCharacterPlayer* MyCharacterBase = Cast<AGHCharacterPlayer>(GetCharacter());
		MyCharacterBase->TryCrafting();

		// 여기서 스테이트 부여해야할듯 
		GetState();

		// 캐릭터스탯부여

		
		
		// DT의 Row를 다음 행으로 1.무기 강화 개수, 2.무기스탯.
		CurrentWeaponLevel++;
		CurrentStateLevel++;
	}

}

void AGHPlayerController::InputIButtonPressed()
{
	UE_LOG(LogTemp, Warning, TEXT("InputIButtonPressed Called"));

	if (!InventoryWidget)
	{
		UE_LOG(LogTemp, Error, TEXT("InventoryWidget is not set!"));
		return;
	}

	if (!IsInventoryWidgetVisible)
	{
		if (!CurrentInventoryWidget)
		{
			CurrentInventoryWidget = CreateWidget<UGHInventoryWidget>(this, InventoryWidget);
			UE_LOG(LogTemp, Warning, TEXT("CreateWidget called!"));
		}
		if (CurrentInventoryWidget && !CurrentInventoryWidget->IsInViewport())
		{
			CurrentInventoryWidget->AddToViewport();
			IsInventoryWidgetVisible = true;

			UpdateInventoryUI();
			UE_LOG(LogTemp, Warning, TEXT("InventoryWidget Added to Viewport!"));
		}
	}
	else
	{
		if (CurrentInventoryWidget && CurrentInventoryWidget->IsInViewport())
		{
			CurrentInventoryWidget->RemoveFromParent();
			IsInventoryWidgetVisible = false;
			UE_LOG(LogTemp, Warning, TEXT("InventoryWidget Removed from Viewport!"));
		}
	}
}

void AGHPlayerController::InputEButtonPressed()
{
	AGHCharacterPlayer* PlayerBase = Cast<AGHCharacterPlayer>(GetPawn());
	if (PlayerBase && PlayerBase->CurrentOverlapItem)
	{
		// 현재 겹치고 있는 아이템을 사용할 수 있음
		AGHFarmingBase* OverlapItem = PlayerBase->CurrentOverlapItem;
  		AddItemToInventory(OverlapItem->ItemID, OverlapItem->ItemCount);
		// UI 업데이트.
		UpdateInventoryUI();
		
		// 집었으면 삭제.
		OverlapItem->Destroy();
		PlayerBase->CurrentOverlapItem = nullptr;
		
	}
}
void AGHPlayerController::SetupInputComponent()
{
	
	Super::SetupInputComponent();
	
	
	// 입력 매핑, 함수 바인딩.
	// 이동관련 입력만 컨트롤러에서 
	
	InputComponent->BindAction("Move", IE_Pressed, this, &AGHPlayerController::InputRightMouseButtonPressed);
	
	InputComponent->BindAction("Move", IE_Released, this, &AGHPlayerController::InputRightMouseButtonReleased);

	InputComponent->BindAction("Attack", IE_Pressed, this, &AGHPlayerController::InputLeftMouseButtonPressed);
	
	InputComponent->BindAction("Space", IE_Pressed, this, &AGHPlayerController::InputSpacekeyPressed);

	InputComponent->BindAction("Skill1", IE_Pressed, this, &AGHPlayerController::InputQButtonPressed);

	InputComponent->BindAction("Skill2", IE_Pressed, this, &AGHPlayerController::InputWButtonPressed);

	InputComponent->BindAction("Craft", IE_Pressed, this, &AGHPlayerController::InputZButtonPressed);

	InputComponent->BindAction("Inventory", IE_Pressed, this, &AGHPlayerController::InputIButtonPressed);

	InputComponent->BindAction("GetItem", IE_Pressed, this, &AGHPlayerController::InputEButtonPressed);
	
}

void AGHPlayerController::SetNewDestination(const FVector& Destination)
{
	// 현재 폰의 위치 
	APawn* const MyPawn = GetPawn();

	
	if (MyPawn)
	{
		float const Distance = FVector::Dist(Destination, MyPawn->GetActorLocation());

		//UE_LOG(LogTemp ,Log, TEXT("X: %f, Y :  %f, %f"), Destination.X, Destination.Y, Distance);
		if (Distance > 100.0f)
		{
			UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, Destination);

		}
		
	}
}

void AGHPlayerController::MovetoMouseCursor()
{
	UE_LOG(LogTemp ,Log, TEXT("Call MoveMouseCursor"));
	
	// 마우스로 땅을 찍으면
	FHitResult Hit;
	// 충돌정보 저장하고 해당 함수 호출. ECC_Visibility 충돌을 검출하거나 무시할때 사용하는  ENum
	GetHitResultUnderCursor(ECC_Visibility, false, Hit);
	
	
	// 만약  충돌이 발생한다면.
	if (Hit.bBlockingHit)
	{
		//플레이어 목적지를 마우스 커서 아래쪽으로 설정. ImpactPoint -> 충돌 지점 위치 FVector.
		SetNewDestination(Hit.ImpactPoint);
	}
	
	
}


void AGHPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	
	//마우스 신호 들어오면
	if (bClickRightMouse)
	{
		
		UE_LOG(LogTemp ,Log, TEXT("Call Tick -> MovetoMouseCursor"));
		//함수호출.
		MovetoMouseCursor();
	}
	// 마우스 신호 없으면 멈추게.
}

void AGHPlayerController::PlayHitFX(FVector defination)
{
	UE_LOG(LogTemp, Log, TEXT("Function Call"));
	if (HitFXArray.Num() > 0)
	{
		UE_LOG(LogTemp, Log, TEXT("Index upper 1"));
		int32 RandomIndex = FMath::RandRange(0, HitFXArray.Num() - 1);
		UNiagaraSystem* SelectedFX = HitFXArray[RandomIndex];

		if (SelectedFX)
		{
			UE_LOG(LogTemp, Log, TEXT("Play Niagara"));
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), SelectedFX, defination);
		}
	}
	
}



// FX함수 초기화.
void AGHPlayerController::InitalizeHitFX()
{
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> CursorFX(TEXT("/Script/Niagara.NiagaraSystem'/Game/Cursor/FX_Cursor.FX_Cursor'"));

	if (CursorFX.Succeeded()) HitFXArray.Add(CursorFX.Object);

}

void AGHPlayerController::AddItemToInventory(FName InItemID, int32 InCount)
{
	for (FInventoryItem& Item : Inventory)
	{
		if (Item.ItemID == InItemID)
		{
			UE_LOG(LogTemp, Warning, TEXT("Found existing item %s, Old Count: %d, Adding: %d"), 
				   *Item.ItemID.ToString(), Item.ItemCount, InCount);
			
			Item.ItemCount += InCount;
			//UE_LOG(LogTemp, Warning, TEXT("New Count: %d"), Item.ItemCount);
			
			return;
		}
	}
	// 아이템 이 없으면 생성.
	FInventoryItem NewItem;
	NewItem.ItemID = InItemID;
	NewItem.ItemCount = InCount;
	Inventory.Add(NewItem);

	UE_LOG(LogTemp, Warning, TEXT("Created new item: %s, Count: %d, Total Items: %d"), 
		  *InItemID.ToString(), InCount, Inventory.Num());
}




// 처음에 WeaponLevels[0]을 넣어줘야함.
// 그다음에 다음 레벨가는게 필요하는데
// IsCanEnforce(WeaponLevels[CurrentLevel])
bool AGHPlayerController::IsCanEnforce(FName UpgradeLevel)
{
	// 1단계: DataTable 존재 확인
	if (!WeaponUpgradeDataTable)
	{
		UE_LOG(LogTemp, Error, TEXT("Step 1 Failed: DataTable is null"));
		return false;
	}
	UE_LOG(LogTemp, Log, TEXT("Step 1 OK: DataTable exists"));

	// 2단계: 유효성 확인
	if (!IsValid(WeaponUpgradeDataTable))
	{
		UE_LOG(LogTemp, Error, TEXT("Step 2 Failed: DataTable is invalid"));
		return false;
	}
	UE_LOG(LogTemp, Log, TEXT("Step 2 OK: DataTable is valid"));

	// 3단계: 구조체 확인
	if (WeaponUpgradeDataTable->GetRowStruct() != FWeaponUpgradeData::StaticStruct())
	{
		UE_LOG(LogTemp, Error, TEXT("Step 3 Failed: Wrong struct type"));
		return false;
	}
	UE_LOG(LogTemp, Log, TEXT("Step 3 OK: Correct struct type"));

	// 4단계: 행 존재 확인
	TArray<FName> AllRows = WeaponUpgradeDataTable->GetRowNames();
	UE_LOG(LogTemp, Log, TEXT("Step 4: DataTable has %d rows"), AllRows.Num());
    
	// 5단계: FindRow 실행
	FWeaponUpgradeData* Data = WeaponUpgradeDataTable->FindRow<FWeaponUpgradeData>(UpgradeLevel, TEXT("Debug"));
	if (!Data)
	{
		UE_LOG(LogTemp, Error, TEXT("Step 5 Failed: FindRow returned null for '%s'"), *UpgradeLevel.ToString());
		return false;
	}
	UE_LOG(LogTemp, Log, TEXT("Step 5 OK: Found data - Wood:%d, Stone:%d"), Data->RequiredWood, Data->RequiredStone);


	FString ContextString = TEXT("Finding Upgrade Data");

	FWeaponUpgradeData* UpgradeData = WeaponUpgradeDataTable->FindRow<FWeaponUpgradeData>(UpgradeLevel , ContextString);

	UE_LOG(LogTemp, Log, TEXT("Current Wood : %d. Current Stone : %d "), GetItemCount(TEXT("Wood") ), GetItemCount(TEXT("Stone")));

	int CurrentWood = GetItemCount(TEXT("Wood"));
	int CurrentStone = GetItemCount(TEXT("Stone"));
	
	if(UpgradeData->RequiredWood <= CurrentWood &&  UpgradeData->RequiredStone <= CurrentStone)
	{
		// 강화재료 소진.
		int ReduceWood = -(Data->RequiredWood);
		int ReduceStond = -(Data->RequiredStone);
		AddItemToInventory(TEXT("Wood"), ReduceWood);
		AddItemToInventory(TEXT("Stone"), ReduceStond);

		UE_LOG(LogTemp, Log, TEXT("Current Wood : %d. Current Stone : %d "), GetItemCount(TEXT("Wood") ), GetItemCount(TEXT("Stone")));

		bCanEnforce = true;
		
		return true;
	}
	else
		return false;	

}



int AGHPlayerController::GetItemCount(FName Item)
{
	for (const FInventoryItem& ItemInventory : Inventory)
	{
		if (ItemInventory.ItemID == Item)  // 매개변수 사용
		{
			return ItemInventory.ItemCount;
		}
	}
	return 0;
}

FWeaponStatsData AGHPlayerController::GetState()
{
	FWeaponStatsData Result;

	FName CurrentState = EnforceStatLevels[CurrentStateLevel];
	FWeaponStatsData* UpgradeStat = WeaponState->FindRow<FWeaponStatsData>(CurrentState, TEXT("Debug"));

	if (UpgradeStat)
	{
		Result = *UpgradeStat;
	}
	
	return Result;
}




void AGHPlayerController::UpdateInventoryUI()
{
	if (IsInventoryWidgetVisible && CurrentInventoryWidget)
	{
		UGHInventoryWidget* InventoryUI = Cast<UGHInventoryWidget>(CurrentInventoryWidget);
		if (!InventoryUI)
		{
			UE_LOG(LogTemp, Error, TEXT("InventoryUI 캐스팅 실패! 부모 클래스, 생성 코드, 변수 타입을 확인하세요."));
			return;
		}
		for (const FInventoryItem& Item : Inventory)
		{
			InventoryUI->UpdateSlot(Item.ItemID, Item.ItemCount);
			
		}
	}
}




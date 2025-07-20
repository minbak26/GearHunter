// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/GHAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GHAI.h"

AGHAIController::AGHAIController()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBAssetRef (TEXT("/Script/AIModule.BlackboardData'/Game/AI/BB_NPC.BB_NPC'"));
	if (nullptr != BBAssetRef.Object)
	{
		
		BBAsset = BBAssetRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTAssetRef (TEXT("/Script/AIModule.BehaviorTree'/Game/AI/BT_NPC.BT_NPC'"));
	if (nullptr != BTAssetRef.Object)
	{
		BTAsset = BTAssetRef.Object;
	}
}

void AGHAIController::RunAI()
{
	UBlackboardComponent* BlackboardPtr =  Blackboard.Get();
	if (UseBlackboard(BBAsset, BlackboardPtr))
	{
		Blackboard->SetValueAsVector(BBKEY_HOMEPOS, GetPawn()->GetActorLocation());
		
		bool RunResult = RunBehaviorTree(BTAsset);
		ensure(RunResult);
	}
}

void AGHAIController::StopAI()
{
	UBehaviorTreeComponent* BTcomponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (BTcomponent)
	{
		BTcomponent->StopTree();
	}
}

void AGHAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	RunAI();
	
}

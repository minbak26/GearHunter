// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/GHGameModeBase.h"

AGHGameModeBase::AGHGameModeBase()
{
	static ConstructorHelpers::FClassFinder<APawn> DefaultCharacterRef(
		TEXT("/Script/GearHunter.GHCharacterPlayer"));

	if (DefaultCharacterRef.Class)
	{
		DefaultPawnClass = DefaultCharacterRef.Class;
	}

	static ConstructorHelpers::FClassFinder<APlayerController> DefaultPlayerControllerRef(TEXT("/Script/GearHunter.GHPlayerController"));
	if (DefaultPlayerControllerRef.Class)
	{
		 PlayerControllerClass = DefaultPlayerControllerRef.Class;
	}

	
}

//
//
// #include "TP_TopDownGameMode.h"
// #include "TP_TopDownPlayerController.h"
// #include "TP_TopDownCharacter.h"
// #include "UObject/ConstructorHelpers.h"
//
// ATP_TopDownGameMode::ATP_TopDownGameMode()
// {
// 	// use our custom PlayerController class
// 	PlayerControllerClass = ATP_TopDownPlayerController::StaticClass();
//
// 	// set default pawn class to our Blueprinted character
// 	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownCharacter"));
// 	if (PlayerPawnBPClass.Class != nullptr)
// 	{
// 		DefaultPawnClass = PlayerPawnBPClass.Class;
// 	}
//
// 	// set default controller to our Blueprinted controller
// 	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownPlayerController"));
// 	if(PlayerControllerBPClass.Class != NULL)
// 	{
// 		PlayerControllerClass = PlayerControllerBPClass.Class;
// 	}
// }
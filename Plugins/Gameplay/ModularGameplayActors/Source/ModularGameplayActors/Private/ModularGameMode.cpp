// Fill out your copyright notice in the Description page of Project Settings.


#include "ModularGameMode.h"
#include "ModularPawn.h"
#include "ModularPlayerController.h"


AModularGameModeBase::AModularGameModeBase(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{
	PlayerControllerClass = AModularPlayerController::StaticClass();
	DefaultPawnClass = AModularPawn::StaticClass();
}

AModularGameMode::AModularGameMode(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{
	PlayerControllerClass = AModularPlayerController::StaticClass();
	DefaultPawnClass = AModularPawn::StaticClass();
}

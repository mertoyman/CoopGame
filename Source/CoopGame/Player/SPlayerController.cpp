// Fill out your copyright notice in the Description page of Project Settings.


#include "SPlayerController.h"


ASPlayerController::ASPlayerController(const FObjectInitializer& ObjectInitializer)
{
}

void ASPlayerController::PreInitializeComponents()
{
	Super::PreInitializeComponents();
}

void ASPlayerController::BeginPlay()
{
	Super::BeginPlay();
	SetActorHiddenInGame(false);
}

void ASPlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void ASPlayerController::OnUnPossess()
{
	//TODO: Set avatar actor of ability system component to nullptr

	Super::OnUnPossess();
}

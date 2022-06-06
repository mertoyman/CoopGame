// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonPlayerController.h"
#include "SPlayerController.generated.h"

/**
 * ASPlayerController
 *
 * The base player controller class used by this project.
 */
UCLASS(Config="Game", meta=(ShortTooltip="The base player controller class used by this project."))
class COOPGAME_API ASPlayerController : public ACommonPlayerController
{
	GENERATED_BODY()
	
public:
	ASPlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

#pragma region AActor Interface
	virtual void PreInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
#pragma endregion

#pragma region AController Interface	
	virtual void OnUnPossess() override;
#pragma endregion 	
};

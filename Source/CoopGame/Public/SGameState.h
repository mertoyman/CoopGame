// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "SGameState.generated.h"

UENUM(BlueprintType)
enum class EWaveState : uint8
{
	WaitingToStart,

	WaveInProgress,

	WaveComplete,

	GameOver,
};

/**
 * 
 */
UCLASS()
class COOPGAME_API ASGameState : public AGameStateBase
{
	GENERATED_BODY()

protected:

	UFUNCTION(BlueprintImplementableEvent,Category="GameState")
	void WaveStateChanged(EWaveState NewState, EWaveState OldState);

public:

	UPROPERTY(BlueprintReadOnly, Category="GameState")
	EWaveState WaveState;
};

// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameState.h"

ASGameState::ASGameState() :
	OldWaveState(WaveState)
{
}

void ASGameState::StateChanged(const EWaveState NewState)
{
	OldWaveState = WaveState;
	
	WaveStateChanged(NewState, OldWaveState);
}

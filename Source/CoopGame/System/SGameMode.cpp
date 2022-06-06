// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameMode.h"

#include "EngineUtils.h"
#include "SGameState.h"
#include "CoopGame/Character/SHealthComponent.h"


ASGameMode::ASGameMode() :
NoOfBotsToSpawn(5),
WaveCount(0),
TimeBetweenWaves(10.0f)
{
	GameStateClass = ASGameState::StaticClass();
	
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 1.0f;
}

void ASGameMode::StartPlay()
{
	Super::StartPlay();

	PrepareForNextWave();
}

void ASGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	CheckWaveState();

	CheckAnyPlayerAlive();
}

void ASGameMode::StartWave()
{
	WaveCount++;

	NoOfBotsToSpawn = 2 * WaveCount;
	
	GetWorldTimerManager().SetTimer(TimerHandle_BotSpawner, this, &ASGameMode::SpawnBotSequence, 1.0f, true, 0.0f);

	SetWaveState(EWaveState::WaveInProgress);
}

void ASGameMode::EndWave()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_BotSpawner);

	SetWaveState(EWaveState::WaveComplete);
}

void ASGameMode::PrepareForNextWave()
{
	
	GetWorldTimerManager().SetTimer(TimerHandle_NextWaveStart, this, &ASGameMode::StartWave, 1.0f, false);

	SetWaveState(EWaveState::WaitingToStart);
}

void ASGameMode::SpawnBotSequence()
{
	SpawnNewBot();

	NoOfBotsToSpawn--;

	if (NoOfBotsToSpawn <= 0)
	{
		EndWave();
	}
}

void ASGameMode::CheckWaveState()
{
	const bool bIsPreparingForNextWave = GetWorldTimerManager().IsTimerActive(TimerHandle_NextWaveStart);
	
	if(NoOfBotsToSpawn > 0 || bIsPreparingForNextWave) return;
	
	bool bIsAnyBotsAlive = false;
	
	for (TActorIterator<APawn> It(GetWorld()); It; ++It)
	{
		APawn* TestPawn = *It;
		if (!TestPawn || TestPawn->IsPlayerControlled())
		{
			continue;
		}

		USHealthComponent* HealthComp = Cast<USHealthComponent>(TestPawn->GetComponentByClass(USHealthComponent::StaticClass())); 
		if (HealthComp && HealthComp->GetHealth() > 0.f)
		{
			bIsAnyBotsAlive = true;
			break;
		}
	}

	if (!bIsAnyBotsAlive)
	{
		PrepareForNextWave();

		SetWaveState(EWaveState::WaveComplete);
	}
}

void ASGameMode::GameOver()
{
	EndWave();
	
	SetWaveState(EWaveState::GameOver);
}

void ASGameMode::SetWaveState(EWaveState NewState) const
{
	ASGameState* GS = GetGameState<ASGameState>();
	if (ensureAlways(GS))
	{
		GS->StateChanged(NewState);
	}
	
}

void ASGameMode::CheckAnyPlayerAlive()
{
	for (TActorIterator<APlayerController> It(GetWorld()); It; ++It)
	{
		APlayerController* PC = *It;
		if (PC && PC->GetPawn())
		{
			const APawn* MyPawn = PC->GetPawn();
			USHealthComponent* HealthComp = Cast<USHealthComponent>(MyPawn->GetComponentByClass(USHealthComponent::StaticClass())); 
			if (ensure(HealthComp) && HealthComp->GetHealth() > 0.0f)
			{
				// Player is still alive
				return;
			}
		}
	}

	// Player is dead
	GameOver();
}



// Fill out your copyright notice in the Description page of Project Settings.


#include "SPowerupActor.h"

// Sets default values
ASPowerupActor::ASPowerupActor() :
	PowerupInterval(0.f),
	TotalNumberOfTicks(0)
{
 	
}

// Called when the game starts or when spawned
void ASPowerupActor::BeginPlay()
{
	Super::BeginPlay();

	
}

void ASPowerupActor::OnTickPowerUp()
{
	TicksProcessed++;

	if (TicksProcessed >= TotalNumberOfTicks)
	{
		OnExpired();

		// Delete timer
		GetWorldTimerManager().ClearTimer(TimerHandle_PowerUpTick);
	}
}

void ASPowerupActor::ActivatePowerUp()
{
	if (PowerupInterval > 0.f)
	{
		GetWorldTimerManager().SetTimer(TimerHandle_PowerUpTick, this, &ASPowerupActor::OnTickPowerUp, PowerupInterval, true, 0.f);
	}
	else
	{
		OnTickPowerUp();
	}
}


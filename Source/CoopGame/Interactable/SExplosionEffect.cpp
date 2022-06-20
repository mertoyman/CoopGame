// Fill out your copyright notice in the Description page of Project Settings.


#include "SExplosionEffect.h"

#include "Kismet/GameplayStatics.h"

// Called when the game starts or when spawned
void ASExplosionEffect::BeginPlay()
{
	Super::BeginPlay();

	if(ExplosionFX)
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, ExplosionFX, GetActorLocation(), GetActorRotation());
		SetLifeSpan(2.0);
	}
}


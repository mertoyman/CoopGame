// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SExplosionEffect.generated.h"

UCLASS()
class COOPGAME_API ASExplosionEffect : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category="Effect")
	UParticleSystem* ExplosionFX;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};

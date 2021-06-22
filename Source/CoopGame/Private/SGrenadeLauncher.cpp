// Fill out your copyright notice in the Description page of Project Settings.

#include "SGrenadeLauncher.h"

#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Grenade.h"

static int32 DebugWeaponDrawing = 0;
FAutoConsoleVariableRef CVARDebugWeaponDrawing(
	TEXT("COOP.DebugWeapons"),
	DebugWeaponDrawing,
	TEXT("Draw Debug Lines for Weapons"),
	ECVF_Cheat);

void ASGrenadeLauncher::Fire()
{
	AActor* MyOwner = GetOwner();

	if (MyOwner && RemainingBullets > 0)
	{
		FVector EyeLocation;
		FRotator EyeRotation;
		MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

		const FVector ShotDirection = EyeRotation.Vector();
		const FVector TraceEnd = EyeLocation + (ShotDirection * 10000);

		const auto Projectile = GetWorld()->SpawnActor<AGrenade>(ProjectileConfig.ProjectileClass, MeshComp->GetSocketLocation(MuzzleSocketName),
		                                                         EyeRotation);
		if(Projectile)
		{
			Projectile->LaunchProjectile();
			PlayFireEffects();
			UseAmmo();
		}
	}
}

void ASGrenadeLauncher::ApplyProjectileData(FProjectileWeaponData& Data) const
{
	Data = ProjectileConfig;
}

// Fill out your copyright notice in the Description page of Project Settings.
#include "SWeapon_Instant.h"

#include "SCharacter.h"
#include "CoopGame/CoopGame.h"
#include "PhysicalMaterials/PhysicalMaterial.h"

ASWeapon_Instant::ASWeapon_Instant()
{
	TimeBetweenShots = 60 / InstantConfig.RateOfFire;
}

void ASWeapon_Instant::BurstFire()
{
	CurrentBurstFire++;

	if (CurrentBurstFire <= InstantConfig.BurstFireAmount)
	{
		StartFiring();
	}
	else
	{
		GetWorldTimerManager().ClearTimer(TimerHandle_TimeBetweenShots);
		CurrentBurstFire = 0;
	}
}

void ASWeapon_Instant::Fire()
{
	IsSingleFire = InstantConfig.DefaultFireMode == EFireMode::Semi_Auto ? true : false;
	IsBurstFire = InstantConfig.DefaultFireMode == EFireMode::Burst ? true : false;

	const float FirstDelay = FMath::Max(LastFireTime + TimeBetweenShots - GetWorld()->TimeSeconds, 0.0f);

	if (IsBurstFire)
	{
		InstantConfig.IsSelectiveFireRifle = false;
		GetWorldTimerManager().SetTimer(TimerHandle_TimeBetweenShots, this, &ASWeapon_Instant::BurstFire,
		                                TimeBetweenShots, true, FirstDelay);
	}
	else //Semi-Auto and Full-Auto fire modes
	{
		GetWorldTimerManager().SetTimer(TimerHandle_TimeBetweenShots, this, &ASWeapon_Instant::StartFiring,
		                                TimeBetweenShots,
		                                !IsSingleFire, FirstDelay);
	}
}

void ASWeapon_Instant::OnBurstFinished()
{
	if (IsBurstFire)
	{
		if (CurrentBurstFire >= InstantConfig.BurstFireAmount)
		{
			GetWorldTimerManager().ClearTimer(TimerHandle_TimeBetweenShots);
		}
	}
	else
	{
		GetWorldTimerManager().ClearTimer(TimerHandle_TimeBetweenShots);
	}
}

void ASWeapon_Instant::DealDamage(FHitResult Impact, FVector ShotDirection)
{
	float ActualDamage = InstantConfig.BaseDamage;

	EPhysicalSurface SurfaceType = UPhysicalMaterial::DetermineSurfaceType(Impact.PhysMaterial.Get());

	if (SurfaceType == SURFACE_FLESHVULNERABLE)
	{
		ActualDamage *= 4.f;
	}

	FPointDamageEvent PointDamage;
	PointDamage.HitInfo = Impact;
	PointDamage.ShotDirection = ShotDirection;
	PointDamage.Damage = ActualDamage;

	Impact.GetActor()->TakeDamage(PointDamage.Damage, PointDamage, MyPawn->Controller, this);
}

void ASWeapon_Instant::StartFiring()
{
	AActor* MyOwner = GetOwner();

	if (MyOwner && RemainingBullets > 0)
	{
		FVector EyeLocation;
		FRotator EyeRotation;
		MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

		const FVector ShotDirection = EyeRotation.Vector();
		const FVector TraceEnd = EyeLocation + (ShotDirection * InstantConfig.WeaponRange);

		const FHitResult Impact = WeaponTrace(EyeLocation, TraceEnd);

		PlayFireEffects();

		UseAmmo();

		if (Impact.bBlockingHit)
		{
			DealDamage(Impact, ShotDirection);
		}
	}
}

void ASWeapon_Instant::SwitchFireMode()
{
	if (InstantConfig.IsSelectiveFireRifle)
	{
		InstantConfig.DefaultFireMode = InstantConfig.DefaultFireMode == EFireMode::Auto
			                                ? EFireMode::Semi_Auto
			                                : EFireMode::Auto;
	}
}
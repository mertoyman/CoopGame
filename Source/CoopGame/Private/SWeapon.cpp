// Fill out your copyright notice in the Description page of Project Settings.


#include "SWeapon.h"

#include "CoopGame/CoopGame.h"
#include "SCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
ASWeapon::ASWeapon()
{
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Component"));
	RootComponent = Root;
	
	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);
	
	bWantsToFire = false;
}

void ASWeapon::BeginPlay()
{
	Super::BeginPlay();

	RemainingBullets = WeaponConfig.AmmoPerClip;
}

void ASWeapon::SetOwningPawn(ASCharacter* WeaponOwner)
{
	if(MyPawn != WeaponOwner)
	{
		SetInstigator(WeaponOwner);
		MyPawn = WeaponOwner;
		SetOwner(WeaponOwner);
	}
}

FHitResult ASWeapon::WeaponTrace(FVector TraceStart, FVector TraceEnd) const
{
	
	FCollisionQueryParams TraceParams(SCENE_QUERY_STAT(WeaponTrace), true, GetInstigator());
	TraceParams.bReturnPhysicalMaterial = true;

	FHitResult HitResult;

	GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, COLLISION_WEAPON, TraceParams);
	
	return HitResult;
}

void ASWeapon::StartFire()
{
	if (!bWantsToFire)
	{
		bWantsToFire = true;
		DetermineWeaponState();
	}
}

void ASWeapon::StopFire()
{
	if (bWantsToFire)
	{
		bWantsToFire = false;
		DetermineWeaponState();
	}
}


void ASWeapon::DetermineWeaponState()
{
	EWeaponState::Type NewState = EWeaponState::Idle;
	if (bWantsToFire)
	{
		NewState = EWeaponState::Firing;
	}
	
	SetWeaponState(NewState);
}

void ASWeapon::SetWeaponState(EWeaponState::Type NewState)
{
	const EWeaponState::Type PrevState = CurrentState;
	
	if (PrevState != EWeaponState::Firing && NewState == EWeaponState::Firing)
	{
		OnBurstStarted();
		
	}
	
	CurrentState = NewState;
	
	if (PrevState == EWeaponState::Firing && NewState != EWeaponState::Firing)
	{		
		OnBurstFinished();
	}
}
void ASWeapon::OnBurstStarted()
{
	HandleFiring();	
}

void ASWeapon::OnBurstFinished()
{
}

void ASWeapon::HandleFiring()
{
	Fire();	
}

void ASWeapon::PlayFireEffects() const
{
	if (MuzzleEffect)
	{
		UGameplayStatics::SpawnEmitterAttached(MuzzleEffect, MeshComp, MuzzleSocketName);
	}
	
	if (TracerEffect)
	{
		const FVector MuzzleLocation = MeshComp->GetSocketLocation(MuzzleSocketName);
		UParticleSystemComponent* TracerComp = UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(), TracerEffect, MuzzleLocation);
	}
	
	APawn* MyOwner = Cast<APawn>(GetOwner());
	if (MyOwner)
	{
		APlayerController* PC = Cast<APlayerController>(MyOwner->GetController());
		if (PC)
		{
			PC->ClientStartCameraShake(FireCamShake);
		}
	}
}

void ASWeapon::UseAmmo()
{
	if (RemainingBullets > 0)
	{
		RemainingBullets--;
	}
}

void ASWeapon::Reload()
{
	RemainingBullets = WeaponConfig.AmmoPerClip;
}

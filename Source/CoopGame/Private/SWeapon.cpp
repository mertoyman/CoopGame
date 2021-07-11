// Fill out your copyright notice in the Description page of Project Settings.


#include "SWeapon.h"

#include "CoopGame/CoopGame.h"
#include "SCharacter.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASWeapon::ASWeapon()
{
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Component"));
	RootComponent = Root;
	
	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMeshComp"));
	MeshComp->SetupAttachment(RootComponent);

	bWantsToFire = false;

	MuzzleSocketName = "WeaponSocket";
	TracerTargetName = "BeamEnd";
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

bool ASWeapon::WeaponTrace(FHitResult& Hit, FVector TraceStart, FVector TraceEnd) const
{
	FCollisionQueryParams TraceParams(SCENE_QUERY_STAT(WeaponTrace), true, GetInstigator());
	TraceParams.bReturnPhysicalMaterial = true;
	return GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, COLLISION_WEAPON, TraceParams);
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
	EWeaponState::State NewState = EWeaponState::Idle;
	if (bWantsToFire)
	{
		NewState = EWeaponState::Firing;
	}
	
	SetWeaponState(NewState);
}

void ASWeapon::SetWeaponState(EWeaponState::State NewState)
{
	const EWeaponState::State PrevState = CurrentState;
	
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

void ASWeapon::PlayFireEffects()
{
	if (MuzzleEffect)
	{
		UGameplayStatics::SpawnEmitterAttached(MuzzleEffect, MeshComp, MuzzleSocketName);
	}
	
	if (TracerEffect)
	{
		MuzzleLocation = MeshComp->GetSocketLocation(MuzzleSocketName);
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

bool ASWeapon::IsSingleHanded()
{
	if (&WeaponConfig)
	{
		return WeaponConfig.IsSingleHanded;	
	}

	return nullptr;
}

void ASWeapon::Reload()
{
	RemainingBullets = WeaponConfig.AmmoPerClip;
}

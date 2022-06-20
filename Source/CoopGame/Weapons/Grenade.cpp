// Fill out your copyright notice in the Description page of Project Settings.


#include "Grenade.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "CoopGame/CoopGame.h"
#include "CoopGame/Interactable/SExplosionEffect.h"
#include "Kismet/GameplayStatics.h"


AGrenade::AGrenade()
{
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	CollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionComp->SetCollisionObjectType(COLLISION_PROJECTILE);
	CollisionComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	CollisionComp->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
	CollisionComp->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	CollisionComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
	RootComponent = CollisionComp;
	
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);

	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComp"));
	MovementComp->UpdatedComponent = CollisionComp;
	MovementComp->InitialSpeed = 2000.0f;
	MovementComp->MaxSpeed = 2000.0f;
	MovementComp->bRotationFollowsVelocity = true;
	MovementComp->bAutoActivate = false;

	PrimaryActorTick.bCanEverTick = true;
}

void AGrenade::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	MovementComp->OnProjectileStop.AddDynamic(this, &AGrenade::OnImpact);
	CollisionComp->MoveIgnoreActors.Add(GetInstigator());

	ASGrenadeLauncher* OwnerWeapon = Cast<ASGrenadeLauncher>(GetOwner());
	if (OwnerWeapon)
	{
		OwnerWeapon->ApplyProjectileData(ProjectileConfig);
	}
	
	MyController = GetInstigatorController();
}

void AGrenade::OnImpact(const FHitResult& HitResult)
{
	Explode(HitResult);
	DisableAndDestroy();
}


void AGrenade::Explode(const FHitResult& Impact)
{
	// effects and damage origin shouldn't be placed inside mesh at impact point
	const FVector NudgedImpactLocation = Impact.ImpactPoint + Impact.ImpactNormal * 10.0f;
	
	if(ProjectileConfig.ExplosionDamage > 0 && ProjectileConfig.ExplosionRadius > 0 && ProjectileConfig.DamageType)
	{
		UGameplayStatics::ApplyRadialDamageWithFalloff(GetWorld(), ProjectileConfig.ExplosionDamage, 25, NudgedImpactLocation, 150, ProjectileConfig.ExplosionRadius,
			0, ProjectileConfig.DamageType,TArray<AActor*>(),this, MyController.Get(), ECC_Visibility);	
	}

	if (ExplosionTemplate)
	{
		const FTransform SpawnTransform(Impact.ImpactNormal.Rotation(), Impact.Location);
		GetWorld()->SpawnActor<ASExplosionEffect>(ExplosionTemplate, SpawnTransform);
	}
}

void AGrenade::DisableAndDestroy()
{
	MovementComp->StopMovementImmediately();
	Destroy();
}

void AGrenade::LaunchProjectile() const
{
	if(MovementComp)
	{
		MovementComp->SetVelocityInLocalSpace(FVector::ForwardVector * MovementComp->InitialSpeed);
		MovementComp->Activate();	
	}
}

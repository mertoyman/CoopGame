// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/STrackerBot.h"

#include <vcruntime_startup.h>
#include <destructible/ModuleDestructible.h>

#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "SCharacter.h"
#include "SExplosionEffect.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ASTrackerBot::ASTrackerBot() :
RequiredDistanceToTarget(50.f),
ForceAmount(300.f),
BaseDamage(500.f),
DamageRadius(300.f),
bExploded(false)
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Comp"));
	MeshComp->SetupAttachment(RootComponent);
	MeshComp->SetCanEverAffectNavigation(false);
	MeshComp->SetSimulatePhysics(true);

	HealthComp = CreateDefaultSubobject<USHealthComponent>(TEXT("HealthComp"));
	HealthComp->OnHealthChanged.AddDynamic(this, &ASTrackerBot::HandleTakeDamage);
}

// Called when the game starts or when spawned
void ASTrackerBot::BeginPlay()
{
	Super::BeginPlay();

	// Find intial move to
	NextPathPoint = GetNextPathPoint();
	
}

FVector ASTrackerBot::GetNextPathPoint()
{
	// Hack to get player location
	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);
	UNavigationPath* NavPath = UNavigationSystemV1::FindPathToActorSynchronously(
		this,
		GetActorLocation(),
		PlayerCharacter);
	
	if (NavPath->PathPoints.Num() > 1 )
	{
		// Return next point in the path
		return NavPath->PathPoints[1];
	}

	
	return GetActorLocation();
}

void ASTrackerBot::HandleTakeDamage(USHealthComponent* OwningHealthComponent, float Health, float DeltaHealth,
	const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	// Explode on hitpoints == 0

	//TODO: Pulse the material on hit
	if (!MatInst)
	{
		MatInst = MeshComp->CreateAndSetMaterialInstanceDynamicFromMaterial(0,MeshComp->GetMaterial(0));
	}
	else
	{
		MatInst->SetScalarParameterValue("LastTimeDamageTaken", GetWorld()->TimeSeconds);
	}

	if(Health <= 0)
	{
		SelfDestruct();
	}
}

void ASTrackerBot::SelfDestruct()
{
	if(bExploded) return;

	bExploded = true;
	
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, GetActorLocation());

	TArray<AActor*> IgnoredActors;
	IgnoredActors.Add(this);

	UGameplayStatics::ApplyRadialDamage(this, BaseDamage, GetActorLocation(), DamageRadius, UDamageType::StaticClass(), IgnoredActors);
	
	// Delete actor immediately
	Destroy();
}

// Called every frame
void ASTrackerBot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	const float DistanceToTarget = FVector::Distance(GetActorLocation(), NextPathPoint);
	
	if (DistanceToTarget >= RequiredDistanceToTarget)
	{
		// Keep moving towards next target
		FVector ForceDirection = NextPathPoint - GetActorLocation();
		ForceDirection.Normalize();

		ForceDirection *= ForceAmount;

		MeshComp->AddForce(ForceDirection, NAME_None, true);
	}
	else
	{
		NextPathPoint = GetNextPathPoint();
	}

}




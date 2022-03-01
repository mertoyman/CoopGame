// Fill out your copyright notice in the Description page of Project Settings.


#include "Explosive.h"

#include "Components/SHealthComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicsEngine/RadialForceComponent.h"

// Sets default values
AExplosive::AExplosive() :
	bExploded(false),
	ExplosionImpulse(400)
{
	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	StaticMeshComp->SetupAttachment(RootComponent);
	StaticMeshComp->SetSimulatePhysics(true);
	
	HealthComp = CreateDefaultSubobject<USHealthComponent>(TEXT("HealthComp"));
	HealthComp->OnHealthChanged.AddDynamic(this, &AExplosive::OnHealthChanged);

	RadialForceComp = CreateDefaultSubobject<URadialForceComponent>(TEXT("RadialForceComp"));
	RadialForceComp->SetupAttachment(RootComponent);
	RadialForceComp->Radius = 250.f;
	RadialForceComp->bImpulseVelChange = true;
	RadialForceComp->bIgnoreOwningActor = true;
	RadialForceComp->bAutoActivate = false;
}

// Called when the game starts or when spawned
void AExplosive::BeginPlay()
{
	Super::BeginPlay();

}

void AExplosive::OnHealthChanged(USHealthComponent* USHealthComponent, float Health, float DeltaHealth,
	const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (Health <= 0.0f && !bExploded)
	{
		bExploded = true;

		if (ExplosionEffect)
		{
			// Play FX on explosion
			UGameplayStatics::SpawnEmitterAtLocation(this, ExplosionEffect, GetActorLocation());

			// Override mesh material on explosion 
			StaticMeshComp->SetMaterial(0, ExplodedMaterial);

			// Boost the explosive upwards
			const FVector Impulse = FVector::ZAxisVector * ExplosionImpulse;
			StaticMeshComp->AddImpulse(Impulse, NAME_None, true);

			// Blast away nearby physics actors
			RadialForceComp->FireImpulse();
			
		}
	}
}


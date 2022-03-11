// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/STrackerBot.h"

#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "SCharacter.h"
#include "SExplosionEffect.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"


// Sets default values
ASTrackerBot::ASTrackerBot() :
RequiredDistanceToTarget(50.f),
ForceAmount(300.f),
ExplosionDamage(500.f),
DamageRadius(300.f),
bExploded(false),
ExplosionTimerInSeconds(3.0f)
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Comp"));
	MeshComp->SetupAttachment(RootComponent);
	MeshComp->SetCanEverAffectNavigation(false);
	MeshComp->SetSimulatePhysics(true);

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Comp"));
	SphereComp->SetupAttachment(RootComponent);
	SphereComp->SetSphereRadius(200);
	SphereComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	
	HealthComp = CreateDefaultSubobject<USHealthComponent>(TEXT("HealthComp"));
	HealthComp->OnHealthChanged.AddDynamic(this, &ASTrackerBot::HandleTakeDamage);
}

// Called when the game starts or when spawned
void ASTrackerBot::BeginPlay()
{
	Super::BeginPlay();

	// Find intial move to
	NextPathPoint = GetNextPathPoint();

	// Every second we update our power-level based on nearby bots
	FTimerHandle TimerHandle_CheckPowerLevel;
	GetWorldTimerManager().SetTimer(TimerHandle_CheckPowerLevel, this, &ASTrackerBot::OnCheckNearbyBots, 1.0f, true);
	
}

// Called every frame
void ASTrackerBot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	OnCheckNearbyBots();

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

FVector ASTrackerBot::GetNextPathPoint()
{
	// Hack to get player location
	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);
	UNavigationPath* NavPath = UNavigationSystemV1::FindPathToActorSynchronously(
		this,
		GetActorLocation(),
		PlayerCharacter);
	
	if (NavPath && NavPath->PathPoints.Num() > 1 )
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
	
	UGameplayStatics::PlaySoundAtLocation(this, ExplosionSound, GetActorLocation());
	
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, GetActorLocation());

	TArray<AActor*> IgnoredActors;
	IgnoredActors.Add(this);

	// Increase damage based on the power level
	const float ActualDamage = ExplosionDamage + (PowerLevel * ExplosionDamage); 

	UGameplayStatics::ApplyRadialDamage(this, ActualDamage, GetActorLocation(), DamageRadius, UDamageType::StaticClass(), IgnoredActors);
	
	// Delete actor immediately
	Destroy();
}


void ASTrackerBot::NotifyActorBeginOverlap(AActor* OtherActor)
{
	if(!bStartedSelfDestruction)
	{
		ASCharacter* PlayerPawn = Cast<ASCharacter>(OtherActor);
		if (PlayerPawn)
		{
			// Start self destruction sequence
			GetWorldTimerManager().SetTimer(TimerHandle_SelfDamage, this, &ASTrackerBot::DamageSelf, ExplosionTimerInSeconds, true);
	
			bStartedSelfDestruction = true;
			
			UGameplayStatics::SpawnSoundAttached(SelfDestructSound, RootComponent);
		}
	}
}

void ASTrackerBot::DamageSelf()
{
	UGameplayStatics::ApplyDamage(this, ExplosionDamage, GetInstigatorController(), this, UDamageType::StaticClass());

	UGameplayStatics::PlaySoundAtLocation(this, ExplosionSound, GetActorLocation());
}

void ASTrackerBot::OnCheckNearbyBots()
{
	// Distance to check for nearby bots
	constexpr float Radius = 600.f;

	// Create temporary collision shape for overlaps
	FCollisionShape CollShape;
	CollShape.SetSphere(Radius);

	// Only find Pawns (eg. players and AI bots)
	FCollisionObjectQueryParams QueryParams;
	// Our tracker bot's mesh component is set to Physics Body in Blueprint (default profile of physics simulated actors)
	QueryParams.AddObjectTypesToQuery(ECC_PhysicsBody);
	QueryParams.AddObjectTypesToQuery(ECC_Pawn);

	TArray<FOverlapResult> Overlaps;
	GetWorld()->OverlapMultiByObjectType(Overlaps, GetActorLocation(), FQuat::Identity, QueryParams, CollShape);

	int32 NumberOfBots = 0;

	// Loop over the results using a "range based for loop"
	for (auto Result : Overlaps)
	{
		// Check if overlapped with another tracker bot (ignoring players and other bot types)
		ASTrackerBot* Bot = Cast<ASTrackerBot>(Result.GetActor());
		if (Bot && Bot != this)
		{
			NumberOfBots++;
		}
	}

	constexpr int32 MaxPowerLevel = 4;

	// Clamp between min=0, max=4
	PowerLevel = FMath::Clamp(NumberOfBots, 0, MaxPowerLevel);

	// Update material color
	if (!MatInst)
	{
		MatInst = MeshComp->CreateAndSetMaterialInstanceDynamicFromMaterial(0,MeshComp->GetMaterial(0));
	}
	else
	{
		const float Alpha = PowerLevel / static_cast<float>(MaxPowerLevel);
		
		MatInst->SetScalarParameterValue("PowerLevelAlpha", Alpha);
	}
}



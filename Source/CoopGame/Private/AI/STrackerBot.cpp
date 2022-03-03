// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/STrackerBot.h"

#include "DrawDebugHelpers.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "SCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASTrackerBot::ASTrackerBot() :
ForceAmount(300.f),
RequiredDistanceToTarget(50.f)
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Comp"));
	MeshComp->SetupAttachment(RootComponent);
	MeshComp->SetCanEverAffectNavigation(false);
	MeshComp->SetSimulatePhysics(true);
}

// Called when the game starts or when spawned
void ASTrackerBot::BeginPlay()
{
	Super::BeginPlay();

	NextPathPoint = GetNextPathPoint();
	
}

FVector ASTrackerBot::GetNextPathPoint()
{
	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);
	UNavigationPath* NavPath = UNavigationSystemV1::FindPathToActorSynchronously(
		this,
		GetActorLocation(),
		PlayerCharacter);
	
	if (NavPath->PathPoints.Num() > 1 )
	{
		return NavPath->PathPoints[1];
	}

	
	return GetActorLocation();
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




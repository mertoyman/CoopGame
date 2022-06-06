// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"
#include "Components/CapsuleComponent.h"
#include "SPawnExtensionComponent.h"

ASCharacter::ASCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Avoid ticking characters if possible.
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	UCapsuleComponent* CapsuleComp = GetCapsuleComponent();
	check(CapsuleComp);
	CapsuleComp->InitCapsuleSize(40.f, 90.f);

	USkeletalMeshComponent* MeshComp = GetMesh();
	check(MeshComp);
	MeshComp->SetRelativeRotation(FRotator(0.f, -90.f, 0.f)); // Rotate mesh to be X forward since it's exported as Y forward.

	PawnExtComponent = CreateDefaultSubobject<USPawnExtensionComponent>(TEXT("PawnExtensionComponent"));
}

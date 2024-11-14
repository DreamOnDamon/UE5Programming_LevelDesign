// Fill out your copyright notice in the Description page of Project Settings.


#include "DodgeballFunctionLibrary.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "CollisionQueryParams.h"

bool UDodgeballFunctionLibrary :: CanSeeActor(const UWorld* World,
	FVector Location,
	const AActor* TargetActor,
	TArray<const AActor*> IgnoreActors) 
{
	if (TargetActor == nullptr) return false;

	FHitResult Hit;
	FVector TraceLocationStart = Location;
	FVector TraceLocationEnd = TargetActor->GetActorLocation();

	ECollisionChannel Channel = ECollisionChannel::ECC_GameTraceChannel1;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActors(IgnoreActors);
	World->LineTraceSingleByChannel(Hit, TraceLocationStart, TraceLocationEnd, Channel, QueryParams);

	DrawDebugLine(World, TraceLocationStart, TraceLocationEnd, FColor::Red);

	return !Hit.bBlockingHit;
}

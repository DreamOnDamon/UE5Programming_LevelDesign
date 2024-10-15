// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void AEnemyCharacter::LookAtActor(AActor* TargetActor)
{
	if (TargetActor == nullptr) return;
	if (CanSeeActor(TargetActor)) {
		FVector Start = GetActorLocation();
		FVector End = TargetActor->GetActorLocation();

		FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(Start, End);
		SetActorRotation(LookAtRotation);
	 }

}

bool AEnemyCharacter::CanSeeActor(const AActor* TargetActor) const
{
	if (TargetActor == nullptr) return false;

	FHitResult Hit;
	FVector TraceLocationStart = GetActorLocation();
	FVector TraceLocationEnd = TargetActor->GetActorLocation();

	ECollisionChannel Channel = ECollisionChannel::ECC_Visibility;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	QueryParams.AddIgnoredActor(TargetActor);
	GetWorld()->LineTraceSingleByChannel(Hit, TraceLocationStart, TraceLocationEnd, Channel, QueryParams);

	DrawDebugLine(GetWorld(), TraceLocationStart, TraceLocationEnd, FColor::Red);

	return !Hit.bBlockingHit;
}

// Called every frame
void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);
	LookAtActor(PlayerCharacter);
}

// Called to bind functionality to input
void AEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


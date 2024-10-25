// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"
#include "Engine/World.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "Dodgeball.h"
#include "DodgeballFunctionLibrary.h"
#include "GameFrameWork/ProjectileMovementComponent.h"

// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SightSource = CreateDefaultSubobject<USceneComponent>(TEXT("SightSource"));
	SightSource->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

bool AEnemyCharacter::LookAtActor(AActor* TargetActor)
{
	if (TargetActor == nullptr) return false;

	TArray<const AActor*> IgnoreActors = {this, TargetActor};

	if (UDodgeballFunctionLibrary::CanSeeActor(GetWorld(), SightSource->GetComponentLocation(), TargetActor,IgnoreActors)){
		FVector Start = GetActorLocation();
		FVector End = TargetActor->GetActorLocation();
		FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(Start, End);
		SetActorRotation(LookAtRotation);

		return true;
	 }

	return false;
}

//bool AEnemyCharacter::CanSeeActor(const AActor* TargetActor) const
//{
//	if (TargetActor == nullptr) return false;
//
//	FHitResult Hit;
//	FVector TraceLocationStart = SightSource->GetComponentLocation();
//	FVector TraceLocationEnd = TargetActor->GetActorLocation();
//
//	ECollisionChannel Channel = ECollisionChannel::ECC_GameTraceChannel1;
//	FCollisionQueryParams QueryParams;
//	QueryParams.AddIgnoredActor(this);
//	QueryParams.AddIgnoredActor(TargetActor);
//	GetWorld()->LineTraceSingleByChannel(Hit, TraceLocationStart, TraceLocationEnd, Channel, QueryParams);
//
//	DrawDebugLine(GetWorld(), TraceLocationStart, TraceLocationEnd, FColor::Red);

	/*FQuat Rotation = FQuat::Identity;
	FCollisionShape Shape = FCollisionShape::MakeBox(FVector(20.f, 20.f, 20.f));
	GetWorld()->SweepSingleByChannel(Hit,
		TraceLocationStart,
		TraceLocationEnd,
		Rotation,
		Channel,
		Shape);*/
//
//	return !Hit.bBlockingHit;
//}

void AEnemyCharacter::ThrowDodgaball()
{
	if (DodgeballClass == nullptr) return;
	FVector ForwardVector = GetActorForwardVector();
	float SpawnDistance = 40.f;
	FVector SpawnLocation = GetActorLocation() + ForwardVector * SpawnDistance;
	//GetWorld()->SpawnActor<ADodgeball>(DodgeballClass, SpawnLocation, GetActorRotation())

	// The tansform use to spawn the actor(Rotation, Location).
	FTransform SpawnTransform = FTransform(GetActorRotation(), SpawnLocation);
	// Spawn the Dedgeball
	ADodgeball* Ball = GetWorld()->SpawnActorDeferred<ADodgeball>(DodgeballClass, SpawnTransform);
	// Set the Initial Speed for the dodgeball. 
	Ball->GetProjectileMovementComponent()->InitialSpeed = 2000.f;
	// We can config the dodgeball before we call this functio, after we calling this function, the ball will be spawned
	Ball->FinishSpawning(SpawnTransform);
}

// Called every frame
void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);
	bCanSeeActor = LookAtActor(PlayerCharacter);
	if (bCanSeeActor != bCanSeeActorPrevious) { // If CanSee state is different from the last tick
		if (bCanSeeActor) {
			// Start throwing dodgeball towards player
			GetWorldTimerManager().SetTimer(ThrowTimerHandle, this, &AEnemyCharacter::ThrowDodgaball,
				ThrowingInterval, true, ThrowingDelay);
		}
		else {
			// Otherwise stop throwing dodgeball
			GetWorldTimerManager().ClearTimer(ThrowTimerHandle);
		}
	}
	bCanSeeActorPrevious = bCanSeeActor;
}

// Called to bind functionality to input
void AEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


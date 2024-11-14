// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/SceneComponent.h"
#include "EnemyCharacter.generated.h"

UCLASS()
class PROGRAMMINGLEARNING_API AEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = LookAt, meta = (AllowPrivateAccess = "true"))
	
	class ULookAtActorComponent* LookAtActorComponent;

	FTimerHandle ThrowTimerHandle;

	float ThrowingInterval = 2.f;

	float ThrowingDelay = 1.f;

	bool bCanSeeActor = false;
	bool bCanSeeActorPrevious = false;

public:
	// A class used to spawn a dodgeball object
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Dogedball)
	TSubclassOf<class ADodgeball> DodgeballClass;
	// Sets default values for this character's properties
	AEnemyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//bool LookAtActor(AActor* TargetActor);

	//bool CanSeeActor(const AActor* TargetActor) const;

	void ThrowDodgaball();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};

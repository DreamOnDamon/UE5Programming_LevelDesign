// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "LookAtActorComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROGRAMMINGLEARNING_API ULookAtActorComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ULookAtActorComponent();

	FORCEINLINE void SetTarget(AActor* NewTarget) { TargetActor = NewTarget; }

	FORCEINLINE bool CanSeeTarget() const { return bCanSeeActor; }

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	bool LookAtActor();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	AActor* TargetActor;
	bool bCanSeeActor = false;
		
};

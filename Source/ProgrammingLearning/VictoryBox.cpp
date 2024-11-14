// Fill out your copyright notice in the Description page of Project Settings.


#include "VictoryBox.h"
#include "Components/BoxComponent.h"
#include "Dodgeball.h"
#include "MyThirdPersonChar.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetStringLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"


// Sets default values
AVictoryBox::AVictoryBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = RootScene;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBox->SetupAttachment(RootComponent);
	CollisionBox->SetBoxExtent(FVector(60.f, 60.f, 60.f));
	CollisionBox->SetRelativeLocation(FVector(0.f, 0.f, 120.f));

	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AVictoryBox::OnBeginOverlap);
	//FName("OnBeginOverlap")
}

void AVictoryBox::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	if (Cast<AMyThirdPersonChar>(OtherActor)) {
		UKismetSystemLibrary::QuitGame(
			GetWorld(), 
			UGameplayStatics::GetPlayerController(GetWorld(), 0),
			EQuitPreference::Quit,
			true);
	}

}

// Called when the game starts or when spawned
void AVictoryBox::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AVictoryBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


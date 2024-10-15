// Fill out your copyright notice in the Description page of Project Settings.


#include "PracticeCharacterFile.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
APracticeCharacterFile::APracticeCharacterFile()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->SetCapsuleSize(45.f, 96.f);
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 500.f;
	CameraBoom->SocketOffset = FVector(0.f, 0.f, 50.f);
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom,USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	GetCharacterMovement()->bOrientRotationToMovement = false;
}

// Called when the game starts or when spawned
void APracticeCharacterFile::BeginPlay()
{
	Super::BeginPlay();
	
}

void APracticeCharacterFile::Move(const FInputActionValue& Value)
{
	FVector2D InputValue = Value.Get<FVector2D>();
	if (Controller != nullptr && (InputValue.X != 0.f || InputValue.Y !=0.f)) {
		const FRotator YawRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);
		if (InputValue.X != 0.f)
		{
			const FVector RightDirection = UKismetMathLibrary::GetRightVector(YawRotation);
			AddMovementInput(RightDirection, InputValue.X);
		}

		if (InputValue.Y != 0.f) {
			const FVector ForwardDirection = YawRotation.Vector();
			AddMovementInput(ForwardDirection, InputValue.Y);
		}
	}
}

// Called every frame
void APracticeCharacterFile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APracticeCharacterFile::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedPlayerInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	if (EnhancedPlayerInputComponent != nullptr) {
		APlayerController* PlayerController = Cast<APlayerController>(GetController());

		if (PlayerController != nullptr) {

			UEnhancedInputLocalPlayerSubsystem* EnhancedSubsystem =
				ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());

			if (EnhancedSubsystem != nullptr) {
				EnhancedSubsystem->AddMappingContext(IMC_Character, 1);
			}
		}
		EnhancedPlayerInputComponent->BindAction(IA_Move, ETriggerEvent::Triggered, this, &APracticeCharacterFile::Move);
	}

}


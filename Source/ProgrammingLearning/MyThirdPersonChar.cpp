// Fill out your copyright notice in the Description page of Project Settings.


#include "MyThirdPersonChar.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/Controller.h"
#include "DodgeballPlayerController.h"

// Sets default values
AMyThirdPersonChar::AMyThirdPersonChar()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// Set the size of the collision capsule.
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);
	// Don't rotate when the controller rotates.Let that just affext the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	// Configure character movement.
	GetCharacterMovement()->bOrientRotationToMovement = true;

	// Create a camera boom (pulls in towards the player if there is a collision).
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SocketOffset = FVector(0.f, 0.f, 55.f);
	CameraBoom->TargetArmLength = 900.f;
	CameraBoom->SetRelativeRotation(FRotator(-70.f, 0.f, 0.f));
	CameraBoom->bUsePawnControlRotation = false;
	CameraBoom->bInheritPitch = false;
	CameraBoom->bInheritRoll = false;
	CameraBoom->bInheritYaw = false;
	CameraBoom->bEnableCameraLag = true;
	CameraBoom->CameraLagSpeed = 2.5f;
	CameraBoom->CameraLagMaxDistance = 125.f;

	// Enhanced input sysytem initilization
	IA_Jump = nullptr;
	IA_Look = nullptr;
	IA_Move = nullptr;
	IA_Walk = nullptr;
	IC_Character = nullptr;

	// Create a camera that will follow the character. 
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom,USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	// Create the health component for character
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
}

void AMyThirdPersonChar::OnDeath_Implementation()
{
	//UKismetSystemLibrary::QuitGame(this, nullptr, EQuitPreference::Quit, true);
	ADodgeballPlayerController* PlayerController = Cast<ADodgeballPlayerController>(GetController());
	if (PlayerController != nullptr)
	{
		PlayerController->ShowRestartWidget();
	}
}

void AMyThirdPersonChar::OnTakeDamage_Implementation()
{
	ADodgeballPlayerController* PlayerController = Cast<ADodgeballPlayerController>(GetController());
	if (PlayerController != nullptr)
	{
		PlayerController->UpdateHealthPercent(HealthComponent->GetHealthPercent());
	}
}

// Called when the game starts or when spawned
void AMyThirdPersonChar::BeginPlay()
{
	Super::BeginPlay();

	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("BeginPlay"));
	
}

void AMyThirdPersonChar::Move(const FInputActionValue& Value) {
	FVector2D InputValue = Value.Get<FVector2D>();
	if (Controller != nullptr && (InputValue.X != 0.f || InputValue.Y != 0.f)) {

		const FRotator YawRotation(0,Controller->GetControlRotation().Yaw, 0);
		
		if (InputValue.X != 0.f) {
			const FVector RightDirection = UKismetMathLibrary::GetRightVector(YawRotation);
			AddMovementInput(RightDirection, InputValue.X);
		}
		if (InputValue.Y != 0.f) {
			const FVector ForwardDirection = YawRotation.Vector();
			AddMovementInput(ForwardDirection, InputValue.Y);
		}
	}
}

void AMyThirdPersonChar::Look(const FInputActionValue& Value)
{
	/*FVector2D InputValue = Value.Get<FVector2D>();

	if (InputValue.X != 0.f) {
		AddControllerYawInput(InputValue.X);
	}
	if (InputValue.Y != 0.f) {
		AddControllerPitchInput(InputValue.Y);
	}*/
}

void AMyThirdPersonChar::BeginWalking()
{
	GetCharacterMovement()->MaxWalkSpeed *= 0.4f;
}

void AMyThirdPersonChar::StopWalking()
{
	GetCharacterMovement()->MaxWalkSpeed *= 2.5f;
}

// Called every frame
void AMyThirdPersonChar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMyThirdPersonChar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// To tell Engine that use EnhancedInput system for this project.
	UEnhancedInputComponent* EnhancedPlayerInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	
	// If Cast successfully, transfer Pawn Controller into a Player Controller.
	if (EnhancedPlayerInputComponent != nullptr) {
		APlayerController* PlayerController = Cast<APlayerController>(GetController());
		if (PlayerController != nullptr) {
			UEnhancedInputLocalPlayerSubsystem* EnhancedSubsystem =
				ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
			if (EnhancedSubsystem != nullptr) {
				EnhancedSubsystem->AddMappingContext(IC_Character, 1);
			}
		}
		EnhancedPlayerInputComponent->BindAction(IA_Move, ETriggerEvent::Triggered, this, &AMyThirdPersonChar::Move);
		EnhancedPlayerInputComponent->BindAction(IA_Jump, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedPlayerInputComponent->BindAction(IA_Jump, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		//EnhancedPlayerInputComponent->BindAction(IA_Look, ETriggerEvent::Triggered, this, &AMyThirdPersonChar::Look);
		EnhancedPlayerInputComponent->BindAction(IA_Walk, ETriggerEvent::Started, this, &AMyThirdPersonChar::BeginWalking);
		EnhancedPlayerInputComponent->BindAction(IA_Walk, ETriggerEvent::Completed, this, &AMyThirdPersonChar::StopWalking);
	}

}


// Fill out your copyright notice in the Description page of Project Settings.


#include "RestartWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"
#include "DodgeballPlayerController.h"

void URestartWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();
    if (RestartButton != nullptr)
    {
        RestartButton->OnClicked.AddDynamic(this, &URestartWidget::OnRestartClicked);
    }

    if (ExitButton != nullptr)
    {
        ExitButton->OnClicked.AddDynamic(this, &URestartWidget::OnExitClicked);
    }
}

void URestartWidget::OnRestartClicked()
{
    UGameplayStatics::OpenLevel(this, FName(*UGameplayStatics::GetCurrentLevelName(this)));
    ADodgeballPlayerController* PlayerController = Cast<ADodgeballPlayerController>(GetOwningPlayer());
    if (PlayerController != nullptr)
    {
        PlayerController->HideRestartWidget();
    }
}

void URestartWidget::OnExitClicked()
{
    UKismetSystemLibrary::QuitGame(
        GetWorld(),
        UGameplayStatics::GetPlayerController(GetWorld(), 0), 
        EQuitPreference::Quit, 
        true);
}

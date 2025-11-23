// Copyright (C) Microsoft Corporation.  
// Copyright (C) 2025 IAMAI CONSULTING CORP
//
// MIT License. All rights reserved.

#include "ProjectAirSimGameMode.h"

#include <exception>

#include "Runtime/Core/Public/Misc/Paths.h"

AProjectAirSimGameMode::AProjectAirSimGameMode(
    const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer),
      UnrealSimLoader(FPaths::ConvertRelativePathToFull(FPaths::ProjectDir())) {
  DefaultPawnClass = nullptr;
  FApp::bUseFixedSeed = true;  // for determinism, persists in UE project
}

void AProjectAirSimGameMode::StartPlay() {
  Super::StartPlay();

  UnrealSimLoader.LaunchSimulation(this->GetWorld());

  if (!SettingsMenuClass)
  {
    FStringClassReference WidgetClassRef(TEXT("/Game/UI/WBP_SettingsMenu.WBP_SettingsMenu_C"));
    UClass* WidgetClass = WidgetClassRef.TryLoadClass<USettingsMenu>();
    if (WidgetClass)
    {
      SettingsMenuClass = WidgetClass;
    }
  }

  UE_LOG(LogTemp, Warning, TEXT("WIDGET: Attempting to add."));
  if (SettingsMenuClass)
  {
    UE_LOG(LogTemp, Warning, TEXT("WIDGET: Past if SettingsMenuClass statement."));
    if (!SettingsMenuInstance)
    {
      UE_LOG(LogTemp, Warning, TEXT("WIDGET: Past if !SettingsMenuInstance statement."));
      APlayerController* PC = GetWorld()->GetFirstPlayerController();
      if (PC)
      {
        UE_LOG(LogTemp, Warning, TEXT("WIDGET: Past if PC statement."));
        SettingsMenuInstance = CreateWidget<USettingsMenu>(PC, SettingsMenuClass);
        if (SettingsMenuInstance) 
        {
          UE_LOG(LogTemp, Warning, TEXT("WIDGET: Past if SettingsMenuInstance statement."));
          SettingsMenuInstance->AddToViewport();
          FInputModeUIOnly InputMode;
          InputMode.SetWidgetToFocus(SettingsMenuInstance->TakeWidget());
          PC->SetInputMode(InputMode);
          PC->bShowMouseCursor = true;
        }
      }
    }
  }
    
}

void AProjectAirSimGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason) {
  Super::EndPlay(EndPlayReason);

  UnrealSimLoader.TeardownSimulation();
  FApp::bUseFixedSeed = false;  // reset back to default from App.cpp
}

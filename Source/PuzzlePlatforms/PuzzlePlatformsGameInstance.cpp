// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzlePlatformsGameInstance.h"
#include "PlatformTrigger.h"
#include "Blueprint/UserWidget.h"

UPuzzlePlatformsGameInstance::UPuzzlePlatformsGameInstance()
{
	ConstructorHelpers::FClassFinder<UUserWidget> BPMenuClass(TEXT("/Game/MenuSystem/WBP_MainMenu"));
	
	MenuClass = BPMenuClass.Class;
}

void UPuzzlePlatformsGameInstance::Init()
{
	Super::Init();

	UE_LOG(LogTemp, Warning, TEXT("FoundClass %s"), *MenuClass->GetName());
}

void UPuzzlePlatformsGameInstance::LoadMenu()
{
	UUserWidget* Menu = CreateWidget<UUserWidget>(this, MenuClass);
	if (!Menu) return;
	Menu->AddToViewport();
	Menu->bIsFocusable = true;

	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!PlayerController) return;
	
	FInputModeUIOnly InputModeData;
	InputModeData.SetWidgetToFocus(Menu->TakeWidget());
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

	PlayerController->SetInputMode(InputModeData);
	PlayerController->bShowMouseCursor = true;
}

void UPuzzlePlatformsGameInstance::Host()
{
	GEngine->AddOnScreenDebugMessage(0, 5.f, FColor::Green, TEXT("Hosting"));

	UWorld* World = GetWorld();
	if (World)
	{
		World->ServerTravel("/Game/ThirdPerson/Maps/ThirdPersonMap?listen");
	}
}

void UPuzzlePlatformsGameInstance::Join(const FString& Address)
{
	GEngine->AddOnScreenDebugMessage(0, 5.f, FColor::Green, FString::Printf(TEXT("Joining %s"), *Address));

	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!PlayerController) return;
	
	PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
}

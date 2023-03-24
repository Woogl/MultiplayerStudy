// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzlePlatformsGameInstance.h"
#include "PlatformTrigger.h"
#include "Blueprint/UserWidget.h"
#include "MenuSystem/MainMenu.h"
#include "MenuSystem/InGameMenu.h"
#include "MenuSystem/MenuWidget.h"
#include "OnlineSubsystem.h"

UPuzzlePlatformsGameInstance::UPuzzlePlatformsGameInstance()
{
	ConstructorHelpers::FClassFinder<UUserWidget> BPMenuClass(TEXT("/Game/MenuSystem/WBP_MainMenu"));
	if (BPMenuClass.Succeeded())
	{
		MenuClass = BPMenuClass.Class;
	}

	ConstructorHelpers::FClassFinder<UUserWidget> BPInGameMenuClass(TEXT("/Game/MenuSystem/WBP_InGameMenu"));
	if (BPInGameMenuClass.Succeeded())
	{
		InGameMenuClass = BPInGameMenuClass.Class;
	}
}

void UPuzzlePlatformsGameInstance::Init()
{
	Super::Init();

	IOnlineSubsystem* SubSystem = IOnlineSubsystem::Get();
	if (!SubSystem)
	{
		UE_LOG(LogTemp, Warning, TEXT("Found %s"), *SubSystem->GetSubsystemName().ToString());
	}
	else
	{
		MYLOG("subsystem null");
	}
}

void UPuzzlePlatformsGameInstance::LoadInGameMenu()
{
	UMenuWidget* NewMenu = CreateWidget<UMenuWidget>(this, InGameMenuClass);
	if (!NewMenu) return;

	NewMenu->Setup();
	NewMenu->SetMenuInterface(this);
}

void UPuzzlePlatformsGameInstance::Host()
{
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

void UPuzzlePlatformsGameInstance::LoadMainMenu()
{
	MYDEBUG("LoadMainMenu");
	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!PlayerController) return;
	
	PlayerController->ClientTravel("/Game/Maps/MainMenu", ETravelType::TRAVEL_Absolute);
}

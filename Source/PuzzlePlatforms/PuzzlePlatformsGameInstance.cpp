// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzlePlatformsGameInstance.h"
#include "PlatformTrigger.h"
#include "Blueprint/UserWidget.h"
#include "MenuSystem/MainMenu.h"
#include "MenuSystem/InGameMenu.h"
#include "MenuSystem/MenuWidget.h"
#include <../Plugins/Online/OnlineSubsystem/Source/Public/OnlineSessionSettings.h>
#include <../Plugins/Online/OnlineSubsystem/Source/Public/Interfaces/OnlineSessionInterface.h>

const static FName SESSION_NAME = TEXT("My Session Name");

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
	if (SubSystem != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Found %s"), *SubSystem->GetSubsystemName().ToString());
		SessionInterface = SubSystem->GetSessionInterface();
		if (SessionInterface.IsValid())
		{
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UPuzzlePlatformsGameInstance::OnCreateSessionComplete);
			SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UPuzzlePlatformsGameInstance::OnDestroySessionComplete);
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UPuzzlePlatformsGameInstance::OnFindSessionComplete);

			SessionSearch = MakeShareable(new FOnlineSessionSearch());
			if (SessionSearch.IsValid())
			{
				SessionSearch->bIsLanQuery = true;
				MYDEBUG("Starting Find Session");
				SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
			}
		}
	}
	else
	{
		MYLOG("Fail to load subsystem");
	}
}

void UPuzzlePlatformsGameInstance::LoadMenu()
{
	if (!ensure(MenuClass != nullptr)) return;

	Menu = CreateWidget<UMainMenu>(this, MenuClass);
	if (!ensure(Menu != nullptr)) return;

	Menu->Setup();
	Menu->SetMenuInterface(this);
}

void UPuzzlePlatformsGameInstance::LoadInGameMenu()
{
	UMenuWidget* MenuWidget = CreateWidget<UMenuWidget>(this, InGameMenuClass);
	if (!Menu) return;

	MenuWidget->Setup();
	MenuWidget->SetMenuInterface(this);
}

void UPuzzlePlatformsGameInstance::Host()
{
	if (SessionInterface.IsValid())
	{
		FNamedOnlineSession* ExistingSession = SessionInterface->GetNamedSession(SESSION_NAME);
		if (ExistingSession != nullptr)
		{
			SessionInterface->DestroySession(SESSION_NAME);
		}
		else
		{
			CreateSession();
		}
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

void UPuzzlePlatformsGameInstance::OnCreateSessionComplete(FName SessionName, bool Success)
{
	if (!Success)
	{
		MYDEBUG("Could not create session");
		return;
	}
	UWorld* World = GetWorld();
	if (World)
	{
		World->ServerTravel("/Game/ThirdPerson/Maps/ThirdPersonMap?listen");
	}
}

void UPuzzlePlatformsGameInstance::OnDestroySessionComplete(FName SessionName, bool Success)
{
	if (Success)
	{
		CreateSession();
	}
}

void UPuzzlePlatformsGameInstance::OnFindSessionComplete(bool Success)
{
	if (Success && SessionSearch.IsValid())
	{
		for (const FOnlineSessionSearchResult& SearchResult : SessionSearch->SearchResults)
		{
			MYLOG("Finished Found Session");
			UE_LOG(LogTemp, Warning, TEXT("Session ID: %s"), *SearchResult.GetSessionIdStr());
		}

	}
}

void UPuzzlePlatformsGameInstance::CreateSession()
{
	if (SessionInterface.IsValid())
	{
		FOnlineSessionSettings SessionSettings;
		SessionSettings.bIsLANMatch = true;
		SessionSettings.NumPublicConnections = 2;
		SessionSettings.bShouldAdvertise = true;
		SessionInterface->CreateSession(0, SESSION_NAME, SessionSettings);
	}
}

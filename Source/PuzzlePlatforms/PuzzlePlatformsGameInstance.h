// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MenuSystem/MenuInterface.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "PuzzlePlatformsGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API UPuzzlePlatformsGameInstance : public UGameInstance, public IMenuInterface
{
	GENERATED_BODY()

public:
	UPuzzlePlatformsGameInstance();

	virtual void Init();

	UFUNCTION(BlueprintCallable)
	void LoadMenuWidget();

	UFUNCTION(BlueprintCallable)
	void LoadInGameMenu();

	// Menu Interface Functions
	UFUNCTION(Exec)
	void Host(FString ServerName);

	UFUNCTION(Exec)
	void Join(uint32 Index);

	void StartSession();

	UFUNCTION(BlueprintCallable)
	virtual void LoadMainMenu() override;

	UFUNCTION()
	void RefreshServerList() override;

private:
	TSubclassOf<class UUserWidget> MenuClass;
	TSubclassOf<class UUserWidget> InGameMenuClass;

	class UMainMenu* Menu;

	void OnCreateSessionComplete(FName SessionName, bool Success);
	void OnDestroySessionComplete(FName SessionName, bool Success);
	void OnFindSessionComplete(bool Success);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	void OnNetWorkFailure(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorString);
	void CreateSession();

	IOnlineSessionPtr SessionInterface;
	TSharedPtr<class FOnlineSessionSearch> SessionSearch;
	FString DesiredServerName;
};

// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameMenu.h"
#include <UMG/Public/Components/Button.h>

bool UInGameMenu::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return false;

	if (CancelButton)
	{
		CancelButton->OnClicked.AddDynamic(this, &UInGameMenu::CancelPressed);
	}

	if (QuitButton)
	{
		QuitButton->OnClicked.AddDynamic(this, &UInGameMenu::QuitPressed);
	}

	return true;
}

void UInGameMenu::CancelPressed()
{
	TearDown();
}

void UInGameMenu::QuitPressed()
{
	if (MenuInterface)
	{
		MenuInterface->LoadMainMenu();
		TearDown();
	}
}
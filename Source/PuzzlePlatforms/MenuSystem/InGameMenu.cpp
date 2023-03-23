// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameMenu.h"
#include <UMG/Public/Components/Button.h>

bool UInGameMenu::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return false;

	return true;

	if (CancleButton)
	{
		CancleButton->OnClicked.AddDynamic(this, &UInGameMenu::Cancle);
	}

	if (QuitButton)
	{
		QuitButton->OnClicked.AddDynamic(this, &UInGameMenu::OpenMainMenu);
	}
}

void UInGameMenu::Cancle()
{

}

void UInGameMenu::OpenMainMenu()
{

}
// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableTextBox.h"

bool UMainMenu::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return false;

	if (HostButton)
	{
		HostButton->OnClicked.AddDynamic(this, &UMainMenu::HostServer);
	}
	
	if (JoinButton)
	{
		JoinButton->OnClicked.AddDynamic(this, &UMainMenu::OpenJoinMenu);
	}

	if (CancleJoinButton)
	{
		CancleJoinButton->OnClicked.AddDynamic(this, &UMainMenu::OpenMainMenu);
	}

	if (ConfirmJoinButton)
	{
		ConfirmJoinButton->OnClicked.AddDynamic(this, &UMainMenu::JoinServer);
	}

	return true;
}

void UMainMenu::HostServer()
{
	if (MenuInterface)
	{
		MenuInterface->Host();
	}
}

void UMainMenu::JoinServer()
{
	if (MenuInterface || IPAdressField)
	{
		const FString& Address = IPAdressField->GetText().ToString();
		MenuInterface->Join(Address);
	}
}

void UMainMenu::OpenJoinMenu()
{
	if (!MenuSwitcher || !JoinMenu) return;
	MenuSwitcher->SetActiveWidget(JoinMenu);
}

void UMainMenu::OpenMainMenu()
{
	if (!MenuSwitcher || !MainMenu) return;
	MenuSwitcher->SetActiveWidget(MainMenu);
}


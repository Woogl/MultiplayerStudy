// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableTextBox.h"
#include "ServerRow.h"
#include <UMG/Public/Components/TextBlock.h>

UMainMenu::UMainMenu()
{
	ConstructorHelpers::FClassFinder<UUserWidget> ServerRowBPClass(TEXT("/Game/MenuSystem/WBP_ServerRow"));
	if (ServerRowBPClass.Succeeded())
	{
		ServerRowClass = ServerRowBPClass.Class;
	}
}

void UMainMenu::SetServerList(TArray<FServerData> ServerNames)
{
	UWorld* World = this->GetWorld();
	if (World == nullptr) return;

	ServerList->ClearChildren();

	uint32 i = 0;
	for (const FServerData& ServerData : ServerNames)
	{
		UServerRow* Row = CreateWidget<UServerRow>(this, ServerRowClass);
		if (!ensure(Row != nullptr)) return;

		Row->ServerName->SetText(FText::FromString(ServerData.Name));
		Row->HostUser->SetText(FText::FromString(ServerData.HostUsername));
		FString FractionText = FString::Printf(TEXT("%d/%d"), ServerData.CurrentPlayers, ServerData.MaxPlayers);
		Row->ConnectionFraction->SetText(FText::FromString(FractionText));
		Row->Setup(this, i);
		++i;

		ServerList->AddChild(Row);
	}
}

void UMainMenu::SelectIndex(uint32 Index)
{
	SelectedIndex = Index;
	UpdateChildren();
}

bool UMainMenu::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return false;

	if (HostButton)
	{
		HostButton->OnClicked.AddDynamic(this, &UMainMenu::OpenHostMenu);
	}

	if (CancelHostMenuButton)
	{
		CancelHostMenuButton->OnClicked.AddDynamic(this, &UMainMenu::OpenMainMenu);
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

	if (ConfirmHostMenuButton)
	{
		ConfirmHostMenuButton->OnClicked.AddDynamic(this, &UMainMenu::HostServer);
	}

	if (QuitButton)
	{
		QuitButton->OnClicked.AddDynamic(this, &UMainMenu::QuitPressed);
	}

	return true;
}

void UMainMenu::HostServer()
{
	if (MenuInterface)
	{
		FString ServerName = ServerHostName->Text.ToString();
		MenuInterface->Host(ServerName);
	}
}

void UMainMenu::JoinServer()
{
	if (SelectedIndex.IsSet() && MenuInterface != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Selected Index: %d."), SelectedIndex.GetValue());
		MenuInterface->Join(SelectedIndex.GetValue());
	}
	else
	{
		MYLOG("Seleceted Index Not Set");
	}
}

void UMainMenu::OpenJoinMenu()
{
	if (!MenuSwitcher || !JoinMenu) return;
	MenuSwitcher->SetActiveWidget(JoinMenu);

	if (MenuInterface)
	{
		MenuInterface->RefreshServerList();
	}
}

void UMainMenu::OpenMainMenu()
{
	if (!MenuSwitcher || !MainMenu) return;
	MenuSwitcher->SetActiveWidget(MainMenu);
}

void UMainMenu::OpenHostMenu()
{
	MenuSwitcher->SetActiveWidget(HostMenu);
}

void UMainMenu::QuitPressed()
{
	UWorld* World = GetWorld();
	if (!World) return;

	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (!PlayerController) return;

	PlayerController->ConsoleCommand("quit");
}

void UMainMenu::UpdateChildren()
{
	for (int32 i = 0; i<ServerList->GetChildrenCount(); i++)
	{
		UServerRow* Row = Cast<UServerRow>(ServerList->GetChildAt(i));
		if (Row != nullptr)
		{
			Row->Selected = (SelectedIndex.IsSet() && SelectedIndex.GetValue() == i);
		}
	}
}


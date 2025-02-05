// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Menu/MSLoginInWidget.h"
#include "../../../Public/GameFramework/HUD/MMainMenuHUD.h"

#define LOCTEXT_NAMESPACE "LoginInMenu"

void MSLoginInWidget::Construct(const FArguments& InArgs)
{
	bCanSupportFocus = true;

	OwnerHUD = InArgs._OwnerHUD;

	ChildSlot
	[
		SNew(SOverlay)
			+ SOverlay::Slot().HAlign(HAlign_Center).VAlign(VAlign_Center)
			[
				SNew(SBorder).BorderImage(FAppStyle::Get().GetBrush("Brushes.Panel"))
					[
						SNew(SVerticalBox)
							+ SVerticalBox::Slot().AutoHeight().Padding(UMWidgetStyle::GetButtontPadding())
							[
								SNew(STextBlock).Font(UMWidgetStyle::GetTitleTextStyle()).Text(LOCTEXT("LoginInMenu", "Login In")).Justification(ETextJustify::Center)
							]

							+ SVerticalBox::Slot().AutoHeight().Padding(UMWidgetStyle::GetButtontPadding())
							[
								SNew(SHorizontalBox)
									+ SHorizontalBox::Slot().AutoWidth().Padding(UMWidgetStyle::GetButtontPadding())
									[
										SNew(SVerticalBox)
											+ SVerticalBox::Slot().AutoHeight().Padding(UMWidgetStyle::GetButtontPadding())
											[
												SNew(STextBlock).Font(UMWidgetStyle::GetTitleTextStyle()).Text(LOCTEXT("LoginInMenu", "Login")).Justification(ETextJustify::Center)
											]

											+ SVerticalBox::Slot().AutoHeight().Padding(UMWidgetStyle::GetButtontPadding())
											[
												SNew(STextBlock).Font(UMWidgetStyle::GetTitleTextStyle()).Text(LOCTEXT("LoginInMenu", "Password")).Justification(ETextJustify::Center)
											]
									]

									+ SHorizontalBox::Slot().AutoWidth().Padding(UMWidgetStyle::GetButtontPadding())
									[
										SNew(SVerticalBox)
											+ SVerticalBox::Slot().AutoHeight().Padding(UMWidgetStyle::GetButtontPadding())
											[
												SAssignNew(LoginBoxPtr, SEditableTextBox)
													.MinDesiredWidth(400.0f)
													.ClearKeyboardFocusOnCommit(false)
													.OnTextCommitted(this, &MSLoginInWidget::OnTextCommitted)
													.Font(UMWidgetStyle::GetTitleTextStyle())
													.HintText(LOCTEXT("LoginInMenu", "Login"))
											]

											+ SVerticalBox::Slot().AutoHeight().Padding(UMWidgetStyle::GetButtontPadding())
											[
												SAssignNew(PasswordBoxPtr, SEditableTextBox)
													.MinDesiredWidth(400.0f)
													.ClearKeyboardFocusOnCommit(false)
													.IsPassword(true)
													.Font(UMWidgetStyle::GetTitleTextStyle())
													.HintText(LOCTEXT("LoginInMenu", "Password"))
											]
									]
							]

							+ SVerticalBox::Slot().Padding(UMWidgetStyle::GetButtontPadding())
							[
								SNew(SButton).OnClicked(this, &MSLoginInWidget::OnSingIn)
									[
										SNew(STextBlock).Font(UMWidgetStyle::GetButtonTextStyle()).Text(LOCTEXT("LoginInMenu", "Sign in")).Justification(ETextJustify::Center)
									]
							]

							+ SVerticalBox::Slot().Padding(UMWidgetStyle::GetButtontPadding())
							[
								SNew(SButton).OnClicked(this, &MSLoginInWidget::OnSignUp)
									[
										SNew(STextBlock).Font(UMWidgetStyle::GetButtonTextStyle()).Text(LOCTEXT("LoginInMenu", "Sign up")).Justification(ETextJustify::Center)
									]
							]

							+ SVerticalBox::Slot().Padding(UMWidgetStyle::GetButtontPadding())
							[
								SNew(SButton).OnClicked(this, &MSLoginInWidget::OnQuit)
									[
										SNew(STextBlock).Font(UMWidgetStyle::GetButtonTextStyle()).Text(LOCTEXT("LoginInMenu", "Quit")).Justification(ETextJustify::Center)
									]
							]
					]
			]
	];
}

bool MSLoginInWidget::SupportsKeyboardFocus() const
{
	return true;
}

void MSLoginInWidget::SetFocus()
{
	FString Login, Password;

	GetLoginAndPassword(Login, Password);

	if (Login.IsEmpty() || (!Password.IsEmpty() && !Login.IsEmpty()))
	{
		FSlateApplication::Get().SetKeyboardFocus(LoginBoxPtr);
	}
	else if (Password.IsEmpty())
	{
		FSlateApplication::Get().SetKeyboardFocus(PasswordBoxPtr);
	}
}

FReply MSLoginInWidget::OnSingIn() const
{
	FString Login, Password;

	GetLoginAndPassword(Login, Password);

	if (!Login.IsEmpty() && !Password.IsEmpty())
	{
		if (IsValid(OwnerHUD->GetGameInstance()))
		{
			if (UMPlayerInfoSubsystem* PlayerInfoSubsystem = OwnerHUD->GetGameInstance()->GetSubsystem<UMPlayerInfoSubsystem>())
			{
				if (PlayerInfoSubsystem->IsUserDataCorrect(Login, Password))
				{
					SetDefault();
					SetNextWidget(ETypeOfWidget::GameTypeMenu);
				}
				else
				{
					ShowInformWidget(FText::FromString("Login or password is not correct."));
					SetStyle("", "");
				}
			}
		}
	}
	else
	{
		ShowInformWidget(FText::FromString(GetInformMessage(Login, Password)));
		SetStyle(Login, Password);
	}

	return FReply::Handled();
}

FReply MSLoginInWidget::OnQuit() const
{
	if (IsValid(OwnerHUD->GetOwningPlayerController()))
	{
		OwnerHUD->GetOwningPlayerController()->ConsoleCommand("quit");
	}

	return FReply::Handled();
}

FReply MSLoginInWidget::OnSignUp() const
{
	SetDefault();

	SetNextWidget(ETypeOfWidget::Registration);

	return FReply::Handled();
}

void MSLoginInWidget::ShowInformWidget(FText Text) const
{
	if (AMMainMenuHUD* HUD = Cast<AMMainMenuHUD>(OwnerHUD.Get()))
	{
		FInformWidgetData InformWidgetData = FInformWidgetData(Text, true, false,
			ETypeOfWidget::None, ETypeOfWidget::LoginIn);
		HUD->ShowInformWidget(InformWidgetData);
	}
}

FString MSLoginInWidget::GetInformMessage(FString Login, FString Password) const
{
	FString TextForMessage = "";
	int32 count = 0;

	TextForMessage += Login.IsEmpty() ? "Login" : "";
	count = Login.IsEmpty() ? 1 : 0;

	TextForMessage += Password.IsEmpty() ? (!TextForMessage.IsEmpty() ? " , password" : "Password") : "";
	count = Password.IsEmpty() ? count + 1 : count;

	return !TextForMessage.IsEmpty() ? TextForMessage + (count == 1 ? " is" : " are") + " not set." : FString();
}

void MSLoginInWidget::SetStyle(FString Login, FString Password) const
{
	LoginBoxPtr->SetColorAndOpacity(Login.IsEmpty() ? FColor::Red : FColor::White);

	PasswordBoxPtr->SetColorAndOpacity(Password.IsEmpty() ? FColor::Red : FColor::White);
}

void MSLoginInWidget::SetDefault() const
{
	LoginBoxPtr->SetText(FText::GetEmpty());

	PasswordBoxPtr->SetText(FText::GetEmpty());
	
	SetStyle("1", "1");
}

void MSLoginInWidget::SetNextWidget(ETypeOfWidget NextWidgetType) const
{
	if (AMMainMenuHUD* HUD = Cast<AMMainMenuHUD>(OwnerHUD.Get()))
	{
		HUD->ShowWidget(NextWidgetType);
	}
}

void MSLoginInWidget::OnTextCommitted(const FText& InText, ETextCommit::Type CommitMethod)
{
	if (CommitMethod == ETextCommit::OnEnter)
	{
		if (PasswordBoxPtr.IsValid() && !LoginBoxPtr->GetText().IsEmpty())
		{
			FSlateApplication::Get().SetKeyboardFocus(PasswordBoxPtr);
		}
	}
}

void MSLoginInWidget::GetLoginAndPassword(FString& Login, FString& Password) const
{
	PasswordBoxPtr->SetIsPassword(false);

	Login = LoginBoxPtr->GetText().ToString();
	Password = PasswordBoxPtr->GetText().ToString();

	PasswordBoxPtr->SetIsPassword(true);
}

#undef LOCTEXT_NAMESPACE
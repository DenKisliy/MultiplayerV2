// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Menu/MSLoginInWidget.h"
#include "../../../Public/GameFramework/MPlayerHUD.h"

#define LOCTEXT_NAMESPACE "LoginInMenu"

void MSLoginInWidget::Construct(const FArguments& InArgs)
{
	bCanSupportFocus = true;

	OwnerHUD = InArgs._OwnerHUD;

	FStyleWidgetData* StyleData = new FStyleWidgetData();

	ChildSlot
		[
			SNew(SOverlay)
				+ SOverlay::Slot().HAlign(HAlign_Center).VAlign(VAlign_Center)
				[
					SNew(SBorder).BorderImage(FAppStyle::Get().GetBrush("Brushes.Panel"))
						[
							SNew(SVerticalBox)
								+ SVerticalBox::Slot().AutoHeight().Padding(StyleData->ButtontPadding)
								[
									SNew(STextBlock).Font(StyleData->TitleTextStyle).Text(LOCTEXT("LoginInMenu", "Login In")).Justification(ETextJustify::Center)
								]

								+ SVerticalBox::Slot().AutoHeight().Padding(StyleData->ButtontPadding)
								[
									SNew(SHorizontalBox)
										+ SHorizontalBox::Slot().AutoWidth().Padding(StyleData->ButtontPadding)
										[
											SNew(SVerticalBox)
												+ SVerticalBox::Slot().AutoHeight().Padding(StyleData->ButtontPadding)
												[
													SNew(STextBlock).Font(StyleData->TitleTextStyle).Text(LOCTEXT("LoginInMenu", "Login")).Justification(ETextJustify::Center)
												]

												+ SVerticalBox::Slot().AutoHeight().Padding(StyleData->ButtontPadding)
												[
													SNew(STextBlock).Font(StyleData->TitleTextStyle).Text(LOCTEXT("LoginInMenu", "Password")).Justification(ETextJustify::Center)
												]
										]

										+ SHorizontalBox::Slot().AutoWidth().Padding(StyleData->ButtontPadding)
										[
											SNew(SVerticalBox)
												+ SVerticalBox::Slot().AutoHeight().Padding(StyleData->ButtontPadding)
												[
													SAssignNew(LoginBoxPtr, SEditableTextBox).MinDesiredWidth(400.0f).Font(StyleData->TitleTextStyle).HintText(LOCTEXT("LoginInMenu", "Login"))
												]

												+ SVerticalBox::Slot().AutoHeight().Padding(StyleData->ButtontPadding)
												[
													SAssignNew(PasswordBoxPtr, SEditableTextBox).MinDesiredWidth(400.0f).IsPassword(true).Font(StyleData->TitleTextStyle).HintText(LOCTEXT("LoginInMenu", "Password"))
												]
										]
								]

								+ SVerticalBox::Slot().Padding(StyleData->ButtontPadding)
								[
									SNew(SButton).OnClicked(this, &MSLoginInWidget::OnSingIn)
										[
											SNew(STextBlock).Font(StyleData->ButtonTextStyle).Text(LOCTEXT("LoginInMenu", "Sign in")).Justification(ETextJustify::Center)
										]
								]

								+ SVerticalBox::Slot().Padding(StyleData->ButtontPadding)
								[
									SNew(SButton).OnClicked(this, &MSLoginInWidget::OnSignUp)
										[
											SNew(STextBlock).Font(StyleData->ButtonTextStyle).Text(LOCTEXT("LoginInMenu", "Sign up")).Justification(ETextJustify::Center)
										]
								]

								+ SVerticalBox::Slot().Padding(StyleData->ButtontPadding)
								[
									SNew(SButton).OnClicked(this, &MSLoginInWidget::OnQuit)
										[
											SNew(STextBlock).Font(StyleData->ButtonTextStyle).Text(LOCTEXT("LoginInMenu", "Quit")).Justification(ETextJustify::Center)
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

FReply MSLoginInWidget::OnSingIn() const
{
	PasswordBoxPtr->SetIsPassword(false);

	FString Login = LoginBoxPtr->GetText().ToString();
	FString Password = PasswordBoxPtr->GetText().ToString();

	PasswordBoxPtr->SetIsPassword(true);

	if (!Login.IsEmpty() && !Password.IsEmpty())
	{
		if (IsValid(OwnerHUD->GetGameInstance()))
		{
			if (UMPlayerInfoSubsystem* PlayerInfoSubsystem = OwnerHUD->GetGameInstance()->GetSubsystem<UMPlayerInfoSubsystem>())
			{
				bool bResult = PlayerInfoSubsystem->IsUserDataCorrect(Login, Password);
				
				if (bResult)
				{
					SetDefault();

					SetNextWidget(ETypeOfWidget::Menu);
				}
				else
				{
					ShowInformWidget(new FInformativeWidgetData(FText::FromString("Login or password is not correct."), true));
				}
			}
		}
	}
	else
	{
		ShowInformWidget(new FInformativeWidgetData(FText::FromString(GetInformMessage(Login, Password)), true));
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

void MSLoginInWidget::ShowInformWidget(FInformativeWidgetData* InformWidgetData) const
{
	if (AMPlayerHUD* HUD = Cast<AMPlayerHUD>(OwnerHUD.Get()))
	{
		HUD->ShowInformWidget(InformWidgetData);
	}
}

FString MSLoginInWidget::GetInformMessage(FString Login, FString Password) const
{
	FString TextForMessage = "";
	int32 count = 0;

	TextForMessage = TextForMessage + (Login.IsEmpty() ? "Login" : "");
	count = Login.IsEmpty() ? 1 : 0;

	TextForMessage = TextForMessage + (Password.IsEmpty() ? (!TextForMessage.IsEmpty() ? " , password" : "Password") : "");
	count = Password.IsEmpty() ? count + 1 : count;

	return !TextForMessage.IsEmpty() ? TextForMessage + (count == 1 ? " is" : +" are") + " not set." : FString();
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
	if (AMPlayerHUD* HUD = Cast<AMPlayerHUD>(OwnerHUD.Get()))
	{
		HUD->ShowNextWidget(NextWidgetType);
	}
}

#undef LOCTEXT_NAMESPACE
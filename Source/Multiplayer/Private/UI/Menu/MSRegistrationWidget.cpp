// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Menu/MSRegistrationWidget.h"
#include "../../../Public/GameFramework/HUD/MMainMenuHUD.h"
#include "../../../Public/GameFramework/MPlayerHUD.h"

#define LOCTEXT_NAMESPACE "Registration"

void MSRegistrationWidget::Construct(const FArguments& InArgs)
{
	bCanSupportFocus = true;

	OwnerHUD = InArgs._OwnerHUD;

	SetDefaultValuesForWidgets();

	ChildSlot
		[
			SNew(SOverlay)
				+ SOverlay::Slot().HAlign(HAlign_Center).VAlign(VAlign_Center).Padding(UMWidgetStyle::GetContentPadding())
				[
					SNew(SBorder).BorderImage(FAppStyle::Get().GetBrush("Brushes.Panel"))
						[
							SNew(SVerticalBox)
								+ SVerticalBox::Slot().AutoHeight().Padding(UMWidgetStyle::GetButtontPadding())
								[
									SNew(STextBlock).Font(UMWidgetStyle::GetTitleTextStyle()).Text(LOCTEXT("Registration", "Registration")).Justification(ETextJustify::Center)
								]

								+ SVerticalBox::Slot().AutoHeight().Padding(UMWidgetStyle::GetButtontPadding())
								[
									SNew(SHorizontalBox)
										+ SHorizontalBox::Slot().AutoWidth().Padding(UMWidgetStyle::GetButtontPadding())
										[
											SNew(SVerticalBox)
												+ SVerticalBox::Slot().AutoHeight().Padding(UMWidgetStyle::GetButtontPadding())
												[
													SNew(STextBlock).Font(UMWidgetStyle::GetTitleTextStyle()).Text(LOCTEXT("Registration", "Login")).Justification(ETextJustify::Left)
												]

												+ SVerticalBox::Slot().AutoHeight().Padding(UMWidgetStyle::GetButtontPadding())
												[
													SNew(STextBlock).Font(UMWidgetStyle::GetTitleTextStyle()).Text(LOCTEXT("Registration", "Password")).Justification(ETextJustify::Left)
												]

												+ SVerticalBox::Slot().AutoHeight().Padding(UMWidgetStyle::GetButtontPadding())
												[
													SNew(STextBlock).Font(UMWidgetStyle::GetTitleTextStyle()).Text(LOCTEXT("Registration", "Character type")).Justification(ETextJustify::Left)
												]
										]

										+ SHorizontalBox::Slot().AutoWidth().Padding(UMWidgetStyle::GetButtontPadding())
										[
											SNew(SVerticalBox)
												+ SVerticalBox::Slot().AutoHeight().Padding(UMWidgetStyle::GetButtontPadding())
												[
													SAssignNew(LoginBoxPtr, SEditableTextBox)
														.MinDesiredWidth(400.0f)
														.OnTextCommitted(this, &MSRegistrationWidget::OnTextCommitted)
														.ClearKeyboardFocusOnCommit(false)
														.Font(UMWidgetStyle::GetTitleTextStyle())
														.HintText(LOCTEXT("Registration", "Login"))
												]

												+ SVerticalBox::Slot().AutoHeight().Padding(UMWidgetStyle::GetButtontPadding())
												[
													SAssignNew(PasswordBoxPtr, SEditableTextBox)
														.MinDesiredWidth(400.0f)
														.ClearKeyboardFocusOnCommit(false)
														.IsPassword(true)
														.Font(UMWidgetStyle::GetTitleTextStyle())
														.HintText(LOCTEXT("Registration", "Password"))
												]

												+ SVerticalBox::Slot().AutoHeight().Padding(UMWidgetStyle::GetButtontPadding())
												[
													SAssignNew(TypeOfCharacterComboBox, SComboBox<TSharedPtr<FText>>).InitiallySelectedItem(SelectedTypeOfCharacter).OptionsSource(&TypeOfCharacterArray)
														.OnSelectionChanged_Lambda([this](TSharedPtr<FText> NewSelection, ESelectInfo::Type SelectInfo)
															{
																SelectedTypeOfCharacter = NewSelection;
															})
														.OnGenerateWidget_Lambda([](TSharedPtr<FText> Option)
															{
																return SNew(STextBlock).Font(FCoreStyle::GetDefaultFontStyle("EmbossedText", 60)).Text(*Option);
															})
																[
																	SAssignNew(CheckBoxPtr, STextBlock).MinDesiredWidth(400.0f).Font(FCoreStyle::GetDefaultFontStyle("EmbossedText", 60))
																		.Text_Lambda([this]()
																			{
																				return SelectedTypeOfCharacter.IsValid() ? *SelectedTypeOfCharacter : FText::GetEmpty();
																			})
																]
												]
										]
								]

								+ SVerticalBox::Slot().AutoHeight().Padding(UMWidgetStyle::GetButtontPadding())
								[
									SNew(SHorizontalBox)
										+ SHorizontalBox::Slot().Padding(UMWidgetStyle::GetButtontPadding())
										[
											SNew(SButton).OnClicked(this, &MSRegistrationWidget::OnSingUn)
												[
													SNew(STextBlock).Font(UMWidgetStyle::GetButtonTextStyle()).Text(LOCTEXT("Registration", "Registrat")).Justification(ETextJustify::Center)
												]
										]

										+ SHorizontalBox::Slot().Padding(UMWidgetStyle::GetButtontPadding())
										[
											SNew(SButton).OnClicked(this, &MSRegistrationWidget::OnCancel)
												[
													SNew(STextBlock).Font(UMWidgetStyle::GetButtonTextStyle()).Text(LOCTEXT("Registration", "Cancel")).Justification(ETextJustify::Center)
												]
										]
								]

								+ SVerticalBox::Slot().AutoHeight().Padding(UMWidgetStyle::GetButtontPadding())
								[
									SNew(SHorizontalBox)
										+ SHorizontalBox::Slot().Padding(UMWidgetStyle::GetButtontPadding()).HAlign(HAlign_Center)
										[
											SNew(SButton).OnClicked(this, &MSRegistrationWidget::OnBackToPreviousMenu)
												[
													SNew(STextBlock).Font(UMWidgetStyle::GetButtonTextStyle()).Text(LOCTEXT("Registration", "Back to previous menu")).Justification(ETextJustify::Center)
												]
										]
								]
						]
				]
		];
}

bool MSRegistrationWidget::SupportsKeyboardFocus() const
{
	return true;
}

void MSRegistrationWidget::SetFocus()
{
	PasswordBoxPtr->SetIsPassword(false);

	FString Login = LoginBoxPtr->GetText().ToString();
	FString Password = PasswordBoxPtr->GetText().ToString();

	PasswordBoxPtr->SetIsPassword(true);

	if (Login.IsEmpty() || (!Password.IsEmpty() && !Login.IsEmpty()))
	{
		FSlateApplication::Get().SetKeyboardFocus(LoginBoxPtr);
	}
	else if (Password.IsEmpty())
	{
		FSlateApplication::Get().SetKeyboardFocus(PasswordBoxPtr);
	}
}

FReply MSRegistrationWidget::OnSingUn() const
{
	PasswordBoxPtr->SetIsPassword(false);
	
	FString Login = LoginBoxPtr->GetText().ToString();
	FString Password = PasswordBoxPtr->GetText().ToString();
	FString Type = CheckBoxPtr.Get()->GetText().ToString();

	PasswordBoxPtr->SetIsPassword(true);

	SetStyle(Login, Password, Type);

	if (GetInformMessage(Login, Password, Type).IsEmpty())
	{
		FPlayerInfoData* PlayerInfo = new FPlayerInfoData(Login, Password, TypeOfCharacterArray.Find(SelectedTypeOfCharacter) + 1);

		if (IsValid(OwnerHUD->GetGameInstance()))
		{
			if (UMPlayerInfoSubsystem* PlayerInfoSubsystem = OwnerHUD->GetGameInstance()->GetSubsystem<UMPlayerInfoSubsystem>())
			{
				if (!PlayerInfoSubsystem->IsPlayerRegistered(PlayerInfo->Login))
				{
					if (PlayerInfoSubsystem->RegisterPlayerData(PlayerInfo))
					{
						ShowInformWidget(FText::FromString("The player is successfully registered."), false, ETypeOfWidget::LoginIn);
					}
					else
					{
						ShowInformWidget(FText::FromString("Failed to register player."));
					}
				}
				else
				{
					ShowInformWidget(FText::FromString("A user with this login already exists."));
				}
			}
		}

		delete PlayerInfo;
	}
	else
	{
		ShowInformWidget(FText::FromString(GetInformMessage(Login, Password, Type)));
	}

	return FReply::Handled();
}

FReply MSRegistrationWidget::OnCancel() const
{
	LoginBoxPtr->SetColorAndOpacity(FColor::White);
	PasswordBoxPtr->SetColorAndOpacity(FColor::White);
	CheckBoxPtr->SetColorAndOpacity(FColor::White);

	LoginBoxPtr->SetText(FText::GetEmpty());
	PasswordBoxPtr->SetText(FText::GetEmpty());
	TypeOfCharacterComboBox->SetSelectedItem(DefaultTypeOfCharacter);

	return FReply::Handled();
}

FReply MSRegistrationWidget::OnBackToPreviousMenu() const
{
	if (AMMainMenuHUD* HUD = Cast<AMMainMenuHUD>(OwnerHUD.Get()))
	{
		OnCancel();
		HUD->ShowWidget(ETypeOfWidget::LoginIn);
	}

	return FReply::Handled();
}

void MSRegistrationWidget::ShowInformWidget(FText Text, bool bWarning, ETypeOfWidget NextWidget, ETypeOfWidget PreviousWidget) const
{
	if (GEngine && GEngine->GameViewport)
	{
		if (AMMainMenuHUD* HUD = Cast<AMMainMenuHUD>(OwnerHUD.Get()))
		{
			FInformWidgetData InformWidgetData = FInformWidgetData(Text, bWarning, false,
				NextWidget, PreviousWidget);
			HUD->ShowInformWidget(InformWidgetData);
		}
	}
}

FString MSRegistrationWidget::GetInformMessage(FString Login, FString Password, FString Type) const
{
	FString TextForMessage = "";
	int32 count = 0;

	TextForMessage = TextForMessage + (Login.IsEmpty() ? "Login" : "");
	count = Login.IsEmpty() ? 1 : 0;

	TextForMessage = TextForMessage + (Password.IsEmpty() ? (!TextForMessage.IsEmpty() ? " , password" : "Password") : "");
	count = Password.IsEmpty() ? count + 1 : count;

	TextForMessage = TextForMessage + (Type.IsEmpty() || Type == "None" ? (!TextForMessage.IsEmpty() ? " , type" : "Type") : "");
	count = Type.IsEmpty() || Type == "None" ? count + 1 : count;

	return !TextForMessage.IsEmpty() ? TextForMessage + (count == 1 ? " is" : +" are") + " not set." : FString();
}

void MSRegistrationWidget::SetStyle(FString Login, FString Password, FString Type) const
{
	LoginBoxPtr->SetColorAndOpacity(Login.IsEmpty() ? FColor::Red : FColor::White);

	PasswordBoxPtr->SetColorAndOpacity(Password.IsEmpty() ? FColor::Red : FColor::White);

	CheckBoxPtr->SetColorAndOpacity(Type.IsEmpty() || Type == "None" ? FColor::Red : FColor::White);

	if (Type.IsEmpty() || Type == "None")
	{
		TypeOfCharacterComboBox->SetSelectedItem(DefaultTypeOfCharacter);
	}
}

void MSRegistrationWidget::SetDefaultValuesForWidgets()
{
	if (UEnum* enumObj = FindObject<UEnum>(nullptr, TEXT("/Script/Multiplayer.ETypeOfCharacter")))
	{
		for (int i = 0; i < enumObj->GetMaxEnumValue(); i++)
		{
			FString value = UEnum::GetValueAsString(ETypeOfCharacter(i));
			value.RemoveFromStart("ETypeOfCharacter::");

			if (i == 0)
			{
				SelectedTypeOfCharacter = MakeShareable(new FText(FText::FromString(value)));
				DefaultTypeOfCharacter = MakeShareable(new FText(FText::FromString(value)));
			}
			else
			{
				TypeOfCharacterArray.Add(MakeShareable(new FText(FText::FromString(value))));
			}
		}
	}
}

void MSRegistrationWidget::OnTextCommitted(const FText& InText, ETextCommit::Type CommitMethod)
{
	if (CommitMethod == ETextCommit::OnEnter)
	{
		if (PasswordBoxPtr.IsValid() && !LoginBoxPtr->GetText().IsEmpty())
		{
			FSlateApplication::Get().SetKeyboardFocus(PasswordBoxPtr);
		}
	}
}

#undef LOCTEXT_NAMESPACE

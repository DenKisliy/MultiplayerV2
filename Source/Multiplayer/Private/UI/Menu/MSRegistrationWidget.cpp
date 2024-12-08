// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Menu/MSRegistrationWidget.h"
#include "../../../Public/GameFramework/MPlayerHUD.h"

#define LOCTEXT_NAMESPACE "Registration"

void MSRegistrationWidget::Construct(const FArguments& InArgs)
{
	bCanSupportFocus = true;

	OwnerHUD = InArgs._OwnerHUD;

	FStyleWidgetData* StyleData = new FStyleWidgetData();

	SetDefaultValuesForWidgets();

	ChildSlot
		[
			SNew(SOverlay)
				+ SOverlay::Slot().HAlign(HAlign_Fill).VAlign(VAlign_Fill)
				[
					SNew(SImage).ColorAndOpacity(FColor::Black)
				]

				+ SOverlay::Slot().HAlign(HAlign_Center).VAlign(VAlign_Center).Padding(StyleData->ContentPadding)
				[
					SNew(SVerticalBox)
						+ SVerticalBox::Slot().AutoHeight().Padding(StyleData->ButtontPadding)
						[
							SNew(STextBlock).Font(StyleData->TitleTextStyle).Text(LOCTEXT("Registration", "Registration")).Justification(ETextJustify::Center)
						]

						+ SVerticalBox::Slot().AutoHeight().Padding(StyleData->ButtontPadding)
						[
							SNew(SHorizontalBox)
								+ SHorizontalBox::Slot().AutoWidth().Padding(StyleData->ButtontPadding)
								[
									SNew(SVerticalBox)
										+ SVerticalBox::Slot().AutoHeight().Padding(StyleData->ButtontPadding)
										[
											SNew(STextBlock).Font(StyleData->TitleTextStyle).Text(LOCTEXT("Registration", "Login")).Justification(ETextJustify::Left)
										]

										+ SVerticalBox::Slot().AutoHeight().Padding(StyleData->ButtontPadding)
										[
											SNew(STextBlock).Font(StyleData->TitleTextStyle).Text(LOCTEXT("Registration", "Password")).Justification(ETextJustify::Left)
										]

										+ SVerticalBox::Slot().AutoHeight().Padding(StyleData->ButtontPadding)
										[
											SNew(STextBlock).Font(StyleData->TitleTextStyle).Text(LOCTEXT("Registration", "Character type")).Justification(ETextJustify::Left)
										]
								]

								+ SHorizontalBox::Slot().AutoWidth().Padding(StyleData->ButtontPadding)
								[
									SNew(SVerticalBox)
										+ SVerticalBox::Slot().AutoHeight().Padding(StyleData->ButtontPadding)
										[
											SAssignNew(LoginBoxPtr, SEditableTextBox).MinDesiredWidth(400.0f).Font(StyleData->TitleTextStyle).HintText(LOCTEXT("Registration", "Login"))
										]

										+ SVerticalBox::Slot().AutoHeight().Padding(StyleData->ButtontPadding)
										[
											SAssignNew(PasswordBoxPtr, SEditableTextBox).MinDesiredWidth(400.0f).IsPassword(true).Font(StyleData->TitleTextStyle).HintText(LOCTEXT("Registration", "Password"))
										]

										+ SVerticalBox::Slot().AutoHeight().Padding(StyleData->ButtontPadding)
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

						+ SVerticalBox::Slot().AutoHeight().Padding(StyleData->ButtontPadding)
						[
							SNew(SHorizontalBox)
								+ SHorizontalBox::Slot().Padding(StyleData->ButtontPadding)
								[
									SNew(SButton).OnClicked(this, &MSRegistrationWidget::OnSingUn)
										[
											SNew(STextBlock).Font(StyleData->ButtonTextStyle).Text(LOCTEXT("Registration", "Registrat")).Justification(ETextJustify::Center)
										]
								]

								+ SHorizontalBox::Slot().Padding(StyleData->ButtontPadding)
								[
									SNew(SButton).OnClicked(this, &MSRegistrationWidget::OnCancel)
										[
											SNew(STextBlock).Font(StyleData->ButtonTextStyle).Text(LOCTEXT("Registration", "Cancel")).Justification(ETextJustify::Center)
										]
								]
						]

						+ SVerticalBox::Slot().AutoHeight().Padding(StyleData->ButtontPadding)
						[
							SNew(SHorizontalBox)
								+ SHorizontalBox::Slot().Padding(StyleData->ButtontPadding).HAlign(HAlign_Center)
								[
									SNew(SButton).OnClicked(this, &MSRegistrationWidget::OnBackToPreviousMenu)
										[
											SNew(STextBlock).Font(StyleData->ButtonTextStyle).Text(LOCTEXT("Registration", "Back to previous menu")).Justification(ETextJustify::Center)
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

FReply MSRegistrationWidget::OnSingUn() const
{
	PasswordBoxPtr->SetIsPassword(false);
	
	FString Login = LoginBoxPtr->GetText().ToString();
	FString Password = PasswordBoxPtr->GetText().ToString();
	FString Type = CheckBoxPtr.Get()->GetText().ToString();

	PasswordBoxPtr->SetIsPassword(true);

	FString TextForMessage = GetInformMessage(Login, Password, Type);
	SetStyle(Login, Password, Type);

	FInformativeWidgetData* InformWidgetData = nullptr;

	if (TextForMessage.IsEmpty())
	{
		FPlayerInfoData PlayerInfo = FPlayerInfoData(Login, Password, TypeOfCharacterArray.Find(SelectedTypeOfCharacter) + 1);

		if (IsValid(OwnerHUD->GetGameInstance()))
		{
			if (UMPlayerInfoSubsystem* PlayerInfoSubsystem = OwnerHUD->GetGameInstance()->GetSubsystem<UMPlayerInfoSubsystem>())
			{
				if (!PlayerInfoSubsystem->IsPlayerRegistered(PlayerInfo.Login))
				{
					bool bResult = PlayerInfoSubsystem->RegisterPlayerData(&PlayerInfo);
					InformWidgetData = new FInformativeWidgetData(bResult ? FText::FromString("The player is successfully registered.") : FText::FromString("Failed to register player."),
						!bResult, bResult ? ETypeOfWidget::LoginIn : ETypeOfWidget::None);
				}
				else
				{
					InformWidgetData = new FInformativeWidgetData(FText::FromString("A user with this login already exists."), true);
				}
			}
		}
	}
	else
	{
		InformWidgetData = new FInformativeWidgetData(FText::FromString(TextForMessage), true);
	}

	if (!InformWidgetData->Text.IsEmpty())
	{
		ShowInformWidget(InformWidgetData);
	}

	if (InformWidgetData->TypeOfNextWidget != ETypeOfWidget::None)
	{
		OnCancel();
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
	if (AMPlayerHUD* HUD = Cast<AMPlayerHUD>(OwnerHUD.Get()))
	{
		OnCancel();
		HUD->ShowNextWidget(ETypeOfWidget::LoginIn);
	}

	return FReply::Handled();
}

void MSRegistrationWidget::ShowInformWidget(FInformativeWidgetData* InformWidgetData) const
{
	if (AMPlayerHUD* HUD = Cast<AMPlayerHUD>(OwnerHUD.Get()))
	{
		HUD->ShowInformWidget(InformWidgetData);
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

#undef LOCTEXT_NAMESPACE

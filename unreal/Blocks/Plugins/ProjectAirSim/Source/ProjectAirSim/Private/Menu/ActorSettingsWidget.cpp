#include "Menu/ActorSettingsWidget.h"
#include "Misc/DefaultValueHelper.h"

void UActorSettingsWidget::Init(UActorSettings* OwnerIn) 
{
	this->Owner = OwnerIn;

	NameDisplay->OnTextCommitted.AddDynamic(this, &UActorSettingsWidget::CommitName);
	
	XDisplay->OnTextCommitted.AddDynamic(this, &UActorSettingsWidget::CommitX);
	YDisplay->OnTextCommitted.AddDynamic(this, &UActorSettingsWidget::CommitY);
	ZDisplay->OnTextCommitted.AddDynamic(this, &UActorSettingsWidget::CommitZ);
	
	RollDisplay->OnTextCommitted.AddDynamic(this, &UActorSettingsWidget::CommitRoll);
	PitchDisplay->OnTextCommitted.AddDynamic(this, &UActorSettingsWidget::CommitPitch);
	YawDisplay->OnTextCommitted.AddDynamic(this, &UActorSettingsWidget::CommitYaw);
}

void UActorSettingsWidget::CommitName(const FText& Text, ETextCommit::Type CommitType)
{
	if (CommitType == ETextCommit::OnEnter || CommitType == ETextCommit::OnUserMovedFocus)
    {
        if (!Text.IsEmpty())
		{
			Owner->Name = Text.ToString();
		}
    }
	NameDisplay->SetText(FText::FromString(Owner->Name));
}


void UActorSettingsWidget::CommitX(const FText& Text, ETextCommit::Type CommitType)
{
	if (CommitType == ETextCommit::OnEnter || CommitType == ETextCommit::OnUserMovedFocus)
    {
        if (!Text.IsEmpty())
		{
			float Value;
			if (FDefaultValueHelper::ParseFloat(Text.ToString(), Value) && Text.ToString().IsNumeric())
			{
				UE_LOG(LogTemp, Log, TEXT("Parsed float: %f"), Value);
				Owner->XYZ.X = Value;
			}
		}
    }
	XDisplay->SetText(FText::AsNumber(Owner->XYZ.X));
}

void UActorSettingsWidget::CommitY(const FText& Text, ETextCommit::Type CommitType)
{
	if (CommitType == ETextCommit::OnEnter || CommitType == ETextCommit::OnUserMovedFocus)
    {
        if (!Text.IsEmpty())
		{
			float Value;
			if (FDefaultValueHelper::ParseFloat(Text.ToString(), Value) && Text.ToString().IsNumeric())
			{
				UE_LOG(LogTemp, Log, TEXT("Parsed float: %f"), Value);
				Owner->XYZ.Y = Value;
			}
		}
    }
	YDisplay->SetText(FText::AsNumber(Owner->XYZ.Y));
}

void UActorSettingsWidget::CommitZ(const FText& Text, ETextCommit::Type CommitType)
{
	if (CommitType == ETextCommit::OnEnter || CommitType == ETextCommit::OnUserMovedFocus)
    {
        if (!Text.IsEmpty())
		{
			float Value;
			if (FDefaultValueHelper::ParseFloat(Text.ToString(), Value) && Text.ToString().IsNumeric())
			{
				UE_LOG(LogTemp, Log, TEXT("Parsed float: %f"), Value);
				Owner->XYZ.Z = Value;
			}
		}
    }
	ZDisplay->SetText(FText::AsNumber(Owner->XYZ.Z));
}

void UActorSettingsWidget::CommitRoll(const FText& Text, ETextCommit::Type CommitType)
{
	if (CommitType == ETextCommit::OnEnter || CommitType == ETextCommit::OnUserMovedFocus)
    {
        if (!Text.IsEmpty())
		{
			float Value;
			if (FDefaultValueHelper::ParseFloat(Text.ToString(), Value) && Text.ToString().IsNumeric())
			{
				UE_LOG(LogTemp, Log, TEXT("Parsed float: %f"), Value);
				Owner->RPYDeg.X = Value;
			}
		}
    }
	RollDisplay->SetText(FText::AsNumber(Owner->RPYDeg.X));
}

void UActorSettingsWidget::CommitPitch(const FText& Text, ETextCommit::Type CommitType)
{
	if (CommitType == ETextCommit::OnEnter || CommitType == ETextCommit::OnUserMovedFocus)
    {
        if (!Text.IsEmpty())
		{
			float Value;
			if (FDefaultValueHelper::ParseFloat(Text.ToString(), Value) && Text.ToString().IsNumeric())
			{
				UE_LOG(LogTemp, Log, TEXT("Parsed float: %f"), Value);
				Owner->RPYDeg.Y = Value;
			}
		}
    }
	PitchDisplay->SetText(FText::AsNumber(Owner->RPYDeg.Y));
}

void UActorSettingsWidget::CommitYaw(const FText& Text, ETextCommit::Type CommitType)
{
	if (CommitType == ETextCommit::OnEnter || CommitType == ETextCommit::OnUserMovedFocus)
    {
        if (!Text.IsEmpty())
		{
			float Value;
			if (FDefaultValueHelper::ParseFloat(Text.ToString(), Value) && Text.ToString().IsNumeric())
			{
				UE_LOG(LogTemp, Log, TEXT("Parsed float: %f"), Value);
				Owner->RPYDeg.Z = Value;
			}
		}
    }
	YawDisplay->SetText(FText::AsNumber(Owner->RPYDeg.Z));
}
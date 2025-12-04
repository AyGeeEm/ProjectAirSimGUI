#include "Menu/CaptureSettingsWidget.h"
#include "Misc/DefaultValueHelper.h"

void UCaptureSettingsWidget::Init(UCaptureSettings* OwnerIn) 
{
	this->Owner = OwnerIn;

	ImageTypeDisplay->OnTextCommitted.AddDynamic(this, &UCaptureSettingsWidget::CommitImageType);
	FOVDegreesDisplay->OnTextCommitted.AddDynamic(this, &UCaptureSettingsWidget::CommitFOVDegrees);
	WidthDisplay->OnTextCommitted.AddDynamic(this, &UCaptureSettingsWidget::CommitWidth);
	HeightDisplay->OnTextCommitted.AddDynamic(this, &UCaptureSettingsWidget::CommitHeight);
	MotionBlurAmountDisplay->OnTextCommitted.AddDynamic(this, &UCaptureSettingsWidget::CommitMotionBlurAmount);
}

void UCaptureSettingsWidget::CommitImageType(const FText& Text, ETextCommit::Type CommitType)
{
	if (CommitType == ETextCommit::OnEnter || CommitType == ETextCommit::OnUserMovedFocus)
    {
        if (!Text.IsEmpty())
		{
			float Value;
			if (FDefaultValueHelper::ParseFloat(Text.ToString(), Value) && Text.ToString().IsNumeric())
			{
				if (Value == 0 ||
					Value == 1 ||
					Value == 2 ||
					Value == 3 ||
					Value == 4 ||
					Value == 5 ||
					Value == 6)
				{
					Owner->ImageType = Value;
				}
				
			}
		}
    }
	ImageTypeDisplay->SetText(FText::AsNumber(Owner->ImageType));
}

void UCaptureSettingsWidget::CommitFOVDegrees(const FText& Text, ETextCommit::Type CommitType)
{
	if (CommitType == ETextCommit::OnEnter || CommitType == ETextCommit::OnUserMovedFocus)
    {
        if (!Text.IsEmpty())
		{
			float Value;
			if (FDefaultValueHelper::ParseFloat(Text.ToString(), Value) && Text.ToString().IsNumeric())
			{
				if (Value >= 5 && Value <= 170)
				{
					Owner->FOVDegrees = Value;
				}
				
			}
		}
    }
	FOVDegreesDisplay->SetText(FText::AsNumber(Owner->FOVDegrees));
}

void UCaptureSettingsWidget::CommitWidth(const FText& Text, ETextCommit::Type CommitType)
{
	if (CommitType == ETextCommit::OnEnter || CommitType == ETextCommit::OnUserMovedFocus)
    {
        if (!Text.IsEmpty())
		{
			int32 Value;
			if (FDefaultValueHelper::ParseInt(Text.ToString(), Value) && Text.ToString().IsNumeric())
			{
				if (Value > 0)
				{
					Owner->Width = Value;
				}
				
			}
		}
    }
	WidthDisplay->SetText(FText::AsNumber(Owner->Width));
}

void UCaptureSettingsWidget::CommitHeight(const FText& Text, ETextCommit::Type CommitType)
{
	if (CommitType == ETextCommit::OnEnter || CommitType == ETextCommit::OnUserMovedFocus)
    {
        if (!Text.IsEmpty())
		{
			int32 Value;
			if (FDefaultValueHelper::ParseInt(Text.ToString(), Value) && Text.ToString().IsNumeric())
			{
				if (Value > 0)
				{
					Owner->Height = Value;
				}
				
			}
		}
    }
	HeightDisplay->SetText(FText::AsNumber(Owner->Height));
}

void UCaptureSettingsWidget::CommitMotionBlurAmount(const FText& Text, ETextCommit::Type CommitType)
{
	if (CommitType == ETextCommit::OnEnter || CommitType == ETextCommit::OnUserMovedFocus)
    {
        if (!Text.IsEmpty())
		{
			float Value;
			if (FDefaultValueHelper::ParseFloat(Text.ToString(), Value) && Text.ToString().IsNumeric())
			{
				if (Value >= 0 && Value <= 1)
				{
					Owner->MotionBlurAmount = Value;
				}
				
			}
		}
    }
	MotionBlurAmountDisplay->SetText(FText::AsNumber(Owner->MotionBlurAmount));
}
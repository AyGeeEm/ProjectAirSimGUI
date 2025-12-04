#include "Menu/SensorSettingsWidget.h"
#include "Misc/DefaultValueHelper.h"

void USensorSettingsWidget::Init(USensorSettings* OwnerIn) 
{
	this->Owner = OwnerIn;

	IDDisplay->OnTextCommitted.AddDynamic(this, &USensorSettingsWidget::CommitID);
	TypeDisplay->OnTextCommitted.AddDynamic(this, &USensorSettingsWidget::CommitType);
	EnabledDisplay->OnTextCommitted.AddDynamic(this, &USensorSettingsWidget::CommitEnabled);
	CaptureIntervalDisplay->OnTextCommitted.AddDynamic(this, &USensorSettingsWidget::CommitCaptureInterval);
}

void USensorSettingsWidget::CommitID(const FText& Text, ETextCommit::Type CommitType)
{
	if (CommitType == ETextCommit::OnEnter || CommitType == ETextCommit::OnUserMovedFocus)
    {
        if (!Text.IsEmpty())
		{
			Owner->ID = Text.ToString();
		}
    }
	IDDisplay->SetText(FText::FromString(Owner->ID));
}

void USensorSettingsWidget::CommitType(const FText& Text, ETextCommit::Type ETextCommitType)
{
	if (ETextCommitType == ETextCommit::OnEnter || ETextCommitType == ETextCommit::OnUserMovedFocus)
    {
        if (!Text.IsEmpty())
		{
			FString TextString = Text.ToString().ToLower();
			if (TextString.Equals("airspeed") ||
				TextString.Equals("barometer") ||
				TextString.Equals("camera") ||
				TextString.Equals("gps") ||
				TextString.Equals("imu") ||
				TextString.Equals("lidar") ||
				TextString.Equals("magnetometer") ||
				TextString.Equals("radar")) 
			{
				Owner->Type = TextString;
			}
		}
    }
	TypeDisplay->SetText(FText::FromString(Owner->Type));
}

void USensorSettingsWidget::CommitEnabled(const FText& Text, ETextCommit::Type CommitType)
{
	if (CommitType == ETextCommit::OnEnter || CommitType == ETextCommit::OnUserMovedFocus)
    {
        if (!Text.IsEmpty())
		{
			if (Text.ToString().ToLower().Equals("true") ||
				Text.ToString().ToLower().Equals("false"))
			{
				Owner->Enabled = Text.ToString().ToLower().Equals("true");
			}
		}
    }
	EnabledDisplay->SetText(FText::FromString(Owner->Enabled ? TEXT("true") : TEXT("false")));
}


void USensorSettingsWidget::CommitCaptureInterval(const FText& Text, ETextCommit::Type CommitType)
{
	if (CommitType == ETextCommit::OnEnter || CommitType == ETextCommit::OnUserMovedFocus)
    {
        if (!Text.IsEmpty())
		{
			float Value;
			if (FDefaultValueHelper::ParseFloat(Text.ToString(), Value) && Text.ToString().IsNumeric() && Value > 0)
			{
				Owner->CaptureInterval = Value;
			}
		}
    }
	CaptureIntervalDisplay->SetText(FText::AsNumber(Owner->CaptureInterval));
}
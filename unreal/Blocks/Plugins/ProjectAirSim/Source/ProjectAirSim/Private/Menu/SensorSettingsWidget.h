#pragma once

#include "CoreMinimal.h"
#include "Menu/CaptureSettingsWidget.h"
#include "SensorSettingsWidget.generated.h"

class USensorSettings;

UCLASS()
class USensorSettingsWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	void Init(USensorSettings* OwnerIn);

	UFUNCTION()
	void CommitID(const FText& Text, ETextCommit::Type CommitType);
	UFUNCTION()
	void CommitType(const FText& Text, ETextCommit::Type CommitType);
	UFUNCTION()
	void CommitEnabled(const FText& Text, ETextCommit::Type CommitType);
	UFUNCTION()
	void CommitCaptureInterval(const FText& Text, ETextCommit::Type CommitType);

	UPROPERTY(meta = (BindWidget))
    UEditableTextBox* IDDisplay;

	UPROPERTY(meta = (BindWidget))
    UEditableTextBox* TypeDisplay;

	UPROPERTY(meta = (BindWidget))
    UEditableTextBox* EnabledDisplay;

	UPROPERTY(meta = (BindWidget))
    UEditableTextBox* CaptureIntervalDisplay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Capture Settings")
	TSubclassOf<UCaptureSettingsWidget> CaptureSettingsWidgetClass;

	UPROPERTY(meta = (BindWidget))
    UVerticalBox* CaptureSettingsContainer;

private:

	USensorSettings* Owner;
	
};
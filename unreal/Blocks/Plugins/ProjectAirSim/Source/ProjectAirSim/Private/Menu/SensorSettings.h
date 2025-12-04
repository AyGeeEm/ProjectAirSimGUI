#pragma once
#include "CoreMinimal.h"
#include "Menu/JsonManager.h"
#include "Menu/CaptureSettings.h"
#include "Menu/SensorSettingsWidget.h"
#include "SensorSettings.generated.h"

UCLASS()
class USensorSettings : public UObject {

	GENERATED_BODY()

public:

	void Init(TSharedPtr<FJsonObject> RootIn, USettingsMenu* MenuIn);

	void PopulateGUI();

	void ApplyChanges();

	bool LoadAll();
	bool LoadID();
    bool LoadType();
    bool LoadEnabled();
	bool LoadCaptureInterval();
    bool LoadCaptureSettings();

	USensorSettingsWidget* Widget;

	UPROPERTY()
	FString ID;

	UPROPERTY()
	FString Type;
	
	UPROPERTY()
	bool Enabled;

	UPROPERTY()
	float CaptureInterval;

private:
	
	USettingsMenu* Menu;

	TSharedPtr<FJsonObject> Root;

	TArray<UCaptureSettings*> CaptureSettings;
};
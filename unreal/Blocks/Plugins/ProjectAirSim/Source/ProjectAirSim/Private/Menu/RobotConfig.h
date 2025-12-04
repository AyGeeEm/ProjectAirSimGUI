#pragma once
#include "CoreMinimal.h"
#include "Menu/JsonManager.h"
#include "Menu/SensorSettings.h"
#include "Menu/RobotConfigWidget.h"
#include "RobotConfig.generated.h"

UCLASS()
class URobotConfig : public UObject
{

	GENERATED_BODY()

public:

	void Init(TSharedPtr<FJsonObject> RootIn, const FString& FileNameIn, USettingsMenu* MenuIn);

	bool LoadSensors();

	void PopulateGUI();

	void ApplyChanges();

	URobotConfigWidget* Widget;

	UPROPERTY()
	FString FileName;

	UPROPERTY()
	TArray<USensorSettings*> Sensors;

private:

	TSharedPtr<FJsonObject> Root;

	UPROPERTY()
	USettingsMenu* Menu;

};
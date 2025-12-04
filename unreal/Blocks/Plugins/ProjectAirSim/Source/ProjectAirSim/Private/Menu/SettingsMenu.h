#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Menu/JsonManager.h"
#include "Menu/SceneConfig.h"
#include "Menu/RobotConfig.h"
#include "Menu/ActorSettingsWidget.h"
#include "Menu/RobotConfigWidget.h"
#include "Components/EditableTextBox.h"
#include "Components/Button.h"
#include "SettingsMenu.generated.h"


UCLASS()
class USettingsMenu : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;

	void OnFileSelected(const FString& FileName);

	bool ValidateFile(const FString& FileName);

	bool LoadSceneConfig(const FString& FileName);

	bool LoadRobotConfig(const FString& FileName);
	
	bool PopulateGUI();

	UFUNCTION()
	void ApplyChanges();
	
	FString ConfigFolderPath;

	TMap<FString, USceneConfig*> SceneConfigMap;
	TMap<FString, URobotConfig*> RobotConfigMap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Actors")
	TSubclassOf<UActorSettingsWidget> ActorSettingsWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Robot Config")
	TSubclassOf<URobotConfigWidget> RobotConfigWidgetClass;

	UPROPERTY(meta = (BindWidget))
    UEditableTextBox* SceneConfigFileNameDisplay;

	USceneConfig* SceneConfig;

private:

	FProcHandle CurrentPythonProcess;
	
    FString ScriptFolderPath;
	FString PythonScriptName;
	FString VirtualEnvActivatePath;

	UPROPERTY(meta = (BindWidget))
    UVerticalBox* ActorSettingsContainer;

	UPROPERTY(meta = (BindWidget))
    UVerticalBox* RobotConfigContainer;

	UPROPERTY(meta = (BindWidget))
    UEditableTextBox* PythonScriptNameDisplay;

	UPROPERTY(meta = (BindWidget))
    UButton* SaveButton;

	UPROPERTY(meta = (BindWidget))
    UButton* RunScriptButton;

	UFUNCTION()
    void RunScript();
};

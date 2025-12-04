#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Menu/JsonManager.h"
#include "Components/EditableTextBox.h"
#include "Components/Button.h"
#include "SettingsMenu.generated.h"


UCLASS()
class USettingsMenu : public UUserWidget
{
	GENERATED_BODY()

  public:
	virtual void NativeConstruct() override;

	FString GetID();
    //bool SetID(const FVector& NewValue);

	FString GetActorType();
    //bool USettingsMenu::SetActorType();

	FString GetActorName();
    //bool SetActorName();


	FString GetRobotConfig();
    //bool SetRobotConfig();

	FString GetClockType();

    int64 GetClockStepNs();

	int64 GetClockRealTimeUpdateRate();

	bool GetClockPauseOnStart();

	FVector GetOriginXYZ();
	bool SetOriginXYZ(const FVector& NewValue);

	FVector GetOriginRPY();
	bool SetOriginRPY(const FVector& NewValue);

	bool SetJsonFileName(const FString& NewJsonFileName);
	bool SetPythonFileName(const FString& NewPythonFileName);

	FString GetSceneType();
	bool SetSceneType(const FString& NewValue);


	void ReadJsonData();

  private:
	UPROPERTY()
	UJsonManager* JsonManager;
	FString ConfigFolderPath;
	FString PythonFolderPath;
	FString JsonFileName;
	FString PythonFileName;
	FString VirtualEnvActivatePath;
	FProcHandle CurrentPythonProcess;

	UPROPERTY(meta = (BindWidget))
    UEditableTextBox* JsonFileNameInput;

	UPROPERTY(meta = (BindWidget))
    UEditableTextBox* PythonFileNameInput;

	UPROPERTY(meta = (BindWidget))
    UEditableTextBox* XInput;
	UPROPERTY(meta = (BindWidget))
    UEditableTextBox* YInput;
	UPROPERTY(meta = (BindWidget))
    UEditableTextBox* ZInput;

	UPROPERTY(meta = (BindWidget))
    UEditableTextBox* RollInput;
	UPROPERTY(meta = (BindWidget))
    UEditableTextBox* PitchInput;
	UPROPERTY(meta = (BindWidget))
    UEditableTextBox* YawInput;

	UPROPERTY(meta = (BindWidget))
    UEditableTextBox* SceneTypeInput;

	UPROPERTY(meta = (BindWidget))
    UEditableTextBox* IDInput;

	UPROPERTY(meta = (BindWidget))
    UEditableTextBox* ActorTypeInput;
        
	UPROPERTY(meta = (BindWidget))
    UEditableTextBox* ActorNameInput;

	UPROPERTY(meta = (BindWidget))
    UEditableTextBox* RobotConfigInput;

	UPROPERTY(meta = (BindWidget))
    UEditableTextBox* ClockTypeInput;

	UPROPERTY(meta = (BindWidget))
    UEditableTextBox* StepNsInput;

	UPROPERTY(meta = (BindWidget))
    UEditableTextBox* RealTimeRateInput;

	UPROPERTY(meta = (BindWidget))
    UEditableTextBox* PauseOnStartInput;


	UPROPERTY(meta = (BindWidget))
    UButton* SaveButton;

	UPROPERTY(meta = (BindWidget))
    UButton* RunPythonScriptButton;

	UPROPERTY(meta = (BindWidget))
    UButton* KillCurrentProcessButton;

	UFUNCTION()
    void OnSaveClicked();

	UFUNCTION()
    void RunPythonScript();

	UFUNCTION()
    void KillCurrentProcess();
};

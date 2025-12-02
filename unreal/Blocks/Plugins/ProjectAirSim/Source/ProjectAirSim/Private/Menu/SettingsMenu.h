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

	FVector GetOriginXYZ();
	bool SetOriginXYZ(const FVector& NewValue);

	FVector GetOriginRPY();
	bool SetOriginRPY(const FVector& NewValue);

	bool SetJsonFileName(const FString& NewJsonFileName);
	bool SetPythonFileName(const FString& NewPythonFileName);

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

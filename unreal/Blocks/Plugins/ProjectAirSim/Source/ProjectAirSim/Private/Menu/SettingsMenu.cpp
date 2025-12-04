#include "Menu/SettingsMenu.h"
#include "HAL/PlatformProcess.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "HAL/PlatformFilemanager.h"

void USettingsMenu::NativeConstruct()
{
    Super::NativeConstruct();

    ConfigFolderPath = TEXT("../../client/python/example_user_scripts/sim_config/"); //this path only works for those building from source
    ScriptFolderPath = TEXT("C:\\Lee\\Repos\\AirSim\\ProjectAirSim\\client\\python\\example_user_scripts\\"); //REPLACE WITH YOURS
    PythonScriptName = TEXT("camera_pose.py");
    VirtualEnvActivatePath = TEXT("C:\\Lee\\Repos\\AirSim\\airsim-venv\\Scripts\\activate"); //SAME HERE

    if (!IsDesignTime())
    {
        if (SaveButton) SaveButton->OnClicked.AddDynamic(this, &USettingsMenu::ApplyChanges);
        if (RunScriptButton) RunScriptButton->OnClicked.AddDynamic(this, &USettingsMenu::RunScript);
        
        OnFileSelected(PythonScriptName);
    }
}

void USettingsMenu::RunScript()
{

    IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

    if (!PlatformFile.FileExists(*(ScriptFolderPath + PythonScriptName)))
    {
        UE_LOG(LogTemp, Warning, TEXT("File not found: %s"), *(ScriptFolderPath + PythonScriptName));
        return;
    }

    FString Cmd = TEXT("C:\\Windows\\System32\\cmd.exe");
    
    FString Args = FString::Printf(
        TEXT("/K \"call %s && cd /d %s && python %s && cmd\""),
        *VirtualEnvActivatePath,
        *ScriptFolderPath,
        *PythonScriptName);
    
    if (FPlatformProcess::IsProcRunning(CurrentPythonProcess)) FPlatformProcess::TerminateProc(CurrentPythonProcess, true);

    CurrentPythonProcess = FPlatformProcess::CreateProc(
        *Cmd, 
        *Args, //params
        false, //bLaunchDetatched
        false, //bLaunchHidden
        false, //bLaunchReallyHidden
        nullptr, 0, nullptr, nullptr);
}

void USettingsMenu::OnFileSelected(const FString& FileName)
{
    if (!ValidateFile(FileName)) 
    {
        UE_LOG(LogTemp, Warning, TEXT("Could not validate file: %s"), *(FileName));
        return;
    }

    PythonScriptName = FileName;
    
    FString SceneConfigFileName;
    if (!UJsonManager::ExtractSceneConfigFromPython(ScriptFolderPath + PythonScriptName, SceneConfigFileName))
    {
        UE_LOG(LogTemp, Warning, TEXT("Could not extract scene config file."));
        return;
    }


    if (!LoadSceneConfig(SceneConfigFileName))
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to load scene config file: %s"), *(SceneConfigFileName));
    }

    if (!PopulateGUI())
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to populate GUI."));
    }
}

bool USettingsMenu::ValidateFile(const FString& FileName)
{
    IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

    if (PlatformFile.FileExists(*(ScriptFolderPath + FileName)))
    {
        if (FileName.EndsWith(".py")) 
        {
            return true;
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("File does not end in .py: %s"), *(FileName));
            return false;
        }
    }
    else 
    {
        UE_LOG(LogTemp, Warning, TEXT("File not found: %s"), *(ScriptFolderPath + FileName));
        return false;
    }
}

bool USettingsMenu::LoadSceneConfig(const FString& FileName) 
{
    if (SceneConfigMap.Contains(FileName))
    {
        SceneConfig = SceneConfigMap[FileName];
    }
    else
    {
        TSharedPtr<FJsonObject> Root;
        if (!UJsonManager::LoadJsonObject(ConfigFolderPath + FileName, Root)) 
        {
        	UE_LOG(LogTemp, Warning, TEXT("Could not load Json: %s"), *(FileName));
        	return false;
        }
        SceneConfig = NewObject<USceneConfig>(this);
        SceneConfig->Init(Root, FileName, this);
        
        SceneConfigMap.Add(FileName, SceneConfig);
    }
    return true;
}

bool USettingsMenu::LoadRobotConfig(const FString& FileName)
{
    if (!RobotConfigMap.Contains(FileName))
    {
        TSharedPtr<FJsonObject> Root;
        if (!UJsonManager::LoadJsonObject(ConfigFolderPath + FileName, Root)) 
        {
        	UE_LOG(LogTemp, Warning, TEXT("Could not load Json: %s"), *(FileName));
        	return false;
        }
        URobotConfig* RobotConfig = NewObject<URobotConfig>(this);
        RobotConfig->Init(Root, FileName, this);
        
        RobotConfigMap.Add(FileName, RobotConfig);
    }
    return true;

}

bool USettingsMenu::PopulateGUI() 
{
    if (PythonScriptNameDisplay)
    {
        PythonScriptNameDisplay->SetIsReadOnly(true);
        PythonScriptNameDisplay->SetIsEnabled(false);
        PythonScriptNameDisplay->SetText(FText::FromString(PythonScriptName));
    }
    

    for (int i = 0; i < SceneConfig->Actors.Num(); i++)
    {
        SceneConfig->Actors[i]->Widget = CreateWidget<UActorSettingsWidget>(this, ActorSettingsWidgetClass);
        SceneConfig->Actors[i]->Widget->Init(SceneConfig->Actors[i]);
        ActorSettingsContainer->AddChild(SceneConfig->Actors[i]->Widget);

        if (SceneConfig->Actors[i]->RobotConfig->Widget == nullptr)
        {
            SceneConfig->Actors[i]->RobotConfig->Widget = CreateWidget<URobotConfigWidget>(this, RobotConfigWidgetClass);
            RobotConfigContainer->AddChild(SceneConfig->Actors[i]->RobotConfig->Widget);
        }
    }

    SceneConfig->PopulateGUI();


    return true;
}

void USettingsMenu::ApplyChanges()
{
    SceneConfig->ApplyChanges();
}
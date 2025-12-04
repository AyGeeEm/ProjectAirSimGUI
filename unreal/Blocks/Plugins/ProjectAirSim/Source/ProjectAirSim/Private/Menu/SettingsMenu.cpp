#include "Menu/SettingsMenu.h"
#include "HAL/PlatformProcess.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "HAL/PlatformFilemanager.h"

void USettingsMenu::NativeConstruct()
{
    Super::NativeConstruct();

    JsonManager = NewObject<UJsonManager>();
    ConfigFolderPath = TEXT("../../client/python/example_user_scripts/sim_config/"); //this path only works for those building from source
    PythonFolderPath = TEXT("C:\\Users\\compu\\Documents\\GitHub\\ProjectAirSimGUI_New\\client\\python\\example_user_scripts\\"); //REPLACE WITH YOURS
    JsonFileName = TEXT("scene_basic_drone_chase.jsonc");
    PythonFileName = TEXT("camera_pose.py");
    VirtualEnvActivatePath = TEXT("C:\\Users\\compu\\Documents\\GitHub\\ProjectAirSimGUI_New\\airsim-venv\\Scripts\\activate"); //SAME HERE

    
    if (!IsDesignTime())
    {
        if (SaveButton)
        {
            SaveButton->OnClicked.AddDynamic(this, &USettingsMenu::OnSaveClicked);
        }
        if (RunPythonScriptButton)
        {
            RunPythonScriptButton->OnClicked.AddDynamic(this, &USettingsMenu::RunPythonScript);
        }
        if (KillCurrentProcessButton)
        {
            KillCurrentProcessButton->OnClicked.AddDynamic(this, &USettingsMenu::KillCurrentProcess);
        }
        if (PythonFileNameInput) PythonFileNameInput->SetText(FText::FromString(PythonFileName));
        ReadJsonData();
    }
}

bool USettingsMenu::SetJsonFileName(const FString& NewJsonFileName)
{
    TSharedPtr<FJsonObject> Root;
  if (!JsonManager->LoadJsonObject(ConfigFolderPath + NewJsonFileName, Root)) 
  {
      if (JsonFileNameInput) JsonFileNameInput->SetText(FText::FromString(JsonFileName));
      UE_LOG(LogTemp, Warning, TEXT("File not found: %s"), *(NewJsonFileName));
      return false;
  }
  
    JsonFileName = NewJsonFileName;

    ReadJsonData();

    return true;

}

bool USettingsMenu::SetPythonFileName(const FString& NewPythonFileName) {
  
    IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

    if (PlatformFile.FileExists(*(PythonFolderPath + NewPythonFileName)))
    {
        UE_LOG(LogTemp, Warning, TEXT("File exists!!!: %s"), *(PythonFolderPath + NewPythonFileName));
        PythonFileName = NewPythonFileName;
        ReadJsonData();
        return true;
    }
    else 
    {
        if (PythonFileNameInput) PythonFileNameInput->SetText(FText::FromString(PythonFileName));
        UE_LOG(LogTemp, Warning, TEXT("File not found: %s"), *(PythonFolderPath + NewPythonFileName));
        return false;
    }
    
    
}

FString USettingsMenu::GetID() {
  TSharedPtr<FJsonObject> Root;
  if (!JsonManager->LoadJsonObject(ConfigFolderPath + JsonFileName, Root))
    return "";

  FString IDString;
  if (!JsonManager->GetStringField(Root, "id", IDString))
    return "";

  return IDString;
}

FString USettingsMenu::GetActorType() {
  TSharedPtr<FJsonObject> Root;
  if (!JsonManager->LoadJsonObject(ConfigFolderPath + JsonFileName, Root))
    return "";

  if (!Root->HasField("actors")) return "";

   const TArray<TSharedPtr<FJsonValue>>* ActorsArray;
  Root->TryGetArrayField("actors", ActorsArray);
   if (!ActorsArray || ActorsArray->Num() == 0) return "";

       TSharedPtr<FJsonObject> FirstActor = (*ActorsArray)[0]->AsObject();
   if (!FirstActor.IsValid()) return "";

    FString TypeStr;
   if (!JsonManager->GetStringField(FirstActor, TEXT("type"), TypeStr))
     return TEXT("");

   return TypeStr;

}

FString USettingsMenu::GetActorName() {
  TSharedPtr<FJsonObject> Root;
  if (!JsonManager->LoadJsonObject(ConfigFolderPath + JsonFileName, Root))
    return "";

  if (!Root->HasField("actors")) return "";

  const TArray<TSharedPtr<FJsonValue>>* ActorsArray;
  Root->TryGetArrayField("actors", ActorsArray);
  if (!ActorsArray || ActorsArray->Num() == 0) return "";

  TSharedPtr<FJsonObject> FirstActor = (*ActorsArray)[0]->AsObject();
  if (!FirstActor.IsValid()) return "";

  FString NameStr;
  if (!JsonManager->GetStringField(FirstActor, TEXT("name"), NameStr))
    return TEXT("");

  return NameStr;
}

FString USettingsMenu::GetRobotConfig() {
  TSharedPtr<FJsonObject> Root;
  if (!JsonManager->LoadJsonObject(ConfigFolderPath + JsonFileName, Root))
    return "";

  if (!Root->HasField("actors")) return "";

  const TArray<TSharedPtr<FJsonValue>>* ActorsArray;
  Root->TryGetArrayField("actors", ActorsArray);
  if (!ActorsArray || ActorsArray->Num() == 0) return "";

  TSharedPtr<FJsonObject> FirstActor = (*ActorsArray)[0]->AsObject();
  if (!FirstActor.IsValid()) return "";

  FString RobotConfigStr;
  if (!JsonManager->GetStringField(FirstActor, TEXT("robot-config"), RobotConfigStr))
    return TEXT("");

  return RobotConfigStr;
}

FVector USettingsMenu::GetOriginXYZ()
{
    TSharedPtr<FJsonObject> Root;
    if (!JsonManager->LoadJsonObject(ConfigFolderPath + JsonFileName, Root)) return FVector::ZeroVector;

    if (!Root->HasField("actors")) return FVector::ZeroVector;

    const TArray<TSharedPtr<FJsonValue>>* ActorsArray;
    Root->TryGetArrayField("actors", ActorsArray);
    if (!ActorsArray || ActorsArray->Num() == 0) return FVector::ZeroVector;

    TSharedPtr<FJsonObject> FirstActor = (*ActorsArray)[0]->AsObject();
    if (!FirstActor.IsValid()) return FVector::ZeroVector;

    TSharedPtr<FJsonObject> Origin = FirstActor->GetObjectField("origin");
    if (!Origin.IsValid()) return FVector::ZeroVector;

    FString VecStr;
    if (!JsonManager->GetStringField(Origin, "xyz", VecStr)) return FVector::ZeroVector;

    FVector Result;
    if (!JsonManager->ParseVectorString(VecStr, Result)) return FVector::ZeroVector;
    return Result;
}

FVector USettingsMenu::GetOriginRPY()
{
    TSharedPtr<FJsonObject> Root;
    if (!JsonManager->LoadJsonObject(ConfigFolderPath + JsonFileName, Root)) return FVector::ZeroVector;

    if (!Root->HasField("actors")) return FVector::ZeroVector;

    const TArray<TSharedPtr<FJsonValue>>* ActorsArray;
    Root->TryGetArrayField("actors", ActorsArray);
    if (!ActorsArray || ActorsArray->Num() == 0) return FVector::ZeroVector;

    TSharedPtr<FJsonObject> FirstActor = (*ActorsArray)[0]->AsObject();
    if (!FirstActor.IsValid()) return FVector::ZeroVector;

    TSharedPtr<FJsonObject> Origin = FirstActor->GetObjectField("origin");
    if (!Origin.IsValid()) return FVector::ZeroVector;

    FString VecStr;
    if (!JsonManager->GetStringField(Origin, "rpy-deg", VecStr)) return FVector::ZeroVector;

    FVector Result;
    if (!JsonManager->ParseVectorString(VecStr, Result)) return FVector::ZeroVector;


    return Result;
}

FString USettingsMenu::GetClockType() {
  TSharedPtr<FJsonObject> Root;
  if (!JsonManager->LoadJsonObject(ConfigFolderPath + JsonFileName, Root))
    return "";

  if (!Root->HasField("clock")) return "";

  TSharedPtr<FJsonObject> ClockObj = Root->GetObjectField("clock");
  if (!ClockObj.IsValid()) return "";

  FString ClockType;
  if (!JsonManager->GetStringField(ClockObj, TEXT("type"), ClockType))
    return "";

  return ClockType;
}

int64 USettingsMenu::GetClockStepNs() {
  TSharedPtr<FJsonObject> Root;
  if (!JsonManager->LoadJsonObject(ConfigFolderPath + JsonFileName, Root))
    return 0;

  if (!Root->HasField("clock")) return 0;

  TSharedPtr<FJsonObject> ClockObj = Root->GetObjectField("clock");
  if (!ClockObj.IsValid()) return 0;

  int64 StepNs = 0;
  // Use FJsonObject’s API directly for numbers
  if (!ClockObj->TryGetNumberField(TEXT("step-ns"), StepNs)) return 0;

  return StepNs;
}

int64 USettingsMenu::GetClockRealTimeUpdateRate() {
  TSharedPtr<FJsonObject> Root;
  if (!JsonManager->LoadJsonObject(ConfigFolderPath + JsonFileName, Root))
    return 0;

  if (!Root->HasField("clock")) return 0;

  TSharedPtr<FJsonObject> ClockObj = Root->GetObjectField("clock");
  if (!ClockObj.IsValid()) return 0;

  int64 UpdateRate = 0;
  if (!ClockObj->TryGetNumberField(TEXT("real-time-update-rate"), UpdateRate))
    return 0;

  return UpdateRate;
}

bool USettingsMenu::GetClockPauseOnStart() {
  TSharedPtr<FJsonObject> Root;
  if (!JsonManager->LoadJsonObject(ConfigFolderPath + JsonFileName, Root))
    return false;

  if (!Root->HasField("clock")) return false;

  TSharedPtr<FJsonObject> ClockObj = Root->GetObjectField("clock");
  if (!ClockObj.IsValid()) return false;

  bool bPauseOnStart = false;
  if (!ClockObj->TryGetBoolField(TEXT("pause-on-start"), bPauseOnStart))
    return false;

  return bPauseOnStart;
}


// Get the scene type from the JSON configuration file
FString USettingsMenu::GetSceneType()
{
    
    TSharedPtr<FJsonObject> Root;
    if (!JsonManager->LoadJsonObject(ConfigFolderPath + JsonFileName, Root)) return "";
    

    FString SceneTypeString;
    if (!JsonManager->GetStringField(Root, "scene-type", SceneTypeString)) return "";
    

    return SceneTypeString;
}


bool USettingsMenu::SetOriginXYZ(const FVector& NewValue)
{
    TSharedPtr<FJsonObject> Root;
    if (!JsonManager->LoadJsonObject(ConfigFolderPath + JsonFileName, Root)) return false;

    if (!Root->HasField("actors")) return false;

    const TArray<TSharedPtr<FJsonValue>>* ActorsArray;
    Root->TryGetArrayField("actors", ActorsArray);
    if (!ActorsArray || ActorsArray->Num() == 0) return false;

    TSharedPtr<FJsonObject> FirstActor = (*ActorsArray)[0]->AsObject();
    if (!FirstActor.IsValid()) return false;

    TSharedPtr<FJsonObject> Origin = FirstActor->GetObjectField("origin");
    if (!Origin.IsValid()) return false;

    JsonManager->SetStringField(Origin, "xyz", JsonManager->VectorToString(NewValue));
    return JsonManager->SaveJsonObject(ConfigFolderPath + JsonFileName, Root);
}


bool USettingsMenu::SetOriginRPY(const FVector& NewValue)
{
    TSharedPtr<FJsonObject> Root;
    if (!JsonManager->LoadJsonObject(ConfigFolderPath + JsonFileName, Root)) return false;

    if (!Root->HasField("actors")) return false;

    const TArray<TSharedPtr<FJsonValue>>* ActorsArray;
    Root->TryGetArrayField("actors", ActorsArray);
    if (!ActorsArray || ActorsArray->Num() == 0) return false;

    TSharedPtr<FJsonObject> FirstActor = (*ActorsArray)[0]->AsObject();
    if (!FirstActor.IsValid()) return false;

    TSharedPtr<FJsonObject> Origin = FirstActor->GetObjectField("origin");
    if (!Origin.IsValid()) return false;

    JsonManager->SetStringField(Origin, "rpy-deg", JsonManager->VectorToString(NewValue));
    return JsonManager->SaveJsonObject(ConfigFolderPath + JsonFileName, Root);
}

bool USettingsMenu::SetSceneType(const FString& NewValue) {
  TSharedPtr<FJsonObject> Root;
  if (!JsonManager->LoadJsonObject(ConfigFolderPath + JsonFileName, Root)) return false;

        

  if (!NewValue.Equals("UnrealNative", ESearchCase::CaseSensitive) &&
      !NewValue.Equals("CustomGIS", ESearchCase::CaseSensitive)) {
      FString SceneTypeString;
      JsonManager->GetStringField(Root, "scene-type", SceneTypeString);
      SceneTypeInput->SetText(FText::FromString(SceneTypeString));
   return false;

   }

  JsonManager->SetStringField(Root, "scene-type", NewValue);
  return JsonManager->SaveJsonObject(ConfigFolderPath + JsonFileName, Root);
}


void USettingsMenu::OnSaveClicked()
{
    FVector NewOriginXYZ;
    if (XInput) NewOriginXYZ.X = FCString::Atof(*XInput->GetText().ToString());
    if (YInput) NewOriginXYZ.Y = FCString::Atof(*YInput->GetText().ToString());
    if (ZInput) NewOriginXYZ.Z = FCString::Atof(*ZInput->GetText().ToString());
    SetOriginXYZ(NewOriginXYZ);

    FVector NewOriginRPY;
    if (RollInput) NewOriginRPY.X = FCString::Atof(*RollInput->GetText().ToString());
    if (PitchInput) NewOriginRPY.Y = FCString::Atof(*PitchInput->GetText().ToString());
    if (YawInput) NewOriginRPY.Z = FCString::Atof(*YawInput->GetText().ToString());
    SetOriginRPY(NewOriginRPY);

    FString NewSceneType;
    if (SceneTypeInput) NewSceneType = *SceneTypeInput->GetText().ToString();
    SetSceneType(NewSceneType);


    if (JsonFileNameInput) SetJsonFileName(JsonFileNameInput->GetText().ToString());
    if (PythonFileNameInput) SetPythonFileName(PythonFileNameInput->GetText().ToString());

}

void USettingsMenu::ReadJsonData()
{
    FVector OriginXYZ = GetOriginXYZ();
    if (XInput) XInput->SetText(FText::AsNumber(OriginXYZ.X));
    if (YInput) YInput->SetText(FText::AsNumber(OriginXYZ.Y));
    if (ZInput) ZInput->SetText(FText::AsNumber(OriginXYZ.Z));

    FVector OriginRPY = GetOriginRPY();
    if (RollInput) RollInput->SetText(FText::AsNumber(OriginRPY.X));
    if (PitchInput) PitchInput->SetText(FText::AsNumber(OriginRPY.Y));
    if (YawInput) YawInput->SetText(FText::AsNumber(OriginRPY.Z));

    FString SceneType = GetSceneType();
    if (SceneTypeInput) SceneTypeInput->SetText(FText::FromString(SceneType));

    FString ID = GetID();
    if (IDInput) IDInput->SetText(FText::FromString(ID));

    FString ActorType = GetActorType();
    if (ActorTypeInput) ActorTypeInput->SetText(FText::FromString(ActorType));

    FString ActorName = GetActorName();
    if (ActorNameInput) ActorNameInput->SetText(FText::FromString(ActorName));
    
    FString RobotConfig = GetRobotConfig();
    if (RobotConfigInput) RobotConfigInput->SetText(FText::FromString(RobotConfig));

    FString ClockType = GetClockType();
    if (ClockTypeInput) ClockTypeInput->SetText(FText::FromString(ClockType));

    int64 StepNs = GetClockStepNs();
    if (StepNsInput) StepNsInput->SetText(FText::AsNumber(StepNs));

    int64 RealTimeRate = GetClockRealTimeUpdateRate();
    if (RealTimeRateInput)
      RealTimeRateInput->SetText(FText::AsNumber(RealTimeRate));

    bool bPauseOnStart = GetClockPauseOnStart();
    if (PauseOnStartInput)
      PauseOnStartInput->SetText(
          FText::FromString(bPauseOnStart ? "true" : "false"));

    
    if (JsonFileNameInput) JsonFileNameInput->SetText(FText::FromString(JsonFileName));
}

void USettingsMenu::RunPythonScript()
{

    IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

    if (!PlatformFile.FileExists(*(PythonFolderPath + PythonFileName)))
    {
        UE_LOG(LogTemp, Warning, TEXT("File not found: %s"), *(PythonFolderPath + PythonFileName));
        return;
    }

    FString Cmd = TEXT("C:\\Windows\\System32\\cmd.exe");
    
    FString Args = FString::Printf(
        TEXT("/K \"call %s && cd /d %s && python %s && cmd\""),
        *VirtualEnvActivatePath,
        *PythonFolderPath,
        *PythonFileName);
    
    if (FPlatformProcess::IsProcRunning(CurrentPythonProcess)) FPlatformProcess::TerminateProc(CurrentPythonProcess, true);

    CurrentPythonProcess = FPlatformProcess::CreateProc(
        *Cmd, 
        *Args, //params
        false, //bLaunchDetatched
        false, //bLaunchHidden
        false, //bLaunchReallyHidden
        nullptr, 0, nullptr, nullptr);
}

void USettingsMenu::KillCurrentProcess()
{
    if (FPlatformProcess::IsProcRunning(CurrentPythonProcess)) FPlatformProcess::TerminateProc(CurrentPythonProcess, true);
    else UE_LOG(LogTemp, Warning, TEXT("No current process active!"));

}
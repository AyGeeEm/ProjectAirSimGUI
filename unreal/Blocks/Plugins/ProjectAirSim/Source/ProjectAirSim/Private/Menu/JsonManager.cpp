#include "Menu/JsonManager.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Misc/RegexUtils.h"

bool UJsonManager::LoadJsonFile(const FString& RelativeFilePath, FString& OutJsonString)
{
    const FString FullPath = FPaths::ProjectDir() / RelativeFilePath;
    return FFileHelper::LoadFileToString(OutJsonString, *FullPath);
}

bool UJsonManager::SaveJsonFile(const FString& RelativeFilePath, const FString& JsonString)
{
    const FString FullPath = FPaths::ProjectDir() / RelativeFilePath;
    return FFileHelper::SaveStringToFile(JsonString, *FullPath);
}

bool UJsonManager::LoadJsonObject(const FString& RelativeFilePath, TSharedPtr<FJsonObject>& OutObject)
{
    FString JsonString;
    if (!LoadJsonFile(RelativeFilePath, JsonString)) return false;
    JsonString = StripJsonComments(JsonString);
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);
    return FJsonSerializer::Deserialize(Reader, OutObject);
}

bool UJsonManager::SaveJsonObject(const FString& RelativeFilePath, const TSharedPtr<FJsonObject>& JsonObject)
{
    FString JsonString;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonString);
    if (!FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer)) return false;
    return SaveJsonFile(RelativeFilePath, JsonString);
}

bool UJsonManager::SetNumberField(TSharedPtr<FJsonObject> JsonObject, const FString& FieldName, double Value)
{
    if (!JsonObject.IsValid()) return false;
    JsonObject->SetNumberField(FieldName, Value);
    return true;
}

bool UJsonManager::GetNumberField(TSharedPtr<FJsonObject> JsonObject, const FString& FieldName, double& OutValue)
{
    if (!JsonObject.IsValid() || !JsonObject->HasTypedField<EJson::Number>(FieldName)) return false;
    OutValue = JsonObject->GetNumberField(FieldName);
    return true;
}

bool UJsonManager::ParseVectorString(const FString& VectorString, FVector& OutVector)
{
    TArray<FString> Parts;
    VectorString.ParseIntoArray(Parts, TEXT(" "), true);
    if (Parts.Num() != 3) return false;

    OutVector.X = FCString::Atod(*Parts[0]);
    OutVector.Y = FCString::Atod(*Parts[1]);
    OutVector.Z = FCString::Atod(*Parts[2]);
    return true;
}

FString UJsonManager::VectorToString(const FVector& Vector)
{
    return FString::Printf(TEXT("%f %f %f"), Vector.X, Vector.Y, Vector.Z);
}

bool UJsonManager::SetBoolField(TSharedPtr<FJsonObject> JsonObject, const FString& FieldName, bool Value)
{
    if (!JsonObject.IsValid()) return false;
    JsonObject->SetBoolField(FieldName, Value);
    return true;
}

bool UJsonManager::GetBoolField(TSharedPtr<FJsonObject> JsonObject, const FString& FieldName, bool& OutValue)
{
    if (!JsonObject.IsValid() || !JsonObject->HasTypedField<EJson::Boolean>(FieldName)) return false;
    OutValue = JsonObject->GetBoolField(FieldName);
    return true;
}

bool UJsonManager::SetStringField(TSharedPtr<FJsonObject> JsonObject, const FString& FieldName, const FString& Value)
{
    if (!JsonObject.IsValid()) return false;
    JsonObject->SetStringField(FieldName, Value);
    return true;
}


bool UJsonManager::GetStringField(TSharedPtr<FJsonObject> JsonObject, const FString& FieldName, FString& OutValue)
{
    if (!JsonObject.IsValid() || !JsonObject->HasTypedField<EJson::String>(FieldName)) return false;
    OutValue = JsonObject->GetStringField(FieldName);
    return true;
}

bool UJsonManager::GetNestedObject(TSharedPtr<FJsonObject> ParentObject, const FString& FieldName, TSharedPtr<FJsonObject>& OutObject)
{
    if (!ParentObject.IsValid()) return false;
    const TSharedPtr<FJsonObject>* NestedPtr = nullptr;

    if (!ParentObject->TryGetObjectField(FieldName, NestedPtr) || !NestedPtr || !NestedPtr->IsValid()) {
      return false;
    }

    OutObject = *NestedPtr;
    return true;
}

bool UJsonManager::GetObjectArray(TSharedPtr<FJsonObject> ParentObject, const FString& FieldName, TArray<TSharedPtr<FJsonValue>>& OutArray)
{
    if (!ParentObject.IsValid()) return false;
    const TArray<TSharedPtr<FJsonValue>>* ArrayPtr = nullptr;

    if (!ParentObject->TryGetArrayField(FieldName, ArrayPtr) || !ArrayPtr) {
        return false;
    }

    OutArray = *ArrayPtr;
    return true;
}

bool UJsonManager::GetObjectFromArray(const TArray<TSharedPtr<FJsonValue>>& Array, int32 Index, TSharedPtr<FJsonObject>& OutObject)
{
    if (Index < 0 || Index >= Array.Num()) return false;
    const TSharedPtr<FJsonValue>& Value = Array[Index];
    if (!Value.IsValid() || Value->Type != EJson::Object) return false;

    OutObject = Value->AsObject();
    return OutObject.IsValid();
}

bool UJsonManager::LoadReferencedRobotConfig(const FString& SceneConfigPath, const FString& ConfigFolderPath, TSharedPtr<FJsonObject>& OutRobotConfig, FString& OutRobotConfigFileName)
{
    //Load the scene config
    TSharedPtr<FJsonObject> SceneConfig;
    if (!LoadJsonObject(SceneConfigPath, SceneConfig)) {
        UE_LOG(LogTemp, Warning, TEXT("Failed to load scene config: %s"), *SceneConfigPath);
        return false;
    }

    //Get actors array
    TArray<TSharedPtr<FJsonValue>> ActorsArray;
    if (!GetObjectArray(SceneConfig, TEXT("actors"), ActorsArray) || ActorsArray.Num() == 0) {
        UE_LOG(LogTemp, Warning, TEXT("No actors found in scene config"));
        return false;
    }

    //Get the first actor
    TSharedPtr<FJsonObject> FirstActor;
    if (!GetObjectFromArray(ActorsArray, 0, FirstActor)) {
        UE_LOG(LogTemp, Warning, TEXT("Failed to get first actor from array"));
        return false;
    }

    //Extract robot-config filename
    FString RobotConfigFileName;
    if (!GetStringField(FirstActor, TEXT("robot-config"), RobotConfigFileName)) {
        UE_LOG(LogTemp, Warning, TEXT("No robot-config field found in actor"));
        return false;
    }

    //Load the referenced robot config file
    FString RobotConfigPath = ConfigFolderPath + RobotConfigFileName;
    if (!LoadJsonObject(RobotConfigPath, OutRobotConfig)) {
        UE_LOG(LogTemp, Warning, TEXT("Failed to load robot config: %s"), *RobotConfigPath);
        return false;
    }

    OutRobotConfigFileName = RobotConfigFileName;
    UE_LOG(LogTemp, Log, TEXT("Successfully loaded robot config: %s"), *RobotConfigFileName);
    return true;
}

bool UJsonManager::GetNestedField(TSharedPtr<FJsonObject> RootObject, const FString& DotPath, FString& OutValue)
{
    if (!RootObject.IsValid() || DotPath.IsEmpty()) return false;

    TArray<FString> PathParts;
    DotPath.ParseIntoArray(PathParts, TEXT("."), true);

    TSharedPtr<FJsonObject> CurrentObject = RootObject;

    for (int32 i = 0; i < PathParts.Num(); i++) {
        const FString& Part = PathParts[i];
        bool bIsLastPart = (i == PathParts.Num() - 1);

        if (Part.IsNumeric()) {
            int32 Index = FCString::Atoi(*Part);
            // CHECK IF WE'RE ACCESSING AN ARRAY FIELD FIRST!
            UE_LOG(LogTemp, Warning, TEXT("Array access in dot notation not fully supported yet"));
            return false;
        }

        if (bIsLastPart) {
            // Try to get as string
            if (CurrentObject->HasTypedField<EJson::String>(Part)) {
                OutValue = CurrentObject->GetStringField(Part);
                return true;
            }
            // Try to get as number and convert to string
            else if (CurrentObject->HasTypedField<EJson::Number>(Part)) {
                OutValue = FString::SanitizeFloat(CurrentObject->GetNumberField(Part));
                return true;
            }
            // Try to get as bool and convert to string
            else if (CurrentObject->HasTypedField<EJson::Boolean>(Part)) {
                OutValue = CurrentObject->GetBoolField(Part) ? TEXT("true") : TEXT("false");
                return true;
            }
            return false;
        } else {
            // Navigate to nested object
            if (!GetNestedObject(CurrentObject, Part, CurrentObject)) {
                return false;
            }
        }
    }

    return false;
}

bool UJsonManager::SetNestedField(TSharedPtr<FJsonObject> RootObject, const FString& DotPath, const FString& Value)
{
    if (!RootObject.IsValid() || DotPath.IsEmpty()) return false;

    TArray<FString> PathParts;
    DotPath.ParseIntoArray(PathParts, TEXT("."), true);

    TSharedPtr<FJsonObject> CurrentObject = RootObject;

    for (int32 i = 0; i < PathParts.Num(); i++) {
        const FString& Part = PathParts[i];
        bool bIsLastPart = (i == PathParts.Num() - 1);

        if (bIsLastPart) {
            CurrentObject->SetStringField(Part, Value);
            return true;
        } else {
            if (!GetNestedObject(CurrentObject, Part, CurrentObject)) {
                return false;
            }
        }
    }

    return false;
}

bool UJsonManager::ExtractSceneConfigFromPython(const FString& PythonFilePath, FString& OutSceneConfigName)
{
    FString PythonContent;
    if (!FFileHelper::LoadFileToString(PythonContent, *PythonFilePath)) {
        UE_LOG(LogTemp, Warning, TEXT("Failed to load Python file: %s"), *PythonFilePath);
        return false;
    }

    FRegexPattern Pattern(TEXT("World\\s*\\(\\s*[^,]+,\\s*[\"']([^\"']+\\.jsonc)[\"']"));
    FRegexMatcher Matcher(Pattern, PythonContent);

    if (Matcher.FindNext()) {
        OutSceneConfigName = Matcher.GetCaptureGroup(1);
        UE_LOG(LogTemp, Log, TEXT("Found scene config in Python: %s"), *OutSceneConfigName);
        return true;
    }

    FRegexPattern AltPattern(TEXT("scene_config\\s*=\\s*[\"']([^\"']+\\.jsonc)[\"']"));
    FRegexMatcher AltMatcher(AltPattern, PythonContent);

    if (AltMatcher.FindNext()) {
        OutSceneConfigName = AltMatcher.GetCaptureGroup(1);
        UE_LOG(LogTemp, Log, TEXT("Found scene config in Python (alt pattern): %s"), *OutSceneConfigName);
        return true;
    }

    UE_LOG(LogTemp, Warning, TEXT("No scene config reference found in Python file"));
    return false;
}

bool UJsonManager::UpdateSceneConfigInPython(const FString& PythonFilePath, const FString& NewSceneConfigName)
{
    FString PythonContent;
    if (!FFileHelper::LoadFileToString(PythonContent, *PythonFilePath)) {
        UE_LOG(LogTemp, Warning, TEXT("Failed to load Python file: %s"), *PythonFilePath);
        return false;
    }

    FRegexPattern Pattern(TEXT("(World\\s*\\(\\s*[^,]+,\\s*)[\"'][^\"']+\\.jsonc[\"']"));

    FString Replacement = FString::Printf(TEXT("$1\"%s\""), *NewSceneConfigName);
    FString UpdatedContent = FRegexPattern::Replace(PythonContent, Pattern, Replacement);

    if (UpdatedContent == PythonContent) {
        UE_LOG(LogTemp, Warning, TEXT("No replacements made in Python file"));
        return false;
    }

    if (!FFileHelper::SaveStringToFile(UpdatedContent, *PythonFilePath)) {
        UE_LOG(LogTemp, Warning, TEXT("Failed to save updated Python file"));
        return false;
    }

    UE_LOG(LogTemp, Log, TEXT("Updated scene config in Python to: %s"), *NewSceneConfigName);
    return true;
}

FString UJsonManager::StripJsonComments(const FString& JsonString) {
    FString Result;
    Result.Reserve(JsonString.Len());

    bool bInString = false;
    bool bInSingleLineComment = false;
    bool bInMultiLineComment = false;

    for (int32 i = 0; i < JsonString.Len(); i++) {
        TCHAR Current = JsonString[i];
        TCHAR Next = (i + 1 < JsonString.Len()) ? JsonString[i + 1] : '\0';

        //Handle string literals
        if (!bInSingleLineComment && !bInMultiLineComment) {
            if (Current == '"' && (i == 0 || JsonString[i - 1] != '\\')) {
                bInString = !bInString;
            }
        }

        //Comment start
        if (!bInString && !bInSingleLineComment && !bInMultiLineComment) {
            if (Current == '/' && Next == '/') {
                bInSingleLineComment = true;
                i++;
                continue;
            } else if (Current == '/' && Next == '*') {
                bInMultiLineComment = true;
                i++;
                continue;
            }
        }

        //Comment end
        if (bInSingleLineComment) {
            if (Current == '\n' || Current == '\r') {
                bInSingleLineComment = false;
                Result.AppendChar(Current);
            }
            continue;
        }

        if (bInMultiLineComment) {
            if (Current == '*' && Next == '/') {
                bInMultiLineComment = false;
                i++;
            }
            continue;
        }

        Result.AppendChar(Current);
    }

    return Result;
}

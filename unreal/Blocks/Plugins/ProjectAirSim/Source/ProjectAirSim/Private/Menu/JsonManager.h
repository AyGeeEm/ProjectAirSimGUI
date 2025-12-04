#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "JsonManager.generated.h"

UCLASS()
class UJsonManager : public UObject
{
	GENERATED_BODY()

  public:
	bool LoadJsonFile(const FString& RelativeFilePath, FString& OutJsonString);
	bool SaveJsonFile(const FString& RelativeFilePath, const FString& JsonString);
	bool LoadJsonObject(const FString& RelativeFilePath, TSharedPtr<FJsonObject>& OutObject);
	bool SaveJsonObject(const FString& RelativeFilePath, const TSharedPtr<FJsonObject>& JsonObject);
	bool SetNumberField(TSharedPtr<FJsonObject> JsonObject, const FString& FieldName, double Value);
	bool GetNumberField(TSharedPtr<FJsonObject> JsonObject, const FString& FieldName, double& OutValue);
	bool ParseVectorString(const FString& VectorString, FVector& OutVector);
	FString VectorToString(const FVector& Vector);
	bool SetBoolField(TSharedPtr<FJsonObject> JsonObject, const FString& FieldName, bool Value);
	bool GetBoolField(TSharedPtr<FJsonObject> JsonObject, const FString& FieldName, bool& OutValue);
	bool SetStringField(TSharedPtr<FJsonObject> JsonObject, const FString& FieldName, const FString& Value);
	bool GetStringField(TSharedPtr<FJsonObject> JsonObject, const FString& FieldName, FString& OutValue);
    bool GetNestedObject(TSharedPtr<FJsonObject> ParentObject, const FString& FieldName, TSharedPtr<FJsonObject>& OutObject);
    bool GetObjectArray(TSharedPtr<FJsonObject> ParentObject, const FString& FieldName, TArray<TSharedPtr<FJsonValue>>& OutArray);
    bool GetObjectFromArray(const TArray<TSharedPtr<FJsonValue>>& Array, int32 Index, TSharedPtr<FJsonObject>& OutObject);
    bool LoadReferencedRobotConfig(const FString& SceneConfigPath, const FString& ConfigFolderPath, TSharedPtr<FJsonObject>& OutRobotConfig, FString& OutRobotConfigFileName);
    bool GetNestedField(TSharedPtr<FJsonObject> RootObject, const FString& DotPath, FString& OutValue);
    bool SetNestedField(TSharedPtr<FJsonObject> RootObject, const FString& DotPath, const FString& Value);
    bool ExtractSceneConfigFromPython(const FString& PythonFilePath, FString& OutSceneConfigName);
    bool UpdateSceneConfigInPython(const FString& PythonFilePath, const FString& NewSceneConfigName);
};

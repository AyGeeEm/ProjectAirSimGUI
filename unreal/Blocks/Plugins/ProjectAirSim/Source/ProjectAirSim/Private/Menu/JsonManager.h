#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Templates/SharedPointer.h"

#include "JsonManager.generated.h"

UCLASS()
class UJsonManager : public UObject
{
	GENERATED_BODY()

  public:
	static bool LoadJsonFile(const FString& RelativeFilePath, FString& OutJsonString);
	static bool SaveJsonFile(const FString& RelativeFilePath, const FString& JsonString);
	static bool LoadJsonObject(const FString& RelativeFilePath, TSharedPtr<FJsonObject>& OutObject);
	static bool SaveJsonObject(const FString& RelativeFilePath, const TSharedPtr<FJsonObject>& JsonObject);
	static bool SetNumberField(TSharedPtr<FJsonObject> JsonObject, const FString& FieldName, double Value);
	static bool GetNumberField(TSharedPtr<FJsonObject> JsonObject, const FString& FieldName, double& OutValue);
	static bool ParseVectorString(const FString& VectorString, FVector& OutVector);
	static FString VectorToString(const FVector& Vector);
	static bool SetBoolField(TSharedPtr<FJsonObject> JsonObject, const FString& FieldName, bool Value);
	static bool GetBoolField(TSharedPtr<FJsonObject> JsonObject, const FString& FieldName, bool& OutValue);
	static bool SetStringField(TSharedPtr<FJsonObject> JsonObject, const FString& FieldName, const FString& Value);
	static bool GetStringField(TSharedPtr<FJsonObject> JsonObject, const FString& FieldName, FString& OutValue);
    static bool GetObjectArray(TSharedPtr<FJsonObject> ParentObject, const FString& FieldName, TArray<TSharedPtr<FJsonValue>>& OutArray);
    static bool ExtractSceneConfigFromPython(const FString& PythonFilePath, FString& OutSceneConfigName);
    static FString StripJsonComments(const FString& JsonString);
};

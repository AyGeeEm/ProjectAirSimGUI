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
};

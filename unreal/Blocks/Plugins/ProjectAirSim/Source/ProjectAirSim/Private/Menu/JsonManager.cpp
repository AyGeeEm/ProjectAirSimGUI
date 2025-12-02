#include "Menu/JsonManager.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"

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
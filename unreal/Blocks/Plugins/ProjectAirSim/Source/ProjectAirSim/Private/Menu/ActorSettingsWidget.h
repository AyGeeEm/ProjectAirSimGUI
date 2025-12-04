#pragma once

#include "CoreMinimal.h"
#include "ActorSettingsWidget.generated.h"

class UActorSettings;

UCLASS()
class UActorSettingsWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	void Init(UActorSettings* OwnerIn);

	UFUNCTION()
	void CommitName(const FText& Text, ETextCommit::Type CommitType);
	UFUNCTION()
	void CommitX(const FText& Text, ETextCommit::Type CommitType);
	UFUNCTION()
	void CommitY(const FText& Text, ETextCommit::Type CommitType);
	UFUNCTION()
	void CommitZ(const FText& Text, ETextCommit::Type CommitType);
	UFUNCTION()
	void CommitRoll(const FText& Text, ETextCommit::Type CommitType);
	UFUNCTION()
	void CommitPitch(const FText& Text, ETextCommit::Type CommitType);
	UFUNCTION()
	void CommitYaw(const FText& Text, ETextCommit::Type CommitType);

	UPROPERTY(meta = (BindWidget))
    UEditableTextBox* NameDisplay;

	UPROPERTY(meta = (BindWidget))
    UEditableTextBox* XDisplay;

	UPROPERTY(meta = (BindWidget))
    UEditableTextBox* YDisplay;

	UPROPERTY(meta = (BindWidget))
    UEditableTextBox* ZDisplay;

	UPROPERTY(meta = (BindWidget))
    UEditableTextBox* RollDisplay;

	UPROPERTY(meta = (BindWidget))
    UEditableTextBox* PitchDisplay;

	UPROPERTY(meta = (BindWidget))
    UEditableTextBox* YawDisplay;

	UPROPERTY(meta = (BindWidget))
    UEditableTextBox* RobotConfigNameDisplay;

private:

	UActorSettings* Owner;
	
};
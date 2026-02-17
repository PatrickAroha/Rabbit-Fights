#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Deep/Public/HUDTypes.h"
#include "MyHUD.generated.h"

UCLASS()
class AMyHUD : public AHUD
{
	GENERATED_BODY()

	/* Current HUD state */
	EHUDState CurrentState;

public:
	UFUNCTION(BlueprintCallable, Category = "HUD")
	EHUDState GetCurrentState() const;

	/* Event hook to update HUD state (eg. to determine visibility of widgets) */
	UFUNCTION(BlueprintNativeEvent, Category = "HUDEvents")
	void OnStateChanged(EHUDState NewState);
};
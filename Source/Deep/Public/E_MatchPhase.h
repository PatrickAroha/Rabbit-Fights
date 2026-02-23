#pragma once
#include "CoreMinimal.h"
#include "E_MatchPhase.generated.h"

UENUM(BlueprintType)
enum class EMatchPhase : uint8
{
	WaitingPlayers,
	SelectingGameRule,
	DrawGameRule,
	IntroducingMinigame,
	Countdown,
	Playing,
	FinishMatch,
	FinishGame
};
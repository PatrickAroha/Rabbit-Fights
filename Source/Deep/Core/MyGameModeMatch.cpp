#include "MyGameModeMatch.h"
#include "MyGameInstance.h"
#include "MyGameState.h"
#include "GameFramework/GameStateBase.h"


void AMyGameModeMatch::InitGameState()
{
	Super::InitGameState();

	if (MyGI)
	MyGI = GetGameInstance<UMyGameInstance>();

	if (MyGS)
	MyGS = GetGameState<AMyGameState>();
}
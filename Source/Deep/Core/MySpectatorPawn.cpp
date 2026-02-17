#include "MySpectatorPawn.h"


AMySpectatorPawn::AMySpectatorPawn()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMySpectatorPawn::SetupPlayerInputComponent(UInputComponent* InInputComponent)
{
	Super::SetupPlayerInputComponent(InInputComponent);

	InInputComponent->BindAction("ViewNext", IE_Pressed, this, &ThisClass::ViewNextPlayer);
	InInputComponent->BindAction("ViewPrev", IE_Pressed, this, &ThisClass::ViewPrevPlayer);
}

void AMySpectatorPawn::ViewNextPlayer()
{
	if (APlayerController* PC = GetController<APlayerController>())
	{
		PC->ServerViewNextPlayer();
	}
}

void AMySpectatorPawn::ViewPrevPlayer()
{
	if (APlayerController* PC = GetController<APlayerController>())
	{
		PC->ServerViewPrevPlayer();
	}
}

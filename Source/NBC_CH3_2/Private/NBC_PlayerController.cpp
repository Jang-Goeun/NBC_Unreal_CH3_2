#include "NBC_PlayerController.h"
#include "EnhancedInputSubsystems.h"

ANBC_PlayerController::ANBC_PlayerController()
	:   InputMappingContext(nullptr),
		MoveAction(nullptr),
		LookAction(nullptr)
{
}

void ANBC_PlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (InputMappingContext)
			{
				Subsystem->AddMappingContext(InputMappingContext, 0);
			}
		}
	}
}

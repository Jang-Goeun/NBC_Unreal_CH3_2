#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "NBC_PlayerController.generated.h"

class UInputMappingContext;
class UInputAction;

UCLASS()
class NBC_CH3_2_API ANBC_PlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ANBC_PlayerController();

	// IMC
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	UInputMappingContext* InputMappingContext;

	// IA_Move
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	UInputAction* MoveAction;

	// IA_Look
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	UInputAction* LookAction;

	virtual void BeginPlay() override;
};

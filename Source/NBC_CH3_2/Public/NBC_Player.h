#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Materials/MaterialInterface.h"
#include "NBC_Player.generated.h"

class UCapsuleComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class USpringArmComponent;
struct FInputActionValue;

UCLASS()
class NBC_CH3_2_API ANBC_Player : public APawn
{
	GENERATED_BODY()

public:
	ANBC_Player();

protected:
	virtual void BeginPlay() override;

	// Capsule 컴포넌트 포인터
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCapsuleComponent* CapsuleComponent;
	// SkeletalMesh 컴포넌트 포인터
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USkeletalMeshComponent* SkeletalMeshComponent;
	// SpringArm 컴포넌트 포인터
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USpringArmComponent* SpringArmComponent;
	// Camera 컴포넌트 포인터
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCameraComponent* CameraComponent;

	// 입력 바인딩 처리 함수
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// 액션 함수
	UFUNCTION()
	void Move(const FInputActionValue& Value);
	UFUNCTION()
	void Look(const FInputActionValue& Value);
	UFUNCTION()
	void Roll(const FInputActionValue& Value);

};

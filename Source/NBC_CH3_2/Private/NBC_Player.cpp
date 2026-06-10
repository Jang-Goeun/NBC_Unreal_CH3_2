#include "NBC_Player.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "NBC_PlayerController.h"
#include "Kismet/GameplayStatics.h"

ANBC_Player::ANBC_Player()
{
 	PrimaryActorTick.bCanEverTick = false;

	// 충돌 컴포넌트 생성 후 루트로 설정
	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	SetRootComponent(CapsuleComponent);

	// Physics 설정(루트 충돌체)
	if (CapsuleComponent)
	{
		CapsuleComponent->SetSimulatePhysics(false);
	}

	// 나머지 컴포넌트들 부착
	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	SkeletalMeshComponent->SetupAttachment(CapsuleComponent);

	// Physics 설정(Mesh)
	if (SkeletalMeshComponent)
	{
		SkeletalMeshComponent->SetSimulatePhysics(false);
	}

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(CapsuleComponent);
	SpringArmComponent->TargetArmLength = 300.0f;
	SpringArmComponent->bUsePawnControlRotation = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent);
	CameraComponent->bUsePawnControlRotation = false;

	// SkeletalMesh 코드에서 설정
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshAsset(TEXT("/Game/Characters/Mannequin_UE4/Meshes/SK_Mannequin.SK_Mannequin"));
	if (MeshAsset.Succeeded())
	{
		SkeletalMeshComponent->SetSkeletalMesh(MeshAsset.Object);
	}

	// material 설정
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> MaterialAsset(TEXT("/Game/Characters/Mannequin_UE4/Materials/M_MannequinUE4_Body.M_MannequinUE4_Body"));
	if (MaterialAsset.Succeeded())
	{
		SkeletalMeshComponent->SetMaterial(0, MaterialAsset.Object);
	}
}

void ANBC_Player::BeginPlay()
{
	Super::BeginPlay();

}

void ANBC_Player::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Enhanced InputComponent로 캐스팅
	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// NBC_PlayerController로 캐스팅
		if (ANBC_PlayerController* PlayerController = Cast<ANBC_PlayerController>(GetController()))
		{
			// IA_Move 액션 키를 누르고 있는 동안 Move() 호출
			if (PlayerController->MoveAction)
			{
				EnhancedInput->BindAction(
					PlayerController->MoveAction,
					ETriggerEvent::Triggered,
					this,
					&ANBC_Player::Move
				);
			}

			if (PlayerController->LookAction)
			{
				EnhancedInput->BindAction(
					PlayerController->LookAction,
					ETriggerEvent::Triggered,
					this,
					&ANBC_Player::Look
				);
			}
		}
	}
}

void ANBC_Player::Move(const FInputActionValue& Value)
{
	if (!Controller) return;

	// (X=1, Y=0) → 전진 / (X=-1, Y=0) → 후진 / (X=0, Y=1) → 오른쪽 / (X=0, Y=-1) → 왼쪽
	const FVector2D MoveInput = Value.Get<FVector2D>();
	float DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(GetWorld());
	float MoveSpeed = 500.0f;

	FVector DeltaLocation = FVector::ZeroVector;

	// 전진 혹은 후진
	if (!FMath::IsNearlyZero(MoveInput.X))
	{
		DeltaLocation.X = MoveInput.X * MoveSpeed * DeltaTime;
	}

	// 오른쪽 혹은 왼쪽
	if (!FMath::IsNearlyZero(MoveInput.Y))
	{
		DeltaLocation.Y = MoveInput.Y * MoveSpeed * DeltaTime;
	}

	AddActorLocalOffset(DeltaLocation, true);
}

void ANBC_Player::Look(const FInputActionValue& Value)
{
	if (!Controller) return;

	// LookInput.X = 마우스 좌우 이동량 (Yaw 회전)
	// LookInput.Y = 마우스 위아래 이동량 (Pitch 회전)
	const FVector2D LookInput = Value.Get<FVector2D>();
	float DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(GetWorld());
	float RotationSpeed = 45.0f;

	FRotator DeltaRotation = FRotator::ZeroRotator;

	// 전진 혹은 후진
	if (!FMath::IsNearlyZero(LookInput.X))
	{
		DeltaRotation.Yaw = LookInput.X * RotationSpeed * DeltaTime;
	}

	// 오른쪽 혹은 왼쪽
	if (!FMath::IsNearlyZero(LookInput.Y))
	{
		DeltaRotation.Pitch = LookInput.Y * RotationSpeed * DeltaTime;
	}

	AddActorLocalRotation(DeltaRotation, true);
}

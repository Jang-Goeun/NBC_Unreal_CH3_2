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
 	PrimaryActorTick.bCanEverTick = true;

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
	SpringArmComponent->bUsePawnControlRotation = false;

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

void ANBC_Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 중력
	VerticalVelocity += Gravity * DeltaTime;

	// 지면 충돌 감지
	FHitResult HitResult;
	FVector Start = GetActorLocation();
	float CapsuleHalfHeight = CapsuleComponent ? CapsuleComponent->GetScaledCapsuleHalfHeight() : 88.0f;

	// 라인트레이스
	FVector End = Start + FVector(0, 0, -(CapsuleHalfHeight + 5.0f));
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);

	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		Start,
		End,
		ECC_Visibility,
		CollisionParams
		);

	// 충돌 여부에 따른 행동
	if (bHit && VerticalVelocity < 0)
	{
		bIsGrounded = true;
		VerticalVelocity = 0.0f;

		FVector NewLocation = GetActorLocation();
		NewLocation.Z = HitResult.Location.Z + CapsuleHalfHeight;
		SetActorLocation(NewLocation);
	}
	else
	{
		bIsGrounded = false;
		AddActorLocalOffset(FVector(0, 0, VerticalVelocity * DeltaTime), true);
	}
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

			if (PlayerController->RollAction)
			{
				EnhancedInput->BindAction(
					PlayerController->RollAction,
					ETriggerEvent::Triggered,
					this,
					&ANBC_Player::Roll
				);
			}
		}
	}
}

void ANBC_Player::Move(const FInputActionValue& Value)
{
	if (!Controller) return;

	// (X=1, Y=0) → 전진 / (X=-1, Y=0) → 후진 / (X=0, Y=1) → 오른쪽 / (X=0, Y=-1) → 왼쪽
	const FVector MoveInput = Value.Get<FVector>();
	float DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(GetWorld());
	float CurrentSpeed = BaseMoveSpeed;

	if (!bIsGrounded)
	{
		CurrentSpeed *= AirControlFactor;
	}
	FVector Forward = GetActorForwardVector();
	FVector Right = GetActorRightVector();
	FVector Up = GetActorUpVector();

	if (MoveInput.Z > 0 && VerticalVelocity < 0)
	{
		VerticalVelocity = 0.0f;
	}

	FVector Direction = (Forward * MoveInput.X) + (Right * MoveInput.Y) + (Up * MoveInput.Z);

	AddActorLocalOffset(Direction * CurrentSpeed * DeltaTime, true);
}

void ANBC_Player::Look(const FInputActionValue& Value)
{
	if (!Controller) return;

	// LookInput.X = 마우스 좌우 이동량 (Yaw 회전)
	// LookInput.Y = 마우스 위아래 이동량 (Pitch 회전)
	const FVector2D LookInput = Value.Get<FVector2D>();
	float DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(GetWorld());
	float RotationSpeed = 50.0f;

	FRotator DeltaRotation(
		-LookInput.Y * RotationSpeed * DeltaTime, // Pitch (마우스 위아래)
	     LookInput.X * RotationSpeed * DeltaTime,  // Yaw (마우스 좌우)
	     0.0f
	);

	AddActorLocalRotation(DeltaRotation);
}

void ANBC_Player::Roll(const FInputActionValue& Value)
{
     if (!Controller) return;

     const float RollInput = Value.Get<float>();
     float DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(GetWorld());
	 float RollSpeed = 150.0f;

     // 로컬 X축(Forward)을 기준으로 회전
     AddActorLocalRotation(FRotator(0.0f, 0.0f, RollInput * RollSpeed *DeltaTime));
}

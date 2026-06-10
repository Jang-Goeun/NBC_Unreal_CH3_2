#include "NBC_Player.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"

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
	if (CapsuleComponent)
	{
		CapsuleComponent->SetSimulatePhysics(false);
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

void ANBC_Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ANBC_Player::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


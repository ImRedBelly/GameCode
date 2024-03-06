#include "FPPlayerCharacter.h"

#include "Components/CapsuleComponent.h"
#include "Controllers/GCPlayerController.h"
#include "GameCode/GameCodeTypes.h"
#include "GameCode/Components/MovementComponents/GCBaseCharacterMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

AFPPlayerCharacter::AFPPlayerCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	FirstPersonMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	FirstPersonMeshComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonMeshComponent->SetRelativeLocation(FVector(0.0f, 0.0f, -86.0f));
	FirstPersonMeshComponent->CastShadow = false;
	FirstPersonMeshComponent->bCastDynamicShadow = false;
	FirstPersonMeshComponent->SetOnlyOwnerSee(true);

	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(FirstPersonMeshComponent, SocketFPCamera);
	FirstPersonCameraComponent->bUsePawnControlRotation = true;


	GetMesh()->SetOwnerNoSee(true);
	GetMesh()->bCastHiddenShadow = true;

	CameraComponent->bAutoActivate = false;

	SpringArmComponent->bAutoActivate = false;
	SpringArmComponent->bUsePawnControlRotation = false;

	GetCharacterMovement()->bOrientRotationToMovement = false;
	bUseControllerRotationYaw = true;
}

void AFPPlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	GCPlayerController = Cast<AGCPlayerController>(NewController);
}

void AFPPlayerCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (IsFPMontagePlaying() && GCPlayerController.IsValid())
	{
		FRotator TargetControlRotation = GCPlayerController->GetControlRotation();
		TargetControlRotation.Pitch = 0;
		float BlendSpeed = 300;
		TargetControlRotation = FMath::RInterpTo(GCPlayerController->GetControlRotation(), TargetControlRotation,
		                                         DeltaSeconds, BlendSpeed);
		GCPlayerController->SetControlRotation(TargetControlRotation);
	}
}

void AFPPlayerCharacter::OnStartCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust)
{
	Super::OnStartCrouch(HalfHeightAdjust, ScaledHalfHeightAdjust);
	const AFPPlayerCharacter* DefaultCharacter = GetDefault<AFPPlayerCharacter>(GetClass());
	FVector& FirstPersonMeshRelativeLocation = FirstPersonCameraComponent->GetRelativeLocation_DirectMutable();
	FirstPersonMeshRelativeLocation.Z = DefaultCharacter->FirstPersonCameraComponent->GetRelativeLocation().Z +
		HalfHeightAdjust;
}

void AFPPlayerCharacter::OnEndCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust)
{
	Super::OnEndCrouch(HalfHeightAdjust, ScaledHalfHeightAdjust);
	const AFPPlayerCharacter* DefaultCharacter = GetDefault<AFPPlayerCharacter>(GetClass());
	FVector& FirstPersonMeshRelativeLocation = FirstPersonCameraComponent->GetRelativeLocation_DirectMutable();
	FirstPersonMeshRelativeLocation.Z = DefaultCharacter->FirstPersonCameraComponent->GetRelativeLocation().Z;
}

void AFPPlayerCharacter::OnMantling(const FMantlingSettings& MantlingSettings, float MantlingAnimationStartTime)
{
	Super::OnMantling(MantlingSettings, MantlingAnimationStartTime);
	UAnimInstance* FPAnimInstance = FirstPersonMeshComponent->GetAnimInstance();
	if (IsValid(FPAnimInstance) && MantlingSettings.FPMantlingMontage)
	{
		if (GCPlayerController.IsValid())
		{
			GCPlayerController->SetIgnoreLookInput(true);
			GCPlayerController->SetIgnoreMoveInput(true);
		}
		float MontageDuration = FPAnimInstance->Montage_Play(MantlingSettings.FPMantlingMontage, 1,
		                                                     EMontagePlayReturnType::Duration,
		                                                     MantlingAnimationStartTime);
		GetWorld()->GetTimerManager().SetTimer(FPMontageTimer, this, &AFPPlayerCharacter::OnFPMontageTimerElapsed,
		                                       MontageDuration, false);
	}
}

FRotator AFPPlayerCharacter::GetViewRotation() const
{
	FRotator Result = Super::GetViewRotation();
	if (IsFPMontagePlaying())
	{
		FRotator SocketRotation = FirstPersonMeshComponent->GetSocketRotation(SocketFPCamera);
		Result.Pitch += SocketRotation.Pitch;
		Result.Yaw = SocketRotation.Yaw;
		Result.Roll = SocketRotation.Roll;
	}
	return Result;
}

void AFPPlayerCharacter::OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PrevMovementMode, PreviousCustomMode);
	if (GetBaseCharacterMovementComponent()->IsOnLadder())
	{
		OnLadderStarted();
	}
	else if (PreviousCustomMode == (uint8)ECustomMovementMode::CMOVE_Ladder)
	{
		OnLadderStopped();
	}
}

void AFPPlayerCharacter::OnFPMontageTimerElapsed()
{
	if (GCPlayerController.IsValid())
	{
		GCPlayerController->SetIgnoreLookInput(false);
		GCPlayerController->SetIgnoreMoveInput(false);
	}
}

bool AFPPlayerCharacter::IsFPMontagePlaying() const
{
	UAnimInstance* FPAnimInstance = FirstPersonMeshComponent->GetAnimInstance();
	return IsValid(FPAnimInstance) && FPAnimInstance->IsAnyMontagePlaying();
}

void AFPPlayerCharacter::OnLadderStarted()
{
	if (GCPlayerController.IsValid())
	{
		GCPlayerController->SetIsIgnoreCameraPitch(true);
		bUseControllerRotationYaw = false;
		APlayerCameraManager* CameraManager = GCPlayerController->PlayerCameraManager;
		CameraManager->ViewPitchMin = LadderCameraMinPitch;
		CameraManager->ViewPitchMax = LadderCameraMaxPitch;
		CameraManager->ViewYawMin = LadderCameraMinYaw;
		CameraManager->ViewYawMax = LadderCameraMaxYaw;
	}
}

void AFPPlayerCharacter::OnLadderStopped()
{
	if (GCPlayerController.IsValid())
	{
		GCPlayerController->SetIsIgnoreCameraPitch(false);
		bUseControllerRotationYaw = true;
		APlayerCameraManager* CameraManager = GCPlayerController->PlayerCameraManager;
		APlayerCameraManager* DefaultCameraManager = CameraManager->GetClass()->GetDefaultObject<
			APlayerCameraManager>();

		CameraManager->ViewPitchMin = DefaultCameraManager->ViewPitchMin;
		CameraManager->ViewPitchMax = DefaultCameraManager->ViewPitchMax;
		CameraManager->ViewYawMin = DefaultCameraManager->ViewYawMin;
		CameraManager->ViewYawMax = DefaultCameraManager->ViewYawMax;
	}
}

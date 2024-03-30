#include "AIController.h"
#include "Components/CapsuleComponent.h"
#include "Curves/CurveVector.h"
#include "GameCode/GameCodeTypes.h"
#include "GameCode/Components/CharacterComponents/CharacterEquipmentComponent.h"
#include "GameCode/Components/CharacterComponents/CharacterInventoryComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameCode/Components/MovementComponents/GCBaseCharacterMovementComponent.h"
#include "GameCode/UI/Widgets/World/GCAttributeProgressBar.h"

AGCBaseCharacter::AGCBaseCharacter(const FObjectInitializer& ObjectInitializer) : Super(
	ObjectInitializer.SetDefaultSubobjectClass<UGCBaseCharacterMovementComponent>(CharacterMovementComponentName))
{
	GCBaseCharacterMovementComponent = StaticCast<UGCBaseCharacterMovementComponent*>(GetCharacterMovement());
	LedgeDetectorComponent = CreateDefaultSubobject<ULedgeDetectorComponent>(TEXT("LedgeDetector"));
	GetMesh()->CastShadow = true;
	GetMesh()->bCastDynamicShadow = true;

	CharacterAttributeComponent = CreateDefaultSubobject<UCharacterAttributeComponent>(TEXT("CharacterAttribute"));
	CharacterEquipmentComponent = CreateDefaultSubobject<UCharacterEquipmentComponent>(TEXT("CharacterEquipment"));
	CharacterInventoryComponent = CreateDefaultSubobject<UCharacterInventoryComponent>(TEXT("CharacterInventory"));

	HealthBarProgressComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBarProgressComponent"));
	HealthBarProgressComponent->SetupAttachment(GetCapsuleComponent());
}

void AGCBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	CharacterAttributeComponent->OnDeathEvent.AddUObject(this, &AGCBaseCharacter::OnDeath);
	InitializeHealthProgress();
}

void AGCBaseCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (OnInteractableObjectFound.IsBound())
		OnInteractableObjectFound.Unbind();
	Super::EndPlay(EndPlayReason);
}

void AGCBaseCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	TryChangeSprintState();
	TraceLineOfSight();
}

void AGCBaseCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	AAIController* AIController = Cast<AAIController>(NewController);
	if (IsValid(AIController))
	{
		FGenericTeamId TeamId((uint8)Team);
		AIController->SetGenericTeamId(TeamId);
	}
}

void AGCBaseCharacter::RegisterInteractiveActor(AInteractiveActor* InteractiveActor)
{
	AvailableInteractiveActors.AddUnique(InteractiveActor);
}

void AGCBaseCharacter::UnregisterInteractiveActor(AInteractiveActor* InteractiveActor)
{
	AvailableInteractiveActors.RemoveSingleSwap(InteractiveActor);
}

void AGCBaseCharacter::ChangeCrouchState()
{
	if (GetCharacterMovement()->IsCrouching())
	{
		UnCrouch();
	}
	else
	{
		Crouch();
	}
}

void AGCBaseCharacter::StartSprint()
{
	bIsSprintRequested = true;
	if (bIsCrouched)
		UnCrouch();
}

void AGCBaseCharacter::StopSprint()
{
	bIsSprintRequested = false;
}

void AGCBaseCharacter::StartFire()
{
	if (CharacterEquipmentComponent->GetIsEquipping())
		return;

	ARangeWeaponItem* CurrentRangeWeapon = CharacterEquipmentComponent->GetCurrentRangeWeapon();
	if (IsValid(CurrentRangeWeapon))
		CurrentRangeWeapon->StartFire();
}

void AGCBaseCharacter::StopFire()
{
	ARangeWeaponItem* CurrentRangeWeapon = CharacterEquipmentComponent->GetCurrentRangeWeapon();
	if (IsValid(CurrentRangeWeapon))
		CurrentRangeWeapon->StopFire();
}

void AGCBaseCharacter::StartAiming()
{
	ARangeWeaponItem* CurrentRangeWeapon = CharacterEquipmentComponent->GetCurrentRangeWeapon();
	if (!IsValid(CurrentRangeWeapon))
		return;
	bIsAiming = true;
	CurrentAimingMovementSpeed = CurrentRangeWeapon->GetAimMovementMaxSpeed();
	CurrentRangeWeapon->StartAiming();
	OnStartAiming();
}

void AGCBaseCharacter::StopAiming()
{
	if (!bIsAiming)
		return;

	ARangeWeaponItem* CurrentRangeWeapon = CharacterEquipmentComponent->GetCurrentRangeWeapon();
	if (IsValid(CurrentRangeWeapon))
		CurrentRangeWeapon->StopAiming();

	bIsAiming = false;
	CurrentAimingMovementSpeed = 0;
	OnStopAiming();
}

void AGCBaseCharacter::NextItem()
{
	CharacterEquipmentComponent->EquipNextItem();
}

void AGCBaseCharacter::PreviousItem()
{
	CharacterEquipmentComponent->EquipPreviousItem();
}

void AGCBaseCharacter::EquipPrimaryItem()
{
	CharacterEquipmentComponent->EquipItemInSlot(EEquipmentSlots::PrimaryItem);
}

void AGCBaseCharacter::Reload()
{
	if (IsValid(CharacterEquipmentComponent->GetCurrentRangeWeapon()))
		CharacterEquipmentComponent->ReloadCurrenWeapon();
}

void AGCBaseCharacter::PrimaryMeleeAttack()
{
	AMeleeWeaponItem* CurrentMeleeWeapon = CharacterEquipmentComponent->GetCurrentMeleeWeapon();
	if (IsValid(CurrentMeleeWeapon))
	{
		CurrentMeleeWeapon->StartAttack(EMeleeAttackType::Primary);
	}
}

void AGCBaseCharacter::SecondaryMeleeAttack()
{
	AMeleeWeaponItem* CurrentMeleeWeapon = CharacterEquipmentComponent->GetCurrentMeleeWeapon();
	if (IsValid(CurrentMeleeWeapon))
	{
		CurrentMeleeWeapon->StartAttack(EMeleeAttackType::Secondary);
	}
}

void AGCBaseCharacter::Interact()
{
	if (LineOfSightObject.GetInterface())
		LineOfSightObject->Interact(this);
}

void AGCBaseCharacter::AddEquipmentItem(const TSubclassOf<class AEquipableItem> EquipableItemClass)
{
	CharacterEquipmentComponent->AddEquipmentItem(EquipableItemClass);
}

bool AGCBaseCharacter::PickupItem(TWeakObjectPtr<UInventoryItem> ItemToPickup)
{
	bool Result = false;
	if (CharacterInventoryComponent->HasFreeSlot())
	{
		CharacterInventoryComponent->AddItem(ItemToPickup, 1);
		Result = true;
	}

	return Result;
}

void AGCBaseCharacter::UseInventory(APlayerController* PlayerController)
{
	if (!IsValid(PlayerController))
		return;

	if (!CharacterInventoryComponent->IsViewVisible())
	{
		CharacterInventoryComponent->OpenViewInventory(PlayerController);
		PlayerController->SetInputMode(FInputModeGameAndUI{});
		PlayerController->bShowMouseCursor = true;
	}
	else
	{
		CharacterInventoryComponent->CloseViewInventory();
		PlayerController->SetInputMode(FInputModeGameOnly{});
		PlayerController->bShowMouseCursor = false;
	}
}

void AGCBaseCharacter::InitializeHealthProgress()
{
	UGCAttributeProgressBar* Widget = Cast<UGCAttributeProgressBar>(HealthBarProgressComponent->GetUserWidgetObject());
	if (!IsValid(Widget))
	{
		HealthBarProgressComponent->SetVisibility(false);
		return;
	}
	if (IsPlayerControlled() && IsLocallyControlled())
		HealthBarProgressComponent->SetVisibility(false);

	CharacterAttributeComponent->OnHealthChangedEvent.AddUObject(Widget, &UGCAttributeProgressBar::SetProgressPercentage);
	CharacterAttributeComponent->OnDeathEvent.AddLambda([=] { HealthBarProgressComponent->SetVisibility(false); });
	Widget->SetProgressPercentage(CharacterAttributeComponent->GetHealthPercent());
}

FGenericTeamId AGCBaseCharacter::GetGenericTeamId() const
{
	return FGenericTeamId((uint8)Team);
}

void AGCBaseCharacter::OnStartAiming_Implementation()
{
	OnStartAimingInternal();
}

void AGCBaseCharacter::OnStopAiming_Implementation()
{
	OnStopAimingInternal();
}

float AGCBaseCharacter::GetAimingMovementSpeed() const
{
	return CurrentAimingMovementSpeed;
}

void AGCBaseCharacter::Mantle(bool bForce /*= false*/)
{
	if (!(CanMantling() || bForce))
		return;

	FLedgeDescription LedgeDescription;
	if (LedgeDetectorComponent->DetectLedge(LedgeDescription))
	{
		FMantlingMovementParameters MantlingParameters;
		MantlingParameters.InitialLocation = GetActorLocation();
		MantlingParameters.InitialRotation = GetActorRotation();
		MantlingParameters.TargetLocation = LedgeDescription.Location;
		MantlingParameters.TargetRotation = LedgeDescription.Rotation;

		float MantlingHeight = (MantlingParameters.TargetLocation - MantlingParameters.InitialLocation).Z;
		const FMantlingSettings& MantlingSettings = GetMantlingSettings(MantlingHeight);

		float MinRange;
		float MaxRange;
		MantlingSettings.MantlingCurve->GetTimeRange(MinRange, MaxRange);
		MantlingParameters.Duration = MaxRange - MinRange;

		MantlingParameters.MantlingCurve = MantlingSettings.MantlingCurve;

		FVector2D SourceRange(MantlingSettings.MinHeight, MantlingSettings.MaxHeight);
		FVector2D TargetRange(MantlingSettings.MinHeightStartTime, MantlingSettings.MaxHeightStartTime);
		MantlingParameters.StartTime = FMath::GetMappedRangeValueClamped(SourceRange, TargetRange, MantlingHeight);
		MantlingParameters.InitialAnimationLocation = MantlingParameters.TargetLocation - MantlingSettings.
			AnimationCorrectionZ * FVector::UpVector + MantlingSettings.AnimationCorrectionXY * LedgeDescription.
			LedgeNormal;


		GCBaseCharacterMovementComponent->StartMantle(MantlingParameters);

		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		AnimInstance->Montage_Play(MantlingSettings.MantlingMontage, 1.0f, EMontagePlayReturnType::Duration,
		                           MantlingParameters.StartTime);
		OnMantling(MantlingSettings, MantlingParameters.StartTime);
	}
}

bool AGCBaseCharacter::CanMantling() const
{
	return GetBaseCharacterMovementComponent()->IsOnLadder();
}

bool AGCBaseCharacter::IsAiming() const
{
	return bIsAiming;
}

void AGCBaseCharacter::OnMantling(const FMantlingSettings& MantlingSettings, float MantlingAnimationStartTime)
{
}

bool AGCBaseCharacter::CanJumpInternal_Implementation() const
{
	return Super::CanJumpInternal_Implementation() && !GetBaseCharacterMovementComponent()->IsMantling();
}

void AGCBaseCharacter::ClimbLadderUp(float Value)
{
	if (GetBaseCharacterMovementComponent()->IsOnLadder() && !FMath::IsNearlyZero(Value))
	{
		FVector LadderUpVector = GetBaseCharacterMovementComponent()->GetCurrentLadder()->GetActorUpVector();
		AddMovementInput(LadderUpVector, Value);
	}
}

void AGCBaseCharacter::InteractWithLadder()
{
	if (GetBaseCharacterMovementComponent()->IsOnLadder())
	{
		GetBaseCharacterMovementComponent()->DetachFromLadder(EDetachFromLadderMethod::JumpOff);
	}
	else
	{
		const ALadder* AvailableLadder = GetAvailableLadder();
		if (IsValid(AvailableLadder))
		{
			if (AvailableLadder->GetIsOnTop())
			{
				PlayAnimMontage(AvailableLadder->GetAttachFromTopAnimMontage());
			}
			GetBaseCharacterMovementComponent()->AttachToLadder(AvailableLadder);
		}
	}
}

const ALadder* AGCBaseCharacter::GetAvailableLadder() const
{
	const ALadder* Result = nullptr;
	for (const AInteractiveActor* InteractiveActor : AvailableInteractiveActors)
	{
		if (InteractiveActor->IsA<ALadder>())
		{
			Result = StaticCast<const ALadder*>(InteractiveActor);
			break;
		}
	}
	return Result;
}

void AGCBaseCharacter::Falling()
{
	Super::Falling();
	GetBaseCharacterMovementComponent()->bNotifyApex = true;
}

void AGCBaseCharacter::NotifyJumpApex()
{
	Super::NotifyJumpApex();
	CurrentFallApex = GetActorLocation();
}

void AGCBaseCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
	float FallHeight = ((CurrentFallApex - GetActorLocation()).Z) * 0.01;
	if (IsValid(FallDamageCurve))
	{
		float DamageAmount = FallDamageCurve->GetFloatValue(FallHeight);
		TakeDamage(DamageAmount, FDamageEvent(), GetController(), Hit.Actor.Get());
	}
}

UCharacterEquipmentComponent* AGCBaseCharacter::GetCharacterEquipmentComponent_Mutable() const
{
	return CharacterEquipmentComponent;
}

const UCharacterEquipmentComponent* AGCBaseCharacter::GetCharacterEquipmentComponent() const
{
	return CharacterEquipmentComponent;
}

UCharacterAttributeComponent* AGCBaseCharacter::GetCharacterAttributeComponent_Mutable() const
{
	return CharacterAttributeComponent;
}

const UCharacterAttributeComponent* AGCBaseCharacter::GetCharacterAttributeComponent() const
{
	return CharacterAttributeComponent;
}

bool AGCBaseCharacter::CanSprint()
{
	return true;
}

void AGCBaseCharacter::OnDeath()
{
	GetCharacterMovement()->DisableMovement();
	float DurationMontage = PlayAnimMontage(OnDeathAnimMontage);
	if (DurationMontage == 0)
	{
		EnableRagdoll();
	}
}

void AGCBaseCharacter::OnStartAimingInternal()
{
	if (OnAimingStateChanged.IsBound())
		OnAimingStateChanged.Broadcast(true);
}

void AGCBaseCharacter::OnStopAimingInternal()
{
	if (OnAimingStateChanged.IsBound())
		OnAimingStateChanged.Broadcast(false);
}

void AGCBaseCharacter::TraceLineOfSight()
{
	if (!IsPlayerControlled())
		return;

	FVector ViewLocation;
	FRotator ViewRotation;

	APlayerController* PlayerController = GetController<APlayerController>();
	PlayerController->GetPlayerViewPoint(ViewLocation, ViewRotation);

	FVector ViewDirection = ViewRotation.Vector();
	FVector TraceEnd = ViewLocation + ViewDirection * LineOfSightDistance;

	FHitResult HitResult;
	GetWorld()->LineTraceSingleByChannel(HitResult, ViewLocation, TraceEnd, ECC_Visibility);
	if (LineOfSightObject.GetObject() != HitResult.Actor)
	{
		LineOfSightObject = HitResult.Actor.Get();

		FName ActionName;
		if (LineOfSightObject.GetInterface())
		{
			ActionName = LineOfSightObject->GetActionEventName();
		}
		else
		{
			ActionName = NAME_None;
		}

		OnInteractableObjectFound.ExecuteIfBound(ActionName);
	}
}

void AGCBaseCharacter::TryChangeSprintState()
{
	if (bIsSprintRequested && !GCBaseCharacterMovementComponent->IsSprinting() && CanSprint())
	{
		GCBaseCharacterMovementComponent->StartSprint();
		OnSprintStart();
	}
	if (!bIsSprintRequested && GCBaseCharacterMovementComponent->IsSprinting())
	{
		GCBaseCharacterMovementComponent->StopSprint();
		OnSprintEnd();
	}
}

const FMantlingSettings& AGCBaseCharacter::GetMantlingSettings(float LedgeHeight) const
{
	return LedgeHeight > LowMantleMaxHeight ? HeightMantlingSettings : LowMantlingSettings;
}

void AGCBaseCharacter::EnableRagdoll()
{
	GetMesh()->SetCollisionProfileName(CollisionProfilePawnRagdoll);
	GetMesh()->SetSimulatePhysics(true);
}

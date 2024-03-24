#include "RangeWeaponItem.h"

#include "GameCode/GameCodeTypes.h"
#include "GameCode/Characters/GCBaseCharacter.h"

ARangeWeaponItem::ARangeWeaponItem()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("WeaponRoot"));
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(RootComponent);

	WeaponBarell = CreateDefaultSubobject<UWeaponBarellComponent>(TEXT("WeaponBarell"));
	WeaponBarell->SetupAttachment(WeaponMesh, SocketWeaponMuzzle);

	ReticleType = EReticleType::Default;

	EquippedSocketName = SocketCharacterWeapon;
}


void ARangeWeaponItem::StartFire()
{
	if (GetWorld()->GetTimerManager().IsTimerActive(ShotTimer))
		return;

	bIsFiring = true;
	MakeShot();
}

void ARangeWeaponItem::StopFire()
{
	bIsFiring = false;
}

bool ARangeWeaponItem::IsFiring() const
{
	return bIsFiring;
}

bool ARangeWeaponItem::IsReloading() const
{
	return bIsReloading;
}

void ARangeWeaponItem::StartAiming()
{
	bIsAiming = true;
}

void ARangeWeaponItem::StopAiming()
{
	bIsAiming = false;
}

void ARangeWeaponItem::StartReload()
{
	AGCBaseCharacter* CharacterOwner = GetCharacterOwner();
	if (!IsValid(CharacterOwner))
		return;

	bIsReloading = true;
	if (IsValid(CharacterReloadMontage))
	{
		float MontageDuration = CharacterOwner->PlayAnimMontage(CharacterReloadMontage);
		PlayAnimMontage(WeaponReloadMontage);
		if (ReloadType == EReloadType::FullClip)
			GetWorld()->GetTimerManager().SetTimer(ReloadTimer, [this]() { StopReload(true); }, MontageDuration, false);
	}
	else
	{
		StopReload(true);
	}
}

void ARangeWeaponItem::StopReload(bool bIsSuccess)
{
	if (!bIsReloading)
		return;

	AGCBaseCharacter* CharacterOwner = GetCharacterOwner();

	if (!bIsSuccess)
	{
		if (IsValid(CharacterOwner))
			CharacterOwner->StopAnimMontage(CharacterReloadMontage);
		StopAnimMontage(WeaponReloadMontage);
	}
	if (ReloadType == EReloadType::ByBullet)
	{
		UAnimInstance* CharacterAnimInstance = IsValid(CharacterOwner) ? CharacterOwner->GetMesh()->GetAnimInstance() : nullptr;
		if (IsValid(CharacterAnimInstance))
		{
			CharacterAnimInstance->Montage_JumpToSection(SectionMontageReloadEnd, CharacterReloadMontage);
		}
		UAnimInstance* WeaponAnimInstance = WeaponMesh->GetAnimInstance();
		if (IsValid(WeaponAnimInstance))
		{
			WeaponAnimInstance->Montage_JumpToSection(SectionMontageReloadEnd, WeaponReloadMontage);
		}
	}
	GetWorld()->GetTimerManager().ClearTimer(ReloadTimer);

	bIsReloading = false;
	if (bIsSuccess && OnReloadComplete.IsBound())
		OnReloadComplete.Broadcast();
}

float ARangeWeaponItem::GetAimFOV() const
{
	return AimFOV;
}

float ARangeWeaponItem::GetAimMovementMaxSpeed() const
{
	return AimMovementMaxSpeed;
}

FTransform ARangeWeaponItem::GetForeGripTransform() const
{
	return WeaponMesh->GetSocketTransform(SocketWeaponForeGrip);
}

int32 ARangeWeaponItem::GetAmmo() const
{
	return Ammo;
}

int32 ARangeWeaponItem::GetMaxAmmo() const
{
	return MaxAmmo;
}

void ARangeWeaponItem::SetAmmo(int32 NewAmmo)
{
	Ammo = NewAmmo;
	if (OnAmmoChanged.IsBound())
		OnAmmoChanged.Broadcast(Ammo);
}

bool ARangeWeaponItem::CanShoot() const
{
	return Ammo > 0;
}

EAmmunitionType ARangeWeaponItem::GetAmmoType() const
{
	return AmmoType;
}

EReticleType ARangeWeaponItem::GetReticleType() const
{
	return bIsAiming ? AimReticleType : ReticleType;
}

void ARangeWeaponItem::BeginPlay()
{
	Super::BeginPlay();
	SetAmmo(MaxAmmo);
}

float ARangeWeaponItem::GetCurrentBulletSpreadAngle() const
{
	float AngleInDegress = bIsAiming ? AimSpreadAngle : SpreadAngle;
	return FMath::DegreesToRadians(AngleInDegress);
}

void ARangeWeaponItem::MakeShot()
{
	AGCBaseCharacter* CharacterOwner = GetCharacterOwner();
	if (!IsValid(CharacterOwner))
		return;

	if (!CanShoot())
	{
		StopFire();
		if (Ammo == 0 && bAutoReload)
		{
			CharacterOwner->Reload();
		}
		return;
	}

	StopReload(false);
	CharacterOwner->PlayAnimMontage(CharacterFireMontage);
	PlayAnimMontage(WeaponFireMontage);


	FVector ShotLocation;
	FRotator ShotDirection;
	if (CharacterOwner->IsPlayerControlled())
	{
		APlayerController* Controller = CharacterOwner->GetController<APlayerController>();
		Controller->GetPlayerViewPoint(ShotLocation, ShotDirection);
	}
	else
	{
		ShotLocation = WeaponBarell->GetComponentLocation();
		ShotDirection = CharacterOwner->GetBaseAimRotation();
	}


	FVector ViewDirection = ShotDirection.RotateVector(FVector::ForwardVector);

	SetAmmo(Ammo - 1);
	WeaponBarell->Shot(ShotLocation, ViewDirection, GetCurrentBulletSpreadAngle());

	GetWorld()->GetTimerManager().SetTimer(ShotTimer, this, &ARangeWeaponItem::OnShotTimerElapsed, GetShotTimerInterval(), false);
}

void ARangeWeaponItem::OnShotTimerElapsed()
{
	if (!bIsFiring)
		return;
	switch (WeaponFireMode)
	{
	case EWeaponFireMode::Single:
		{
			StopFire();
			break;
		}
	case EWeaponFireMode::FullAuto:
		{
			MakeShot();
			break;
		}
	}
}


float ARangeWeaponItem::GetShotTimerInterval()
{
	return 60.0f / RateOfFire;
}

float ARangeWeaponItem::PlayAnimMontage(UAnimMontage* AnimMontage)
{
	UAnimInstance* WeaponAnimInstance = WeaponMesh->GetAnimInstance();
	float Result = 0;
	if (IsValid(WeaponAnimInstance))
		Result = WeaponAnimInstance->Montage_Play(AnimMontage);
	return Result;
}

void ARangeWeaponItem::StopAnimMontage(UAnimMontage* AnimMontage, float BlendOutTime)
{
	UAnimInstance* WeaponAnimInstance = WeaponMesh->GetAnimInstance();
	if (IsValid(WeaponAnimInstance))
		WeaponAnimInstance->Montage_Stop(BlendOutTime, AnimMontage);
}

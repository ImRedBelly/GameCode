#include "WeaponBarellComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Components/DecalComponent.h"
#include "GameCode/GameCodeTypes.h"
#include "GameCode/Actors/Equipment/Weapons/RangeWeaponItem.h"
#include "GameCode/SubSystems/DebugSubsystem.h"

void UWeaponBarellComponent::Shot(FVector ShotStart, FVector ShotDirection, AController* Controller, float SpreadAngle)
{
	for (int i = 0; i < BulletsPerShot; i++)
	{
		ShotDirection += GetBulletSpreadOffset(FMath::RandRange(0.0f, SpreadAngle), ShotDirection.ToOrientationRotator());

		FVector MuzzleLocation = GetComponentLocation();
		FVector ShotEnd = ShotStart + FiringRange * ShotDirection;

		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), MuzzleFlashFX, MuzzleLocation, GetComponentRotation());

		FHitResult ShotResult;

#if ENABLE_DRAW_DEBUG
		UDebugSubsystem* DebugSubsystem = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<UDebugSubsystem>();
		bool bIsDebugEnable = DebugSubsystem->IsCategoryEnabled(DebugCategoryRangeWeapon);
#elif
	bool bIsDebugEnable = false;
#endif


		if (GetWorld()->LineTraceSingleByChannel(ShotResult, ShotStart, ShotEnd, ECC_Bullet))
		{
			ShotEnd = ShotResult.ImpactPoint;
			AActor* HitActor = ShotResult.GetActor();
			if (IsValid(HitActor))
			{
				FPointDamageEvent DamageEvent;
				DamageEvent.HitInfo = ShotResult;
				DamageEvent.ShotDirection = ShotDirection;
				DamageEvent.DamageTypeClass = DamageTypeClass;

				HitActor->TakeDamage(DamageAmount, DamageEvent, Controller, GetOwner());
			}
			UDecalComponent* DecalComponent = UGameplayStatics::SpawnDecalAtLocation(
				GetWorld(), DefaultDecalInfo.DecalMaterial, DefaultDecalInfo.DecalSize,
				ShotResult.ImpactPoint, ShotResult.ImpactNormal.ToOrientationRotator());
			if (IsValid(DecalComponent))
			{
				DecalComponent->SetFadeScreenSize(0.001f);
				DecalComponent->SetFadeOut(DefaultDecalInfo.DecalLifeTime, DefaultDecalInfo.DecalFadeOutTime);
			}
			if (bIsDebugEnable)
				DrawDebugSphere(GetWorld(), ShotEnd, 10, 24, FColor::Red, false, 1);
		}


		UNiagaraComponent* TraceFXComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(), TraceFX, MuzzleLocation, GetComponentRotation());
		TraceFXComponent->SetVectorParameter(FXParamTraceEnd, ShotEnd);
		if (bIsDebugEnable)
			DrawDebugLine(GetWorld(), MuzzleLocation, ShotEnd, FColor::Red, false, 1, 0, 3);
	}
}

FVector UWeaponBarellComponent::GetBulletSpreadOffset(float Angle, FRotator ShotRotation)
{
	float SpreadSize = FMath::Tan(Angle);
	float RotationAngle = FMath::RandRange(0.0f, 2 * PI);

	float SpreadY = FMath::Cos(RotationAngle);
	float SpreadZ = FMath::Sin(RotationAngle);

	FVector Result = (ShotRotation.RotateVector(FVector::UpVector) * SpreadZ
		+ ShotRotation.RotateVector(FVector::RightVector) * SpreadY) * SpreadSize;

	return Result;
}

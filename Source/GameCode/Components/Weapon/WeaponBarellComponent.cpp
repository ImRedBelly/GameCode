#include "WeaponBarellComponent.h"

#include "DrawDebugHelpers.h"
#include "GameCode/GameCodeTypes.h"
#include "GameCode/SubSystems/DebugSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

void UWeaponBarellComponent::Shot(FVector ShotStart, FVector ShotDirection, AController* Controller)
{
	FVector MuzzleLocation = GetComponentLocation();
	FVector ShotEnd = ShotStart + FireRange * ShotDirection;

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
			HitActor->TakeDamage(DamageAmount, FDamageEvent{}, Controller, GetOwner());
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

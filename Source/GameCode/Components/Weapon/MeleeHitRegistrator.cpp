#include "MeleeHitRegistrator.h"
#include "GameCode/GameCodeTypes.h"
#include "GameCode/Utils/GCTraceUtils.h"
#include "GameCode/SubSystems/DebugSubsystem.h"
#include "WeaponBarellComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"

UMeleeHitRegistrator::UMeleeHitRegistrator()
{
	PrimaryComponentTick.bCanEverTick = true;
	SphereRadius = 5;
	SetCollisionProfileName(CollisionProfileNoCollision);
}

void UMeleeHitRegistrator::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bIsHitRegistration)
	{
		ProcessHitRegistration();
	}
	PreviousComponentLocation = GetComponentLocation();
}

void UMeleeHitRegistrator::ProcessHitRegistration()
{
	FVector CurrentLocation = GetComponentLocation();
	FHitResult HitResult;


#if ENABLE_DRAW_DEBUG
	UDebugSubsystem* DebugSubsystem = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<UDebugSubsystem>();
	bool bIsDebugEnable = DebugSubsystem->IsCategoryEnabled(DebugCategoryMeleeWeapon);
#elif
	bool bIsDebugEnable = false;
#endif


	bool bIsHit = GCTraceUtils::SweepSphereSingleByChanel(
		GetWorld(),
		HitResult,
		PreviousComponentLocation,
		CurrentLocation,
		GetScaledSphereRadius(),
		ECC_Melee,
		FCollisionQueryParams::DefaultQueryParam,
		FCollisionResponseParams::DefaultResponseParam,
		bIsDebugEnable,
		5
	);

	if (bIsHit)
	{
		FVector Direction = (CurrentLocation - PreviousComponentLocation).GetSafeNormal();
		if (OnMeleeHitRegistred.IsBound())
			OnMeleeHitRegistred.Broadcast(HitResult, Direction);
	}
}

void UMeleeHitRegistrator::SetIsHitRegistrationEnabled(bool bIsEnabled_In)
{
	bIsHitRegistration = bIsEnabled_In;
}

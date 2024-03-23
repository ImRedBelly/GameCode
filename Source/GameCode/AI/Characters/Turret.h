#pragma once

#include "CoreMinimal.h"
#include "GameCode/GameCodeTypes.h"
#include "GameCode/Components/Weapon/WeaponBarellComponent.h"
#include "GameFramework/Pawn.h"
#include "Turret.generated.h"

UENUM(BlueprintType)
enum class ETurretState: uint8
{
	Searching,
	Firing,
};

UCLASS()
class GAMECODE_API ATurret : public APawn
{
	GENERATED_BODY()

public:
	virtual void Tick(float DeltaTime) override;
	virtual void PossessedBy(AController* NewController) override;
	void SetCurrentTarget(AActor* NewTarget);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	USceneComponent* TurretBaseComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	USceneComponent* TurretBarellComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UWeaponBarellComponent* WeaponBarellComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Turret parameters", meta=(UIMin = 0, ClampMin = 0))
	float BaseSearchingRotationRate = 60;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Turret parameters", meta=(UIMin = 0, ClampMin = 0))
	float BaseFiringInterpSpeed = 5;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Turret parameters", meta=(UIMin = 0, ClampMin = 0))
	float BarellPitchRotationRate = 60;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Turret parameters", meta=(UIMin = 0, ClampMin = 0))
	float MaxBarellPitchAngle = 60;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Turret parameters", meta=(UIMin = 0, ClampMin = 0))
	float MinBarellPitchAngle = -30;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Turret parameters| Fire", meta=(UIMin = 1, ClampMin = 1))
	float RateOfFire = 300;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Turret parameters| Fire", meta=(UIMin = 0, ClampMin = 0))
	float BulletSpreadAngle = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Turret parameters| Fire", meta=(UIMin = 0, ClampMin = 0))
	float FireDelayTime = 3;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Turret parameters | Team")
	ETeams Team = ETeams::Enemy;

private:
	ATurret();

	void SearchingMovement(float DeltaTime);
	void FiringMovement(float DeltaTime);

	void SetTurretState(ETurretState NewState);

	virtual FVector GetPawnViewLocation() const override;
	virtual FRotator GetViewRotation() const override;

	ETurretState TurretState = ETurretState::Searching;

	AActor* CurrentTarget = nullptr;

	void MakeShot();
	float GetFireInterval() const;
	FTimerHandle ShotTimer;
};

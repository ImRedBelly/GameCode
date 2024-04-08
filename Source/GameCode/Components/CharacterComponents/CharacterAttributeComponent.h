#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameCode/SubSystems/SaveSubsystem/SaveSubsystemInterface.h"
#include "CharacterAttributeComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnDeathEventSignature)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHealthChanged, float)

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GAMECODE_API UCharacterAttributeComponent : public UActorComponent, public ISaveSubsystemInterface
{
	GENERATED_BODY()

public:
	UCharacterAttributeComponent();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	FOnDeathEventSignature OnDeathEvent;
	FOnHealthChanged OnHealthChangedEvent;

	bool IsAlive() { return Health > 0; }
	float GetHealthPercent() const;
	void AddHealth(float HeathToAdd);

	virtual void OnLevelDeserialized_Implementation() override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Health", meta=(UIMin = 0))
	float MaxHealth = 100;

private:
	TWeakObjectPtr<class AGCBaseCharacter> CachedBaseCharacterOwner;

	UPROPERTY(ReplicatedUsing=OnRep_Health, SaveGame)
	float Health = 0;

	UFUNCTION()
	void OnRep_Health();
	void OnHealthChanged();

	UFUNCTION()
	void OnTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType,
	                     class AController* InstigatedBy, AActor* DamageCauser);

#if UE_BUILD_DEBUG || UE_BUILD_DEVELOPMENT
	void DebugDrawAttributes();
#endif
};

#pragma once

#include "CoreMinimal.h"
#include "GameCode/Characters/GCBaseCharacter.h"
#include "GameCode/UI/Widgets/PlayerHUDWidget.h"
#include "GameFramework/PlayerController.h"
#include "GCPlayerController.generated.h"


UCLASS()
class GAMECODE_API AGCPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void SetPawn(APawn* InPawn) override;
	bool GetIsIgnoreCameraPitch() const;
	void SetIsIgnoreCameraPitch(bool bIgnoreCameraPitch_In);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Widgets")
	TSubclassOf<UPlayerHUDWidget> PlayerHUDWidgetClass;

	virtual void SetupInputComponent() override;

private:
	void MoveForward(float Value);
	void MoveRight(float Value);
	void Turn(float Value);
	void LookUp(float Value);
	void TurnAtRate(float Value);
	void LookUpAtRate(float Value);
	void Mantle();
	void Jump();
	void ChangeCrouchState();

	void StartSprint();
	void StopSprint();

	void ClimbLadderUp(float Value);
	void InteractWithLadder();

	virtual void SwimForward(float Value);
	virtual void SwimRight(float Value);
	virtual void SwimUp(float Value);

	void PlayerStartFire();
	void PlayerStopFire();
	void StartAiming();
	void StopAiming();

	void NextItem();
	void PreviousItem();
	void EquipPrimaryItem();

	void Reload();

	void PrimaryMeleeAttack();
	void SecondaryMeleeAttack();

	void OnInteractableObjectFound(FName ActionName);
	void Interact();
	void UseInventory();
	void ConfirmWeaponWheelSelection();

	void QuickSaveGame();
	void QuickLoadGame();

	void CreateAndInitializeWidgets();

	TSoftObjectPtr<AGCBaseCharacter> CachedBaseCharacter;

	UPlayerHUDWidget* PlayerHUDWidget;
	bool bIsIgnoreCameraPitch = false;
};

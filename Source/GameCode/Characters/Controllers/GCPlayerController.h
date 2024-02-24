#pragma once

#include "CoreMinimal.h"
#include "GameCode/Characters/GCBaseCharacter.h"
#include "GameFramework/PlayerController.h"
#include "GCPlayerController.generated.h"


UCLASS()
class GAMECODE_API AGCPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void SetPawn(APawn* InPawn) override;

protected:
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



	
	TSoftObjectPtr<AGCBaseCharacter> CachedBaseCharacter;
	
};

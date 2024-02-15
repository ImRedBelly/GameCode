#include "GCPlayerController.h"

void AGCPlayerController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);
	CachedBaseCharacter = Cast<AGCBaseCharacter>(InPawn);
}

void AGCPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAxis("MoveForward", this, &AGCPlayerController::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &AGCPlayerController::MoveRight);
	InputComponent->BindAxis("Turn", this, &AGCPlayerController::Turn);
	InputComponent->BindAxis("LookUp", this, &AGCPlayerController::LookUp);

	InputComponent->BindAxis("TurnAtRate", this, &AGCPlayerController::TurnAtRate);
	InputComponent->BindAxis("LookUpRate", this, &AGCPlayerController::LookUpAtRate);

	InputComponent->BindAxis("SwimForward", this, &AGCPlayerController::SwimForward);
	InputComponent->BindAxis("SwimRight", this, &AGCPlayerController::SwimRight);
	InputComponent->BindAxis("SwimUp", this, &AGCPlayerController::SwimUp);

	InputComponent->BindAction("Mantle", IE_Pressed, this, &AGCPlayerController::Mantle);
	InputComponent->BindAction("Jump", IE_Pressed, this, &AGCPlayerController::Jump);
	InputComponent->BindAction("Crouch", IE_Pressed, this, &AGCPlayerController::ChangeCrouchState);

	InputComponent->BindAction("Sprint", IE_Pressed, this, &AGCPlayerController::StartSprint);
	InputComponent->BindAction("Sprint", IE_Released, this, &AGCPlayerController::StopSprint);
}

void AGCPlayerController::MoveForward(float Value)
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->MoveForward(Value);
	}
}

void AGCPlayerController::MoveRight(float Value)
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->MoveRight(Value);
	}
}

void AGCPlayerController::Turn(float Value)
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->Turn(Value);
	}
}

void AGCPlayerController::LookUp(float Value)
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->LookUp(Value);
	}
}

void AGCPlayerController::TurnAtRate(float Value)
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->TurnAtRate(Value);
	}
}

void AGCPlayerController::LookUpAtRate(float Value)
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->LookUpAtRate(Value);
	}
}

void AGCPlayerController::Mantle()
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->Mantle();
	}
}

void AGCPlayerController::Jump()
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->Jump();
	}
}

void AGCPlayerController::ChangeCrouchState()
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->ChangeCrouchState();
	}
}

void AGCPlayerController::StartSprint()
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->StartSprint();
	}
}

void AGCPlayerController::StopSprint()
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->StopSprint();
	}
}

void AGCPlayerController::SwimForward(float Value)
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->SwimForward(Value);
	}
}

void AGCPlayerController::SwimRight(float Value)
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->SwimRight(Value);
	}
}

void AGCPlayerController::SwimUp(float Value)
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->SwimUp(Value);
	}
}

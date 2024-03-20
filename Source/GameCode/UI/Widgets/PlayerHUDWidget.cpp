#include "PlayerHUDWidget.h"

#include "AmmoWidget.h"
#include "Blueprint/WidgetTree.h"
#include "GameCode/Characters/GCBaseCharacter.h"

UReticleWidget* UPlayerHUDWidget::GetReticleWidget()
{
	return WidgetTree->FindWidget<UReticleWidget>(ReticleWidgetName);
}

UAmmoWidget* UPlayerHUDWidget::GetAmmoWidget()
{
	return WidgetTree->FindWidget<UAmmoWidget>(AmmoWidgetName);
}

float UPlayerHUDWidget::GetHealthPercent() const
{
	float Result = 1;
	APawn* Pawn = GetOwningPlayerPawn();
	AGCBaseCharacter* Character = Cast<AGCBaseCharacter>(Pawn);
	if (IsValid(Character))
	{
		const UCharacterAttributeComponent* CharacterAttributeComponent = Character->GetCharacterAttributeComponent();
		Result = CharacterAttributeComponent->GetHealthPercent();
	}

	return Result;
}

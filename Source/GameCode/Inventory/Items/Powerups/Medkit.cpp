#include "Medkit.h"

bool UMedkit::Consume(AGCBaseCharacter* ConsumeTarget)
{
	UCharacterAttributeComponent* CharacterAttribute = ConsumeTarget->GetCharacterAttributeComponent_Mutable();
	CharacterAttribute->AddHealth(Health);
	return true;
}

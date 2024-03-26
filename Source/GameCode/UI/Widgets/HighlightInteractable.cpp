#include "HighlightInteractable.h"

void UHighlightInteractable::SetActionText(FName KeyName)
{
	if (IsValid(ActionText))
	{
		ActionText->SetText(FText::FromName(KeyName));
	}
}

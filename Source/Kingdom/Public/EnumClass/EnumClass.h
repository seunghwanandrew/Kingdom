#pragma once
UENUM(BlueprintType)
enum class EPlayerActionState : uint8
{
	EPAS_None UMETA(DisplayName = "None State"),
	EPAS_Combat UMETA(DisplayName = "Combat State")
};
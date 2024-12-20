#pragma once
UENUM(BlueprintType)
enum class EPlayerActionState : uint8
{
	EPAS_Equipping UMETA(DisplayName = "On Equipping"),
	EPAS_Attacking UMETA(DisplayName = "On Attacking"),
	EPAS_None UMETA(DisplayName = "None State"),
	EPAS_Combat UMETA(DisplayName = "Combat State")
};

UENUM(BlueprintType)
enum class EItemState : uint8
{
	EIS_NONE UMETA(DisplayName = "None State"),
	EIS_Rotating UMETA(DisplayName = "Rotating"),
	EIS_Hovering UMETA(DisplayName = "Hovering"),
	EIS_Equipped UMETA(DisplayName = "Equipped"),
};

UENUM(BlueprintType)
enum class EStats : uint8
{
	ES_Nont UMETA(DisplayName = "None Selected"),
	ES_Health UMETA(DisplayName = "Current Health"),
	ES_MaxHealth UMETA(DisplayName = "Max Heatlh"),
	ES_Stamina UMETA(DisplayName = "Current Stamina"),
	ES_MaxStamina UMETA(DisplayName = "Max Stamina"),
	ES_Strength UMETA(DisplayName = "Strength")
};
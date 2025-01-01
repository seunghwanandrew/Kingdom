#pragma once
UENUM(BlueprintType)
enum class EPlayerActionState : uint8
{
	EPAS_Death UMETA(DisplayName = "Death"),
	EPAS_Equipping UMETA(DisplayName = "On Equipping"),
	EPAS_Attacking UMETA(DisplayName = "On Attacking"),
	EPAS_Combat UMETA(DisplayName = "Combat State"),
	EPAS_None UMETA(DisplayName = "None State")
};

UENUM(BlueprintType)
enum class EDeathPose : uint8
{
	EDP_None UMETA(DisplayName = "None"),
	EDP_Death_01 UMETA(DisplayName = "Death 01"),
	EDP_Death_02 UMETA(DisplayName = "Death 02"),
	EDP_Death_03 UMETA(DisplayName = "Death 03"),
	EDP_Death_04 UMETA(DisplayName = "Death 04")
};

UENUM(BlueprintType)
enum class EItemState : uint8
{
	EIS_None UMETA(DisplayName = "None State"),
	EIS_Rotating UMETA(DisplayName = "Rotating"),
	EIS_Hovering UMETA(DisplayName = "Hovering"),
	EIS_Equipped UMETA(DisplayName = "Equipped"),
};

UENUM(BlueprintType)
enum class EStats : uint8
{
	ES_None UMETA(DisplayName = "None Selected"),
	ES_Health UMETA(DisplayName = "Current Health"),
	ES_MaxHealth UMETA(DisplayName = "Max Heatlh"),
	ES_Stamina UMETA(DisplayName = "Current Stamina"),
	ES_MaxStamina UMETA(DisplayName = "Max Stamina"),
	ES_Strength UMETA(DisplayName = "Strength")
};

UENUM(BlueprintType)
enum class EHitFrom : uint8
{
	EHF_Front UMETA(DisplayName = "Hit From Front"),
	EHF_Left UMETA(DisplayName = "Hit From Left"),
	EHF_Right UMETA(DisplayName = "Hit From Right"),
	EHF_Back UMETA(DisplayName = "Hit From Back")
};


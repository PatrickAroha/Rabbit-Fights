#include "GE_ExecuteCalculation.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"
#include "GameplayEffectExtension.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayEffectExecutionCalculation.h"
#include "Deep/GameplayAbilitySystem/AttributeSets/BasicAttributeSet.h"


struct FDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Health);

	FDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBasicAttributeSet, Health, Target, false);
	}
};

static FDamageStatics& DamageStatics()
{
	static FDamageStatics Statics;
	return Statics;
}

UExecCalc_Damage::UExecCalc_Damage()
{
	RelevantAttributesToCapture.Add(DamageStatics().HealthDef);
}

void UExecCalc_Damage::Execute_Implementation(
	const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput
) const
{
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();
	UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();

	if (!TargetASC) return;

	AActor* TargetActor = TargetASC->GetAvatarActor();
	AActor* SourceActor = SourceASC ? SourceASC->GetAvatarActor() : nullptr;

	const FGameplayTag ShieldTag = FGameplayTag::RequestGameplayTag(FName("Status.Shild"));

	// pega dano vindo do SetByCaller
	const float Damage = Spec.GetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Data.Damage")), false, 0.f);

	float FinalDamage = Damage;

	if (TargetASC->HasMatchingGameplayTag(ShieldTag))
	{
		FinalDamage = 0.f;

		FGameplayEventData Payload;
		Payload.EventTag = FGameplayTag::RequestGameplayTag(FName("Event.ActivateAbility.ShieldBreak"));
		Payload.Instigator = SourceActor;
		Payload.Target = TargetActor;

		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
			TargetActor,
			Payload.EventTag,
			Payload
		);
	}

	if (FinalDamage > 0.f)
	{
		OutExecutionOutput.AddOutputModifier(
			FGameplayModifierEvaluatedData(
				UBasicAttributeSet::GetHealthAttribute(),
				EGameplayModOp::Additive,
				-FinalDamage
			)
		);
	}
}
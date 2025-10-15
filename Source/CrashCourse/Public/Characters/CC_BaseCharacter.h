// Copyright ArgoMel

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"

#include "CC_BaseCharacter.generated.h"

namespace CrashTags
{
	extern CRASHCOURSE_API const FName Player;
}

struct FOnAttributeChangeData;
class UAttributeSet;
class UGameplayAbility;
class UGameplayEffect;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FASCInitialized, UAbilitySystemComponent*, ASC, UAttributeSet*, AS);

UCLASS(Abstract)
class CRASHCOURSE_API ACC_BaseCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ACC_BaseCharacter();
protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
public:
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

public:
	virtual UAttributeSet* GetAttributeSet() const { return nullptr; }
	bool IsAlive() const { return bAlive; }
	void SetAlive(bool bAliveStatus) { bAlive = bAliveStatus; }

	UFUNCTION(BlueprintCallable, Category = "Crash|Death")
	virtual void HandleRespawn();

	UFUNCTION(BlueprintCallable, Category = "Crash|Attributes")
	void ResetAttributes() const;

	UFUNCTION(BlueprintImplementableEvent)
	void RotateToTarget(AActor* RotateTarget);
	
protected:
	void GiveStartupAbilities();
	void InitializeAttributes() const;

	void OnHealthChanged(const FOnAttributeChangeData& AttributeChangeData);
	virtual void HandleDeath();

public:
	UPROPERTY(BlueprintAssignable)
	FASCInitialized OnASCInitialized;

	UPROPERTY(EditAnywhere, Category = "Crash|AI")
	float SearchRange{1000.f};

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Crash|Damage")
	float DamageNumberVerticalOffset{200.f};
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "Crash|Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;
	UPROPERTY(EditDefaultsOnly, Category = "Crash|Effects")
	TSubclassOf<UGameplayEffect> InitializeAttributesEffect;
	UPROPERTY(EditDefaultsOnly, Category = "Crash|Effects")
	TSubclassOf<UGameplayEffect> ResetAttributesEffect;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Replicated)
	bool bAlive = true;
};

// Copyright ArgoMel

#include "Characters/CC_EnemyCharacter.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AIController.h"
#include "AbilitySystem/CC_AbilitySystemComponent.h"
#include "AbilitySystem/CC_AttributeSet.h"
#include "GameplayTags/CCTags.h"
#include "Net/UnrealNetwork.h"

ACC_EnemyCharacter::ACC_EnemyCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	AbilitySystemComponent = CreateDefaultSubobject<UCC_AbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AttributeSet = CreateDefaultSubobject<UCC_AttributeSet>("AttributeSet");
}

void ACC_EnemyCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, bIsBeingLaunched);
}

void ACC_EnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (!IsValid(GetAbilitySystemComponent()))
	{
		return;
	}
	GetAbilitySystemComponent()->InitAbilityActorInfo(this, this);
	OnASCInitialized.Broadcast(GetAbilitySystemComponent(), GetAttributeSet());

	if (!HasAuthority())
	{
		return;
	}
	GiveStartupAbilities();
	InitializeAttributes();

	const UCC_AttributeSet* CC_AttributeSet = Cast<UCC_AttributeSet>(GetAttributeSet());
	if (!IsValid(CC_AttributeSet))
	{
		return;
	}
	GetAbilitySystemComponent()->GetGameplayAttributeValueChangeDelegate(CC_AttributeSet->GetHealthAttribute()).AddUObject(this, &ThisClass::OnHealthChanged);
}

UAttributeSet* ACC_EnemyCharacter::GetAttributeSet() const
{
	return AttributeSet;
}

void ACC_EnemyCharacter::HandleDeath()
{
	Super::HandleDeath();

	AAIController* AIController = GetController<AAIController>();
	if (!IsValid(AIController)) return;
	AIController->StopMovement();
}

UAbilitySystemComponent* ACC_EnemyCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ACC_EnemyCharacter::StopMovementUntilLanded()
{
	bIsBeingLaunched = true;
	AAIController* AIController = GetController<AAIController>();
	if (!IsValid(AIController)) return;
	AIController->StopMovement();
	if (!LandedDelegate.IsAlreadyBound(this, &ThisClass::EnableMovementOnLanded))
	{
		LandedDelegate.AddDynamic(this, &ThisClass::EnableMovementOnLanded);
	}
}

void ACC_EnemyCharacter::EnableMovementOnLanded(const FHitResult& Hit)
{
	bIsBeingLaunched = false;
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, CCTags::Events::Enemy::EndAttack, FGameplayEventData());
	LandedDelegate.RemoveAll(this);
}


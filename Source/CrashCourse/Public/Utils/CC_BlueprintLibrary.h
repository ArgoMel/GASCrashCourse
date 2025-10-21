// Copyright ArgoMel

#pragma once

#include "CoreMinimal.h"
#include "GameplayTags/CCTags.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CC_BlueprintLibrary.generated.h"

class UGameplayEffect;
struct FGameplayTag;
struct FGameplayEventData;

UENUM(BlueprintType)
enum class EHitDirection : uint8
{
	Left,
	Right,
	Forward,
	Back
};

USTRUCT(BlueprintType)
struct FClosestActorWithTagResult
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite)
	TWeakObjectPtr<AActor> Actor;

	UPROPERTY(BlueprintReadWrite)
	float Distance{0.f};
};

UCLASS()
class CRASHCOURSE_API UCC_BlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintPure, Category = "Crash|Math")
	static EHitDirection GetHitDirection(const FVector& TargetForward, const FVector& ToInstigator);
	UFUNCTION(BlueprintPure, Category = "Crash|Math")
	static FName GetHitDirectionName(const EHitDirection& HitDirection);
	UFUNCTION(BlueprintCallable, Category = "Crash|Math")
	static FClosestActorWithTagResult FindClosestActorWithTag(AActor* SourceActor, const FVector& Origin, const FName& Tag, float SearchRange);

	UFUNCTION(BlueprintCallable, Category = "Crash|Send")
	static void SendDamageEventToPlayer(AActor* Target, const TSubclassOf<UGameplayEffect>& DamageEffect, UPARAM(ref) FGameplayEventData& Payload, const FGameplayTag& DataTag, float Damage, const FGameplayTag& EventTagOverride, UObject* OptionalParticleSystem = nullptr);
	UFUNCTION(BlueprintCallable, Category = "Crash|Send")
	static void SendDamageEventToPlayers(TArray<AActor*> Targets, const TSubclassOf<UGameplayEffect>& DamageEffect, UPARAM(ref) FGameplayEventData& Payload, const FGameplayTag& DataTag, float Damage, const FGameplayTag& EventTagOverride, UObject* OptionalParticleSystem = nullptr);
	
	UFUNCTION(BlueprintCallable, Category = "Crash|Abilities")
	static TArray<AActor*> HitBoxOverlapTest(AActor* AvatarActor, float HitBoxRadius, float HitBoxForwardOffset = 0.f, float HitBoxElevationOffset = 0.f, bool bDrawDebugs = false);
	UFUNCTION(BlueprintCallable, Category = "Crash|Abilities")
	static TArray<AActor*> ApplyKnockback(AActor* AvatarActor, const TArray<AActor*>& HitActors, float InnerRadius, float OuterRadius, float LaunchForceMagnitude, float RotationAngle = 45.f, bool bDrawDebugs = false);

private:
	static void DrawHitBoxOverlapDebugs(const UObject* WorldContextObject, const TArray<FOverlapResult>& OverlapResults, const FVector& HitBoxLocation, float HitBoxRadius);
};

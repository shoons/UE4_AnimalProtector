// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "HunterAnimInstance.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHunterAttackCheckDelegate);

UCLASS()
class ECOACTOR_API UHunterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UHunterAnimInstance();
	
	virtual void NativeBeginPlay() override;

	virtual void NativeUpdateAnimation(float deltaSeconds) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	float CurrSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = State, Meta = (AllowPrivateAccess = true))
	bool bIsHyperMode;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	bool bIsAttacking;

private:
	UFUNCTION()
	void AnimNotify_HunterAttack();

public:
	FOnHunterAttackCheckDelegate OnHunterAttack;

	void SetHunterToAttack();

private:
	class AEcoActorCharacter* Player;

	UAnimationAsset* DeadAnim;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Hit, Meta = (AllowPrivateAccess = true))
	bool bHit;

public:
	void PlayDeadAnim();

	void SetHunterToHyperMode();

	UFUNCTION(BlueprintCallable, Category=Hit)
	void SetHit(bool isHit);
};

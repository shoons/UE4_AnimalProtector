// Fill out your copyright notice in the Description page of Project Settings.


#include "EcoActorCharacterAnimInstance.h"
#include "EcoActorCharacter.h"
#include "Kismet/GameplayStatics.h"

UEcoActorCharacterAnimInstance::UEcoActorCharacterAnimInstance()
{
	CurrSpeed = FVector::ZeroVector;
	bInAir = false;

	static ConstructorHelpers::FObjectFinder<UAnimMontage>Combo(TEXT("/Game/Main/Anim/Fight/AM_FullCombo.AM_FullCombo"));
	if (Combo.Succeeded())
	{
		FullCombo = Combo.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimationAsset> SHOT(TEXT("/Game/Main/Anim/Character/WithGun/retargettedFire_Rifle_Ironsights.retargettedFire_Rifle_Ironsights"));
	if (SHOT.Succeeded())
	{
		ShotAnim = SHOT.Object;
	}
	bIsEquipping = false;

	static ConstructorHelpers::FObjectFinder<UAnimationAsset> DEAD(TEXT("/Game/Main/Anim/Character/Character_Death_2.Character_Death_2"));
	if (DEAD.Succeeded())
	{
		DeadAnim = DEAD.Object;
	}
}

void UEcoActorCharacterAnimInstance::NativeUpdateAnimation(float deltaSeconds)
{
	Super::NativeUpdateAnimation(deltaSeconds);
	
	auto Pawn = TryGetPawnOwner();
	if (::IsValid(Pawn))
	{
		auto speedVec = Pawn->GetVelocity();
		speedVec.Z = 0.0f;
		CurrSpeed = speedVec;
		auto Character = Cast<ACharacter>(Pawn);
		if (Character)
		{
			bInAir = Character->GetMovementComponent()->IsFalling();
			auto EcoActor = Cast<AEcoActorCharacter>(Character);
			bIsAttacking = EcoActor->isAttacking();
			bIsEquipped = EcoActor->isEquipped();
			EcoActor->SetIsEquipping(bIsEquipping);
		}
	}
}

void UEcoActorCharacterAnimInstance::AnimNotify_StartCombo()
{
	OnStartCombo.Broadcast();
}

void UEcoActorCharacterAnimInstance::AnimNotify_ComboHitCheck()
{
	OnComboHitCheck.Broadcast();
}

void UEcoActorCharacterAnimInstance::AnimNotify_NextComboCheck()
{
	OnNextComboCheck.Broadcast();
}

void UEcoActorCharacterAnimInstance::AnimNotify_EquipStart()
{
	bIsEquipping = true;
	LOG(Warning, TEXT("equip start"));
}
void UEcoActorCharacterAnimInstance::AnimNotify_EquipEnd()
{
	bIsEquipping = false;
	LOG(Warning, TEXT("equip end"));
}


void UEcoActorCharacterAnimInstance::PlayComboMontage()
{
	if (!Montage_IsPlaying(FullCombo))
	{
		Montage_Play(FullCombo);
	}
}

void UEcoActorCharacterAnimInstance::JumpToComboSection(int32 currCombo)
{
	if (currCombo == 1)Montage_JumpToSection(getComboNontageSectionName(currCombo), FullCombo);
	else Montage_SetNextSection(getComboNontageSectionName(currCombo-1), getComboNontageSectionName(currCombo), FullCombo);
	LOG(Warning, TEXT("%d"), currCombo);
}

FName UEcoActorCharacterAnimInstance::getComboNontageSectionName(int32 currCombo)
{
	return FName(*FString::Printf(TEXT("Combo%d"), currCombo));
}


void UEcoActorCharacterAnimInstance::AnimNotify_ShotTriggered()
{
	OnShotTriggered.Broadcast();
}

void UEcoActorCharacterAnimInstance::PlayDeadAnim()
{
	auto Player = Cast<AEcoActorCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	if (IsValid(Player))
	{
		Player->GetMesh()->PlayAnimation(DeadAnim, false);
	}
}
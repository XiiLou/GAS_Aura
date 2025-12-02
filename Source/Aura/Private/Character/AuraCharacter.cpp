// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AuraCharacter.h"

#include "AbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/AuraPlayerController.h"
#include "Player/AuraPlayerState.h"
#include "UI/HUD/AuraHUD.h"


AAuraCharacter::AAuraCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	//GetCharacterMovement()->bOrientRotationToMovement = true;
	//GetCharacterMovement()->RotationRate = FRotator(0.0f, 400.0f, 0.0f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
}

void AAuraCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	APlayerController* PC = Cast<APlayerController>(GetController());
	if (!PC) return;

	FHitResult Hit;
	if (PC->GetHitResultUnderCursor(ECC_Visibility, false, Hit))
	{
		FVector TargetLocation = Hit.ImpactPoint;

		// 只在水平面上转向
		FVector ToTarget = TargetLocation - GetActorLocation();
		ToTarget.Z = 0.f;

		if (!ToTarget.IsNearlyZero())
		{
			FRotator TargetRot = ToTarget.Rotation();

			// 平滑转向
			FRotator NewRot = FMath::RInterpTo(
				GetActorRotation(),
				TargetRot,
				DeltaSeconds,
				25.f          // 转身速度，自己可以调
			);

			SetActorRotation(NewRot);
		}
	}
}

void AAuraCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	//Init Ability Actor Info for the Server
	InitAbilityActorInfo();
}

void AAuraCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	
	//Init Ability Actor Info for the Client
	InitAbilityActorInfo();
}

void AAuraCharacter::InitAbilityActorInfo()
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	AuraPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(AuraPlayerState, this);
	AbilitySystemComponent = AuraPlayerState->GetAbilitySystemComponent();
	AttributeSet = AuraPlayerState->GetAttributeSet();

	if (AAuraPlayerController* AuraPlayerController = Cast<AAuraPlayerController>(GetController()))
	{
		if (AAuraHUD* AuraHUD = Cast<AAuraHUD>(AuraPlayerController->GetHUD()))
		{
			AuraHUD->InitOverlay(AuraPlayerController, AuraPlayerState, AbilitySystemComponent, AttributeSet);
		}
	}
}

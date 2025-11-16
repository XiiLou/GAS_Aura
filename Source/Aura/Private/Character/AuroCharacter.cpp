// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AuroCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"

AAuroCharacter::AAuroCharacter()
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

void AAuroCharacter::Tick(float DeltaSeconds)
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

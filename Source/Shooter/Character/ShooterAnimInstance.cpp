// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterAnimInstance.h"

#include "ShooterCharacter.h"

//Components
#include "GameFramework/CharacterMovementComponent.h"


void UShooterAnimInstance::NativeInitializeAnimation()
{
	ShooterCharacter = Cast<AShooterCharacter>(TryGetPawnOwner());

	
}

void UShooterAnimInstance::UpdateAnimationProprerties(float DeltaTime)
{
	if (!ShooterCharacter)
	{
		UE_LOG(LogTemp, Error, TEXT("Shooter Character is nullptr in UShooterAnimInstance"))
			return;
	}

	FVector Velocity{ ShooterCharacter->GetVelocity() };
	Velocity.Z = 0.f;
	Speed = Velocity.Size();

	bIsInAir = ShooterCharacter->GetCharacterMovement()->IsFalling();


	const float HeroAcelerating = ShooterCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size();

	(HeroAcelerating > 0.f) ? bIsAccelerating = true : bIsAccelerating = false;


}

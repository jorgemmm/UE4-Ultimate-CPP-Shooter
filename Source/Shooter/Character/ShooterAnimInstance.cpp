// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterAnimInstance.h"

#include "ShooterCharacter.h"

//Components
#include "GameFramework/CharacterMovementComponent.h"

//utils
#include "Kismet/KismetMathLibrary.h"


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

	FRotator AimRotator = ShooterCharacter->GetBaseAimRotation();
	
	//Vamos a obtener los grados resp al eje x del mvto-> rotator
	FRotator MovementRotator = UKismetMathLibrary::MakeRotFromX(
		ShooterCharacter->GetVelocity() //vector del mvto
	);
	MovementOffsetYaw = UKismetMathLibrary::NormalizedDeltaRotator(MovementRotator, AimRotator).Yaw;
	
	if (ShooterCharacter->GetVelocity().Size() > 0.01f)
	   { 
		 LastMovementOffsetYaw = MovementOffsetYaw; 
	   }

	FString RotationMessage = FString::Printf(TEXT("Base Aim Rotation: %f"), AimRotator.Yaw);
	FString MovementRotatorMessage = FString::Printf(TEXT("MovementRotator Rotation: %f"), MovementRotator.Yaw);
	FString  OffsetMessage = FString::Printf(TEXT("Movement Offset Yaw: %f"), MovementOffsetYaw);
	FString  LastMovementOffsetYawMessage = FString::Printf(TEXT("Last Movement Offset Yaw: %f"), LastMovementOffsetYaw);
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1, 0.f, FColor::White, RotationMessage);
		GEngine->AddOnScreenDebugMessage(2, 0.f, FColor::Yellow, MovementRotatorMessage);
		GEngine->AddOnScreenDebugMessage(3, 0.f, FColor::Emerald, OffsetMessage);
		GEngine->AddOnScreenDebugMessage(4, 0.f, FColor::Blue, LastMovementOffsetYawMessage);
	}

}

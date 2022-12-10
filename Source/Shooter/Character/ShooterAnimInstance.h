// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ShooterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTER_API UShooterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	// Sets default values for this classes properties
	virtual void NativeInitializeAnimation() override;

	UFUNCTION(BlueprintCallable)
	void UpdateAnimationProprerties(float DeltaTime);
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Movement, Meta = ( AllowPrivateAccess="true"))
	class AShooterCharacter* ShooterCharacter;
	/**The Speed of the character*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, Meta = (AllowPrivateAccess = "true"))
	float Speed;
	/**Whether or not the character is in the air */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, Meta = (AllowPrivateAccess = "true"))
	bool bIsInAir;
	/**Whether or not the character is moving */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= movement, Meta = (AllowPrivateAccess="true"))
	bool bIsAccelerating;
	/**Offset Yaw used for strafing ametrallar*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = movement, Meta = (AllowPrivateAccess = "true"))
	float  MovementOffsetYaw;

	/**Offset Yaw the frame before we stopped movig*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = movement, Meta = (AllowPrivateAccess = "true"))
		float  LastMovementOffsetYaw;
};

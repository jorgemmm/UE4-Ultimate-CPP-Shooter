// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ShooterCharacter.generated.h"

UCLASS()
class SHOOTER_API AShooterCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AShooterCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	//Inputs	
	void MoveForward(float Value);	
	void MoveRight(float Value);
	/**
	* Called to turn at a given rate
	* Input value rate rate [ 0, 1]
	* Rate-> 1.0 means 100% of desired turn rate -> BaseTurnRate or BaseLookUpRate 
	*/
	void TurnAtRate(float Rate );
	void LookUpRate(float Rate);
	
	void FireWeapon();

	
	UFUNCTION()
	void FireWeaponHandle();

	
	bool TraceUnderCursor();//(FHitResult ScreenTracerHit);
	void FXFire(FHitResult HitResult);//, FVector& BeamEndLocation);


	UFUNCTION(Server, Reliable)
		void Server_FireWeapon(const FVector_NetQuantize& TraceHitTarget);

	UFUNCTION(NetMulticast, Reliable)
		void Multicast_FireWeapon(const FVector_NetQuantize& TraceHitTarget);



private:
	/**Camera Boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= Camera, meta=( AllowPrivateAccess ="true"))
		class USpringArmComponent* CameraBoom;

	/**Camera tha follows character*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FollowCamera;
	
	/** Base  turn rate, in deg/sec, Other scaling may affect final turn rate*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
		float BaseTurnRate;

	/** Base  Look Up/Down, in deg/sec, Other scaling may affect final turn rate*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
		float BaseLookUpRate;

	/** Base  Yaw, in deg */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
		float PawYawRate;  

	/** !!Atached in Fire Montage!!
	*   Sound fx for fire shot,  
	*   If you want handle here you have to Disable 
	 *   in Fire montage ( Null in Sound Notification )
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	class USoundCue* FireSound;
	
	/** !!Atached in Fire Montage!!
	* flash spwaned particle fx for fire shot at Barrel socket
	*  If you want handle here you have to Disable 
	*  in Fire montage ( Null in Sound Notification )
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	   class UParticleSystem* MuzzleFlash;

	/**particle fx  spwaned  upon bullet impact*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
		class UParticleSystem* ImpactParticles;

	/**Smoke trail for bullet*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
		class UParticleSystem* BeamParticles;


	/** flasd spwaned particle fx for fire shot at Barrel socket*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
		class UAnimMontage* HipFireMontage;




public:
	/** Returns CameraBooms subobject*/
	USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	UCameraComponent* GetFollowCamera() const { return FollowCamera; }



};

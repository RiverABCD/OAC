// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Runtime/Engine/Classes/GameFramework/SpringArmComponent.h"
#include "Runtime/Engine/Classes/Camera/CameraComponent.h"
#include "Runtime/Engine/Classes/Components/ArrowComponent.h"
#include "Runtime/Engine/Classes/GameFramework/CharacterMovementComponent.h"
#include "Runtime/Engine/Classes/Particles/ParticleSystemComponent.h"
#include "DrawDebugHelpers.h"
#include "BaseCharacter.generated.h"

UCLASS()
class OAC_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditDefaultsOnly, Category = "Base Character")
		USpringArmComponent* SpringArm;
	UPROPERTY(EditDefaultsOnly, Category = "Base Character")
		UCameraComponent* Camera;
	UPROPERTY(EditDefaultsOnly, Category = "Base Character")
		UArrowComponent* ForwardDirection;
	UPROPERTY(EditDefaultsOnly, Category = "Base Character")
		UParticleSystemComponent* ExplosionEffect;
	// property for determining whether the firing action has been initiated   --used for animation
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Character")
		bool bIsFiring;

	// property for sprinting
	UPROPERTY(EditDefaultsOnly, Category = "Base Character")
		bool bIsSprinting;
	UPROPERTY(EditDefaultsOnly, Category = "Base Character")
		float MaxSprint;
	UPROPERTY(EditDefaultsOnly, Category = "Base Character")
		float BaseRunSpeed;
	UPROPERTY(EditDefaultsOnly, Category = "Base Character")
		float TimeToMaxSpeed;
	
		float CurrentSprint;
	// input bindings
	void MoveForward(float amount);
	void MoveRight(float amount);
	void CameraRotate(float amount);
	void CameraHeight(float amount);
	void SprintStart();
	void SprintEnd();

	// use two function to handle the firing animation
	// so we can tie in to animations later
	UFUNCTION(BlueprintCallable, Category = "Base Character")
		void FireStart();
	UFUNCTION(BlueprintCallable, Category = "Base Character")
		void Fired();
};

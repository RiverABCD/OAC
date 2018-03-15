// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Runtime/Engine/Classes/Components/SphereComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "MyCollisionActor.generated.h"


UCLASS()
class OAC_API AMyCollisionActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyCollisionActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Sphere collision component
	UPROPERTY(EditDefaultsOnly, Category = "MyCollisionActor")
		USphereComponent* collisionSphere;
	UPROPERTY(EditDefaultsOnly, Category = "MyCollisionActor")
		UPhysicalMaterial* physicsMaterial;
	UPROPERTY(EditAnywhere, Category = "MyCollisionActor")
		TEnumAsByte<ECollisionResponse> responseToCollisionChannel;
};

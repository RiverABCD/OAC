// Fill out your copyright notice in the Description page of Project Settings.

#include "OACPlayerController.h"

void AOACPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	//calls our Fire() function whenever the key bound to the "Fire" action is released
	InputComponent->BindAction("Fire", EInputEvent::IE_Released, this, &AOACPlayerController::Fire);
}

void AOACPlayerController::Fire()
{
	//distance we want raycast to travel
	float distance = 1000.f;
	//direction we want the raycast to go in,in this case,the same direction the camera is facing
	FVector direction = PlayerCameraManager->GetActorForwardVector();
	//start location of raycast
	FVector start = PlayerCameraManager->GetCameraLocation() + (direction*40.f);
	//end location of raycast
	FVector end = start + (direction*distance);
/*
	//hold raycast result
	FHitResult outHit;
	bool hasHitSomething = GetWorld()->LineTraceSingleByChannel(outHit, start, end, ECollisionChannel::ECC_Camera);
	DrawDebugLine(GetWorld(), start, end, FColor::Purple, true, 10.f, (uint8)0, 10.f);
	//deal with the hit 
	if (hasHitSomething) {}
*/
	TArray<FHitResult> outHits;
	bool hasHitSomething = GetWorld()->LineTraceMultiByChannel(outHits, start, end, ECollisionChannel::ECC_Camera);
	for (auto& elem : outHits)
	{
		// pick random color to represent the hit object
		FColor color = FColor::MakeRandomColor();
		// use the impact point as the end of our debug line visualization
		FVector impactPoint = elem.ImpactPoint;
		//draw debug line
		DrawDebugLine(GetWorld(), start, impactPoint, color, true, 2.f, (uint8)0, 10.f);
		//display the name of object hits for clarity
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, color, elem.GetActor()->GetName());
		}
	}
}


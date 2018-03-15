// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "DrawDebugHelpers.h"
#include "OACPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class OAC_API AOACPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	//sets up the bindings to input controllers
	virtual void SetupInputComponent() override;
	//called when player calls the "fire" action binding
	void Fire();	
};

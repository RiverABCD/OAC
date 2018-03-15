// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "OACGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class OAC_API AOACGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	//constructor
	AOACGameModeBase();
	virtual void BeginPlay()override;
	virtual void Tick(float DeltaTime)override;
private:
	FVector2D object;
	FVector2D TranslateObject(FVector2D objectToTranslate, FVector2D byAmonut);
};

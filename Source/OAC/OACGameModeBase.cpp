// Fill out your copyright notice in the Description page of Project Settings.

#include "OACGameModeBase.h"

AOACGameModeBase::AOACGameModeBase()
{
	//enable our tick function
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 10.f;
}

void AOACGameModeBase::BeginPlay()
{
	//remember to call super
	Super::BeginPlay();
	//You always want to ensure that your reference to GEngine is valid
	if (GEngine) 
	{	//print version string
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("version0.0.1"));
	}
	//initalize object
	object = FVector2D(0, 0);
}

void AOACGameModeBase::Tick(float DeltaTime)
{
	//never forget to call your super
	Super::Tick(DeltaTime);
	
	//translate the object by 0.1,0.1 every frame
	object = TranslateObject(object, FVector2D(0.1f, 0.1f));
	
	//print the object's value every frame
	if (GEngine)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Blue, object.ToString());
	}
}

FVector2D AOACGameModeBase::TranslateObject(FVector2D objectToTranslate, FVector2D byAmonut)
{
	FVector2D translatedObject = objectToTranslate + byAmonut;

	if (GEngine)
	{
		//build debug string
		FString debugString;
		debugString = debugString.Append(TEXT("Initial Value:"));
		debugString = debugString.Append(objectToTranslate.ToString());
		debugString = debugString.Append(TEXT(" | byAmount:"));
		debugString = debugString.Append(byAmonut.ToString());
		debugString = debugString.Append(TEXT(" | translatedObject:"));
		debugString = debugString.Append(translatedObject.ToString());
		
		//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Purple, *debugString);
	}

	return translatedObject;
}

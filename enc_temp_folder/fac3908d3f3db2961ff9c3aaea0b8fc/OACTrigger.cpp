// Fill out your copyright notice in the Description page of Project Settings.

#include "OACTrigger.h"


// Sets default values
AOACTrigger::AOACTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// TriggerBox for trigger action
	TriggerVolume = CreateDefaultSubobject<UBoxComponent>(FName("TriggerBox"));
	if (!ensure(TriggerVolume != nullptr)) return;
	RootComponent = TriggerVolume;

	// Binds a UObject instance and a UObject method address to this multi-cast delegate. 
	TriggerVolume->OnComponentBeginOverlap.AddDynamic(this, &AOACTrigger::OnOverlapBegin);
	TriggerVolume->OnComponentEndOverlap.AddDynamic(this, &AOACTrigger::OnOverlapEnd);
}

// Called when the game starts or when spawned
void AOACTrigger::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AOACTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AOACTrigger::OnOverlapBegin(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Activated"));
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Activated"));
	}
}

void AOACTrigger::OnOverlapEnd(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("Deactivated"));
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Deactivated"));
	}
}


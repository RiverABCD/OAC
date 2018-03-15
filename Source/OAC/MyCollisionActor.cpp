// Fill out your copyright notice in the Description page of Project Settings.

#include "MyCollisionActor.h"


// Sets default values
AMyCollisionActor::AMyCollisionActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//add collision sphere
	collisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Collision Sphere"));

	//set a default radius to our sphere
	collisionSphere->SetSphereRadius(32.f);

	//allow the sphere to be visible in-game
	collisionSphere->bHiddenInGame = false;
	collisionSphere->SetVisibility(true);
	
	//enable physics on our sphere
	collisionSphere->SetSimulatePhysics(true);

	//tell the engine that this is a physics object that can be queried by RayCasting
	collisionSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	//tell the engine that the sphere should not overlap with any other object
	//collisionSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);

	//tell the engine that the sphere should not overlap with any other object when performing physics
	collisionSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic,ECollisionResponse::ECR_Block);
	collisionSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);

	//sets the collision sphere as the root component
	RootComponent = collisionSphere;

	//overlap object
	responseToCollisionChannel = ECollisionResponse::ECR_Overlap;
}

// Called when the game starts or when spawned
void AMyCollisionActor::BeginPlay()
{
	Super::BeginPlay();
	
	if (physicsMaterial)
	{
		collisionSphere->BodyInstance.SetPhysMaterialOverride(physicsMaterial);
	}

	collisionSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, responseToCollisionChannel);
}

// Called every frame
void AMyCollisionActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


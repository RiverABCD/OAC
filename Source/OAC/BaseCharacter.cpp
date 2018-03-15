// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseCharacter.h"


// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// create springArm
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	// keep the springArm from rotating with character
	SpringArm->bAbsoluteRotation = true;
	SpringArm->bUsePawnControlRotation = false;
	// define the distance between the character and camera
	SpringArm->TargetArmLength = 300.f;
	// attach the springArm to RootComponent
	SpringArm->SetupAttachment(RootComponent);

	// create camera
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	// give it a larger FOV, 110 degrees
	Camera->FieldOfView = 100.f;
	// attach the camera to the SpringArm
	Camera->SetupAttachment(SpringArm);

	// create forward arrow component
	ForwardDirection = CreateDefaultSubobject<UArrowComponent>(TEXT("Forward Direction"));
	// attach the arrow to the RootComponent
	ForwardDirection->SetupAttachment(RootComponent);
	// turn off the collision for our raycasts on the mesh
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Destructible, ECollisionResponse::ECR_Ignore);

	// set default value for bIsFiring
	bIsFiring = false;

	// ensure our character is always facing the direction he's moving
	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
	// allow character to walk up stairs
	GetCharacterMovement()->SetWalkableFloorAngle(50.f);
	GetCharacterMovement()->MaxStepHeight = 45.f;

	//set up our explosion particle effect system
	ExplosionEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Explosion Effect"));
	//ensure it doesn't activate on startup
	ExplosionEffect->bAutoActivate = false;
	//ensure we don't interrupt our raycast with our particle effect
	ExplosionEffect->SetCollisionResponseToChannel(ECollisionChannel::ECC_Destructible, ECollisionResponse::ECR_Ignore);

	//jump tweaking
	JumpMaxHoldTime = 0.25f;
	//movement control while in the air
	GetCharacterMovement()->AirControl = 0.5f;

	//jump height
	GetCharacterMovement()->JumpZVelocity = 800.f;
	//fall speed
	GetCharacterMovement()->GravityScale = 2.0f;
	//support double jumps
	JumpMaxCount = 2;

	//set walking parameters
	//not sprinting by default
	bIsSprinting = false;
	//default run speed
	BaseRunSpeed = 600.f;
	//set default max sprint to 2x normal speed
	MaxSprint = 2.0f;
	//currentsprint starts at 1x multiple of base speed
	CurrentSprint = 1.0f;
	//set MaxWalkSpeed in Movement Component to base run speed
	GetCharacterMovement()->MaxWalkSpeed = BaseRunSpeed;
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	// set the arm behind the character and a little of ground
	SpringArm->SetRelativeRotation(FQuat::MakeFromEuler(FVector(0, -20, 0)));
	// active the camera on start
	Camera->Activate();
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//how much speed to increase this frame
	float rampThisFrame = (DeltaTime / TimeToMaxSpeed)*MaxSprint;
	
	if (bIsSprinting)
	{
		CurrentSprint += rampThisFrame;
	}
	else {
		CurrentSprint -= rampThisFrame;
	}
	//Clamp currentsprint between 1.0f and MaxSprint
	CurrentSprint = FMath::Clamp(CurrentSprint, 1.f, MaxSprint);
	//apply the movement change to the character
	GetCharacterMovement()->MaxWalkSpeed = BaseRunSpeed * CurrentSprint;
}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// bind action mappings
	InputComponent->BindAction("Jump", EInputEvent::IE_Pressed, this, &ACharacter::Jump);
	InputComponent->BindAction("Jump", EInputEvent::IE_Released, this, &ACharacter::StopJumping);
	InputComponent->BindAction("Fire", EInputEvent::IE_Pressed, this, &ABaseCharacter::FireStart);
	InputComponent->BindAction("Sprint", EInputEvent::IE_Pressed, this, &ABaseCharacter::SprintStart);
	InputComponent->BindAction("Sprint", EInputEvent::IE_Released, this, &ABaseCharacter::SprintEnd);


	// bind axis mappings to function
	InputComponent->BindAxis("MoveForward", this, &ABaseCharacter::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &ABaseCharacter::MoveRight);
	InputComponent->BindAxis("CameraRotate", this, &ABaseCharacter::CameraRotate);
	InputComponent->BindAxis("CameraHeight", this, &ABaseCharacter::CameraHeight);
}

void ABaseCharacter::MoveForward(float amount)
{
	// ensure our controller is valid and that amount is not 0
	if (Controller && amount)
	{
		// add movement in the camera facing direction
		AddMovementInput(SpringArm->GetForwardVector(), amount);
	}
}

void ABaseCharacter::MoveRight(float amount)
{
	// ensure our controller is valid and that amount is not 0
	if (Controller && amount)
	{
		// add movement in the camera right direction
		AddMovementInput(SpringArm->GetRightVector(), amount);
	}
}

void ABaseCharacter::CameraRotate(float amount)
{
	// ensure our controller is valid and that amount is not 0
	if (Controller && amount)
	{
		// get the current spring arm rotation as a vector
		FVector rot = SpringArm->GetComponentRotation().Euler();
		// add the amount to the vector
		rot += FVector(0, 0, amount);
		// apply the new rotation to the springarm
		SpringArm->SetWorldRotation(FQuat::MakeFromEuler(rot));
	}
}

void ABaseCharacter::CameraHeight(float amount)
{
	// ensure our controller is valid and that amount is not 0
	if (Controller && amount)
	{
		// get the current spring arm rotation as a vector
		FVector rot = SpringArm->GetComponentRotation().Euler();
		// separate the height out into its own variable
		float newHeight = rot.Y;
		// add the desired rotation
		newHeight += amount;
		// clamp the height into comfortable levels
		newHeight = FMath::Clamp(newHeight, -45.f, -5.f);
		// rebuild the rotation vector
		rot = FVector(0, newHeight, rot.Z);
		// apply the new rotation vector to the spring arm
		SpringArm->SetWorldRotation(FQuat::MakeFromEuler(rot));
	}
}

void ABaseCharacter::SprintStart()
{
	bIsSprinting = true;
}

void ABaseCharacter::SprintEnd()
{
	bIsSprinting = false;
}

void ABaseCharacter::FireStart()
{
	//make sure we are not already firing
	if (!bIsFiring)
	{
		bIsFiring = true;
	}
}

void ABaseCharacter::Fired()
{
	//if we are firing ,return
	if (!bIsFiring) return;
	//tell the animation engine that we've eaten the call to Fired()
	bIsFiring = false;

	//set fire point
	FVector HandLocation = GetMesh()->GetBoneLocation(FName("RightHandMiddle1"));

	//check to see if an effect has been provided
	if (ExplosionEffect)
	{
		//put the effect to our hand location
		ExplosionEffect->SetWorldLocation(HandLocation);
		//active the effect,restarting the system
		ExplosionEffect->Activate(true);
	}
	//distance we want raycast to travel
	float distance = 1000.f;
	//direction we want the raycast to go in,in this case,the same direction the camera is facing
	FVector direction = ForwardDirection->GetForwardVector();

	//start location of raycast
	//FVector start = GetActorLocation() + (direction * 120.f);
	FVector start = HandLocation;

	//end location of raycast
	FVector end = start + (direction * distance);

	//declare our hit result
	FHitResult outHit;
	//do the raycast
	bool hasHitSomething = GetWorld()->LineTraceSingleByChannel(outHit, start, end, ECollisionChannel::ECC_Destructible);
	//choose a random color
	FColor color = FColor::MakeRandomColor();
#if 0
	for (auto& elem : outHits)
	{
		// use the impact point as the end of our debug line visualization
		FVector impactPoint = elem.ImpactPoint;
		//draw debug line
		DrawDebugLine(GetWorld(), start, impactPoint, color, true, 10.f, (uint8)0, 10.f);
		//display the name of object hits for clarity
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, color, elem.GetActor()->GetName());
		}
	}
#endif

	if (hasHitSomething)
	{
		//draw debug line
		DrawDebugLine(GetWorld(), start, outHit.ImpactPoint, color, true, 2.f, (uint8)0, 10.f);

		//display the name of object hits for clarity
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, color, outHit.GetActor()->GetName());
		}

		//check to make sure the hit actor is movable,return otherwise
		if (!outHit.GetActor()->IsRootComponentMovable()) return;
		//Create an array to hold any mesh components on the actor
		TArray<UStaticMeshComponent*> Components;
		//get all the static mesh components on hit actor
		outHit.GetActor()->GetComponents<UStaticMeshComponent>(Components);
		//iterate through the array and blast away any found meshs
		for (auto& mesh : Components)
		{
			mesh->AddForce(direction*100000000.f);
		}
	}
	else {
		DrawDebugLine(GetWorld(), start, end, color, true, 2.f, (uint8)0, 10.f);
	}
}

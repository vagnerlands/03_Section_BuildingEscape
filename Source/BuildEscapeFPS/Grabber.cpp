// Fill out your copyright notice in the Description page of Project Settings.


#include "Grabber.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	SetupPhysicsHandle();

	SetupInputComponent();

}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	Update();

	if (ObjectInHands)
	{
		PhysicsHandle->SetTargetLocation(EndLocation);
		UE_LOG(LogTemp, Warning, TEXT("   ObjLoc %s"), *EndLocation.ToString());
	}
}

void UGrabber::Update()
{
	// Get the player viewpoint
	APlayerController* pMainCharacter = GetWorld()->GetFirstPlayerController();
	if (pMainCharacter)
	{
		// 50cm in front of us, comming from inside the pawn
		const float RayStartDistance = 50.f;
		// up to 2 meters length
		const float RayEndDistance = 200.f;
		// find the current player location and lookat
		pMainCharacter->GetPlayerViewPoint(EyeLocation, EyeDirection);

		// Recalculate the Start and End location of the line trace for every frame
		StartLocation = EyeLocation + (EyeDirection.Vector() * RayStartDistance);
		EndLocation = EyeLocation + (EyeDirection.Vector() * RayEndDistance);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT(" *** Actor Component (%s) couldn't be found!"), *GetOwner()->GetFName().ToString());
	}
}

void UGrabber::Grab()
{
	// avoid crashing
	if (!PhysicsHandle) return;
	// check if some PhysicsBody is in reach
	FHitResult hit = GetFirstPhysicsBodyInReach();
	// another validation, is there an actor?
	if (hit.GetActor())
	{
		// retrieve the "UPrimitiveComponent" object
		ObjectInHands = hit.GetComponent();
		// use it to attach to our
		PhysicsHandle->GrabComponentAtLocation(
			ObjectInHands,  // component to be attached to ours 
			NAME_None,		// we're not working with skeleton mesh, so no need for this
			EndLocation		// point in the world to be attached
		);

		UE_LOG(LogTemp, Warning, TEXT("   Grabbed (%s)"), *hit.Actor->GetName());
	}
}

void UGrabber::Release()
{
	if (ObjectInHands)
	{
		ObjectInHands = nullptr;
		PhysicsHandle->ReleaseComponent();
		UE_LOG(LogTemp, Warning, TEXT("   Object released!"));
	}
}

void UGrabber::SetupPhysicsHandle()
{
	// check for Physics Handle Component in this member
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	// check if not null
	if (PhysicsHandle)
	{
		UE_LOG(LogTemp, Warning, TEXT("  Physics handle found for (%s)"), *GetOwner()->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT(" *** Physics Handle couldn't be retrieved from (%s) !"), *GetOwner()->GetName())
	}
}

void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("  Input Component found for (%s)"), *GetOwner()->GetName());
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT(" *** Input Component couldn't be retrieved from (%s) !"), *GetOwner()->GetName())
	}
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{

	FHitResult HitObject = FHitResult(ForceInit);

	// ignore ourselves
	FCollisionQueryParams TraceParams(
		// trace tag, for debug
		FName(""),
		// use complex
		false,
		// ignore ourselves
		GetOwner());

	// execute ray-cast
	bool HitSomething = GetWorld()->LineTraceSingleByChannel(
		// out argument, object that was hit
		HitObject,
		StartLocation,
		EndLocation,
		ECollisionChannel::ECC_PhysicsBody,
		TraceParams);
	// was something hit?
	if (HitSomething)
	{
		// some weird bug? sometimes it hit something, but no actor is found...
		if (HitObject.Actor != nullptr)
		{
			//UE_LOG(LogTemp, Warning, TEXT(" We hit (%s) !"), *HitObject.Actor->GetName());
			return HitObject;
		}
	}
	//DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Green, false, 1, 0, 1);

	// return empty result
	return FHitResult();
}


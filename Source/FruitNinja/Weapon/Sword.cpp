// Fill out your copyright notice in the Description page of Project Settings.


#include "Sword.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
ASword::ASword()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultScene"));

	SwordMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SwordMeshComp"));
	SwordMeshComp->SetupAttachment(RootComponent);

	TraceStartPoint = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TraceStartPoint"));
	TraceStartPoint->SetupAttachment(SwordMeshComp);
	TraceEndPoint = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TraceEndPoint"));
	TraceEndPoint->SetupAttachment(SwordMeshComp);
}

// Called when the game starts or when spawned
void ASword::BeginPlay()
{
	Super::BeginPlay();

	TraceInterval = (TraceEndPoint->GetComponentLocation() - TraceStartPoint->GetComponentLocation()) / (NumOfTrace - 1);
	FVector CurrentLoc = TraceStartPoint->GetComponentLocation();
	for (int32 i=0; i < NumOfTrace; i++)
	{
		TraceHistory.Add(CurrentLoc);
		CurrentLoc += TraceInterval;
	}
}

// Called every frame
void ASword::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (TraceHistory.Num() == 0) return;

	TraceInterval = (TraceEndPoint->GetComponentLocation() - TraceStartPoint->GetComponentLocation()) / (NumOfTrace - 1);
	FVector CurrentLoc = TraceStartPoint->GetComponentLocation();
		
	TArray<FHitResult> HitResults;
	TMap<uint32, FHitResult> HitMap;
	TArray<AActor*> IgnoreActors;
	for (int32 i = 0; i < NumOfTrace; i++)
	{
		bool bHit;
		FHitResult HitResult;
		bHit = UKismetSystemLibrary::LineTraceSingle(this, TraceHistory[i], CurrentLoc, ETraceTypeQuery::TraceTypeQuery1, false, IgnoreActors, EDrawDebugTrace::None, HitResult, true, FLinearColor::Red, FLinearColor::Green, 1.f);
		//GetWorld()->LineTraceSingleByChannel(HitResult, TraceHistory[i], CurrentLoc, ECC_Visibility);

		
		if (bHit && IsValid(HitResult.GetActor()) && !HitMap.Contains(HitResult.GetActor()->GetUniqueID()))
		{
			HitMap.Emplace(HitResult.GetActor()->GetUniqueID() ,HitResult);
		}

		TraceHistory[i] = CurrentLoc;
		CurrentLoc += TraceInterval;
	}

	HitMap.GenerateValueArray(HitResults);
	OnTraceHit(HitResults);

	CurrentVelocity = GetActorLocation() - ActorPreLocation;
	ActorPreLocation = GetActorLocation();
}

FVector ASword::GetVelocity() const
{
	return CurrentVelocity;
}


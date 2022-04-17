// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Sword.generated.h"

UCLASS()
class FRUITNINJA_API ASword : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASword();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* SwordMeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* TraceStartPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* TraceEndPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 NumOfTrace = 2;

	FVector TraceInterval;

	UPROPERTY()
	TArray<FVector> TraceHistory;
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnTraceHit(const TArray<FHitResult>& HitResults);

public:
	FVector GetVelocity() const override;

	FVector ActorPreLocation;
	FVector CurrentVelocity;
};

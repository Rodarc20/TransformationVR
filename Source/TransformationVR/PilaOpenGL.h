// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PilaOpenGL.generated.h"

UCLASS()
class TRANSFORMATIONVR_API APilaOpenGL : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APilaOpenGL();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};

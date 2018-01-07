// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Parte.h"
#include "Components/SphereComponent.h"
#include "PieDerecho.generated.h"

/**
 * 
 */
UCLASS()
class TRANSFORMATIONVR_API APieDerecho : public AParte
{
	GENERATED_BODY()
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	APieDerecho();

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Transformation")
    UStaticMeshComponent * PieD;
	
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Transformation")
    UStaticMeshComponent * ArticulacionTobilloD;
	
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Transformation")
	USphereComponent * ColisionTobilloD;
	
	
	
};

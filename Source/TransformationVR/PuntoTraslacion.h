// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PuntoTraslacion.generated.h"

UCLASS()
class TRANSFORMATIONVR_API APuntoTraslacion : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APuntoTraslacion();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

    //por ahor solo sera un puntito, de pronto animado, haciendo se grande y pequeño
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Transformation")
    UStaticMeshComponent * Globo;
	
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Transformation")
    UStaticMeshComponent * Guia;

	
	
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/CapsuleComponent.h"
#include "Parte.generated.h"

UCLASS()
class TRANSFORMATIONVR_API AParte : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AParte();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//colision que envuelve la parte, esta deberia ser la ra�z por ahora no lo es
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Transformation")
	UCapsuleComponent * Colision;

	//Puntero al padre en una jerarqu�a generada expresamente para el objeto
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transformation")
	AParte * Padre;

	//Puntero al padre en una jerarqu�a generada expresamente para el objeto
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transformation")
	TArray<AParte *> Hijos;
	
	
};


//creo que sera mejor poner como centro un scene component, y al com hijos a colision los static mes etc, de esa forma no hay escalas
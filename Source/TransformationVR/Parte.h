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

	//colision que envuelve la parte, esta deberia ser la raíz por ahora no lo es
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Transformation")
	UCapsuleComponent * Colision;

	//Puntero al padre en una jerarquía generada expresamente para el objeto
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transformation")
	AParte * Padre;

	//Puntero al padre en una jerarquía generada expresamente para el objeto
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transformation")
	TArray<AParte *> Hijos;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transformation")
	bool bArticulacionSobrepuesta;//este se vuelve verdadero si cualquiera de las articulaciones se sobrepone, tener cuidado si se sobreponen varias por casualidad
	//este booleano se usa cuando la parte esta sujetada por el control derecho, la cua busca una articulacion apra unir en el muñeco
	
	
};


//creo que sera mejor poner como centro un scene component, y al com hijos a colision los static mes etc, de esa forma no hay escalas
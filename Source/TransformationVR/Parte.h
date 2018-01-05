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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transformation")
	AParte * OverlapedParte;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transformation")
	bool bConectado;//verdadeor si la parte esta conectada al mu�eco, se usa para saber si se esta sujetando una parte o al mu�eco.

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transformation")
	bool bBuscarArticulacion;//verdadeor si la parte esta conectada al mu�eco, se usa para saber si se esta sujetando una parte o al mu�eco.

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transformation")
	bool bArticulacionSobrepuesta;//este se vuelve verdadero si cualquiera de las articulaciones se sobrepone, tener cuidado si se sobreponen varias por casualidad
	//este booleano se usa cuando la parte esta sujetada por el control derecho, la cua busca una articulacion apra unir en el mu�eco
	
	UFUNCTION()
	void BuscarArticulacion();

	UFUNCTION()
	void NoBuscarArticulacion();

	UFUNCTION()
	virtual void BuscandoArticulacion();

	UFUNCTION()
	virtual void UnirConParteSobrepuesta();

    UFUNCTION()
    virtual void OnBeginOverlapArticulacion(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

    UFUNCTION()
    virtual void OnEndOverlapArticulacion(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex);

};


//creo que sera mejor poner como centro un scene component, y al com hijos a colision los static mes etc, de esa forma no hay escalas
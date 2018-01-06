// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Public/Math/Color.h"
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

	//Puntero a los hijos en una jerarquía generada expresamente para el objeto
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transformation")
	TArray<AParte *> Hijos;

	//Array de los spherecomponentes de las articulaciones
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transformation")
	TArray<USphereComponent *> ColisionesArticualciones;

	//Array de los spherecomponentes de las articulaciones
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transformation")
	TArray<UStaticMeshComponent *> MeshesArticulaciones;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transformation")
	AParte * OverlapedParte;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transformation")
	bool bConectado;//verdadeor si la parte esta conectada al muñeco, se usa para saber si se esta sujetando una parte o al muñeco.

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transformation")
	bool bBuscarArticulacion;//verdadeor si la parte esta conectada al muñeco, se usa para saber si se esta sujetando una parte o al muñeco.

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transformation")
	bool bArticulacionSobrepuesta;//este se vuelve verdadero si cualquiera de las articulaciones se sobrepone, tener cuidado si se sobreponen varias por casualidad
	//este booleano se usa cuando la parte esta sujetada por el control derecho, la cua busca una articulacion apra unir en el muñeco

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transformation")
	FLinearColor ColorArticulacionNoConectada;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transformation")
	FLinearColor ColorArticulacionSobrepuesta;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transformation")
	FLinearColor ColorArticulacionConectada;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Transformation")
	int IndiceArticulacionSobrepuesta;
	
	UFUNCTION()
	int IndiceColisionArticulacion(USphereComponent * ArticulacionSphere);

	UFUNCTION()
	void BuscarArticulacion();

	UFUNCTION()
	void NoBuscarArticulacion();

	UFUNCTION()
	virtual void BuscandoArticulacion();

	UFUNCTION()
	virtual void UnirConParteSobrepuesta();

	UFUNCTION(Category = "Transformation")
	virtual void CambiarColorArticulacion(int IndiceArticulacion, FLinearColor NuevoColor);

    UFUNCTION()
    virtual void OnBeginOverlapArticulacion(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

    UFUNCTION()
    virtual void OnEndOverlapArticulacion(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transformation", Meta = (BlueprintPublic = "true"))
    UMaterialInstanceDynamic * ArticulacionMaterialDynamic;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transformation", Meta = (BlueprintPublic = "true"))
    UMaterialInstanceDynamic * ArticulacionUnidaMaterialDynamic;
	//cada iarticulacion deberia tener su porpia isntancia dinamica, o deberia usar dos materiales y cambiar entre ellos cuando lo necesite?
};


//creo que sera mejor poner como centro un scene component, y al com hijos a colision los static mes etc, de esa forma no hay escalas
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Materials/Material.h"
#include "TransformacionWidget.generated.h"

UENUM(BlueprintType)
enum class ETransformacionEje : uint8 {
    EEjeX UMETA(DisplayName = "EjeX"),
    EEjeY UMETA(DisplayName = "EjeY"),
    EEjeZ UMETA(DisplayName = "EjeZ"),
    ENone UMETA(DisplayName = "Ninguno")
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TRANSFORMATIONVR_API UTransformacionWidget : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTransformacionWidget();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Transformation")
    UStaticMeshComponent * FlechaX;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Transformation")
    UStaticMeshComponent * FlechaY;
		
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Transformation")
    UStaticMeshComponent * FlechaZ;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Transformation")
    UStaticMeshComponent * ArcoX;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Transformation")
    UStaticMeshComponent * ArcoY;
		
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Transformation")
    UStaticMeshComponent * ArcoZ;
	
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Transformation")
    UStaticMeshComponent * Origen;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Transformation")
	UMaterial * EjeXNormal;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Transformation")
	UMaterial * EjeXSelected;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Transformation")
	UMaterial * EjeYNormal;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Transformation")
	UMaterial * EjeYSelected;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Transformation")
	UMaterial * EjeZNormal;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Transformation")
	UMaterial * EjeZSelected;
		
	UFUNCTION(BlueprintCallable, Category = "Transformaciones")
	void MostrarWidget();

	UFUNCTION(BlueprintCallable, Category = "Transdormaciones")
	void OcultarWidget();

	UFUNCTION(BlueprintCallable, Category = "Transformaciones")
	void MostrarWidgetTraslacion();

	UFUNCTION(BlueprintCallable, Category = "Transdormaciones")
	void OcultarWidgetTraslacion();

	UFUNCTION(BlueprintCallable, Category = "Transformaciones")
	void MostrarWidgetRotacion();

	UFUNCTION(BlueprintCallable, Category = "Transdormaciones")
	void OcultarWidgetRotacion();

	UFUNCTION(BlueprintCallable, Category = "Transformaciones")
	void MostrarResaltado();
	
	UFUNCTION(BlueprintCallable, Category = "Transformaciones")
	void OculatarResaltado();

	UFUNCTION(BlueprintCallable, Category = "Transformaciones")
	bool ColisionRotacion(FVector Inicio, FVector Fin, ETransformacionEje & EjeColisionado, FVector & Impact); //no se si impact lo debo devolver en espacio local o global, para calcular de forma rapida el angulo

	UFUNCTION(BlueprintCallable, Category = "Transformaciones")
	void SeleccionarEjeRotacion(ETransformacionEje Eje);

	UFUNCTION(BlueprintCallable, Category = "Transformaciones")
	void DeseleccionarEjeRotacion(ETransformacionEje Eje);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transformation")
	FTransform TransformTemporal;//este trasnfor ahora es compia del de este component, spero deberia ser copia del trasnform del actor, sobre el se debe hacer los calculos, habra porbleams ya que ambos tiene padres disitintos
	//si uso la copia del transfor del componen, cuando palique rotaciones a la parte, los calculos usaran es rotacion del padre tambien, cuanod no deberia ser asi, esto por que en el proceo le pare es la parte

	UFUNCTION(BlueprintCallable, Category = "Transformaciones")
	void CopiarTransform();

	UFUNCTION(BlueprintCallable, Category = "Transformaciones")
	FVector InterseccionLineaPlanoXZ(FVector PuntoRecta, FVector VectorRecta);
	FVector InterseccionLineaPlanoXY(FVector PuntoRecta, FVector VectorRecta);
	FVector InterseccionLineaPlanoYZ(FVector PuntoRecta, FVector VectorRecta);
};

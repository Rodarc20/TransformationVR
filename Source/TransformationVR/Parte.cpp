// Fill out your copyright notice in the Description page of Project Settings.

#include "Parte.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/Engine.h"
#include "Public/Math/Color.h"
#include "Robot.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"



// Sets default values
AParte::AParte()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Id = -1;
	ColorArticulacionNoConectada = FLinearColor(0.0f, 0.08336f, 0.234375f, 0.0f);
	ColorArticulacionSobrepuesta = FLinearColor(0.0f, 0.234375f, 0.097833f, 0.0f);
	//ColorArticulacionConectada = FLinearColor(0.0f, 0.234375, 0.212609f, 0.0f);
	ColorArticulacionConectada = FLinearColor(0.234375f, 0.0f, 0.002142f, 0.0f);

    bAnimacion = false;
	bRotar = false;
	bRotarX = false;
	bRotarY = false;
	bRotarZ = false;
	bBuscarArticulacion = true;

	VelocidadRotacion = 10.0f;
	DuracionAnimacionRotacion = 5.0f;
}

// Called when the game starts or when spawned
void AParte::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AParte::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    //AnimacionRotarTick(DeltaTime);
}
void AParte::AnimacionRotarTick(float DeltaTime) {
	//UE_LOG(LogClass, Log, TEXT("bRotar: %b"), bRotar);
    if (bAnimacion) {
        if (bRotar) {

            //UE_LOG(LogClass, Log, TEXT("bRotar: TRUE"));
            float DeltaX = signoX * VelocidadRotacion * DeltaTime;
            //float DeltaX = AngleXFin * DeltaTime;
            AngleXCurrent += DeltaX;
            if (signoX >= 0) {
                if (AngleXCurrent <= AngleXFin) {//este if cambia si el anglefine es negativo
                    FRotator DeltaRotation (0.0f, 0.0f, DeltaX);
                    AddActorLocalRotation(DeltaRotation);
                }
                else {
                    //llegue al angulo debo dejar de rotar
                    bRotarX = false;
                }
            }
            else {
                if (AngleXCurrent >= AngleXFin) {//este if cambia si el anglefine es negativo
                    FRotator DeltaRotation (0.0f, 0.0f, DeltaX);
                    AddActorLocalRotation(DeltaRotation);
                }
                else {
                    //llegue al angulo debo dejar de rotar
                    bRotarX = false;
                }
            }

            float DeltaY = signoY * VelocidadRotacion * DeltaTime;
            //float DeltaX = AngleXFin * DeltaTime;
            AngleYCurrent += DeltaY;

            if (signoY >= 0) {
                if (AngleYCurrent <= AngleYFin) {//este if cambia si el anglefine es negativo
                    FRotator DeltaRotation (DeltaY, 0.0f, 0.0f );
                    AddActorLocalRotation(DeltaRotation);
                }
                else {
                    //llegue al angulo debo dejar de rotar
                    bRotarY = false;
                }
            }
            else {
                if (AngleYCurrent >= AngleYFin) {//este if cambia si el anglefine es negativo
                    FRotator DeltaRotation (DeltaY, 0.0f, 0.0f );
                    AddActorLocalRotation(DeltaRotation);
                }
                else {
                    //llegue al angulo debo dejar de rotar
                    bRotarY = false;
                }
            }


            float DeltaZ = signoZ * VelocidadRotacion * DeltaTime;
            //float DeltaX = AngleXFin * DeltaTime;
            AngleZCurrent += DeltaZ;

            if (signoZ >= 0) {
                if (AngleZCurrent <= AngleZFin) {//este if cambia si el anglefine es negativo
                    FRotator DeltaRotation (0.0f, DeltaZ, 0.0f);
                    AddActorLocalRotation(DeltaRotation);
                }
                else {
                    //llegue al angulo debo dejar de rotar
                    bRotarZ = false;
                }
            }
            else {
                if (AngleZCurrent >= AngleZFin) {//este if cambia si el anglefine es negativo
                    FRotator DeltaRotation (0.0f, DeltaZ, 0.0f);
                    AddActorLocalRotation(DeltaRotation);
                }
                else {
                    //llegue al angulo debo dejar de rotar
                    bRotarZ = false;
                }
            }

            bRotar = bRotarX || bRotarY || bRotarZ;
        }
        else if (!bRotar && IdRotacionActual < InstruccionesRotacion.Num() ) {
            UE_LOG(LogClass, Log, TEXT("Llmando animacon rotar %d"), IdRotacionActual);
            AnimacionRotar(InstruccionesRotacion[IdRotacionActual]);
            IdRotacionActual++;
        }
        else if(IdRotacionActual == InstruccionesRotacion.Num()) {
            bAnimacion = false;
            if (bCicloAnimacion) {
                EjecutarAnimaciones();
            }
        }
    }
}

void AParte::AnimacionRotar(FVector CantidadRotacion) {// es mejor recibir los angulos como vectores, para manejra loas negativos coreectamente, ya que debo recibir el delta
	//separar por ejes, calculad pasos, y 
	//necsito 3 valores, 
	//necisto que la animaicon ocurra en 1 segudno de preferencia,
	//neceisto el valor actual
	UE_LOG(LogClass, Log, TEXT("Ejecutando Animacion %s"), *NombreParte);

	//VelocidadRotacion = 45.0f;
	//VelocidadRotacion = 15.0f;

	bRotar = true;

	bRotarX = true;
	AngleXIni = 0;
	AngleXCurrent = AngleXIni;
	AngleXFin = -CantidadRotacion.X;
	//este signo x sale del valor angle fin
	if (AngleXFin >= 0.0f)
		signoX = 1; //o -1
	else
		signoX = -1;

	bRotarY = true;
	AngleYIni = 0;
	AngleYCurrent = AngleYIni;
	AngleYFin = -CantidadRotacion.Y;
	//este signo x sale del valor angle fin
	if (AngleYFin >= 0.0f)
		signoY = 1; //o -1
	else
		signoY = -1;

	bRotarZ = true;
	AngleZIni = 0;
	AngleZCurrent = AngleZIni;
	AngleZFin = CantidadRotacion.Z;
	//este signo x sale del valor angle fin
	if (AngleZFin >= 0.0f)
		signoZ = 1; //o -1
	else
		signoZ = -1;

    //necesito llamar a esta funcion varias veces, una por cada rotaicon en el arreglo, y el tiepo de ejecucion debe ser de tal forma que todas se ejecuten en un segundo o dos
    // necesito recorrer e array y ejecutar una animacion hasta que funcione, es decir cuanod llame a ejecutar animaciones
}

void AParte::EjecutarAnimaciones() {
    //establecer el tiempo para cada animacion, poner un boleano de animarconjunto
    //entonces mientras este bool sea verdadero debo iterar en el arreglo, cada iteracion se determinar cuando se deteien una animaciones,
    //por ejemplo
    //cambia el bool a verdadero
	UE_LOG(LogClass, Log, TEXT("Ejecutando Animaciones %s"), *NombreParte);
    bAnimacion = true;
    IdRotacionActual = 0;
    CalcularVelociadaRotacion();
    //VelocidadRotacion =, este calculo es complicado, tendira que sumar la cantidad de rotacion que hay en todo y dividirlo en 1 segundo, y esa sera la velocidad
    //por ahora dejar asi

    //pongo el iterador en 0
    //si bRotar = false y el iterador no esta en el tamaño de arreglo de rotacones, entonces llamo a ageregar rotacion
    //incremento el iterador en 1
    //entonces mientrar brotar sea verdadero esta ejecutando la animacion
}

void AParte::IniciarCicloAnimaciones() {
    bCicloAnimacion = true;
    EjecutarAnimaciones();
}

void AParte::DetenerCicloAnimaciones() {
    bCicloAnimacion = false;
}

bool AParte::RotacionesConfirmadas() {
    bool res = true;
    for (int i = 0; i < InstruccionesRotacionConfirmado.Num(); i++) {
        res = res & InstruccionesRotacionConfirmado[i];
    }
    return res;
}

void AParte::CalcularVelociadaRotacion() {
    float acum = 0.0f;
    for (int i = 0; i < InstruccionesRotacion.Num(); i++) {
        if (InstruccionesRotacion[i].X != 0.0f) {
            acum += FMath::Abs(InstruccionesRotacion[i].X);
        }
        else if (InstruccionesRotacion[i].Y != 0.0f) {
            acum += FMath::Abs(InstruccionesRotacion[i].Y);
        }
        else if (InstruccionesRotacion[i].Z != 0.0f) {
            acum += FMath::Abs(InstruccionesRotacion[i].Z);
        }
    }
    VelocidadRotacion = acum / DuracionAnimacionRotacion;//todo debe ejecutarse en 2 segundos
}

int AParte::IndiceColisionArticulacion(USphereComponent * ArticulacionSphere) {
	int Index;
	if (ColisionesArticualciones.Find(ArticulacionSphere, Index)) {
		return Index;
	}
	return -1;
}

void AParte::BuscarArticulacion() {
	bBuscarArticulacion = true;
}

void AParte::NoBuscarArticulacion() {
	bBuscarArticulacion = false;
}

void AParte::BuscandoArticulacion() {
	//creo que esta funcion esta demas, ya que la busqueda estara controlada por la funcion everlaping, de cada articulacion, que reaccionara siempre solo si el bool de buscar articulacion es verdadero
	//si al final no llego a usar esta funcion, la debo eliminar
}

void AParte::UnirConParteSobrepuesta() {//esta union deberia estar en la calse vrpawn o en otro lado, como la jerarquia, el punto es que si necesito esta union fisica de partes, dado que utilizo las partes, 
	//debo pensar com otratar esto junto con las trasnformaciones de forma coherente
	//quiza las trasnformaciones deberian utilizar l ainformacion aqui contenida, 
	if (bArticulacionSobrepuesta) {//si hay una articulacion sobrepuesta entoces si o si habra un overlaperparte, tener en cuenta que esta funcion deberia manerja los caso de articulaciones con varias partes por ahora dejar sencillo
		//solo es verdadero si estoy superopoiniendo a una articulacion que correponda
		//el ide de la articulaion me puede decir si es uira como hijo o como padre, no necesito mas
		Padre = OverlapedParte;
		OverlapedParte->Hijos.Add(this);
		//aqui deberia haber mas partes de color
		bConectado = true;
		if (IndiceArticulacionSobrepuesta != -1 && IndiceArticulacionSobrepuestaOtro != -1) {

			ColorConectadoArticulacion(IndiceArticulacionSobrepuesta);
			//OverlapedParte->ColorConectadoArticulacion(IndiceArticulacionSobrepuestaOtro);
			OverlapedParte->MeshesArticulaciones[IndiceArticulacionSobrepuestaOtro]->SetVisibility(false);

			//ColorResaltadoArticulacion(IndiceArticulacionSobrepuesta);
			//OverlapedParte->ColorResaltadoArticulacion(IndiceArticulacionSobrepuestaOtro);

			//ColorNormalArticulacion(IndiceArticulacionSobrepuesta);
			//OverlapedParte->ColorNormalArticulacion(IndiceArticulacionSobrepuestaOtro);

			//CambiarColorArticulacion(IndiceArticulacionSobrepuesta, ColorArticulacionConectada);
			//OverlapedParte->CambiarColorArticulacion(IndiceArticulacionSobrepuestaOtro, ColorArticulacionConectada);
			FVector OffsetWorld = GetActorLocation() - ColisionesArticualciones[IndiceArticulacionSobrepuesta]->GetComponentLocation();
			SetActorLocation(OverlapedParte->ColisionesArticualciones[IndiceArticulacionSobrepuestaOtro]->GetComponentLocation() + OffsetWorld);
			//la actualizacion del trasnform de la parte se hace fuera de esta funcon, ya que aqui no tengo acceso a la jerarquia

			ColisionesArticualciones[IndiceArticulacionSobrepuesta]->OnComponentBeginOverlap.RemoveDynamic(this, &AParte::OnBeginOverlapArticulacion);
			OverlapedParte->ColisionesArticualciones[IndiceArticulacionSobrepuestaOtro]->OnComponentBeginOverlap.RemoveDynamic(this, &AParte::OnBeginOverlapArticulacion);

			ColisionesArticualciones[IndiceArticulacionSobrepuesta]->OnComponentEndOverlap.RemoveDynamic(this, &AParte::OnEndOverlapArticulacion);
			OverlapedParte->ColisionesArticualciones[IndiceArticulacionSobrepuestaOtro]->OnComponentEndOverlap.RemoveDynamic(this, &AParte::OnEndOverlapArticulacion);
			bArticulacionSobrepuesta = false;




			TArray<AActor *> RobotsEncontrados;
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), ARobot::StaticClass(), RobotsEncontrados);
			//UE_LOG(LogClass, Log, TEXT("Numero de Partes Encontradas: %d"), PartesEncontradas.Num());
			for (int i = 0; i < RobotsEncontrados.Num(); i++) {
				//if(GEngine)//no hacer esta verificación provocaba error al iniciar el editor
					//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, TEXT("PARTES ENCONTRADAS"));
				//UE_LOG(LogClass, Log, TEXT("Partes Encontradas %d"), i);
				ARobot * const RobotEncontrado = Cast<ARobot>(RobotsEncontrados[i]);
				if (RobotEncontrado) {
					if (Id > OverlapedParte->Id) {
						RobotEncontrado->UnirJerarquiaPadreHijo(OverlapedParte->Id, Id);//depende de cual sea el mayor de las partes
						//no se si debo pasare esos ids, o los ide raiz, para que haga la union aporpiada, o que mejor lo haga denotr de la funcion de la clase robot
					}
					else {
						RobotEncontrado->UnirJerarquiaPadreHijo(Id, OverlapedParte->Id);//depende de cual sea el mayor de las partes
					}
				}
			}

		}
		//tambien se deben inhabilitar los de alguna forma estas articulaciones, dejarlo asi por ahora

		//aqui debo actualizar las matrices de la transformaicon con los nuevos valores
		//para que se le pueda plicar los cambios en la jerarquia
		//Debo deshabilitar las articulaciones que acabo de unir, para que no sean detectadas
	}
}

void AParte::CambiarColorArticulacion(int IndiceArticulacion, FLinearColor NuevoColor) {
	if (IndiceArticulacion >= 0 && IndiceArticulacion < ArticulacionesMaterialDynamic.Num()) {
		ArticulacionesMaterialDynamic[IndiceArticulacion]->SetVectorParameterValue(TEXT("Color"), NuevoColor);
	}
	//usar 3 colores, azul para las articulaciones no unidas, turquesa para cuando se este buscando y haya sobreposicion, y verde para cuando la articulacion este unida
}

void AParte::OnBeginOverlapArticulacion(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) {
	//solo si los nombres de las articulaciones son iguales, si no no se deben unir
	if (OverlappedComponent->GetName() == OtherComp->GetName()) {
		AParte * const ParteEncontrada = Cast<AParte>(OtherActor);
		if (ParteEncontrada) {
			if (Id > ParteEncontrada->Id) {
				//debo deshabilitar el del mayor, es decir si la parte es mayor dehabilito su colicion, la de la parte menor, es la colision que tambien se dehabilita, pero su mesh si se amtniene
				USphereComponent * const ArticulacionEncontrada = Cast<USphereComponent>(OtherComp);//en realidad deberia manejar canles para que esto solo se active con los spherecomponents ocnfigurados como articulacion
				if (ArticulacionEncontrada) {
					//debo cambiar el color de esta articulacion y de la otra, parte para ello debo darle, el componente encontrado, y que lo busque en su array de sphere components, el indice me dira cual es su mesh corresondiente para cambiarlo de color
					bArticulacionSobrepuesta = true;
					OverlapedParte = ParteEncontrada;

					if(GEngine)//no hacer esta verificación provocaba error al iniciar el editor
						GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("Articulacion Sobrepuesta"));
					//asi mismo debo saber cual de todas mis articulaciones es la que esta en contacto, para ello esta overlappedcomponent, que debo guardarlo despues de castear para luego buscar y saber su inidice para guardar el puntero de la parte padre, e inhabilitar en ese componente el contacto
					//debo deshabilitar para que en una nueva busqueda de alguna otra parte, no tome en cuenta las rticualciones unidas
					IndiceArticulacionSobrepuesta = IndiceColisionArticulacion(Cast<USphereComponent>(OverlappedComponent));// que busque mejor la parte en lugar de yo hacerlo aqui, por un lado reduzco busquedas si lo hago antes
					if (IndiceArticulacionSobrepuesta != -1)
						ColorResaltadoArticulacion(IndiceArticulacionSobrepuesta);
						//CambiarColorArticulacion(IndiceArticulacionSobrepuesta, ColorArticulacionSobrepuesta);
					IndiceArticulacionSobrepuestaOtro = ParteEncontrada->IndiceColisionArticulacion(ArticulacionEncontrada);
					if(IndiceArticulacionSobrepuestaOtro != -1)
						ParteEncontrada->ColorResaltadoArticulacion(IndiceArticulacionSobrepuestaOtro);
						//ParteEncontrada->CambiarColorArticulacion(IndiceArticulacionSobrepuestaOtro, ColorArticulacionSobrepuesta);
				}
			}
		}
	}
}

void AParte::OnEndOverlapArticulacion(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex) {
	//if (bBuscarArticulacion) {
	if (OverlappedComponent->GetName() == OtherComp->GetName()) {
		AParte * const ParteEncontrada = Cast<AParte>(OtherActor);
		if (ParteEncontrada) {
			USphereComponent * const ArticulacionEncontrada = Cast<USphereComponent>(OtherComp);//en realidad deberia manejar canles para que esto solo se active con los spherecomponents ocnfigurados como articulacion
			if (ArticulacionEncontrada) {
				bArticulacionSobrepuesta = false;
				OverlapedParte = nullptr;
				if (GEngine)//no hacer esta verificación provocaba error al iniciar el editor
					GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("Saliendo Articulacion Sobrepuesta"));
				IndiceArticulacionSobrepuesta = IndiceColisionArticulacion(Cast<USphereComponent>(OverlappedComponent));// que busque mejor la parte en lugar de yo hacerlo aqui, por un lado reduzco busquedas si lo hago antes
				if(IndiceArticulacionSobrepuesta != -1)
					ColorNormalArticulacion(IndiceArticulacionSobrepuesta);
					//CambiarColorArticulacion(IndiceArticulacionSobrepuesta, ColorArticulacionNoConectada);
				IndiceArticulacionSobrepuestaOtro = ParteEncontrada->IndiceColisionArticulacion(ArticulacionEncontrada);
				if(IndiceArticulacionSobrepuestaOtro != -1)
					ParteEncontrada->ColorNormalArticulacion(IndiceArticulacionSobrepuestaOtro);
					//ParteEncontrada->CambiarColorArticulacion(IndiceArticulacionSobrepuestaOtro, ColorArticulacionNoConectada);
			}
		}
		//tambien aplicar los cambios de color adecuados, por ahora solo dejare mensajes en consola
	}
}

void AParte::ActivarResaltado() {
	ParteMesh->SetCustomDepthStencilValue(255);
	bResaltada = true;
}

void AParte::DesactivarResaltado() {
	ParteMesh->SetCustomDepthStencilValue(0);
	bResaltada = false;
}

void AParte::ColorResaltadoArticulacion(int IdArticulacion) {
	CambiarColorArticulacion(IdArticulacion, UKismetMathLibrary::HSVToRGB(HueArticulaciones[IdArticulacion], 1.0f, 2.0f, 1.0f));
}

void AParte::ColorNormalArticulacion(int IdArticulacion) {
	CambiarColorArticulacion(IdArticulacion, UKismetMathLibrary::HSVToRGB(HueArticulaciones[IdArticulacion], 1.0f, 1.0f, 1.0f));
}

void AParte::ColorConectadoArticulacion(int IdArticulacion) {
	CambiarColorArticulacion(IdArticulacion, UKismetMathLibrary::HSVToRGB(HueArticulaciones[IdArticulacion], 0.0f, 1.0f, 1.0f));
	//CambiarColorArticulacion(IdArticulacion, UKismetMathLibrary::HSVToRGB(218.0f, 1.0f, 1.0f, 1.0f));
	//CambiarColorArticulacion(IdArticulacion, UKismetMathLibrary::HSVToRGB(258.0f, 1.0f, 1.0f, 1.0f));
}

void AParte::CopiarTransform() {
	TWidget->TransformTemporal = GetActorTransform();
}

//creo que las articulaciones deberian ser un nuevo componente llamado articulacion, el cual tendria un pshere component y un statickmesh
//asi cuando este reaacione en el overlap, y deba cambiar el color por ejemplo, solo debo llamar a una funcion de este componente, lo mismo cuando usceda el endoverlap


//debereia tener un arreglo de colores, al igual que tengo el arreglo de articulacion, para que cada articulacion tenga un colore que le corresponda.
//debo tener 3 funciones
//ResaltarArticulacion(int IdArticulacion)
//esta funcion tomara el color de articulacion del arreglo dde colores sgund el indice, sumara algun valor al que lo hara mas birllante, y eso lo asignara al material


//Necesito tener las rotaciones iniciales, una vez que se armo el robot
//luego cuando agrego rotaciones se agregan al array, y la poarte se queda en la ultima rotacion, entonces cuand pase a la siguiente tarea debo empezar en la posicion incial
//y luego ejecutar las rotacines del array
//por ultimo si elimino alguna rotacino pues debo recalcular la posicon actual, aplicando las rotaciones que quedan desde la posicion inicial

void AParte::AgregarRotacion(float angle, ETransformacionEje EjeRotacion) {
    switch (EjeRotacion) {
        case ETransformacionEje::EEjeX: {
            InstruccionesRotacion.Add(FVector(angle, 0.0f, 0.0f));
            InstruccionesRotacionConfirmado.Add(false);
            InstruccionesRotacionValido.Add(true);
			//EstablecerRotacionEjeX(IdParte, angle);
        }
        break;//no se como funciona esto
        case ETransformacionEje::EEjeY: {
            InstruccionesRotacion.Add(FVector(0.0f, angle, 0.0f));
            InstruccionesRotacionConfirmado.Add(false);
            InstruccionesRotacionValido.Add(true);
			//EstablecerRotacionEjeY(IdParte, angle);
        }
        break;//no se como funciona esto
        case ETransformacionEje::EEjeZ: {
            InstruccionesRotacion.Add(FVector(0.0f, 0.0f, angle));
            InstruccionesRotacionConfirmado.Add(false);
            InstruccionesRotacionValido.Add(true);
			//EstablecerRotacionEjeZ(IdParte, angle);
        }
        break;//no se como funciona esto
        default:
        case ETransformacionEje::ENone: {//unknown/ default state
            //no hacer nada
        }
        break;//no se como funciona esto
    }
	UE_LOG(LogClass, Log, TEXT("Agreganda rotacion"));
	//Nodos[IdParte]->ActualizarTextRotacion();
	
}

void AParte::ConfirmarRotacion(int IdRotacion) {
    if (IdRotacion >= 0 && IdRotacion < InstruccionesRotacionConfirmado.Num()) {
        InstruccionesRotacionConfirmado[IdRotacion] = true;
    }
}

void AParte::CancelarRotacion(int IdRotacion) {
    //al eliminar del array los otros id que habia en la interfaz se tendrian que cambiar, quiza deberia tener otro array que contenga la informacion sobre quien es uno elimando y quien no
    if (IdRotacion >= 0 && IdRotacion < InstruccionesRotacionValido.Num()) {
        InstruccionesRotacionValido[IdRotacion] = false;
    }

}

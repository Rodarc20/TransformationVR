// Fill out your copyright notice in the Description page of Project Settings.

#include "Jerarquia.h"
#include "Transformacion.h"
#include "Engine/Engine.h"
#include "Nodo.h"
#include "PilaOpenGL.h"
#include "VRPawn.h"
#include "Parte.h"
#include "Robot.h"
#include "MotionControllerComponent.h"
#include "Public/UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include <stack>

// Sets default values
AJerarquia::AJerarquia()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	Root = nullptr;
	TransformacionesPartesPunteros.SetNum(10);
	Nodos.SetNum(10);

    static ConstructorHelpers::FClassFinder<ANodo> NodoClass(TEXT("BlueprintGeneratedClass'/Game/Trasnformation/Blueprints/Jerarquia/Nodo_BP.Nodo_BP_C'"));
    if (NodoClass.Succeeded()) {
        if(GEngine)//no hacer esta verificación provocaba error al iniciar el editor
            GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("TipoNodo encontrado."));
        TipoNodo = NodoClass.Class;
    }

	AnchoNodos = 25.0f;
	AltoNodos = 12.5f;
	//DeltaNiveles = 37.5f;
	DeltaNiveles = 30.0f;
	DeltaHermanos = 35.0f;

	DistanciaLaserMaxima = 300.0f;

	CantidadPartes = 0;
}

// Called when the game starts or when spawned
void AJerarquia::BeginPlay()
{
	Super::BeginPlay();

    /*AVRPawn * MyVRPawn = Cast<AVRPawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
    if (MyVRPawn) {
        RightController = MyVRPawn->MotionControllerRight;
        LeftController = MyVRPawn->MotionControllerLeft;
        //MyVRPawn->Visualization = this;
        //Interaction = MyVRPawn->Interaction;
        Usuario = MyVRPawn;
    }*/
	
}

// Called every frame
void AJerarquia::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    EjecutarAnimacionTick(DeltaTime);

}

void AJerarquia::UnirPadreHijo(int IdPadre, int IdHijo) {
	//TransformacionesPartes[IdHijo].Padre = &(TransformacionesPartes[IdPadre]);
	//TransformacionesPartes[IdPadre].Hijos.Add(&(TransformacionesPartes[IdHijo]));
	//se supone que el hijo es el nuevo nodo a crear, y el padre ya esta creado


	//TransformacionesPartes[IdHijo].ParteAsociada->AttachToActor(TransformacionesPartes[IdPadre].ParteAsociada, FAttachmentTransformRules::KeepWorldTransform);
	//UE_LOG(LogClass, Log, TEXT("Matrices actualizadas despues de unir hijo"));
	//ImprimirMatrices(&TransformacionesPartes[IdHijo]);
	//CrearNodo(TransformacionesPartes[IdHijo].ParteAsociada);
	ActualizarNodos();
}

void AJerarquia::Actualizar() {
	//con una busqueda en profundidad, actualizar la jerarquia, multipllicar los frames
	//y establecer nuevas posiciones
	//la pregunta es si estoy haciendo un calculo de puntos de world a local o de local a world, me parece que es la primera opcion
	//asi mismo esta funcion debe actualizar las posciones de los actores en funion de los nuevos calculos
	//POR AHORA calculare la diferencia entre la posicion de la matriz y la posicino del actor, y esa diferencia la aplicare a toda la jerarquia
	//ya que en cada tick solo aplico traslacion al root y luego actualiza, que es esl que se encarga de ajustar al mungo global,
	//por ahora esta funcionalidad esta mal, ya que se deberia aplicar los mismo paso para todos lo selementos incluso la raiz
	std::stack<Transformacion *> pila;
	Root->ActualizarParte();
	for (int i = 0; i < Root->Hijos.Num(); i++) {//debiria recorrer de forma inversa?
		pila.push(Root->Hijos[i]);
	}
	while (!pila.empty()) {
		Transformacion * T = pila.top();
		pila.pop();
		T->Trasladar(TraslacionTemporal);
		T->ActualizarParte();
		for (int i = 0; i < T->Hijos.Num(); i++) {//debiria recorrer de forma inversa?
			pila.push(T->Hijos[i]);
		}
	}
}

void AJerarquia::ActualizarWorlds() {//se supone que alguna matriz la que se se ha visto modificada de forma externa, es decir quiza rote alguna parte, por lo tanto su matriz global cambio
	//en ese mismo instante debo calcular su matriz local, una vez hecho esto debo llamar a esta funcion, que actualizara todo el arbol, aplicando la nueva traslacion de esas matrices donde corresponde, y tambien actualizando las posicones de las partes
	std::stack<Transformacion *> pila;
	pila.push(Root);
	ImprimirMatrices(Root);
	while (!pila.empty()) {
		Transformacion * T = pila.top();
		pila.pop();
		//T->CalcularHWorld();//esto calcula el HWrold()en funcion de la jerarquia
		T->CalcularHWDesdeH();
		T->ActualizarParte();
		for (int i = 0; i < T->Hijos.Num(); i++) {//debiria recorrer de forma inversa?
			pila.push(T->Hijos[i]);
			ImprimirMatrices(T->Hijos[i]);
		}
	}
	ActualizarNodos();
}

void AJerarquia::CrearNodo(AParte * ParteAsociada) {
        UWorld * const World = GetWorld();
		if (World) {
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = Instigator;

			FVector SpawnLocation(ParteAsociada->Id * 5);
			
			SpawnLocation = GetTransform().TransformPosition(SpawnLocation);
			SpawnLocation += FVector(-300.0f, 0.0f, 200.0f);
			ANodo * const NodoInstanciado = World->SpawnActor<ANodo>(TipoNodo, SpawnLocation, FRotator::ZeroRotator, SpawnParams);
			Nodos[ParteAsociada->Id] = NodoInstanciado;
			NodoInstanciado->IdParte = ParteAsociada->Id;//para el texto del numero, quiza este tipo de funcionalidad deberia estar encapsulada en alguna funcion de la clase nodo
			NodoInstanciado->CambiarNombreParte(ParteAsociada->Id);
			//NodoInstanciado->bActualizado = false;
            NodoInstanciado->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);//segun el compilador de unral debo usar esto
		}

}

void AJerarquia::ImprimirMatriz(FMatrix m) {
    for (int i = 0; i < 4; i++) {
        UE_LOG(LogClass, Log, TEXT("[%.4f,%.4f,%.4f,%.4f]"), m.M[i][0], m.M[i][1], m.M[i][2], m.M[i][3]);
    }
}

void AJerarquia::ImprimirTransformacion(Transformacion * T) {
	UE_LOG(LogClass, Log, TEXT("Trasformacion Id (%d), ParteRaiz (%d)"), T->ParteAsociada->Id, T->ParteAsociada->IdParteRaiz);
	if (T->Padre) {
		UE_LOG(LogClass, Log, TEXT("Padre Id (%d)"), T->Padre->ParteAsociada->Id);
	}
	else {
		UE_LOG(LogClass, Log, TEXT("Padre Id (%d)"), -1);
	}
	for (int i = 0; i < T->Hijos.Num(); i++) {
		UE_LOG(LogClass, Log, TEXT("Hijo (%d)"), T->Hijos[i]->ParteAsociada->Id);
	}
}

void AJerarquia::Imprimir() {
    std::stack<Transformacion *> pila;
    //la raiz es el ultimo nodo
	if (Root) {
		UE_LOG(LogClass, Log, TEXT("Jerarquia (%d)"), Root->ParteAsociada->Id);
	}
	else {
		UE_LOG(LogClass, Log, TEXT("Jerarquia (%d)"), -1);
	}
	pila.push(Root);
    while (!pila.empty()) {
        Transformacion * V = pila.top();
        pila.pop();
		//ejecutar animacion
		if (V) {
			ImprimirTransformacion(V);
			if (V->Hijos.Num()) {
				for (int i = V->Hijos.Num()-1; i >= 0; i--) {
					pila.push(V->Hijos[i]);
				}
			}
		}
    }
}

void AJerarquia::ImprimirMatrices(Transformacion * T) {
	UE_LOG(LogClass, Log, TEXT("%s Local"), *T->ParteAsociada->NombreParte);
	ImprimirMatriz(T->H);
	UE_LOG(LogClass, Log, TEXT("%s World"), *T->ParteAsociada->NombreParte);
	ImprimirMatriz(T->HW);
}

void AJerarquia::ActualizarNodos() {
	for (int i = 0; i < Nodos.Num(); i++) {
		if (Nodos[i]) {//si el nodo existe es por que la parte del cuerpo existe
			//Nodos[i]->CambiarTraslacion(TransformacionesPartes[i].GetLocation());
			//Nodos[i]->CambiarRotacion(TransformacionesPartes[i].GetRotation());
			//Nodos[i]->CambiarTraslacion(TransformacionesPartes[i].GetWorldLocation());
			//Nodos[i]->CambiarTraslacion(TransformacionesPartes[i].GetWorldLocation());
			Nodos[i]->ActualizarTextRotacion();
			Nodos[i]->ActualizarTextTraslacion();
		}
	}
}

void AJerarquia::Calculos(Transformacion * V) {//lo uso dentro de claculos 2, por alguna razon
    //calcular hojas, altura,
    if (V->Hijos.Num() == 0) {//deberia usar si es virtual o no
        V->Hojas = 1;
        V->Altura = 0;
    }
    else {
        V->Hojas = 0;
        V->Altura = 0;
        for (int i = 0; i < V->Hijos.Num(); i++) {
            Calculos(V->Hijos[i]);
            V->Hojas += V->Hijos[i]->Hojas;
            V->Altura = FMath::Max<float>(V->Altura, V->Hijos[i]->Altura);
        }
        V->Altura++;
    }
    //si el arbol tiene 4 niveles, el valor de altura de la raiz es 3
}

void AJerarquia::Calculos2() {//calcula hojas y altura, de otra forma
    //Transformacion * Root = &TransformacionesPartes[TransformacionesPartes.Num() - 1];

    //Calculos(Root->Padre);
	Root->Altura = 0;
	Root->Hojas = 0;
    for (int i = 0; i < Root->Hijos.Num(); i++) {
        Calculos(Root->Hijos[i]);
        Root->Hojas += Root->Hijos[i]->Hojas;
        Root->Altura = FMath::Max<float>(Root->Altura, Root->Hijos[i]->Altura);
    }
    Root->Altura++;
}

void AJerarquia::Calc() {//para hallar niveles
    std::stack<Transformacion *> pila;
    //la raiz es el ultimo nodo
    //Transformacion * Root = &TransformacionesPartes[Nodos.Num() - 1];
	//ya tengo un root
    Root->Nivel = 0;
    //pila.push(Root);//no deberia dsencolarlo
	pila.push(Root);
    while (!pila.empty()) {
        Transformacion * V = pila.top();
        pila.pop();
        if (V->Hijos.Num()) {
            for (int i = V->Hijos.Num()-1; i >= 0; i--) {
                V->Hijos[i]->Nivel = V->Nivel + 1;
                pila.push(V->Hijos[i]);
            }
        }
    }
}

void AJerarquia::Layout() {//en este algoritmo puedo asignar el nivel
	UE_LOG(LogClass, Log, TEXT("Layout!!"));
    Imprimir();
    TQueue<Transformacion * > Cola;
    //la raiz es el ultimo nodo
    //Transformacion * Root = &TransformacionesPartes[Trasn.Num() - 1];
    //Calculos2();
	Calculos(Root);
    Calc();//no estaba antes
    
    Root->WTam = Root->Hojas * DeltaHermanos;
    Root->WInicio = 0;
	Root->PosicionNodo = FVector(0.0f, Root->WTam/2, Root->Nivel * DeltaNiveles);//no habra variaconies en x, y todo sera en espacio local de la jerarquia, para poder ubicarla donde sea visible y rotando hacia el susuario
    //float DeltaPhi = PI / Root->Altura;
	float WTemp = 0;
    //debo tener en cuenta al padre para hacer los calculos, ya que esto esta como arbol sin raiz
	Cola.Enqueue(Root);
    while (!Cola.IsEmpty()) {
        Transformacion * V;
        Cola.Dequeue(V);
        WTemp = V->WInicio;
        UE_LOG(LogClass, Log, TEXT("Nodo (%d) Nivel = %d: (%.4f, %.4f, %.4f)"), V->ParteAsociada->Id, V->Nivel, V->PosicionNodo.X, V->PosicionNodo.Y, V->PosicionNodo.Z);
        for (int i = 0; i < V->Hijos.Num(); i++) {
            V->Hijos[i]->WTam = V->WTam * (float(V->Hijos[i]->Hojas) / V->Hojas);
            V->Hijos[i]->WInicio = WTemp;
			V->Hijos[i]->PosicionNodo = FVector(0.0f, V->Hijos[i]->WInicio + V->Hijos[i]->WTam/2, V->Hijos[i]->Nivel * DeltaNiveles * -1);//no habra variaconies en x, y todo sera en espacio local de la jerarquia, para poder ubicarla donde sea visible y rotando hacia el susuario
            WTemp += V->Hijos[i]->WTam;
            Cola.Enqueue(V->Hijos[i]);
        }
    }
	UE_LOG(LogClass, Log, TEXT("Fin layout."));
}

FString AJerarquia::Texto(Transformacion * T) {
	FString res;
    if (T) {
        if (T->Padre) {
            NumeroIdentaciones++;
            res = Identacion(NumeroIdentaciones) + "glPushMatrix();\n";
            FVector Posicion = T->GetLocation();
            res += Identacion(NumeroIdentaciones) + "glTranslate(" + FString::SanitizeFloat(Posicion.X) + ", " + FString::SanitizeFloat(Posicion.Y) + ", " + FString::SanitizeFloat(Posicion.Z) + ");\n";
            FRotator Rotacion = T->GetRotation();
            res += Identacion(NumeroIdentaciones) + "glRotate(" + FString::SanitizeFloat(Rotacion.Roll) + ", " + FString::SanitizeFloat(Rotacion.Pitch) + ", " + FString::SanitizeFloat(Rotacion.Yaw) + ");\n";
            res += Identacion(NumeroIdentaciones) + T->ParteAsociada->NombreParte + "();\n";
            for (int i = 0; i < T->Hijos.Num(); i++) {
                res += Texto(T->Hijos[i]);
            }
            res += Identacion(NumeroIdentaciones) + "glPopMatrix();\n";
            NumeroIdentaciones--;
        }
        else {
            NumeroIdentaciones = 0;//la raiz
            res += Identacion(NumeroIdentaciones) + T->ParteAsociada->NombreParte + "();\n";
            for (int i = 0; i < T->Hijos.Num(); i++) {
                res += Texto(T->Hijos[i]);
            }
        }
    }
	return res;
}

FString AJerarquia::TextoRotaciones(Transformacion * T) {
	FString res;
    if (T) {
        if (T->Padre) {
            NumeroIdentaciones++;
            res = Identacion(NumeroIdentaciones) + "glPushMatrix();\n";
            //FVector Posicion = T->GetLocation();
            //res += Identacion(NumeroIdentaciones) + "glTranslate(" + FString::SanitizeFloat(Posicion.X) + ", " + FString::SanitizeFloat(Posicion.Y) + ", " + FString::SanitizeFloat(Posicion.Z) + ");\n";
            for (int i = 0; i < T->ParteAsociada->InstruccionesRotacion.Num(); i++) {
                FVector Rotacion = T->ParteAsociada->InstruccionesRotacion[i];
                //FRotator Rotacion = T->GetRotation();
                res += Identacion(NumeroIdentaciones) + "glRotate(" + FString::SanitizeFloat(Rotacion.X) + ", " + FString::SanitizeFloat(Rotacion.Y) + ", " + FString::SanitizeFloat(Rotacion.Z) + ");\n";
            }
            res += Identacion(NumeroIdentaciones) + T->ParteAsociada->NombreParte + "();\n";
            for (int i = 0; i < T->Hijos.Num(); i++) {
                res += TextoRotaciones(T->Hijos[i]);
            }
            res += Identacion(NumeroIdentaciones) + "glPopMatrix();\n";
            NumeroIdentaciones--;
        }
        else {
            NumeroIdentaciones = 0;//la raiz
            //como soy la raiz debo poner mi translate
            res = Identacion(NumeroIdentaciones) + "glPushMatrix();\n";
            TArray<AActor *> RobotsEncontrados;
            UGameplayStatics::GetAllActorsOfClass(GetWorld(), ARobot::StaticClass(), RobotsEncontrados);
            //UE_LOG(LogClass, Log, TEXT("Numero de Partes Encontradas: %d"), PartesEncontradas.Num());
            if (RobotsEncontrados.Num()) {
                ARobot * const RobotEncontrado = Cast<ARobot>(RobotsEncontrados[0]);
                if (RobotEncontrado) {
                    for (int i = 1; i < RobotEncontrado->PuntosTraslacion.Num(); i++) {
                        FVector Posicion = RobotEncontrado->PuntosTraslacion[i];
                        res += Identacion(NumeroIdentaciones) + "glTranslate(" + FString::SanitizeFloat(Posicion.X) + ", " + FString::SanitizeFloat(Posicion.Y) + ", " + FString::SanitizeFloat(Posicion.Z) + ");\n";
                    }
                }
            }
            else {
                FVector Posicion = T->GetLocation();
                res += Identacion(NumeroIdentaciones) + "glTranslate(" + FString::SanitizeFloat(Posicion.X) + ", " + FString::SanitizeFloat(Posicion.Y) + ", " + FString::SanitizeFloat(Posicion.Z) + ");\n";
            }
            res += Identacion(NumeroIdentaciones) + T->ParteAsociada->NombreParte + "();\n";
            for (int i = 0; i < T->Hijos.Num(); i++) {
                res += TextoRotaciones(T->Hijos[i]);
            }
            res += Identacion(NumeroIdentaciones) + "glPopMatrix();\n";
        }
    }
	return res;
}

FString AJerarquia::Identacion(int Tam) {
	FString res;
	for (int i = 0; i < Tam; i++) {
		//res += "\t";
		res += "    ";
	}
	return res;
}

void AJerarquia::ActualizarPila() {
    FString Codigo = Texto(Root);
    PilaCodigo->CambiarCodigo(Codigo);//esto me esta dando error
}

void AJerarquia::ActualizarCodigoConRotaciones() {
    FString Codigo = TextoRotaciones(Root);
    PilaCodigo->CambiarCodigo(Codigo);//esto me esta dando error
}

void AJerarquia::ActualizarCodigoArmado() {
    FString Codigo = TextoRotaciones(Root);
    PilaCodigo->CambiarCodigo(Codigo);//esto me esta dando error
}

void AJerarquia::ActualizarCodigoTraslacion() {
    FString Codigo = TextoRotaciones(Root);
    PilaCodigo->CambiarCodigo(Codigo);//esto me esta dando error
}

void AJerarquia::EstablecerRotacionEjeX(int IdParte, float angle) {
	Nodos[IdParte]->Rotacion.X = angle;
}

void AJerarquia::EstablecerRotacionEjeY(int IdParte, float angle) {
	Nodos[IdParte]->Rotacion.Y = angle;
}

void AJerarquia::EstablecerRotacionEjeZ(int IdParte, float angle) {
	Nodos[IdParte]->Rotacion.Z = angle;
}

void AJerarquia::EstablecerRotacionEje(int IdParte, float angle, ETransformacionEje EjeRotacion) {
    switch (EjeRotacion) {
        case ETransformacionEje::EEjeX: {
			EstablecerRotacionEjeX(IdParte, angle);
        }
        break;//no se como funciona esto
        case ETransformacionEje::EEjeY: {
			EstablecerRotacionEjeY(IdParte, angle);
        }
        break;//no se como funciona esto
        case ETransformacionEje::EEjeZ: {
			EstablecerRotacionEjeZ(IdParte, angle);
        }
        break;//no se como funciona esto
        default:
        case ETransformacionEje::ENone: {//unknown/ default state
            //no hacer nada
        }
        break;//no se como funciona esto
    }
	Nodos[IdParte]->ActualizarTextRotacion();//creo que necesitare conservar una funiocion que manipule el valor de bActualizar para asi actualizar los arreglos botones, etc en la itnerfaz del nodo
	
    //solo importa lo que sigue
    TransformacionesPartesPunteros[IdParte]->ParteAsociada->AgregarRotacion(angle, EjeRotacion);
}

void AJerarquia::ConfirmarRotacion(int IdParte, int IdRotacion) {
    if (IdParte >= 0 && IdParte < TransformacionesPartesPunteros.Num()) {
        TransformacionesPartesPunteros[IdParte]->ParteAsociada->ConfirmarRotacion(IdRotacion);
    }
}

void AJerarquia::CancelarRotacion(int IdParte, int IdRotacion) {
    if (IdParte >= 0 && IdParte < TransformacionesPartesPunteros.Num()) {
        TransformacionesPartesPunteros[IdParte]->ParteAsociada->CancelarRotacion(IdRotacion);
    }
}

AParte * AJerarquia::GetParte(int IdParte) {
    return TransformacionesPartesPunteros[IdParte]->ParteAsociada;
}

bool AJerarquia::AllNodesCreated() {
	bool res = true;
	for (int i = 0; i < Nodos.Num() && res; i++) {
		if (!Nodos[i]) {
			res = false;
		}
	}
	return res;
}

void AJerarquia::AbsorberJerarquia(AJerarquia * Otra, int IdPadre, int IdHijo) {
	//solo es unir las trasnformaciones en el lugar aporpiado
	//pero apra esto necestio saber donde, o quiza ni siqueira deba absorver la jerarquia,
	//antes en la clase robot, solo debo hacer el atachmente de la razi de la jerarauia a absorber a la ptrasnformacion que le corresponde, y esta clase simplemente se encarga de actualizar el layout de los hijo, y sus respectivas aristas
	//y la informacion pertinenete
	//a la jerarquia tamien ya tiene los punteros a los nodos, asi que solo seria denuevo actualizar

	TransformacionesPartesPunteros[IdHijo]->Padre = (TransformacionesPartesPunteros[IdPadre]);
	TransformacionesPartesPunteros[IdPadre]->Hijos.Add((TransformacionesPartesPunteros[IdHijo]));
	//se supone que el hijo es el nuevo nodo a crear, y el padre ya esta creado


	TransformacionesPartesPunteros[IdHijo]->ParteAsociada->AttachToActor(TransformacionesPartesPunteros[IdPadre]->ParteAsociada, FAttachmentTransformRules::KeepWorldTransform);
	//UE_LOG(LogClass, Log, TEXT("Matrices actualizadas despues de unir hijo"));
	//ImprimirMatrices(&TransformacionesPartes[IdHijo]);
	ActualizarNodos();
}

void AJerarquia::ActualizarIdRaizParte(int NuevoIdRaizParte) {
    std::stack<Transformacion *> pila;
    //la raiz es el ultimo nodo
	UE_LOG(LogClass, Log, TEXT("Actualizando IdParteRaiz"));
	pila.push(Root);
    while (!pila.empty()) {
        Transformacion * V = pila.top();
        pila.pop();
		//ejecutar animacion
		if (V) {
			V->ParteAsociada->IdParteRaiz = NuevoIdRaizParte;
			if (V->Hijos.Num()) {
				for (int i = V->Hijos.Num()-1; i >= 0; i--) {
					pila.push(V->Hijos[i]);
				}
			}
		}
    }
}

bool AJerarquia::ArticulacionSobrepuesta() {
	//esta funcion quiza no sea necesario, es decir al soltar deberia comporbar receien en ese momento si hay alguna parte sobre puesta,
	//y en caso de que sea asi, debo unirtas, por lo tanto al solatar hago un recorrido por la jerarquia, veo si las aprtes estan solapaas y las uno
	//tener cuidado 
	bool res = false;
    std::stack<Transformacion *> pila;
    //la raiz es el ultimo nodo
	UE_LOG(LogClass, Log, TEXT("Comprobando si hay parte sobrepuesta"));
	pila.push(Root);
    while (!pila.empty()) {
        Transformacion * V = pila.top();
        pila.pop();
		//ejecutar animacion
		res = res || V->ParteAsociada->bArticulacionSobrepuesta;
        if (V->Hijos.Num()) {
            for (int i = V->Hijos.Num()-1; i >= 0; i--) {
                pila.push(V->Hijos[i]);
            }
        }
    }
	return res;
}

bool AJerarquia::RealizarUniones() {
	//esta funcion quiza no sea necesario, es decir al soltar deberia comporbar receien en ese momento si hay alguna parte sobre puesta,
	//y en caso de que sea asi, debo unirtas, por lo tanto al solatar hago un recorrido por la jerarquia, veo si las aprtes estan solapaas y las uno
	//tener cuidado 
	bool res = false;
    std::stack<Transformacion *> pila;
    //la raiz es el ultimo nodo
	UE_LOG(LogClass, Log, TEXT("Comprobando si hay parte sobrepuesta"));
	pila.push(Root);
    while (!pila.empty()) {
        Transformacion * V = pila.top();
        pila.pop();
		//ejecutar animacion
		res = res || V->ParteAsociada->bArticulacionSobrepuesta;
		if (V->ParteAsociada->bArticulacionSobrepuesta) {
			V->ParteAsociada->UnirConParteSobrepuesta();
		}
        //if (V->Hijos.Num()) {
            for (int i = V->Hijos.Num()-1; i >= 0; i--) {
                pila.push(V->Hijos[i]);
            }
        //}
    }
	return res;
}

void AJerarquia::AplicarLayout() {
	FVector Correccion (0.0f, -Root->Hojas * DeltaHermanos / 2, Root->Altura * DeltaNiveles);

    std::stack<Transformacion *> pila;
	UE_LOG(LogClass, Log, TEXT("Aplicando layout"));
	pila.push(Root);
    while (!pila.empty()) {
        Transformacion * V = pila.top();
        pila.pop();
		//ejecutar animacion
        if (V) {
            Nodos[V->ParteAsociada->Id]->SetActorRelativeLocation(TransformacionesPartesPunteros[V->ParteAsociada->Id]->PosicionNodo + Correccion);
            //UE_LOG(LogClass, Log, TEXT("Nodo (%d) : (%.4f, %.4f, %.4f)"), i, TransformacionesPartesPunteros[i]->PosicionNodo.X, TransformacionesPartesPunteros[i]->PosicionNodo.Y, TransformacionesPartesPunteros[i]->PosicionNodo.Z);
			if (V->Hijos.Num()) {
				for (int i = V->Hijos.Num()-1; i >= 0; i--) {
					pila.push(V->Hijos[i]);
				}
			}
		}
    }

	/*for (int i = 0; i < Nodos.Num(); i++) {//debeir aser solo si le les he hecho algo
		if (Nodos[i]) {
			Nodos[i]->SetActorRelativeLocation(TransformacionesPartesPunteros[i]->PosicionNodo + Correccion);
			//Nodos[i]->SetActorRelativeLocation(TransformacionesPartesPunteros[i]->PosicionNodo);
		}
	}*/
}

void AJerarquia::EjecutarCicloAnimacion(int IdParte) {//para hallar niveles
    std::stack<Transformacion *> pila;
    //la raiz es el ultimo nodo
    //Transformacion * Root = &TransformacionesPartes[Nodos.Num() - 1];
	//ya tengo un root
	UE_LOG(LogClass, Log, TEXT("Ejecutando Animacion"));
    //pila.push(Root);//no deberia dsencolarlo
	pila.push(TransformacionesPartesPunteros[IdParte]);
    while (!pila.empty()) {
        Transformacion * V = pila.top();
        pila.pop();
        UE_LOG(LogClass, Log, TEXT("Tamaño pila %d"), pila.size());
		//ejecutar animacion
		//V->ParteAsociada->AnimacionRotar(Nodos[V->ParteAsociada->Id]->Rotacion);
        V->ParteAsociada->IniciarCicloAnimaciones();
        if (V->Hijos.Num()) {
            for (int i = V->Hijos.Num()-1; i >= 0; i--) {
                pila.push(V->Hijos[i]);
            }
        }
    }
}

void AJerarquia::DetenerCicloAnimacion(int IdParte) {//para hallar niveles
    std::stack<Transformacion *> pila;
    //la raiz es el ultimo nodo
    //Transformacion * Root = &TransformacionesPartes[Nodos.Num() - 1];
	//ya tengo un root
	UE_LOG(LogClass, Log, TEXT("Ejecutando Animacion"));
    //pila.push(Root);//no deberia dsencolarlo
	pila.push(TransformacionesPartesPunteros[IdParte]);
    while (!pila.empty()) {
        Transformacion * V = pila.top();
        pila.pop();
        UE_LOG(LogClass, Log, TEXT("Tamaño pila %d"), pila.size());
		//ejecutar animacion
		//V->ParteAsociada->AnimacionRotar(Nodos[V->ParteAsociada->Id]->Rotacion);
        V->ParteAsociada->DetenerCicloAnimaciones();
        if (V->Hijos.Num()) {
            for (int i = V->Hijos.Num()-1; i >= 0; i--) {
                pila.push(V->Hijos[i]);
            }
        }
    }
}

void AJerarquia::EjecutarAnimacion(int IdParte) {//para hallar niveles
    std::stack<Transformacion *> pila;
    //la raiz es el ultimo nodo
    //Transformacion * Root = &TransformacionesPartes[Nodos.Num() - 1];
	//ya tengo un root
	UE_LOG(LogClass, Log, TEXT("Ejecutando Animacion"));
    //pila.push(Root);//no deberia dsencolarlo
	pila.push(TransformacionesPartesPunteros[IdParte]);
    while (!pila.empty()) {
        Transformacion * V = pila.top();
        pila.pop();
        UE_LOG(LogClass, Log, TEXT("Tamaño pila %d"), pila.size());
		//ejecutar animacion
		//V->ParteAsociada->AnimacionRotar(Nodos[V->ParteAsociada->Id]->Rotacion);
        V->ParteAsociada->EjecutarAnimaciones();
        if (V->Hijos.Num()) {
            for (int i = V->Hijos.Num()-1; i >= 0; i--) {
                pila.push(V->Hijos[i]);
            }
        }
    }
}

void AJerarquia::EjecutarAnimacionTick(float DeltaTime) {
	for (int i = 0; i < TransformacionesPartesPunteros.Num(); i++) {
		TransformacionesPartesPunteros[i]->ParteAsociada->AnimacionRotarTick(DeltaTime);
	}
}

//necestio hacer varias busuqueda, buscar la parte para seleccionar, buscar loos componentes de rotacion y saber cual es cual, esto ultimo despes de haber ya seleccionado una parte, y evidentemente ya se cual es su trasnform
/*
Ejemplo de uso
            if (i & 1) {
                RotacionY = MatrizRotacionY(V->Sons[i]->Phi);
            }
            else {
                RotacionY = MatrizRotacionY(2 * PI - V->Sons[i]->Phi);
            }
            //RotacionY = MatrizRotacionY(2 * PI - V->Sons[i]->Phi);
            //RotacionX = MatrizRotacionX(2 * PI - V->Sons[i]->Phi);
            RotacionZ = MatrizRotacionZ(2 * PI - PI / 2);
            /*if (V->Nivel & 1) {
                RotacionZ = MatrizRotacionZ(PI / 2);
            }
            else {
                RotacionZ = MatrizRotacionZ(2 * PI - PI / 2);
            }* /
            TraslacionV = MatrizTraslacion(V->Sons[i]->X, V->Sons[i]->Y, V->Sons[i]->Z);
            V->Sons[i]->Frame = MultiplicacionMatriz(V->Frame, MultiplicacionMatriz(MultiplicacionMatriz(TraslacionV, RotacionZ), RotacionY));
*/
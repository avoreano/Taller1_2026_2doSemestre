# Integra Medica 2.0 — Taller 02: Clasificación y AVL

Extensión del sistema de gestión de pacientes del Taller 1. Ahora el archivo **no indica el servicio**:
cada paciente se clasifica automáticamente con un **árbol de decisión** y se almacena en el
**árbol AVL** del servicio asignado (clave de ordenamiento: ID).

## Integrantes

Diego Godoy Carvajal | 21.691.839-8 | avoreano | ICCI | C1

## Compilación y ejecución

Desde la carpeta raíz del proyecto:

```bash
g++ -std=c++17 -Wall -Wextra -Idata_structures -Iclasses main.cpp core/System.cpp classes/*.cpp -o main
./main
```

## Estructura del proyecto

```
.
├── main.cpp
├── pacientes.txt              archivo de entrada
├── core/
│   └── System.hpp / .cpp      orquestador: carga, menú, atención, búsqueda
├── classes/
│   ├── Person.hpp / .cpp      clase base
│   ├── Patient.hpp / .cpp     clase derivada de Person
│   ├── Service.hpp / .cpp     servicio: contiene un AVL de pacientes
│   ├── Hospital.hpp / .cpp    lista enlazada de los 8 servicios
│   ├── DecisionNode.hpp       nodo del árbol de decisión
│   └── DecisionTree.hpp / .cpp
└── data_structures/           estructuras genéricas (templates), implementadas a mano
    ├── Node.hpp               nodo de LinkedList, Queue y Stack
    ├── LinkedList.hpp
    ├── Queue.hpp
    ├── Stack.hpp
    ├── AVLNode.hpp
    └── AVLTree.hpp
```

## Menú

```
=== INTEGRA MEDICA 2.0 ===
1. Atender pacientes
2. Ver departamentos
3. Buscar paciente
4. Salir
```

1. **Atender pacientes**: muestra la cola de espera y pide cuántos atender. Por cada paciente (en orden FIFO)
   lo retira de la cola, lo evalúa con el árbol de decisión, muestra la **ruta** seguida y el servicio
   asignado, y lo inserta en el AVL de ese servicio.
2. **Ver departamentos**: muestra el estado de los 8 servicios y permite elegir uno para ver su AVL:
   cantidad de pacientes, **altura**, **factor de balance** de la raíz y los recorridos
   **inorden, preorden y postorden** (cada nodo con su factor de balance).
3. **Buscar paciente**: por **ID** (búsqueda en el AVL, mostrando la ruta de nodos visitados) o por
   **nombre** (sin distinguir mayúsculas). Si el paciente todavía está en la cola lo avisa.
4. **Salir**: libera la memoria dinámica y termina.

## Formato del archivo de pacientes

Una línea por paciente, campos separados por `;`:

```
ID;Nombre;Edad;Temperatura;PresionArterial;Dolor;TipoDolor;Trauma;SintomasNeurologicos;RequiereCirugia;DiasHospitalizacion
001;Juan Perez;25;37.0;120;3;otro;0;0;0;0
```

| Campo | Significado | Rango válido |
|---|---|---|
| ID | identificador único | entero ≥ 1 |
| Nombre | texto | no vacío |
| Edad | años | 0 – 120 |
| Temperatura | °C (decimal) | 30.0 – 45.0 |
| PresionArterial | presión sistólica en **mmHg** | 50 – 250 |
| Dolor | escala de dolor | 0 – 10 |
| TipoDolor | `pecho`, `cabeza`, `abdomen`, `extremidad`, `otro` | (no distingue mayúsculas) |
| Trauma / SintomasNeurologicos / RequiereCirugia | 0 = no, 1 = sí | 0 ó 1 |
| DiasHospitalizacion | días estimados | entero ≥ 0 |

### Validaciones en la carga

Cada línea inválida se **rechaza con un aviso** y el resto del archivo se sigue procesando. Al final
se informa cuántas líneas se cargaron y cuántas se rechazaron.

- Archivo inexistente o que no se puede abrir.
- Línea con columnas faltantes o sobrantes, o con campos vacíos.
- Números mal escritos (por ejemplo `25abc` o `abc`): se exige que todo el texto sea un número.
- Valores fuera de los rangos de la tabla, tipo de dolor desconocido o banderas distintas de 0/1.
- **Pacientes duplicados**: ID ya presente en la cola o en cualquier AVL.
- Líneas en blanco: se ignoran.

## Árbol de decisión

Árbol binario construido manualmente con nodos y punteros (`DecisionNode`). Cada nodo interno es una
pregunta con dos hijos (`yes` / `no`); cada hoja es un servicio. Usa **9 atributos** del paciente y
tiene **11 preguntas y 12 hojas**, con al menos una ruta a cada uno de los 8 servicios.

```
Edad < 14?
├─ Sí: Pediatria
└─ No: Trauma == 1?
    ├─ Sí: RequiereCirugia == 1?
    │   ├─ Sí: Cirugia
    │   └─ No: Traumatologia
    └─ No: SintomasNeurologicos == 1?
        ├─ Sí: Neurologia
        └─ No: TipoDolor == pecho?
            ├─ Sí: Presion >= 180?
            │   ├─ Sí: Urgencias
            │   └─ No: Presion < 90?
            │       ├─ Sí: Urgencias
            │       └─ No: Cardiologia
            └─ No: Temperatura >= 38.5?
                ├─ Sí: Urgencias
                └─ No: Dolor >= 8?
                    ├─ Sí: Urgencias
                    └─ No: Presion < 90?
                        ├─ Sí: Urgencias
                        └─ No: DiasHospitalizacion >= 2?
                            ├─ Sí: Hospitalizacion
                            └─ No: Medicina General
```

### Justificación de cada decisión

| Pregunta | Destino | Justificación |
|---|---|---|
| Edad < 14 | Pediatría | Los menores se atienden en pediatría sea cual sea el cuadro; por eso es la primera pregunta. |
| Trauma == 1 | (sigue) | Un trauma define la especialidad antes que los síntomas generales. |
| ↳ RequiereCirugia == 1 | Cirugía / Traumatología | Si el trauma necesita operación va a Cirugía; si no, a Traumatología. |
| SintomasNeurologicos == 1 | Neurología | Síntomas neurológicos requieren evaluación especializada. |
| TipoDolor == pecho | (sigue) | El dolor de pecho orienta a Cardiología. |
| ↳ Presion ≥ 180 | Urgencias | Presión sistólica muy alta (crisis hipertensiva): no puede esperar consulta de especialidad. |
| ↳ Presion < 90 | Urgencias | Presión baja (hipotensión): signo de inestabilidad. Si no hay presión crítica, va a Cardiología. |
| Temperatura ≥ 38.5 | Urgencias | Fiebre alta. |
| Dolor ≥ 8 | Urgencias | Dolor intenso. |
| Presion < 90 (sin dolor de pecho) | Urgencias | Hipotensión, igual que arriba. |
| DiasHospitalizacion ≥ 2 | Hospitalización | Se estima una estadía de dos o más días. |
| (ninguna anterior) | Medicina General | Caso general sin signos de gravedad. |

La presión arterial se interpreta como **presión sistólica en mmHg**. Los umbrales (180, 90, 38.5, 8, 14, 2)
son criterio del grupo; son comparaciones inclusivas (`>=`) o exclusivas (`<`) según se indica en el árbol.

### Empates y casos ambiguos

- **El orden de las preguntas decide el empate**: se toma el primer camino que corresponda. Ejemplos:
  un niño con trauma va a Pediatría; trauma y síntomas neurológicos a la vez va a Traumatología/Cirugía;
  síntomas neurológicos y dolor de pecho a la vez va a Neurología.
- **Dolor de pecho con fiebre o dolor alto**: va a Cardiología, salvo que la presión sea crítica
  (≥ 180 ó < 90), en cuyo caso va a Urgencias.

### Datos inválidos o insuficientes

Los datos fuera de rango o incompletos **nunca llegan al árbol**, porque se rechazan al cargar el archivo.
Como defensa adicional, `evaluate()` devuelve cadena vacía si recibe un paciente nulo, y el sistema
descarta al paciente con un mensaje en vez de asignarle un servicio.

### Recorridos y evaluación

`DecisionTree` ofrece `build()` (construcción), `evaluate(paciente, ruta)` (devuelve el servicio y deja
la ruta recorrida, por ejemplo `Edad >= 14 -> Trauma == 1 -> RequiereCirugia == 0`) y los recorridos
`showPreOrder()`, `showInOrder()` y `showPostOrder()`.


## Árbol AVL por servicio

Cada servicio (`Service`) contiene un `AVLTree<Patient*>` con el **ID como clave**. `AVLTree<T>` es genérico.

- **Altura**: un árbol vacío mide 0 y una hoja mide 1.
- **Factor de balance** de un nodo = altura(subárbol izquierdo) − altura(subárbol derecho).
  Un nodo está balanceado si vale −1, 0 ó 1.
- **Inserción**: inserta como árbol binario de búsqueda y al volver actualiza alturas y rebalancea.
  Rechaza IDs repetidos.
- **Rotaciones**: simple derecha (izquierda-izquierda), simple izquierda (derecha-derecha) y las dos
  dobles: izquierda-derecha (rotación izquierda en el hijo y luego derecha) y derecha-izquierda
  (rotación derecha en el hijo y luego izquierda). El caso se distingue con el factor de balance del hijo.
- **Búsqueda por ID**: O(log n); entrega la ruta de claves visitadas.
- **Búsqueda por nombre**: recorre el árbol en inorden (O(n)), porque el AVL está ordenado por ID y no por nombre.
- **Eliminación por ID**: cubre hoja, un hijo y dos hijos (reemplazo por el sucesor inorden) y rebalancea.
- **Recorridos**: `inOrder`, `preOrder` y `postOrder` entregan cada nodo (clave, dato, altura y factor de balance).

## Gestión de memoria

Todas las estructuras liberan sus nodos en el destructor. Cuando una estructura contiene punteros a
objetos, la **clase dueña** los elimina antes:

| Objeto | Dueño | Dónde se libera |
|---|---|---|
| `Patient` en la cola | `System` | `~System()` (pacientes sin atender al salir) |
| `Patient` ya clasificado | `Service` | `~Service()` recorre su AVL con `inOrder` y hace `delete` |
| `Service` | `Hospital` | `~Hospital()` |
| nodos de `Queue`, `LinkedList`, `AVLTree` | cada estructura | su propio destructor |
| nodos de `DecisionTree` | `DecisionTree` | `~DecisionTree()` (recursivo) |
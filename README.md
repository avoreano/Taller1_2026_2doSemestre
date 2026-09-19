 # INTEGRA MEDICA 2.0

Diego Godoy 21.691.839-8 avoreano ICCI

Para la compilacion y ejecucion en Windows PowerShell, abre la consola en la carpeta raiz del proyecto y ejecuta:

g++ main.cpp classes\*.cpp core\*.cpp -I. -Idata_structures -o main.exe
if ($LASTEXITCODE -eq 0) { .\main.exe }


El primer comando compila todos los archivos `.cpp`. El segundo inicia el programa solo si la compilacion fue exitosa.


## Uso del programa

Al iniciar, el sistema carga los pacientes desde `pacientes.txt` y muestra un menu:

1. Atender pacientes: muestra la cola de espera y permite indicar cuantos pacientes atender.
2. Ver departamento: permite consultar los pacientes de un departamento.
3. Revisar historial de atencion: muestra las atenciones registradas.
4. Salir: termina el programa.

Los pacientes pasan de la cola de espera al departamento indicado en `pacientes.txt` cuando son atendidos.

## Formato de pacientes.txt

Cada paciente debe ocupar una linea con el formato:

```text
id;nombre;edad;departamento
```

Ejemplo:

```text
001;Juan Perez;25;Cardiologia
002;Maria Soto;67;Urgencias
```

El nombre del departamento debe coincidir con uno de los servicios definidos en `Hospital.cpp`.
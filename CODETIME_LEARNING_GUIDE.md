================================================================================
                    GUÍA COMPLETA DE APRENDIZAJE DE CODETIME
                    Desde Cero hasta Nivel Avanzado
================================================================================

ÍNDICE
================================================================================
1. Introducción a CodeTime
2. Instalación y Configuración
3. Fundamentos del Lenguaje
4. Variables y Tipos de Datos
5. Operadores y Expresiones
6. Control de Flujo
7. Funciones
8. Colecciones
9. Programación Orientada a Objetos
10. Manejo de Errores
11. Características Avanzadas
12. Biblioteca Estándar
13. Proyectos Prácticos
14. Mejores Prácticas
15. Recursos Adicionales

================================================================================
1. INTRODUCCIÓN A CODETIME
================================================================================

CodeTime es un lenguaje de programación moderno diseñado para ser:
- Fácil de leer y escribir
- Tipado estáticamente
- Multi-paradigma
- Compilado a código nativo
- Con sintaxis basada en inglés

¿Por qué aprender CodeTime?
- Sintaxis limpia y expresiva
- Curva de aprendizaje suave
- Comunidad en crecimiento
- Aplicaciones de alto rendimiento
- Ideal para principiantes y expertos

================================================================================
2. INSTALACIÓN Y CONFIGURACIÓN
================================================================================

Requisitos del sistema:
- Linux (actualmente)
- GCC o Clang
- Make o CMake

Instalación desde código fuente:
--------------------------------
1. Clonar o descargar el repositorio
2. Navegar al directorio codetime
3. Ejecutar: make
4. (Opcional) Ejecutar: sudo make install

Verificar instalación:
-----------------------
codetime version
codetime help

Tu primer programa:
-------------------
Crea un archivo llamado hola.cdt:

start
    print "¡Hola, CodeTime!"

Compila y ejecuta:
codetime build hola.cdt
./hola

================================================================================
3. FUNDAMENTOS DEL LENGUAJE
================================================================================

Estructura básica de un programa:
--------------------------------
Un programa CodeTime consiste en:
- Declaraciones de módulos (opcional)
- Importaciones
- Declaraciones (variables, funciones, objetos)
- Punto de entrada (start)

Sintaxis básica:
---------------
- Sin punto y coma al final de líneas
- Sin llaves para bloques
- Indentación define bloques (como Python)
- Palabras clave en inglés

Comentarios:
-----------
# Comentario de una línea

###
Comentario
de múltiples
líneas
###

================================================================================
4. VARIABLES Y TIPOS DE DATOS
================================================================================

Declaración de variables:
-----------------------
let nombre as text = "Juan"
let edad as number = 25
let activo as boolean = true

Inferencia de tipos:
-------------------
let nombre = "María"      # Se infiere como text
let edad = 30             # Se infiere como number
let precio = 19.99        # Se infiere como decimal

Variables mutables:
------------------
let contador as number = 0
change contador to contador + 1

Constantes:
----------
fixed PI as decimal = 3.14159
fixed MAX_USUARIOS as number = 1000

Tipos de datos primitivos:
------------------------
- number: Números enteros (42, -10, 1000)
- decimal: Números decimales (3.14, -0.5, 2.0)
- text: Cadenas de texto ("Hola", "Mundo")
- character: Caracteres individuales ('a', 'Z')
- boolean: Valores verdadero/falso (true, false)
- nothing: Representa ausencia de valor

Ejemplo práctico:
----------------
start
    let nombre as text = "Carlos"
    let edad as number = 28
    let es_estudiante as boolean = true
    
    print "Nombre: {nombre}"
    print "Edad: {edad}"
    print "Es estudiante: {es_estudiante}"

================================================================================
5. OPERADORES Y EXPRESIONES
================================================================================

Operadores aritméticos:
----------------------
+  : Suma
-  : Resta
*  : Multiplicación
/  : División
%  : Módulo (resto)
** : Potencia

Ejemplos:
let a = 10
let b = 3
let suma = a + b        # 13
let resta = a - b       # 7
let producto = a * b    # 30
let division = a / b    # 3
let modulo = a % b      # 1
let potencia = a ** b   # 1000

Operadores de comparación:
------------------------
==  : Igual a
!=  : Diferente de
<   : Menor que
<=  : Menor o igual que
>   : Mayor que
>=  : Mayor o igual que

Ejemplos:
let x = 5
let y = 10
x < y       # true
x == y      # false
x != y      # true

Operadores lógicos:
------------------
and : Y lógico
or  : O lógico
not : Negación lógica

Ejemplos:
let a = true
let b = false
a and b     # false
a or b      # true
not a       # false

Precedencia de operadores (de mayor a menor):
-----------------------------------------
1. ** (potencia)
2. *, /, %
3. +, -
4. <, <=, >, >=
5. ==, !=
6. not
7. and
8. or

================================================================================
6. CONTROL DE FLUJO
================================================================================

Condicionales (when/otherwise):
-------------------------------
when edad >= 18
    print "Eres adulto"
otherwise
    print "Eres menor"

Condicionales múltiples:
-----------------------
when temperatura > 30
    print "Hace calor"
otherwise when temperatura > 20
    print "Está templado"
otherwise
    print "Hace frío"

Bucle repeat (bucle con rango):
------------------------------
repeat i from 1 until 10
    print "Iteración: {i}"

Bucle for each (iterar sobre colecciones):
-----------------------------------------
for each nombre inside nombres
    print "Hola, {nombre}"

Bucle while (bucle condicional):
--------------------------------
while contador < 10
    change contador to contador + 1

Ejemplo práctico - Números pares:
--------------------------------
start
    repeat numero from 1 until 20
        when numero % 2 = 0
            print "{numero} es par"

================================================================================
7. FUNCIONES
================================================================================

Definición de funciones:
-----------------------
define saludar using nombre as text
    print "Hola, {nombre}"

Funciones con retorno:
--------------------
define sumar using a as number and b as number
    give a + b

Llamada a funciones:
------------------
saludar using "Ana"
let resultado = sumar using 5 and 3

Funciones con múltiples parámetros:
-----------------------------------
define calcular_promedio using a as number and b as number and c as number
    give (a + b + c) / 3

let promedio = calcular_promedio using 10 and 20 and 30

Funciones sin retorno (procedimientos):
--------------------------------------
define imprimir_mensaje
    print "Este es un mensaje"

imprimir_mensaje

Ejemplo práctico - Calculadora simple:
--------------------------------------
define sumar using a as number and b as number
    give a + b

define restar using a as number and b as number
    give a - b

define multiplicar using a as number and b as number
    give a * b

define dividir using a as number and b as number
    give a / b

start
    let x = 10
    let y = 5
    
    print "Suma: {sumar using x and y}"
    print "Resta: {restar using x and y}"
    print "Multiplicación: {multiplicar using x and y}"
    print "División: {dividir using x and y}"

================================================================================
8. COLECCIONES
================================================================================

Listas (arrays):
--------------
let frutas = ["manzana", "banana", "naranja"]
print frutas at 0              # "manzana"

frutas add "uva"              # Agregar elemento
frutas remove "banana"         # Eliminar elemento

Maps (diccionarios):
-------------------
let persona = {
    nombre: "Pedro"
    edad: 35
    ciudad: "Madrid"
}

print persona nombre           # "Pedro"
print persona edad             # 35

Operaciones comunes con listas:
------------------------------
let numeros = [1, 2, 3, 4, 5]
numeros add 6                 # [1, 2, 3, 4, 5, 6]
numeros remove 3              # [1, 2, 4, 5, 6]

Ejemplo práctico - Gestión de inventario:
-----------------------------------------
let inventario = {
    manzanas: 10
    naranjas: 15
    peras: 8
}

print "Manzanas: {inventario manzanas}"
change inventario manzanas to inventario manzanas + 5
print "Manzanas actualizadas: {inventario manzanas}"

================================================================================
9. PROGRAMACIÓN ORIENTADA A OBJETOS
================================================================================

Definición de objetos:
---------------------
object Persona
    property nombre as text
    property edad as number

    create using nombre as text and edad as number
        set self nombre to nombre
        set self edad to edad

    action presentarse
        print "Soy {self nombre} y tengo {self edad} años"

Instanciación de objetos:
------------------------
let persona = create Persona using "Laura" and 30
persona presentarse

Objetos con múltiples acciones:
--------------------------------
object Rectangulo
    property ancho as number
    property alto as number

    create using ancho as number and alto as number
        set self ancho to ancho
        set self alto to alto

    action area
        give self ancho * self alto

    action perimetro
        give 2 * (self ancho + self alto)

let rect = create Rectangulo using 10 and 5
print "Área: {rect area}"
print "Perímetro: {rect perimetro}"

Contratos (interfaces):
----------------------
contract Dibujable
    action dibujar

object Circulo follows Dibujable
    property radio as number

    action dibujar
        print "Dibujando círculo de radio {self radio}"

================================================================================
10. MANEJO DE ERRORES
================================================================================

Manejo de errores con attempt/recover:
-------------------------------------
attempt
    let resultado = dividir using 10 and 0
    print resultado
recover error
    print "Ocurrió un error"

Lanzar errores:
--------------
raise "Mensaje de error personalizado"

Ejemplo práctico - Validación de entrada:
----------------------------------------
define procesar_edad using edad as number
    when edad < 0
        raise "La edad no puede ser negativa"
    otherwise when edad > 150
        raise "La edad no es realista"
    otherwise
        print "Edad válida: {edad}"

start
    attempt
        procesar_edad using 25
    recover error
        print "Error: {error}"

================================================================================
11. CARACTERÍSTICAS AVANZADAS
================================================================================

Enumeraciones (choices):
-----------------------
choice DiaSemana
    Lunes
    Martes
    Miércoles
    Jueves
    Viernes
    Sábado
    Domingo

let hoy = DiaSemana Lunes

Pattern matching (choose/case):
-------------------------------
choose valor
    case 1
        print "Uno"
    case 2
        print "Dos"
    case 3
        print "Tres"
    otherwise
        print "Otro valor"

Tipos opcionales:
----------------
let usuario as maybe text

when usuario exists
    print "Usuario: {usuario}"
otherwise
    print "No hay usuario"

Generics:
---------
object Contenedor of Tipo
    property valor as Tipo

let caja_texto as Contenedor of text
let caja_numero as Contenedor of number

Interpolación de strings:
-------------------------
let nombre = "María"
let edad = 28
print "Hola, me llamo {nombre} y tengo {edad} años"

Strings multilínea:
------------------
let mensaje = """
Este es un mensaje
de múltiples líneas
en CodeTime
"""

================================================================================
12. BIBLIOTECA ESTÁNDAR
================================================================================

Importación de módulos:
----------------------
use math
use text
use files

Funciones matemáticas:
--------------------
use math

let raiz = math sqrt 25
let aleatorio = math random

Manipulación de texto:
---------------------
use text

let texto = "Hola Mundo"
let longitud = text length texto
let mayusculas = text upper texto

Operaciones con archivos:
------------------------
use files

let contenido = files read "datos.txt"
files write "salida.txt" with contenido

================================================================================
13. PROYECTOS PRÁCTICOS
================================================================================

Proyecto 1: Calculadora
----------------------
- Implementar operaciones básicas
- Manejo de errores (división por cero)
- Interfaz de usuario simple

Proyecto 2: Gestor de Tareas
-------------------------------
- Lista de tareas pendientes
- Agregar, eliminar, marcar como completadas
- Persistencia en archivo

Proyecto 3: Juego de Adivinanza
-------------------------------
- Generar número aleatorio
- Interacción con usuario
- Contador de intentos

Proyecto 4: Conversor de Unidades
--------------------------------
- Conversión entre diferentes unidades
- Soporte para longitud, peso, temperatura
- Interfaz amigable

Proyecto 5: Analizador de Texto
-------------------------------
- Contar palabras, caracteres, líneas
- Encontrar palabras más frecuentes
- Estadísticas básicas

================================================================================
14. MEJORES PRÁCTICAS
================================================================================

Nombres descriptivos:
--------------------
# Malo
let x = 10
let y = 20

# Bueno
let edad_usuario = 10
let altura_persona = 20

Comentarios adecuados:
---------------------
# Calcula el área del rectángulo
let area = ancho * alto

Funciones pequeñas y enfocadas:
-------------------------------
# Bueno
define calcular_area using ancho as number and alto as number
    give ancho * alto

# Evitar funciones muy largas

Indentación consistente:
-----------------------
# Usar siempre 4 espacios
when condicion
    print "Acción"

Manejo de errores:
-----------------
# Siempre validar entradas
attempt
    procesar datos
recover error
    print "Error al procesar"

================================================================================
15. RECURSOS ADICIONALES
================================================================================

Documentación oficial:
----------------------
- docs/language.md - Especificación del lenguaje
- docs/syntax.md - Referencia de sintaxis
- docs/standard-library.md - Biblioteca estándar

Ejemplos de código:
------------------
- examples/hello.cdt - Hola mundo
- examples/variables.cdt - Variables y tipos
- examples/functions.cdt - Funciones
- examples/conditions.cdt - Condicionales
- examples/loops.cdt - Bucles
- examples/objects.cdt - Objetos
- examples/collections.cdt - Colecciones

Comandos del compilador:
-----------------------
codetime help         - Muestra ayuda
codetime version      - Muestra versión
codetime check archivo - Verifica errores
codetime build archivo - Compila programa
codetime run archivo   - Compila y ejecuta

Comunidad y soporte:
-------------------
- GitHub Issues - Reportar bugs
- Documentación - Guías y tutoriales
- Ejemplos - Código de referencia

================================================================================
RUTA DE APRENDIZAJE SUGERIDA
================================================================================

Nivel 1: Fundamentos (1-2 semanas)
----------------------------------
1. Instalación y primer programa
2. Variables y tipos de datos
3. Operadores básicos
4. Entrada y salida simple
5. Condicionales básicas

Nivel 2: Estructuras de control (2-3 semanas)
---------------------------------------------
1. Bucles (repeat, for each, while)
2. Funciones básicas
3. Arrays y listas
4. Manejo de strings
5. Proyectos pequeños

Nivel 3: Programación modular (3-4 semanas)
------------------------------------------
1. Funciones avanzadas
2. Módulos e imports
3. Programación orientada a objetos básica
4. Manejo de errores
5. Proyectos intermedios

Nivel 4: Características avanzadas (4-6 semanas)
-----------------------------------------------
1. POO avanzada (herencia, polimorfismo)
2. Contratos e interfaces
3. Generics
4. Pattern matching
5. Proyectos complejos

Nivel 5: Maestría (continuo)
----------------------------
1. Optimización de código
2. Biblioteca estándar completa
3. Integración con C
4. Contribución al proyecto
5. Proyectos de producción

================================================================================
CONSEJOS PARA EL APRENDIZAJE EFECTIVO
================================================================================

1. Practica diariamente:
   - Escribe código todos los días
   - Comienza con proyectos pequeños
   - Aumenta la complejidad gradualmente

2. Lee código de otros:
   - Estudia los ejemplos
   - Analiza proyectos open source
   - Aprende diferentes estilos

3. Resuelve problemas:
   - Utiliza plataformas de práctica
   - Participa en desafíos de código
   - Contribuye a proyectos reales

4. Construye proyectos:
   - Aplica lo aprendido
   - Crea algo útil
   - Comparte tu trabajo

5. Sé paciente:
   - El aprendizaje lleva tiempo
   - No te frustres con errores
   - Celebra pequeños logros

================================================================================
EJERCICIOS PRÁCTICOS
================================================================================

Ejercicio 1: Números primos
----------------------------
Escribe un programa que determine si un número es primo.

Ejercicio 2: FizzBuzz
---------------------
Escribe un programa que imprima:
- "Fizz" para múltiplos de 3
- "Buzz" para múltiplos de 5
- "FizzBuzz" para múltiplos de ambos
- El número en otros casos

Ejercicio 3: Palíndromos
------------------------
Escribe un programa que determine si una palabra es palíndromo.

Ejercicio 4: Fibonacci
-----------------------
Escribe un programa que genere la secuencia de Fibonacci.

Ejercicio 5: Ordenamiento
-------------------------
Implementa un algoritmo de ordenamiento de una lista.

================================================================================
CONCLUSIÓN
================================================================================

CodeTime es un lenguaje moderno y poderoso con una sintaxis limpia y 
expressiva. Con esta guía tienes todo lo necesario para comenzar tu viaje
desde principiante hasta convertirte en un programador competente en
CodeTime.

Recuerda:
- La práctica es clave
- Los errores son oportunidades de aprendizaje
- La comunidad está aquí para ayudarte
- Diviértete programando

¡Bienvenido al mundo de CodeTime!

================================================================================
© 2026 CodeTime Project - Guía de Aprendizaje
================================================================================

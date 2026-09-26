# CodeTime — Notas de la versión 0.0.1 (primera versión)

> Primera versión pública del lenguaje CodeTime.  
> Fuente única real: `CODETIME_LEARNING_GUIDE.txt`.

## Esta versión 0.0.1 tiene

### Agregado — Archivos y estructura del programa
- Extensión de archivo **`.cdt`**.
- Punto de entrada **`start`**.
- Bloques definidos por **indentación** (sin llaves `{ }` y sin `;`).
- Comentarios de una línea `#` y multilínea `### … ###`.
- Declaración de módulo `module nombre` (opcional).
- Importaciones `use nombre_modulo` (opcional).

Ejemplo mínimo (cómo se crea el archivo):

```codetime
start
    print "¡Hola, CodeTime!"
```

### Agregado — Tipos de datos
- Primitivos: `number`, `decimal`, `text`, `character`, `boolean`, `nothing`.
- Colecciones: `list` (secuencia ordenada), `map` (clave-valor).
- Tipos opcionales: `maybe T` y `exists`.
- Genéricos sobre objetos: `object X of T`.

### Agregado — Variables
- `let` con inferencia o con anotación: `let x as T = …`
- Variables mutables: `change x to …`
- Constantes: `fixed`

### Agregado — Operadores
- Aritméticos: `+ - * / % **`
- Comparación: `== != < <= > >=`
- Lógicos: `and or not`

### Agregado — Control de flujo
- `when / otherwise when / otherwise`
- `repeat i from … until …`
- `for each … inside …`
- `while …`

### Agregado — Funciones
- Definición: `define nombre using param as T …`
- Retorno: `give`
- Procedimientos (sin retorno)

### Agregado — Objetos
- `object`, `property`, `create`, `action`, `self`, `set`
- Constructores: `create using …`

### Agregado — Contratos (interfaces)
- `contract` y `object … follows …`

### Agregado — Enumeraciones
- `choice Nombre` con constructores `Nombre X`

### Agregado — Manejo de errores
- Captura: `attempt … recover error …`
- Lanzado: `raise "mensaje"`

### Agregado — Pattern matching
- `choose … case … otherwise`

### Agregado — Strings
- Interpolación: `"Texto {variable}"`
- Strings multilínea: `"""…"""`

### Agregado — Compilación y comandos
- `codetime help`
- `codetime version`
- `codetime check archivo.cdt`
- `codetime build archivo.cdt`
- `codetime run archivo.cdt`

## No incluido en 0.0.1 (próximamente)

> La spec en inglés en `docs/` menciona algunas cosas que **aún no existen** en esta versión:
> - **Funciones anónimas / lambdas (`=>`)**: el parser responde *"Lambda expressions not yet implemented"*.
> - **Tipos de colecciones parametrizados** (`list of text`, `map of …`, `set of …`): no los reconoce el parser.
> - **Módulos de la biblioteca estándar concretos** (`math`, `text`, `files`, …): `use` se reconoce pero no resuelve ninguno.

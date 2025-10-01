// Análisis de ordenamiento con diferentes tamaños de arreglos
const fs = require('fs');

// Función para crear arreglo de tamaño específico
function crearArreglo(tamaño) {
    let arr = [];
    for (let i = 1000; i <= tamaño; i += 1000) {
        arr.push(i);
    }
    return arr;
}

// Función de ordenamiento descendente
function ordenarDescendente(arr) {
    return [...arr].sort((a, b) => b - a);
}

// Función de ordenamiento aleatorio (Fisher-Yates shuffle)
function ordenarAleatoriamente(arr) {
    let resultado = [...arr];
    for (let i = resultado.length - 1; i > 0; i--) {
        const j = Math.floor(Math.random() * (i + 1));
        [resultado[i], resultado[j]] = [resultado[j], resultado[i]];
    }
    return resultado;
}

// Función para medir tiempo de ejecución
function medirTiempo(funcion, arr) {
    const inicio = performance.now();
    const resultado = funcion(arr);
    const fin = performance.now();
    return {
        tiempo: (fin - inicio).toFixed(4),
        resultado: resultado
    };
}

// Tamaños a probar (de 1000 hasta 1,000,000 en incrementos de 1000)
const tamaños = [];
for (let t = 1000; t <= 1000000; t += 1000) {
    tamaños.push(t);
}

console.log("=".repeat(70));
console.log("ANÁLISIS DE RENDIMIENTO DE ALGORITMOS DE ORDENAMIENTO".padStart(55));
console.log("=".repeat(70));
console.log(`Total de pruebas a realizar: ${tamaños.length}`);
console.log("Tamaños: desde 1,000 hasta 1,000,000 elementos");
console.log("=".repeat(70));

// Almacenar resultados
let resultados = [];
let resultadosCompletos = {};

// Realizar pruebas para cada tamaño
console.log("\nRealizando pruebas...\n");

tamaños.forEach((tamaño, index) => {
    const arreglo = crearArreglo(tamaño);
    
    // Medir ordenamiento descendente
    const descResult = medirTiempo(ordenarDescendente, arreglo);
    
    // Medir ordenamiento aleatorio
    const aleatorioResult = medirTiempo(ordenarAleatoriamente, arreglo);
    
    resultados.push({
        tamaño: tamaño,
        elementos: arreglo.length,
        tiempoDesc: parseFloat(descResult.tiempo),
        tiempoAleatorio: parseFloat(aleatorioResult.tiempo)
    });
    
    // Guardar resultados completos solo para algunos tamaños seleccionados
    if ([1000, 10000, 50000, 100000, 500000, 1000000].includes(tamaño)) {
        resultadosCompletos[tamaño] = {
            descendente: descResult.resultado,
            aleatorio: aleatorioResult.resultado
        };
    }
    
    // Mostrar progreso cada 100 pruebas
    if ((index + 1) % 100 === 0 || tamaño === 1000000) {
        console.log(`✓ Completadas ${index + 1}/${tamaños.length} pruebas (tamaño actual: ${tamaño.toLocaleString()})`);
    }
});

console.log("\n" + "=".repeat(70));
console.log("RESUMEN DE RESULTADOS".padStart(48));
console.log("=".repeat(70));

// Mostrar tabla de resultados para tamaños seleccionados
console.log("\nTamaño".padEnd(15) + "Elementos".padEnd(12) + "Desc (ms)".padEnd(15) + "Aleatorio (ms)");
console.log("-".repeat(70));

[1000, 10000, 50000, 100000, 250000, 500000, 750000, 1000000].forEach(tamaño => {
    const resultado = resultados.find(r => r.tamaño === tamaño);
    if (resultado) {
        console.log(
            tamaño.toLocaleString().padEnd(15) +
            resultado.elementos.toString().padEnd(12) +
            resultado.tiempoDesc.toFixed(4).padEnd(15) +
            resultado.tiempoAleatorio.toFixed(4)
        );
    }
});

// Calcular estadísticas
const tiemposDesc = resultados.map(r => r.tiempoDesc);
const tiemposAleatorio = resultados.map(r => r.tiempoAleatorio);

const promedioDesc = (tiemposDesc.reduce((a, b) => a + b, 0) / tiemposDesc.length).toFixed(4);
const promedioAleatorio = (tiemposAleatorio.reduce((a, b) => a + b, 0) / tiemposAleatorio.length).toFixed(4);
const maxDesc = Math.max(...tiemposDesc).toFixed(4);
const maxAleatorio = Math.max(...tiemposAleatorio).toFixed(4);
const minDesc = Math.min(...tiemposDesc).toFixed(4);
const minAleatorio = Math.min(...tiemposAleatorio).toFixed(4);

console.log("\n" + "=".repeat(70));
console.log("ESTADÍSTICAS GENERALES".padStart(48));
console.log("=".repeat(70));
console.log("\nOrdenamiento Descendente:");
console.log(`  - Tiempo mínimo:    ${minDesc} ms`);
console.log(`  - Tiempo máximo:    ${maxDesc} ms`);
console.log(`  - Tiempo promedio:  ${promedioDesc} ms`);

console.log("\nOrdenamiento Aleatorio:");
console.log(`  - Tiempo mínimo:    ${minAleatorio} ms`);
console.log(`  - Tiempo máximo:    ${maxAleatorio} ms`);
console.log(`  - Tiempo promedio:  ${promedioAleatorio} ms`);

console.log("\n" + "=".repeat(70));

// Guardar resultados en archivo
function guardarResultados() {
    let contenido = "";
    
    contenido += "=".repeat(80) + "\n";
    contenido += "ANÁLISIS COMPLETO DE RENDIMIENTO DE ALGORITMOS DE ORDENAMIENTO\n";
    contenido += "=".repeat(80) + "\n\n";
    
    contenido += "INFORMACIÓN GENERAL:\n";
    contenido += `- Total de pruebas realizadas: ${resultados.length}\n`;
    contenido += "- Rango de tamaños: 1,000 a 1,000,000 elementos\n";
    contenido += "- Incremento: 1,000 elementos\n";
    contenido += `- Fecha de ejecución: ${new Date().toLocaleString()}\n\n`;
    
    contenido += "=".repeat(80) + "\n";
    contenido += "TABLA COMPLETA DE RESULTADOS:\n";
    contenido += "=".repeat(80) + "\n";
    contenido += "Tamaño".padEnd(15) + "Elementos".padEnd(12) + "Descendente (ms)".padEnd(20) + "Aleatorio (ms)\n";
    contenido += "-".repeat(80) + "\n";
    
    resultados.forEach(r => {
        contenido += 
            r.tamaño.toLocaleString().padEnd(15) +
            r.elementos.toString().padEnd(12) +
            r.tiempoDesc.toFixed(4).padEnd(20) +
            r.tiempoAleatorio.toFixed(4) + "\n";
    });
    
    contenido += "\n" + "=".repeat(80) + "\n";
    contenido += "ESTADÍSTICAS:\n";
    contenido += "=".repeat(80) + "\n\n";
    
    contenido += "Ordenamiento Descendente:\n";
    contenido += `  - Tiempo mínimo:    ${minDesc} ms\n`;
    contenido += `  - Tiempo máximo:    ${maxDesc} ms\n`;
    contenido += `  - Tiempo promedio:  ${promedioDesc} ms\n\n`;
    
    contenido += "Ordenamiento Aleatorio:\n";
    contenido += `  - Tiempo mínimo:    ${minAleatorio} ms\n`;
    contenido += `  - Tiempo máximo:    ${maxAleatorio} ms\n`;
    contenido += `  - Tiempo promedio:  ${promedioAleatorio} ms\n\n`;
    
    // Agregar algunos ejemplos de arreglos completos
    contenido += "=".repeat(80) + "\n";
    contenido += "EJEMPLOS DE ARREGLOS ORDENADOS:\n";
    contenido += "=".repeat(80) + "\n\n";
    
    Object.keys(resultadosCompletos).forEach(tamaño => {
        contenido += `\nTAMAÑO: ${parseInt(tamaño).toLocaleString()} elementos\n`;
        contenido += "-".repeat(80) + "\n";
        
        contenido += "\nDescendente (primeros 20):\n";
        contenido += resultadosCompletos[tamaño].descendente.slice(0, 20).join(", ") + "...\n";
        
        contenido += "\nAleatorio (primeros 20):\n";
        contenido += resultadosCompletos[tamaño].aleatorio.slice(0, 20).join(", ") + "...\n\n";
    });
    
    contenido += "=".repeat(80) + "\n";
    contenido += "FIN DEL ANÁLISIS\n";
    contenido += "=".repeat(80) + "\n";
    
    try {
        fs.writeFileSync('analisis_ordenamiento_completo.txt', contenido, 'utf8');
        console.log("\n✓ Archivo creado exitosamente: analisis_ordenamiento_completo.txt");
        console.log("  Ubicación: " + __dirname + "\\analisis_ordenamiento_completo.txt");
    } catch (error) {
        console.error("\n✗ Error al crear el archivo:", error.message);
    }
}

guardarResultados();

console.log("\n" + "=".repeat(70));
console.log("Análisis completado exitosamente".padStart(52));
console.log("=".repeat(70) + "\n");
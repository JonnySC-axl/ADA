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

// Función para generar archivos de datos y script de gnuplot
function generarArchivoGnuplot() {
    // 1. Crear archivo de datos (.dat)
    let datosGnuplot = "";
    datosGnuplot += "# Tamaño Elementos TiempoDescendente TiempoAleatorio\n";
    
    resultados.forEach(r => {
        datosGnuplot += `${r.tamaño} ${r.elementos} ${r.tiempoDesc} ${r.tiempoAleatorio}\n`;
    });
    
    try {
        fs.writeFileSync('datos_ordenamiento.dat', datosGnuplot, 'utf8');
        console.log("\n✓ Archivo de datos creado: datos_ordenamiento.dat");
    } catch (error) {
        console.error("\n✗ Error al crear archivo de datos:", error.message);
        return;
    }
    
    // 2. Crear script de gnuplot (.gp)
    let scriptGnuplot = `# Script de Gnuplot para análisis de ordenamiento
# Ejecutar con: gnuplot graficar_ordenamiento.gp

# Configuración general
set terminal pngcairo enhanced font 'Arial,12' size 1400,900
set output 'analisis_ordenamiento.png'

# Configuración de estilo
set style line 1 lc rgb '#E74C3C' lt 1 lw 2 pt 7 ps 0.5
set style line 2 lc rgb '#3498DB' lt 1 lw 2 pt 9 ps 0.5
set style line 3 lc rgb '#2ECC71' lt 1 lw 2 pt 5 ps 0.5
set grid ytics xtics linestyle 0 linecolor rgb '#E0E0E0'

# Layout con múltiples gráficas
set multiplot layout 2,2 title "Análisis de Rendimiento de Algoritmos de Ordenamiento" font "Arial,16"

# Gráfica 1: Comparación de tiempos
set title "Comparación de Tiempos de Ejecución"
set xlabel "Tamaño del arreglo (elementos)"
set ylabel "Tiempo (ms)"
set key top left
plot 'datos_ordenamiento.dat' using 1:3 with linespoints ls 1 title "Descendente", \\
     'datos_ordenamiento.dat' using 1:4 with linespoints ls 2 title "Aleatorio"

# Gráfica 2: Tiempo descendente
set title "Ordenamiento Descendente"
set xlabel "Tamaño del arreglo (elementos)"
set ylabel "Tiempo (ms)"
set key off
plot 'datos_ordenamiento.dat' using 1:3 with linespoints ls 1 notitle

# Gráfica 3: Tiempo aleatorio
set title "Ordenamiento Aleatorio (Shuffle)"
set xlabel "Tamaño del arreglo (elementos)"
set ylabel "Tiempo (ms)"
set key off
plot 'datos_ordenamiento.dat' using 1:4 with linespoints ls 2 notitle

# Gráfica 4: Ratio de tiempos
set title "Ratio: Aleatorio / Descendente"
set xlabel "Tamaño del arreglo (elementos)"
set ylabel "Ratio"
set key off
plot 'datos_ordenamiento.dat' using 1:(\$4/\$3) with linespoints ls 3 notitle

unset multiplot

# Generar segunda imagen con escala logarítmica
set output 'analisis_ordenamiento_log.png'
set multiplot layout 1,2 title "Análisis con Escala Logarítmica" font "Arial,16"

# Gráfica con escala log en Y
set title "Comparación (Escala Log en Y)"
set xlabel "Tamaño del arreglo"
set ylabel "Tiempo (ms) - Escala Log"
set logscale y
set key top left
set grid
plot 'datos_ordenamiento.dat' using 1:3 with linespoints ls 1 title "Descendente", \\
     'datos_ordenamiento.dat' using 1:4 with linespoints ls 2 title "Aleatorio"

# Gráfica con escala log en ambos ejes
set title "Comparación (Escala Log en X e Y)"
set xlabel "Tamaño del arreglo - Escala Log"
set ylabel "Tiempo (ms) - Escala Log"
set logscale xy
plot 'datos_ordenamiento.dat' using 1:3 with linespoints ls 1 title "Descendente", \\
     'datos_ordenamiento.dat' using 1:4 with linespoints ls 2 title "Aleatorio"

unset multiplot
unset logscale

# Generar gráfica de dispersión (scatter plot)
set output 'analisis_scatter.png'
set title "Dispersión de Tiempos de Ejecución" font "Arial,16"
set xlabel "Tamaño del arreglo (elementos)"
set ylabel "Tiempo (ms)"
set key top left
set grid
plot 'datos_ordenamiento.dat' using 1:3 with points pt 7 ps 0.8 lc rgb '#E74C3C' title "Descendente", \\
     'datos_ordenamiento.dat' using 1:4 with points pt 9 ps 0.8 lc rgb '#3498DB' title "Aleatorio"

# Generar gráfica de barras para tamaños específicos
set output 'analisis_barras.png'
set title "Comparación en Tamaños Clave" font "Arial,16"
set xlabel "Tamaño del arreglo"
set ylabel "Tiempo (ms)"
set style data histograms
set style histogram cluster gap 1
set style fill solid border -1
set boxwidth 0.9
set xtics rotate by -45
set key top left
set grid ytics

plot 'datos_ordenamiento.dat' every ::0::0 using 3:xtic(1) title "Descendente" lc rgb '#E74C3C', \\
     '' every ::0::0 using 4 title "Aleatorio" lc rgb '#3498DB', \\
     '' every ::9::9 using 3:xtic(1) notitle lc rgb '#E74C3C', \\
     '' every ::9::9 using 4 notitle lc rgb '#3498DB', \\
     '' every ::49::49 using 3:xtic(1) notitle lc rgb '#E74C3C', \\
     '' every ::49::49 using 4 notitle lc rgb '#3498DB', \\
     '' every ::99::99 using 3:xtic(1) notitle lc rgb '#E74C3C', \\
     '' every ::99::99 using 4 notitle lc rgb '#3498DB', \\
     '' every ::499::499 using 3:xtic(1) notitle lc rgb '#E74C3C', \\
     '' every ::499::499 using 4 notitle lc rgb '#3498DB', \\
     '' every ::999::999 using 3:xtic(1) notitle lc rgb '#E74C3C', \\
     '' every ::999::999 using 4 notitle lc rgb '#3498DB'

print "Gráficas generadas exitosamente:"
print "  - analisis_ordenamiento.png (gráfica principal con 4 paneles)"
print "  - analisis_ordenamiento_log.png (escala logarítmica)"
print "  - analisis_scatter.png (dispersión)"
print "  - analisis_barras.png (barras para tamaños clave)"
`;

    try {
        fs.writeFileSync('graficar_ordenamiento.gp', scriptGnuplot, 'utf8');
        console.log("✓ Script de gnuplot creado: graficar_ordenamiento.gp");
        console.log("\n" + "=".repeat(70));
        console.log("INSTRUCCIONES PARA GENERAR LAS GRÁFICAS:".padStart(52));
        console.log("=".repeat(70));
        console.log("\n1. Asegúrate de tener gnuplot instalado:");
        console.log("   - Windows: choco install gnuplot");
        console.log("   - Mac: brew install gnuplot");
        console.log("   - Linux: sudo apt-get install gnuplot");
        console.log("\n2. Ejecuta el script:");
        console.log("   gnuplot graficar_ordenamiento.gp");
        console.log("\n3. Se generarán 4 imágenes PNG:");
        console.log("   ✓ analisis_ordenamiento.png");
        console.log("   ✓ analisis_ordenamiento_log.png");
        console.log("   ✓ analisis_scatter.png");
        console.log("   ✓ analisis_barras.png");
    } catch (error) {
        console.error("\n✗ Error al crear script de gnuplot:", error.message);
    }
}

generarArchivoGnuplot();
generarArchivoGnuplot();

console.log("\n" + "=".repeat(70));
console.log("Análisis completado exitosamente".padStart(52));
console.log("=".repeat(70) + "\n");
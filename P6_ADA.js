// Función para contar ocurrencias de un elemento en un rango del arreglo
function contarEnRango(arr, num, inicio, fin) {
  let count = 0;
  for (let i = inicio; i <= fin; i++) {
    if (arr[i] === num) {
      count++;
    }
  }
  return count;
}

// Función recursiva de divide y vencerás
function encontrarMayoritarioRec(arr, inicio, fin) {
  // Caso base: un solo elemento
  if (inicio === fin) {
    return arr[inicio];
  }
  
  // Dividir el arreglo en dos mitades
  const mitad = Math.floor((inicio + fin) / 2);
  
  // Conquistar: encontrar el mayoritario en cada mitad
  const izq = encontrarMayoritarioRec(arr, inicio, mitad);
  const der = encontrarMayoritarioRec(arr, mitad + 1, fin);
  
  // Si ambas mitades tienen el mismo mayoritario
  if (izq === der) {
    return izq;
  }
  
  // Combinar: contar cuál aparece más veces en el rango completo
  const countIzq = contarEnRango(arr, izq, inicio, fin);
  const countDer = contarEnRango(arr, der, inicio, fin);
  
  // Retornar el que tenga mayor frecuencia
  return countIzq > countDer ? izq : der;
}

// Función principal
function encontrarElementoMayoritario(arr) {
  if (!arr || arr.length === 0) {
    return null;
  }
  
  return encontrarMayoritarioRec(arr, 0, arr.length - 1);
}

// Ejemplos de uso
console.log("=== Ejemplos de Elemento Mayoritario ===\n");

const ejemplo1 = [3, 2, 3];
console.log(`Arreglo: [${ejemplo1}]`);
console.log(`Elemento mayoritario: ${encontrarElementoMayoritario(ejemplo1)}`);
console.log();

const ejemplo2 = [2, 2, 1, 1, 1, 2, 2];
console.log(`Arreglo: [${ejemplo2}]`);
console.log(`Elemento mayoritario: ${encontrarElementoMayoritario(ejemplo2)}`);
console.log();

const ejemplo3 = [1, 1, 1, 1, 2, 3, 4];
console.log(`Arreglo: [${ejemplo3}]`);
console.log(`Elemento mayoritario: ${encontrarElementoMayoritario(ejemplo3)}`);
console.log();

const ejemplo4 = [5, 5, 5, 5, 5, 1, 2, 3, 4];
console.log(`Arreglo: [${ejemplo4}]`);
console.log(`Elemento mayoritario: ${encontrarElementoMayoritario(ejemplo4)}`);
console.log();

const ejemplo5 = [5, 5, 8, 8, 8, 8];
console.log(`Arreglo: [${ejemplo5}]`);
console.log(`Elemento mayoritario: ${encontrarElementoMayoritario(ejemplo5)}`);
console.log();

// Ejemplo con arreglo grande
const ejemploGrande = Array(1000).fill(7).concat(Array(499).fill(3));
console.log(`Arreglo grande con 1000 veces el 7 y 499 veces el 3`);
console.log(`Elemento mayoritario: ${encontrarElementoMayoritario(ejemploGrande)}`);
console.log(`Longitud del arreglo: ${ejemploGrande.length}`);
console.log(`Apariciones necesarias para ser mayoritario: ${ejemploGrande.length / 2}`);
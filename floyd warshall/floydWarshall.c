#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

void calcularDistanciasMinimas(int** grafoEntrada, int** resultadoFinal, int totalNos) {
    for(int noA = 0; noA < totalNos; noA++) {
        for(int noB = 0; noB < totalNos; noB++) {
            resultadoFinal[noA][noB] = grafoEntrada[noA][noB];
        }
    }

    for(int noInter = 0; noInter < totalNos; noInter++) {
        for(int noA = 0; noA < totalNos; noA++) {
            for(int noB = 0; noB < totalNos; noB++) {
                if(resultadoFinal[noA][noInter] != INT_MAX && 
                   resultadoFinal[noInter][noB] != INT_MAX &&
                   resultadoFinal[noA][noB] > resultadoFinal[noA][noInter] + resultadoFinal[noInter][noB]) {
                    resultadoFinal[noA][noB] = resultadoFinal[noA][noInter] + resultadoFinal[noInter][noB];
                }
            }
        }
    }
}
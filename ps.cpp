#include <immintrin.h>
#include <iostream>
#include <string>
#include <chrono>

// programme d'Ã©valuation du gain obtenu en utilisant les routines AVX
// pour le calcul de produits scalaires
// utilise les routines AVX et AVX2 (pour les doubles)
// Compilation : g++  -std=c++11 -march=native -O2 ps.cpp -o ps
//     ou      : g++  -std=c++11 -mavx -mavx2 -O2 ps.cpp -o ps



#define NB 10000  // nombre de fois oÃ¹ le produit scalaire est effectuÃ©
#define SIZE 80000 // taille des vecteurs utilisÃ©s


// -------------------------------------------
// Fonctions d'affichage
// -------------------------------------------
// affiche les 8 rÃ©els contenus dans un __m256
void printf(std::string s, __m256 *v);


// -------------------------------------------
// Fonctions de calcul
// -------------------------------------------
// calcul des produits scalaires sous forme de vecteurs d'entiers
void ps_int(int size);

// calcul des produits scalaires sous forme de vecteurs de float
void ps_float(int size);

// calcul des produits scalaires sous forme de vecteurs de double
void ps_double(int size);





int main(int argc, char*argv[]){

    ps_float(SIZE);



    return 0;
}



// effectue les ps sÃ©quentiel ou avx sur des rÃ©els simple prÃ©cision
// et affiche le temps de calcul correspondants
void ps_float(int size){

    clock_t start;

    // initialisation des deux vecteurs rÃ©els
    alignas(32) float *v1, *v2; // alignement sur 32 bits pour AVX

    v1=(float*)_mm_malloc(size*sizeof(float),32);
    v2=(float*)_mm_malloc(size*sizeof(float),32);

    // initialisation avec des valeurs alÃ©atoires dans [0,9]
    for(int i=0; i<size; i++){
        v1[i] = (float)(rand()%10);
        v2[i] = (float)(rand()%10);
    }

    // produit scalaire sÃ©quentiel
    start =  clock();

    float somme;
    for(int n=0; n<NB; n++){
        somme=0;
        for(int i=0; i<size; i++) somme += v1[i]*v2[i];
    }

    float t1;
    std::cout << "temps ps float seq = " << (t1=(clock()-start)/(double)CLOCKS_PER_SEC);
    std::cout << " secondes - valeur = " << somme << std::endl;

    start =  clock();
    for(int w=0;w<NB;++w) {
        somme = 0;
        // produit scalaire  avx
        __m256 pv1;
        __m256 pv2;
        __m256 somme_vectorielle = _mm256_setzero_ps();
        for (int i = 0; i < size / 8; i++) {
            pv1 = _mm256_load_ps(v1 + (8 * i));
            pv2 = _mm256_load_ps(v2 + (8 * i));
            __m256 res = _mm256_mul_ps(pv1, pv2);
            somme_vectorielle = _mm256_add_ps(somme_vectorielle, res);
        }
        float *res = (float *) &somme_vectorielle;

        for (int i = 0; i < sizeof(res); i++) {
            somme += res[i];
        }
    }
    std::cout << "temps ps float seq = " << (t1=(clock()-start)/(double)CLOCKS_PER_SEC);
    std::cout << " secondes - valeur = " << somme << std::endl;
    // affichage du speedup

    // dÃ©sallocation mÃ©moire
    _mm_free(v1);
    _mm_free(v2);
}


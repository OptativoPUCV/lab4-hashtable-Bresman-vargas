#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "hashmap.h"


typedef struct HashMap HashMap;
int enlarge_called=0;

struct HashMap {
    Pair ** buckets;
    long size; //cantidad de datos/pairs en la tabla
    long capacity; //capacidad de la tabla
    long current; //indice del ultimo dato accedido
};

Pair * createPair( char * key,  void * value) {
    Pair * new = (Pair *)malloc(sizeof(Pair));
    new->key = key;
    new->value = value;
    return new;
}

long hash( char * key, long capacity) {
    unsigned long hash = 0;
     char * ptr;
    for (ptr = key; *ptr != '\0'; ptr++) {
        hash += hash*32 + tolower(*ptr);
    }
    return hash%capacity;
}

int is_equal(void* key1, void* key2){
    if(key1==NULL || key2==NULL) return 0;
    if(strcmp((char*)key1,(char*)key2) == 0) return 1;
    return 0;
}

// Tuve un problema asi que solo voy a copiar mi codigo
// de otro replit mio

void insertMap(HashMap * map, char * key, void * value) {
  //El mapa y la key no deben ser null
  if(map==NULL || key==NULL) return;

  //En caso de que este llena
  if (map->size == map->capacity){
    enlarge(map);
  }
  //Se calcula el indice
  long posicion = hash(key,map->capacity);

  //Se busca otro indice
  while(map->buckets[posicion]!=NULL && map->buckets[posicion]->key!=NULL){
    posicion++;
    posicion = posicion % map->capacity;
  }
  //Si se encontro se agregan los datos
  map->buckets[posicion] = createPair(key,value);
  //Se aumenta el tamaño
  map->size++;
}

void enlarge(HashMap * map) {
  //Se duplica la capacidad 
  int nueva_capacidad = map->capacity * 2;

  //Se crea un nuevo arreglo con la nueva capacidad
  Pair **nuevo_arreglo = (Pair**) malloc(nueva_capacidad * sizeof(Pair*));
  if(nuevo_arreglo == NULL){
    return;
  }
  //Se inicializa en NULL
  for(int i = 0; i < nueva_capacidad; i++){
    nuevo_arreglo[i] = NULL;
  }
  //Traspasamos los datos de un arreglo al otro
  for(int i  = 0; i < map->capacity; i++){
    if(map->buckets[i] != NULL){
      Pair * current = map->buckets[i];

      //Calculamos el hash para el nuevo arreglo
      long nuevo_indice = hash(current->key,nueva_capacidad);

      while(nuevo_arreglo[nuevo_indice] != NULL){
        nuevo_indice = (nuevo_indice + 1) % nueva_capacidad;
      }
      nuevo_arreglo[nuevo_indice] = current;
    }
  }

  free(map->buckets);
  map->buckets = nuevo_arreglo;
  map->capacity = nueva_capacidad;
}


HashMap * createMap(long capacity) {
  //Se crea el mapa
  HashMap *map = (HashMap *)malloc(sizeof(HashMap));
  if(map == NULL){
    return NULL;
  }
  //Se crea el arreglo de buckets
  map->buckets = (Pair **)malloc(sizeof(Pair) * capacity);
  if(map->buckets == NULL){
    return NULL;
  }
  //Se inicializan los valores en null
  for(int i = 0; i < capacity; i++){
    map->buckets[i] = NULL;
  }
  //Se inicializan los valores en 0
  map->size = 0;
  map->capacity = capacity;
  map->current = -1;
  return map;
}

void eraseMap(HashMap * map,  char * key) {    
  if(map == NULL || key == NULL) return;

  long posicion = hash(key,map->capacity);
  //Se busca el indice
  while(map->buckets[posicion]!=NULL){
    if(is_equal(map->buckets[posicion]->key,key)){
      free(map->buckets[posicion]->key);
      free(map->buckets[posicion]);
      map->buckets[posicion] = NULL;
      map->size--;
      return;
    }
    posicion = (posicion+1) % map->capacity;
  }
}

Pair * searchMap(HashMap * map,  char * key) {   
  if(map == NULL || key == NULL){
    return NULL;
  }
  //Se calcula el indice
  long posicion = hash(key,map->capacity);

  //Se busca otro indice
  int start_index = posicion;
  int current = 0;
  int count = 0;
  do {
    //Se busca el dato
    if(map->buckets[posicion] != NULL && strcmp(map->buckets[posicion]->key, key) == 0){
      return map->buckets[posicion];
    }
    if(map->buckets[posicion] == NULL){
      return NULL;
    }
    //Se aumenta el indice
    count++;
    posicion = (posicion + 1) % map->capacity;
    current++;
    
  } while (posicion != start_index && count < map->size);
  return NULL;
}

Pair * firstMap(HashMap * map) {
  if(map == NULL || map->buckets == NULL){
    return NULL;
  }

  //Se busca el primer dato
  for(int i = 0; i < map->capacity; i++){
    if(map->buckets[i] != NULL){
      map->current = i;
      return map->buckets[i]; 
    }
  }
  return NULL;
}

Pair * nextMap(HashMap * map) {

  if(map == NULL){
    return NULL;
  }
  for(int i = map->current + 1 ; i < map->capacity; i++){
    if(map->buckets[i] != NULL){
      map->current = i;
      return map->buckets[i];
    }
  }
  return NULL;
}
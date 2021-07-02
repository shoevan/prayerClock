#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include "util.h"
#include "cJSON.h"

char* curlRequest(char URL[]) {
  CURL *curl_handle;
  CURLcode res;

  struct MemoryStruct chunk;
  chunk.memory = malloc(1);  
  chunk.size = 0;

  curl_handle = curl_easy_init();
  if(curl_handle) {
    curl_easy_setopt(curl_handle, CURLOPT_URL, URL);
    curl_easy_setopt(curl_handle, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&chunk);
    curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");

    res = curl_easy_perform(curl_handle);

    if(res != CURLE_OK) {
      fprintf(stderr, "error: %s\n", curl_easy_strerror(res));
    } else {
      //printf("Size: %lu\n", (unsigned long)chunk.size);
      //printf("Data: %s\n", chunk.memory);
      return chunk.memory;
    }
    curl_easy_cleanup(curl_handle);
    free(chunk.memory);
  }
}

char* getJSONString(cJSON* string){
	char* value = cJSON_Print(string);
	if (string == NULL){
	        fprintf(stderr, "Failed to print.\n");
 	}
	return value;
}

int main(int argc, char *argv[]) {
  if( argc != 3 ) {
    printf("usage: ./curl [Prayer Time Calculation Method] [Prayer Time School] \n \ 
            Prayer Time Calculation Method: 0 - Shia Ithna-Ansari \n \
1 - University of Islamic Sciences, Karachi \n \
2 - Islamic Society of North America \n \
3 - Muslim World League \n \
4 - Umm Al-Qura University, Makkah \n \
5 - Egyptian General Authority of Survey \n \
7 - Institute of Geophysics, University of Tehran \n \
8 - Gulf Region \n \
9 - Kuwait \n \
10 - Qatar \n \
11 - Majlis Ugama Islam Singapura, Singapore \n \
12 - Union Organization islamic de France \n \
13 - Diyanet İşleri Başkanlığı, Turkey \n \
14 - Spiritual Administration of Muslims of Russia \n \
	Prayer Time School: 0: Shafi\n \
			1: Hanafi\n");
//return 1;
  }
  struct json_object *data;
  char prayerTimeBaseURL[] = "http://api.aladhan.com/v1/timings/:date_or_timestamp?";
  int prayerTimeCalcMethod = 3; 
  int prayerTimeSchool = 1;
  if ( argc == 3){
  	prayerTimeCalcMethod = (int)argv[1];
  	prayerTimeSchool = (int)argv[2];
  }
  printf("Here\n");
//printf("Data: %s\n", );
  const cJSON *publicIP = cJSON_Parse(curlRequest("https://api.ipify.org?format=json"));
  char completeURL[500];
  char locationDetails[100];
  //const cJSON *IP = cJSON_GetObjectItemCaseSensitive(publicIP, "ip");
//printf("Data: %s\n", cJSON_GetObjectItem(publicIP, "ip")->valuestring);
  sprintf(locationDetails, "http://ip-api.com/json/%s", cJSON_GetObjectItem(publicIP, "ip")->valuestring);
  //printf("Data: %s\n", locationDetails);
  //printf("location: %s\n", curlRequest(locationDetails));
  const cJSON *location = cJSON_Parse(curlRequest(locationDetails));
  double latitude = cJSON_GetObjectItem(location, "lat")->valuedouble;
  double longitude = cJSON_GetObjectItem(location, "lon")->valuedouble;
  printf("Data: %f\n", latitude);
  //printf("Data: %s\n", location);
  sprintf(completeURL, "%slatitude=%f&longitude=%f&method=%d&school=%d", prayerTimeBaseURL, latitude, longitude, prayerTimeCalcMethod, prayerTimeSchool);
//  curl_handle = curl_easy_init();
  //printf("Data: %s\n", completeURL);
  printf("Data: %s\n", curlRequest(completeURL));
//  data = json_tokener_parse(buffer);

// size_t length;

// json_object_object_get_ex(data, "data", &data);
//  length = json_object_array_length(data)

  cJSON *json = cJSON_Parse(curlRequest(completeURL));
  const cJSON *times = NULL;
  cJSON *fajr = NULL;
  cJSON *dhuhr = NULL;
  cJSON *asr = NULL;
  cJSON *maghrib = NULL;
  cJSON *isha = NULL;
  const cJSON *dataJSON = cJSON_GetObjectItemCaseSensitive(json, "data");
  //char* string = cJSON_Print(dataJSON);
  //printf("datacJSON: %s\n", string);
  const cJSON *dataJSON2 = cJSON_GetObjectItemCaseSensitive(dataJSON, "timings");

//  char* string = cJSON_Print(dataJSON2);
  fajr = cJSON_GetObjectItemCaseSensitive(dataJSON2, "Fajr");
  dhuhr = cJSON_GetObjectItemCaseSensitive(dataJSON2, "Dhuhr");
  asr = cJSON_GetObjectItemCaseSensitive(dataJSON2, "Asr");
  maghrib = cJSON_GetObjectItemCaseSensitive(dataJSON2, "Maghrib");
  isha = cJSON_GetObjectItemCaseSensitive(dataJSON2, "Isha");

  char* string = cJSON_Print(fajr);
  printf("fajr: %s\n", string);
  
  FILE *fptr = fopen("prayerTimes.txt","w");

  if(fptr == NULL)
   {
      printf("Error!");
      exit(1);
   }

  fprintf(fptr,"%s\n",getJSONString(fajr));
  fprintf(fptr,"%s\n",getJSONString(dhuhr));
  fprintf(fptr,"%s\n",getJSONString(asr));
  fprintf(fptr,"%s\n",getJSONString(maghrib));
  fprintf(fptr,"%s\n",getJSONString(isha));
  fclose(fptr);
  //string = cJSON_Print(fajr);
  //if (string == NULL){
//        fprintf(stderr, "Failed to print fajr.\n");
 // }

 // return 0;
}



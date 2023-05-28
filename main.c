#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_COUNTRY_NAME_LENGTH 50
#define MAX_DATA_COUNT 1000
#define MAX_ITERATIONS 100

	struct Country {
    char name[MAX_COUNTRY_NAME_LENGTH];
    float child_mort;
    float exports;
    float health;
    float imports;
    int income;
    float inflation;
    float life_expec;
    float total_fer;
    int gdpp;
    int cluster; // K�me numaras�
    float point[2]; // X ve Y koordinatlar�
};

	float calculate_distance(float x1, float y1, float x2, float y2) {
    float dx = x1 - x2;
    float dy = y1 - y2;
    return sqrt(dx * dx + dy * dy);
}

	void kmeans(struct Country countries[], int data_count, float centroids[][2], int k) {
    int i, j, iter, min_index;
    float min_distance, distance;
    int changed;
    int cluster_size[k];

    // K�melerin ba�lang��ta bo� oldu�unu ve eleman say�lar�n�n 0 oldu�unu belirtiyoruz
    for (i = 0; i < k; i++) {
        centroids[i][0] = 0;
        centroids[i][1] = 0;
        cluster_size[i] = 0;
    }

    // Verileri k�melere atama i�lemini ger�ekle�tiriyoruz
    for (i = 0; i < data_count; i++) {
        min_distance = calculate_distance(countries[i].point[0], countries[i].point[1], centroids[0][0], centroids[0][1]);
        min_index = 0;

        for (j = 1; j < k; j++) {
            distance = calculate_distance(countries[i].point[0], countries[i].point[1], centroids[j][0], centroids[j][1]);
            if (distance < min_distance) {
                min_distance = distance;
                min_index = j;
            }
        }

        // K�me numaras�n� g�ncelliyoruz
        if (countries[i].cluster != min_index) {
            countries[i].cluster = min_index;
            changed = 1;
        }

        // K�menin ortalamas�n� g�ncelliyoruz
        centroids[min_index][0] += countries[i].point[0];
        centroids[min_index][1] += countries[i].point[1];
        cluster_size[min_index]++;
    }

    // K�me merkezlerini g�ncelliyoruz
    for (i = 0; i < k; i++) {
        if (cluster_size[i] > 0) {
            centroids[i][0] /= cluster_size[i];
            centroids[i][1] /= cluster_size[i];
        }
    }
}

int main() {
    struct Country countries[MAX_DATA_COUNT];
    float centroids[MAX_ITERATIONS][2];

    FILE *file = fopen("C:\\Users\\bilic\\OneDrive\\Masa�st�\\Harun\\AlgoritmaKmeans\\Veri.txt", "r");
    if (file == NULL) {
        printf("Dosya acilamadi.\n");
        return 1;
    }

    int data_count = 0;
    char line[100];
    while (fgets(line, sizeof(line), file)) {
        sscanf(line, "%[^,],%f,%f,%f,%f,%d,%f,%f,%f,%d\n",
               countries[data_count].name,
               &countries[data_count].child_mort,
               &countries[data_count].exports,
               &countries[data_count].health,
               &countries[data_count].imports,
               &countries[data_count].income,
               &countries[data_count].inflation,
               &countries[data_count].life_expec,
               &countries[data_count].total_fer,
               &countries[data_count].gdpp);

        countries[data_count].cluster = -1; // K�me numaras�n� ba�lang��ta -1 olarak belirliyoruz
        countries[data_count].point[0] = countries[data_count].child_mort;
        countries[data_count].point[1] = countries[data_count].income;

        data_count++;
    }

    fclose(file);

    // K�meleme i�in ba�lang�� merkezlerini rastgele se�iyoruz
    srand(42); // Rastgelelik i�in seed de�erini ayarl�yoruz
    int i;
    for (i = 0; i < MAX_ITERATIONS; i++) {
        int index = rand() % data_count;
        centroids[i][0] = countries[index].point[0];
        centroids[i][1] = countries[index].point[1];
    }

    int k = 5; // K�me say�s�
    int iter = 0;

    do {
        kmeans(countries, data_count, centroids, k);
        iter++;
    } while (iter < MAX_ITERATIONS);

    // Elde edilen k�me bilgilerini ekrana yazd�r�yoruz
    for (i = 0; i < data_count; i++) {
        printf("%s - Cluster %d\n", countries[i].name, countries[i].cluster);
    }

    return 0;
}


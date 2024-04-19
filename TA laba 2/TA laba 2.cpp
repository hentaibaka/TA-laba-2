#include <iostream>
#include <fstream>
#include <string>
#include <random>
#include <algorithm>
#include <chrono>

void bubbleSort(int mas[], int size) {
    int i, j;
    for (i = 0; i < size - 1; i++) {
        for (j = 0; j < size - i - 1; j++) {
            if (mas[j] > mas[j + 1]) {
                std::swap(mas[j], mas[j + 1]);
            } 
        }
    }
}
void selectionSort(int mas[], int size) {
    int iMin;
    int temp;
    for (int i = 0; i < size - 1; i++) {
        iMin = i;
        for (int j = i + 1; j < size; j++) {
            if (mas[j] < mas[iMin]) iMin = j;
        }
        temp = mas[i];
        mas[i] = mas[iMin];
        mas[iMin] = temp;
    }
 }
void insertionSort(int mas[], int size) {
    int temp, iCur;
    for (int i = 1; i < size; i++) {
        temp = mas[i];
        iCur = i - 1;
        while (iCur >= 0 && mas[iCur] > temp) {
            mas[iCur + 1] = mas[iCur];
            mas[iCur] = temp;
            iCur--;
        }
    }
}
void countingSort(int mas[], int size) {
    int s = 1;
    int tempMas[2001];
    for (int i = 0; i < 2001; i++) tempMas[i] = 0;
    for (int i = 0; i < size; i++) tempMas[mas[i] + 1000]++;
    int j = 0;
    for (int i = 0; i < 2001; i++) {
        for (; tempMas[i] > 0; tempMas[i]--) mas[j++] = i - 1000;
    }
}
void merge(int mas[], int temp[], int i, int l, int size)
{
    int p = i;

    int k = i; //left mas start
    int j = l + i; // right mas start  

    int n1 = std::min(j, size); // left mas end
    int n2 = std::min(j + l, size); // right mas end

    while (k < n1 || j < n2) {
        if (k >= n1 && j < n2) { // если левый массив закончился, а правй нет
            temp[p] = mas[j];
            p++;
            j++;
        }
        else if (j >= n2 && k < n1) { // если правй массив закончился, а левый нет
            temp[p] = mas[k];
            p++;
            k++;
        }
        else if (k < n1 && j < n2) { // если массивы не закончились
            if (mas[k] < mas[j]) {
                temp[p] = mas[k];
                k++;
            }
            else {
                temp[p] = mas[j];
                j++;
            }
            p++;
        }
    }    
}
void mergeSort(int mas[], int size) {
    int* temp = new int[size];
    int l = 1;
    while (l < size) {
        for (int i = 0; i < size; i += 2 * l)  
            merge(mas, temp, i, l, size);
        l *= 2;
        for (int i = 0; i < size; i += 2 * l) 
            merge(temp, mas, i, l, size);
        l *= 2;
    }
}
void quickSort(int mas[], int left, int right) {
    int l = left, r = right;
    int mid = mas[(l + r) / 2];
    while (l < r) {
        while (mas[l] < mid) l++;
        while (mas[r] > mid) r--;
        if (l <= r) std::swap(mas[r--], mas[l++]);
    }
    if (r > left) quickSort(mas, left, r);
    if (l < right) quickSort(mas, l, right);
}
void quickSort2(int mas[], int size) {
    int i = 0;
    int j = size - 1;
    int mid = mas[size / 2];
    do {
        while (mas[i] < mid) i++;
        while (mas[j] > mid) j--;
        if (i <= j) std::swap(mas[i++], mas[j--]);

    } while (i <= j);
    if (j > 0) quickSort2(mas, j + 1);
    if (i < size) quickSort2(&mas[i], size - i);
}
long long timeIt(void (*func)(int[], int), int mas[], int size) {
    auto start_time = std::chrono::steady_clock::now();

    (*func)(mas, size);

    auto end_time = std::chrono::steady_clock::now();
    auto elapsed_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time);
    return elapsed_ns.count();
}
void printMas(int mas[], int size, std::string name, long long time, bool flag = true) {
    std::cout << name << ":(" << time << "ns)" << std::endl;
    if (flag) {
        for (int i = 0; i < size; i++) {
            std::cout << mas[i] << " ";
        }
    }
    std::cout << std::endl;
}
void generateMas(int mas[], int size) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(-1000, 1000);

    for (int i = 0; i < size; i++)
        mas[i] = dist(gen);
}
void test() {
    int* mas = new int[10] { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

    std::cout << mas << " " << &mas[0] << std::endl; // ссылка на массив это ссылка на его первый элемент

    int* m = &mas[5];
    int* k = m;
    std::cout << m << " " << *m << " " << &m << " " << m[1]; // указатель 6 ссылка 7
    int j = 2;
    int* l = &j;
    std::cout << l << " " << *l << " " << &l << " " << l[1]; // указатель 2 ссылка -858993460 
                                                    //(в дебаг режиме это неинициализированная переменная)
}

int main() {
    setlocale(LC_ALL, "rus");

    std::string inputString;
    int size;
    int* origMas;
    int* mas;
    long long time = 0;
    //================

    bool log = true;

    //================

    std::ifstream reader("input.txt");
    
    reader >> inputString;

    try 
    {
        size = stoi(inputString);
    }
    catch(std::invalid_argument ex) 
    {
        std::cout << "неправильно залан размер массива" << std::endl;
    }

    origMas = new int[size];
    mas = new int[size];

    generateMas(origMas, size);

    printMas(origMas, size, "без сортировки", time, log);

    //=====================================================

    delete[] mas;
    mas = new int[size];
    std::copy_n(origMas, size, mas);
    void (*func1)(int[], int) = &bubbleSort;
    time = timeIt(func1, mas, size);

    printMas(mas, size, "пузырьковая сортировка", time, log);

    //=====================================================

    delete[] mas;
    mas = new int[size];
    std::copy_n(origMas, size, mas);
    void (*func2)(int[], int) = &selectionSort;
    time = timeIt(func2, mas, size);

    printMas(mas, size, "сортировка выбором", time, log);

    //=====================================================

    delete[] mas;
    mas = new int[size];
    std::copy_n(origMas, size, mas);
    void (*func3)(int[], int) = &insertionSort;
    time = timeIt(func3, mas, size);

    printMas(mas, size, "сортировка вставками", time, log);

    //=====================================================

    delete[] mas;
    mas = new int[size];
    std::copy_n(origMas, size, mas);
    void (*func4)(int[], int) = &countingSort;
    time = timeIt(func4, mas, size);

    printMas(mas, size, "сортировка подсчётом", time, log);

    //=====================================================

    delete[] mas;
    mas = new int[size];
    std::copy_n(origMas, size, mas);
    void (*func5)(int[], int) = &mergeSort;
    time = timeIt(func5, mas, size);

    printMas(mas, size, "сортировка слиянием", time, log);

    //=====================================================

    delete[] mas;
    mas = new int[size];
    std::copy_n(origMas, size, mas);

    auto start_time = std::chrono::steady_clock::now();

    quickSort(mas, 0, size - 1);

    auto end_time = std::chrono::steady_clock::now();
    auto elapsed_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time);
    time = elapsed_ns.count();



    printMas(mas, size, "быстрая сортировка", time, log);

    //=====================================================

    //delete[] mas;
    //mas = new int[size];
    //std::copy_n(origMas, size, mas);
    //void (*func6)(int[], int) = &quickSort2;
    //time = timeIt(func6, mas, size);

    //printMas(mas, size, "быстрая сортировка 2", time, log);

    //=====================================================

    delete[] mas;

    //test();

    return 0;
}
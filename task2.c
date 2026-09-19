#include <stdio.h>
#include <windows.h>

/* меняет значения переменных без 3ей переменной */
void swap(int *a, int *b) {
    *a ^= *b;
    *b ^= *a;
    *a ^= *b;
}

/* выводит матрицу в табличном виде */
void print_matrix(size_t rows, size_t cols, const int matrix[rows][cols]) {
    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

/* возвращает указатель на макс значение и записывает строчку и колонку по адресам указателей */
const int *find_matrix_max(size_t rows, size_t cols, const int matrix[rows][cols], size_t *max_row, size_t *max_col) {
    const int *ptr_max = matrix[0];
    for (size_t i = 0; i < rows; i++) {
        const int * row = matrix[i];
        for (const int * ptr = row; (size_t)(ptr - row) < cols; ptr++) {
            if (*ptr > *ptr_max) {
                ptr_max = ptr;
                *max_row = i;
                *max_col = (size_t) (ptr - row);
            }
        }
    }
    return ptr_max;
}

/*среднее арифметическое строки*/
double row_average(size_t cols, const int *row) {
    long long sum = 0;
    for (const int *ptr = row; (size_t) (ptr - row) < cols; ptr++) sum += *ptr;
    return (double) sum / (double) cols;
}

/* среднее арифметическое матрицы */
double matrix_average(size_t rows, size_t cols, const int matrix[rows][cols]) {
    double average = 0;
    short flag = 0;
    for (size_t i = 0; i < rows; i++) {
        const int * row = matrix[i];
        average += row_average(cols, row);
    }
    return average / (double) rows;
}

/* количество значений выше среднего */
size_t count_above_average(size_t rows, size_t cols, const int matrix[rows][cols], double average) {
    size_t counter = 0;
    for (size_t i = 0; i < rows; i++) {
        const int * row = matrix[i];
        for (const int * ptr = row; (size_t)(ptr - row) < cols; ptr++) {
            if ((double)*ptr > average) counter++;
        }
    }
    return counter;
}

/* строка с наибольшим средним значением */
size_t hottest_row(size_t rows, size_t cols, const int matrix[rows][cols]) {
    size_t hot_row = 0;
    double hot_average = row_average(cols, matrix[0]);

    for (size_t i = 1; i < rows; i++) {
        double local_average = row_average(cols, matrix[i]);
        if (local_average > hot_average){
            hot_average = local_average;
            hot_row = i;
        }
    }
    return hot_row;
}

/* заполняет строку её средним значением */
void fill_row_with_average(size_t cols, int *row) {
    int average = (int) row_average(cols, row);
    for (int * ptr = row; (size_t) (ptr - row) < cols; ptr++) {
        *ptr = average;
    }
}

/* находит максимальную разницу между соседями */
int max_neighbor_difference(size_t rows, size_t cols, const int matrix[rows][cols]) {
    int max_dif = 0;
    for (size_t i = 0; i < rows; i++) {
        const int * row = matrix[i];
        for (const int * ptr = row; (size_t)(ptr - row) < cols - 1; ptr++) {
            if (*(ptr + 1) - *ptr > max_dif) max_dif = *(ptr + 1) - *ptr;
            if (*ptr - *(ptr + 1) > max_dif) max_dif = *ptr - *(ptr + 1);
        }
    }
    return max_dif;
}

int main(void) {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    
    size_t rows, cols;
    if (scanf("%d%d", &rows, &cols) != 2) {
        printf("не получили числа верно");
        return 1;
    }

    int temperatures[rows][cols];

    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++) {
            if (scanf("%d", &temperatures[i][j]) != 1) {
                printf("не получили числа верно");
                return 1;
            }
        }
    }
    
    printf("Исходная карта:\n");
    print_matrix(rows, cols, temperatures);
    printf("\n");

    const int *ptr_max;
    size_t max_col, max_row;
    ptr_max = find_matrix_max(rows, cols, temperatures, &max_row, &max_col);
    printf("Максимальная температура: %d\nСтрока: %d\nСтолбец: %d\n", *ptr_max, max_row, max_col);

    double average = matrix_average(rows, cols, temperatures);

    printf("Средняя температура карты: %.2f\nКоличество значений выше среднего: %d\n", average, count_above_average(rows, cols, temperatures, average));
    size_t i = hottest_row(rows, cols, temperatures);
    printf("Строка с максимальным средним: %d\nСреднее выбранной строки: %.2f\nКарта после замены выбранной строки:\n", i, row_average(cols, temperatures[i]));
    fill_row_with_average(cols, temperatures[i]);
    print_matrix(rows, cols, temperatures);
    printf("Максимальная разница соседних температур: %d\n", max_neighbor_difference(rows, cols, temperatures));

    return 0;
}
#include "main.h"

char* getUserStringPtr(char* inputString) {
	char* temp = (char*)malloc(MAX_FIELD_LENGTH * sizeof(char));
	if (temp == NULL) {
		puts("Недостаточно памяти в куче для выделения временной строки для пользовательского ввода");
		return NULL;
	}
	printf(inputString);
	gets_s(temp, MAX_FIELD_LENGTH);
	// Выведем перенос строки после считывания
	putchar('\n');

	// Получаем длину введенной пользователем строки и создаем под нее массив размеров в это число символов (для экономии памяти)
	size_t sLen = strlen(temp);
	char* resultString = (char*)malloc((sLen + 1) * sizeof(char));
	if (resultString == NULL) {
		puts("Недостаточно свободной памяти в куче для выделения строки для пользовательского ввода");
		return NULL;
	}
	strcpy(resultString, temp);
	// Не забываем очистить память, выделенную под тестовую строку
	free(temp);

	return resultString;
}

double getUserNumber(char* inputString) {
	double number = 0.0;
	printf(inputString);
	size_t scanned = scanf("%lf", &number);
	if (scanned == 0) {
		puts("\nОшибка считывания числа.\n");
		// Очищаем поток
		while (getchar() != '\n');
		return 0;
	}

	getchar();
	putchar('\n');

	return number;
}

void printBook(Book* bookPtr) {
	if (bookPtr == NULL) return;

	printf("\nНазвание книги: %s                                                                                 \n"
		"Автор книги: %s                                                                                    \n"
		"Жанр произведения: %s                                                                              \n"
		"Год написания: %d                                                                                  \n"
		"Издательство: %s                                                                                   \n"
		"Оценка читателей: %.1f                                                                               \n"
		"Цена книги: %u                                                                                     \n"
		"Краткое описание сюжета: %s                                                                        \n\n"
		"ID книги: %u                                                                                       \n\n"
		"----------------------------------------------------------------------------------------------------\n\n",
		bookPtr->title, bookPtr->author, bookPtr->genre, bookPtr->issueYear, bookPtr->publishingHouse, bookPtr->readersScore,
		bookPtr->price, bookPtr->shortDescription, bookPtr->id);
}

char intToChar(int digit) {
	return '0' + ((digit > 0 && digit < 10) ? digit : 0);
}

char* getStringFromFile(FILE* inputFile) {
	// Временная переменная для сохранения строк, считанных из файла
	char* temp = (char*)malloc((MAX_FIELD_LENGTH + 1) * sizeof(char));
	if (temp == NULL) {
		puts("\nОшибка: недостаточно памяти для сохранения полей книги из файла.\n");
		return NULL;
	}

	// Считываем строку из файла во временную переменную
	fgets(temp, MAX_FIELD_LENGTH, inputFile);

	// Если после считывания строки в temp он равен NULL
	if (temp == NULL) {
		puts("\nОшибка: неудача при считывании файла или достигнут конец файла.\n");
		return NULL;
	}


	// Получаем длину считанной из файла строки и создаем под нее массив размеров в это число символов (для экономии памяти)
	size_t sLen = strlen(temp);
	// Удаляем считанный символ переноса строки и заменяем его на символ конца строки
	temp[sLen - 1] = '\0';

	char* resultString = (char*)malloc(sLen * sizeof(char));
	if (resultString == NULL) {
		puts("Недостаточно свободной памяти в куче для выделения строки для пользовательского ввода");
		return NULL;
	}
	strcpy(resultString, temp);
	// Не забываем очистить память, выделенную под тестовую строку
	free(temp);

	return resultString;
}
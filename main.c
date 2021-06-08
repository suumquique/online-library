#include "main.h"

// Текущее айди книги для добавления, глобальная переменная, чтобы все функции имели к ней доступ и не было коллизий
unsigned int id = 0;

void main() {
	SetConsoleCP(RUS_ENCODING);
	SetConsoleOutputCP(RUS_ENCODING);

	/* Перечисление возможных действий с программой: добавление книги, удаление, редактирование, поиск и вывод, сохранение
	в базу данных и загрузка из нее, вывод справки и выход из программы. С самого начала стоит в значении "Показать справку",
	чтобы пользователю при запуске как минимум один раз её показало. */
	enum Actions { ADD = 1, REMOVE, EDIT, SEARCH, SAVE, DOWNLOAD, PRINT_REFERENCE, EXIT } action = PRINT_REFERENCE;
	
	while (action != EXIT) {
		switch (action) {
			case PRINT_REFERENCE:
				usage();
				break;
			case ADD: {
				Book* newBookPtr = createBook(); 
				insertInAllTrees(newBookPtr);
			}
			case EXIT:
				exit(ERROR_SUCCESS);
			default:
				puts("Вы указали несуществующее действие.\n");
				usage();
				break;
		}

		printf("Какое действие выполнить следующим? > ");
		scanf("%u", &action);
		getchar();
		putchar('\n');
	}
	
}


void usage(){
	printf("\t----------------------Выберите, что нужно сделать:-----------------------\n"
		"\t| 1. Добавить книгу в онлайн-библиотеку                                |\n"
		"\t| 2. Удалить книгу из библиотеки                                       |\n"
		"\t| 3. Изменить информацию о текущей книге (любое поле)                  |\n"
		"\t| 4. Найти книгу по любому полю (автору, жанру и так далее)            |\n"
		"\t| 5. Сохранить все книги из текущей библиотеки в базу данных           |\n"
		"\t| 6. Загрузить все книги из базы данных (файла) в библиотеку           |\n"
		"\t| 7. Снова показать эту справку                                        |\n"
		"\t| 8. Выйти из программы                                                |\n"
		"\t-------------------------------------------------------------------------\n\n");
}

Book* createBook() {
	puts("Так, вы создаете страницу с информацией о новой книге. Давайте выполним пару простых шагов: \n");

	Book* newBook = (Book*) calloc(1, sizeof(Book));
	newBook->title = getUserStringPtr("1. Введите название книги > ");
	newBook->author = getUserStringPtr("2. Укажите автора добавляемой книги > ");
	newBook->publishingHouse = getUserStringPtr("3. Укажите, какое издательство выпустило книгу >");
	newBook->genre = getUserStringPtr("4. Укажите жанр книги > ");
	newBook->shortDescription = getUserStringPtr("5. Напишите краткое описание сюжета к добавляемой книге > ");

	printf("6. Введите цену книги в рублях (число, без дополнительных обозначений) > ");
	scanf("%u", &(newBook->price));
	getchar();
	putchar('\n');
	
	printf("7. Введите информацию о годе написания книги (целое число, если до нашей эры - вводить со знаком минус) > ");
	scanf("%d", &(newBook->issueYear));
	getchar();
	putchar('\n');

	printf("8. Введите оценку книги читателями (по шкале от 0 до 10, дробные оценки допустимы) > ");
	scanf("%f", &(newBook->readersScore));
	getchar();
	putchar('\n');

	// Ставим уникальный id и увеличиваем глобальную переменную, чтобы у следующей книги был другой id
	newBook->id = id++;

	return newBook;
}

char* getUserStringPtr(char* inputString) {
	char* temp = (char*)malloc(MAX_FIELD_LENGTH * sizeof(char));
	printf(inputString);
	gets_s(temp, MAX_FIELD_LENGTH);
	// Выведем перенос строки после считывания
	putchar('\n');

	// Получаем длину введенной пользователем строки и создаем под нее массив размеров в это число символов (для экономии памяти)
	size_t sLen = strlen(temp);
	char* resultString = (char*)malloc((sLen + 1) * sizeof(char));
	strcpy(resultString, temp);
	// Не забываем очистить память, выделенную под тестовую строку
	free(temp);

	return resultString;
}

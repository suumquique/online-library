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
				puts("\nКнига успешно сохранена в библиотеку\n\n");
				break;
			}
			case SEARCH: {
				Book* searchedBookPtr = searchBook();
				puts("Информация о книге, которую вы искали:\n");
				printBook(searchedBookPtr);
				break;
			}
			case REMOVE: {
				puts("\t\t\t\t  Сначала найдем книгу, которую будем удалять.  \t\t\t\t \n");
				Book* bookToRemovePtr = searchBook();
				deleteFromAllTrees(bookToRemovePtr);
				puts("Книга успешно удалена из библиотеки!\n");
				break;
			}
			case EDIT: {
				DWORD retCode = editBook();
				if (retCode != ERROR_SUCCESS) {
					puts("\nОшибка редактирования книги.\n\n");
				}
				else puts("\nКнига успешно отредактирована.\n\n");
				break; 
			}
			case EXIT:
				exit(ERROR_SUCCESS);
			default:
				puts("Вы указали несуществующее действие.\n");
				usage();
				break;
		}

		action = getUserNumber("Какое действие выполнить следующим? > ");
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
	if (newBook == NULL) {
		puts("Ошибка создания книги: недостаточно памяти\n");
		return NULL;
	}

	newBook->title = getUserStringPtr("1. Введите название книги > ");
	newBook->author = getUserStringPtr("2. Укажите автора добавляемой книги > ");
	newBook->publishingHouse = getUserStringPtr("3. Укажите, какое издательство выпустило книгу > ");
	newBook->genre = getUserStringPtr("4. Укажите жанр книги > ");
	newBook->shortDescription = getUserStringPtr("5. Напишите краткое описание сюжета к добавляемой книге > ");
	newBook->price = (unsigned) getUserNumber("6. Введите цену книги в рублях (число, без дополнительных обозначений) > ");
	newBook->issueYear = (int) getUserNumber("7. Введите информацию о годе написания книги (целое число, если до нашей эры - вводить со знаком минус) > ");
	newBook->readersScore = (float) getUserNumber("8. Введите оценку книги читателями (по шкале от 0 до 10, дробные оценки допустимы) > ");

	// Ставим уникальный id и увеличиваем глобальную переменную, чтобы у следующей книги был другой id
	newBook->id = id++;

	return newBook;
}

Book* searchBook(void) {
	
	enum searchTypes searchType;

	printf("\t-------Перед началом поиска требуется определить, по какому полю в информации о книге искать--------\n"
		"\t|                                                                                                  |\n"
		"\t| 1. Искать по названию книги                                                                      |\n"
		"\t| 2. Искать по автору книги                                                                        |\n"
		"\t| 3. Искать по году написания книги                                                                |\n"
		"\t| 4. Найти книгу по цене в рублях (целое число)                                                    |\n"
		"\t| 5. Найти книгу по жанру                                                                          |\n"
		"\t| 6. Найти книгу по оценкам читателей (возможны дробные, от 0 до 10)                               |\n"
		"\t| 7. Найти книгу по названию издательства                                                          |\n"
		"\t|                                                                                                  |\n"
		"\t----------------------------------------------------------------------------------------------------\n\n");

	// Переменная, в которую запишется выбор пользователя (по какому полю в структуре книги искать)
	unsigned choice = (unsigned) getUserNumber("Введите цифру из таблице выше для поиска по определенному полю > ");

	// Если введен некорректный пункт, говорим пользователю об ошибке и спрашиваем его еще раз
	if (choice < 1 || choice > 7) {
		puts("Вы указали несуществующий параметр поиска. Попробуйте ещё раз.");
		return searchBook();
	}

	// Поскольку нумерация в searchTypes идет с нуля, а выбор - с единицы, вычитаем единицу
	searchType = choice - 1;
	putchar('\n');

	/* Создаем пустой экземпляр книги для поиска. Суть в том, что функция search, которая работает напрямую с выбранным деревом,
	* для универсальности при поиске по любому из полей структуры книги принимает в качестве аргумента целую книгу и сравнивает нужное поле
	* с книгами в дереве для поиска книг с аналогичным полем */
	Book* searchBook = (Book*)calloc(1, sizeof(Book));
	if (searchBook == NULL) {
		puts("\nОшибка создания экземляра книги для поиска: недостаточно памяти.\n");
		return NULL;
	}

	switch (searchType) {
	case SEARCH_BY_AUTHOR:
		searchBook->author = getUserStringPtr("Введите автора искомой книги > ");
		break;
	case SEARCH_BY_GENRE:
		searchBook->genre = getUserStringPtr("Введите жанр для поиска > ");
		break;
	case SEARCH_BY_TITLE:
		searchBook->title = getUserStringPtr("Введите название искомой книги > ");
		break;
	case SEARCH_BY_PUBLISHING_HOUSE:
		searchBook->publishingHouse = getUserStringPtr("Введите название издательства для поиска > ");
		break;
	case SEARCH_BY_PRICE: {
		searchBook->price = (unsigned)getUserNumber("Введите цену книги в рублях для поиска (число, без дополнительных обозначений) > ");
		break;
	}
	case SEARCH_BY_SCORE: {
		searchBook->readersScore = getUserNumber("Введите оценку книги читателями для поиска (по шкале от 0 до 10, дробные оценки допустимы) > ");
		break;
	}
	case SEARCH_BY_YEAR: {
		searchBook->issueYear = (int)getUserNumber("Введите год написания книги для поиска (целое число, если до нашей эры - вводить со знаком минус) > ");
		break;
	}
	default:
		puts("Неизвестная ошибка при поиске");
		return NULL;
	}

	// Ищем по требуемому дереву и в качестве ответа получаем ссылку на вершину стека с подходящими под наш запрос книгами
	bookStackNode* searchResultStack = search(trees[searchType], searchBook, searchType);

	// Очистка книги поиска после выполнения поиска
	if (searchBook->author != NULL) free(searchBook->author);
	else if (searchBook->genre != NULL) free(searchBook->genre);
	else if (searchBook->title != NULL) free(searchBook->title);
	else if (searchBook->publishingHouse != NULL) free(searchBook->publishingHouse);

	// Поля с числовыми значениями освобождать не надо, поэтому можем удалять сам экземпляр книги (структуру)
	free(searchBook);

	// Если книга не нашлась, выдаем сообщение об ошибке
	if (searchResultStack == NULL) {
		puts("К сожалению, ничего на нашлось (");
		return NULL;
	}

	// Если нашлась только одна книга, сразу же возвращаем ее
	if (searchResultStack->nextNode == NULL) {
		return searchResultStack->currentBook;
	}

	// Во всех иных случаях в стеке больше чем одна книга, а следовательно нужно вывести все
	puts("\nСписок книг, найденных по вашему запросу: \n");
	for (bookStackNode* currentStackElement = searchResultStack; currentStackElement != NULL; currentStackElement = currentStackElement->nextNode) {
		printBook(currentStackElement->currentBook);
	}

	unsigned searchedID = getUserNumber("Введите id искомой книги(из представленного выше списка книг) > ");
	for (bookStackNode* currentStackElement = searchResultStack; ; currentStackElement = currentStackElement->nextNode) {
		if (currentStackElement == NULL) {
			puts("Вы указали несуществующий id книги\n");
			return NULL;
		}

		// Если мы дошли до книги с введенным пользователем id, возвращаем ее
		if (currentStackElement->currentBook->id == searchedID) {
			return currentStackElement->currentBook;
		}
	};
}

/* Функция, проверяющая, требуется ли редактировать определенное поле книги. 
Принимает в качестве параметров строку, в которой указаны номера редактируемых полей, и проверяемое поле */
bool needToEditField(char* stringWithFieldsToEdit, enum searchTypes fieldType) {
	return strchr(stringWithFieldsToEdit, intToChar(fieldType + 1)) != NULL;
}

DWORD editBook(void) {
	puts("\t\t\t\t  Сначала найдем книгу, которую будем редактировать.  \t\t\t\t \n");
	Book* bookToEdit = searchBook();

	// Если книга не нашлась, возвращаем ошибку
	if (bookToEdit == NULL) {
		puts("К сожалению, отредактировать несуществующую книгу нельзя.\n");
		return ERROR_NODE_NOT_AVAILABLE;
	}
	else printBook(bookToEdit);

	// Создаем экземляр книги, которую будем редактировать, и копируем в неё информацию всех полей редактируемой книги
	Book* editedBook = (Book*)malloc(sizeof(Book));
	if (editedBook == NULL) {
		puts("Недостаточно памяти для создания нового экземляра книги");
		return ERROR_NOT_ENOUGH_MEMORY;
	}
	memcpy(editedBook, bookToEdit, sizeof(Book));
	char* authorCopy = (char*) malloc((strlen(bookToEdit->author) + 1) * sizeof(char));
	char* titleCopy = (char*) malloc((strlen(bookToEdit->title) + 1) * sizeof(char));
	char* genreCopy = (char*) malloc((strlen(bookToEdit->genre) + 1) * sizeof(char));
	char* publishingHouseCopy = (char*) malloc((strlen(bookToEdit->publishingHouse) + 1) * sizeof(char));
	char* shortDescriptionCopy = (char*)malloc((strlen(bookToEdit->shortDescription) + 1) * sizeof(char));
	if (authorCopy == NULL || titleCopy == NULL || publishingHouseCopy == NULL || shortDescriptionCopy == NULL || genreCopy == NULL) {
		puts("Недостаточно памяти для заполнения полей отредактированной книги");
		return ERROR_NOT_ENOUGH_MEMORY;
	}
	strcpy(authorCopy, bookToEdit->author);
	strcpy(titleCopy, bookToEdit->title);
	strcpy(genreCopy, bookToEdit->genre);
	strcpy(publishingHouseCopy, bookToEdit->publishingHouse);
	strcpy(shortDescriptionCopy, bookToEdit->shortDescription);
	editedBook->author = authorCopy;
	editedBook->title = titleCopy;
	editedBook->genre = genreCopy;
	editedBook->publishingHouse = publishingHouseCopy;
	editedBook->shortDescription = shortDescriptionCopy;

	// Удаляем полученную книгу, так как у нас уже есть копия, которую мы будем редактировать
	deleteFromAllTrees(bookToEdit);

	puts("\nТеперь перейдем к редактированию выбранной книги.\n");

	printf("\t----------------------------------------------------------------------------------------------------\n"
		"\t|                     Сначала определим, какие поля требуется редактировать.                       |\n"
		"\t|     Для редактирования нескольких полей одновременно укажите несколько чисел (например, 145)     |\n"
		"\t|                                                                                                  |\n"
		"\t| 1. Отредактировать название книги                                                                |\n"
		"\t| 2. Отредактировать автора книги                                                                  |\n"
		"\t| 3. Отредактировать год написания книги                                                           |\n"
		"\t| 4. Отредактировать цену книги                                                                    |\n"
		"\t| 5. Отредактировать жанр книги                                                                    |\n"
		"\t| 6. Отредактировать оценку книги читателями                                                       |\n"
		"\t| 7. Отредактировать название издательства книги                                                   |\n"
		"\t| 8. Отредактировать краткое описание книги                                                        |\n"
		"\t|                                                                                                  |\n"
		"\t----------------------------------------------------------------------------------------------------\n\n");

	char* fieldsToEdit = getUserStringPtr("Введите цифры, соответствующие полям, которые вы собираетесь редактировать > ");

	if (needToEditField(fieldsToEdit, SEARCH_BY_TITLE)) {
		char* newTitle = getUserStringPtr("Введите отредактированное (обновленное) название книги > ");
		free(editedBook->title);
		editedBook->title = newTitle;
	}

	if (needToEditField(fieldsToEdit, SEARCH_BY_AUTHOR)) {
		char* newAuthor = getUserStringPtr("Корректно введите нового автора книги > ");
		free(editedBook->author);
		editedBook->author = newAuthor;
	}

	if (needToEditField(fieldsToEdit, SEARCH_BY_GENRE)) {
		char* newGenre = getUserStringPtr("Введит исправленный (новый) жанр книги > ");
		free(editedBook->genre);
		editedBook->genre = newGenre;
	}

	if (needToEditField(fieldsToEdit, SEARCH_BY_PUBLISHING_HOUSE)) {
		char* newPublishingHouse = getUserStringPtr("Введите корректное (новое) название издательства > ");
		free(editedBook->publishingHouse);
		editedBook->publishingHouse = newPublishingHouse;
	}

	if (needToEditField(fieldsToEdit, SEARCH_BY_SHORT_DESCRIPTION)) {
		char* newShortDescription = getUserStringPtr("Введите новое краткое описание книги > ");
		free(editedBook->shortDescription);
		editedBook->shortDescription = newShortDescription;
	}

	if (needToEditField(fieldsToEdit, SEARCH_BY_PRICE)) 
		editedBook->price = (unsigned) getUserNumber("Введите новую цену книги > ");

	if (needToEditField(fieldsToEdit, SEARCH_BY_YEAR)) 
		editedBook->issueYear = (int) getUserNumber("Введите исправленный год выхода книги > ");

	if (needToEditField(fieldsToEdit, SEARCH_BY_SCORE)) 
		editedBook->readersScore = (float) getUserNumber("Введите новую оценку книги читателями (от 0 до 10) > ");

	free(fieldsToEdit);

	// Вставляем отредактированную книгу в дерево
	insertInAllTrees(editedBook);

	return ERROR_SUCCESS;
}
#include "main.h"

// ������� ���� ����� ��� ����������, ���������� ����������, ����� ��� ������� ����� � ��� ������ � �� ���� ��������
unsigned int id = 0;

void main() {
	SetConsoleCP(RUS_ENCODING);
	SetConsoleOutputCP(RUS_ENCODING);

	/* ������������ ��������� �������� � ����������: ���������� �����, ��������, ��������������, ����� � �����, ����������
	� ���� ������ � �������� �� ���, ����� ������� � ����� �� ���������. � ������ ������ ����� � �������� "�������� �������",
	����� ������������ ��� ������� ��� ������� ���� ��� � ��������. */
	enum Actions { ADD = 1, REMOVE, EDIT, SEARCH, SAVE, DOWNLOAD, PRINT_REFERENCE, EXIT } action = PRINT_REFERENCE;
	
	while (action != EXIT) {
		switch (action) {
			case PRINT_REFERENCE:
				usage();
				break;
			case ADD: {
				Book* newBookPtr = createBook(); 
				insertInAllTrees(newBookPtr);
				puts("\n����� ������� ��������� � ����������\n\n");
				break;
			}
			case SEARCH: {
				Book* searchedBookPtr = searchBook();
				puts("���������� � �����, ������� �� ������:\n");
				printBook(searchedBookPtr);
				break;
			}
			case REMOVE: {
				puts("\t\t\t\t  ������� ������ �����, ������� ����� �������.  \t\t\t\t \n");
				Book* bookToRemovePtr = searchBook();
				deleteFromAllTrees(bookToRemovePtr);
				puts("����� ������� ������� �� ����������!\n");
				break;
			}
			case EDIT: {
				DWORD retCode = editBook();
				if (retCode != ERROR_SUCCESS) {
					puts("\n������ �������������� �����.\n\n");
				}
				else puts("\n����� ������� ���������������.\n\n");
				break; 
			}
			case EXIT:
				exit(ERROR_SUCCESS);
			default:
				puts("�� ������� �������������� ��������.\n");
				usage();
				break;
		}

		action = getUserNumber("����� �������� ��������� ���������? > ");
	}
	
}


void usage(){
	printf("\t----------------------��������, ��� ����� �������:-----------------------\n"
		"\t| 1. �������� ����� � ������-����������                                |\n"
		"\t| 2. ������� ����� �� ����������                                       |\n"
		"\t| 3. �������� ���������� � ������� ����� (����� ����)                  |\n"
		"\t| 4. ����� ����� �� ������ ���� (������, ����� � ��� �����)            |\n"
		"\t| 5. ��������� ��� ����� �� ������� ���������� � ���� ������           |\n"
		"\t| 6. ��������� ��� ����� �� ���� ������ (�����) � ����������           |\n"
		"\t| 7. ����� �������� ��� �������                                        |\n"
		"\t| 8. ����� �� ���������                                                |\n"
		"\t-------------------------------------------------------------------------\n\n");
}

Book* createBook() {
	puts("���, �� �������� �������� � ����������� � ����� �����. ������� �������� ���� ������� �����: \n");

	Book* newBook = (Book*) calloc(1, sizeof(Book));
	if (newBook == NULL) {
		puts("������ �������� �����: ������������ ������\n");
		return NULL;
	}

	newBook->title = getUserStringPtr("1. ������� �������� ����� > ");
	newBook->author = getUserStringPtr("2. ������� ������ ����������� ����� > ");
	newBook->publishingHouse = getUserStringPtr("3. �������, ����� ������������ ��������� ����� > ");
	newBook->genre = getUserStringPtr("4. ������� ���� ����� > ");
	newBook->shortDescription = getUserStringPtr("5. �������� ������� �������� ������ � ����������� ����� > ");
	newBook->price = (unsigned) getUserNumber("6. ������� ���� ����� � ������ (�����, ��� �������������� �����������) > ");
	newBook->issueYear = (int) getUserNumber("7. ������� ���������� � ���� ��������� ����� (����� �����, ���� �� ����� ��� - ������� �� ������ �����) > ");
	newBook->readersScore = (float) getUserNumber("8. ������� ������ ����� ���������� (�� ����� �� 0 �� 10, ������� ������ ���������) > ");

	// ������ ���������� id � ����������� ���������� ����������, ����� � ��������� ����� ��� ������ id
	newBook->id = id++;

	return newBook;
}

Book* searchBook(void) {
	
	enum searchTypes searchType;

	printf("\t-------����� ������� ������ ��������� ����������, �� ������ ���� � ���������� � ����� ������--------\n"
		"\t|                                                                                                  |\n"
		"\t| 1. ������ �� �������� �����                                                                      |\n"
		"\t| 2. ������ �� ������ �����                                                                        |\n"
		"\t| 3. ������ �� ���� ��������� �����                                                                |\n"
		"\t| 4. ����� ����� �� ���� � ������ (����� �����)                                                    |\n"
		"\t| 5. ����� ����� �� �����                                                                          |\n"
		"\t| 6. ����� ����� �� ������� ��������� (�������� �������, �� 0 �� 10)                               |\n"
		"\t| 7. ����� ����� �� �������� ������������                                                          |\n"
		"\t|                                                                                                  |\n"
		"\t----------------------------------------------------------------------------------------------------\n\n");

	// ����������, � ������� ��������� ����� ������������ (�� ������ ���� � ��������� ����� ������)
	unsigned choice = (unsigned) getUserNumber("������� ����� �� ������� ���� ��� ������ �� ������������� ���� > ");

	// ���� ������ ������������ �����, ������� ������������ �� ������ � ���������� ��� ��� ���
	if (choice < 1 || choice > 7) {
		puts("�� ������� �������������� �������� ������. ���������� ��� ���.");
		return searchBook();
	}

	// ��������� ��������� � searchTypes ���� � ����, � ����� - � �������, �������� �������
	searchType = choice - 1;
	putchar('\n');

	/* ������� ������ ��������� ����� ��� ������. ���� � ���, ��� ������� search, ������� �������� �������� � ��������� �������,
	* ��� ��������������� ��� ������ �� ������ �� ����� ��������� ����� ��������� � �������� ��������� ����� ����� � ���������� ������ ����
	* � ������� � ������ ��� ������ ���� � ����������� ����� */
	Book* searchBook = (Book*)calloc(1, sizeof(Book));
	if (searchBook == NULL) {
		puts("\n������ �������� ��������� ����� ��� ������: ������������ ������.\n");
		return NULL;
	}

	switch (searchType) {
	case SEARCH_BY_AUTHOR:
		searchBook->author = getUserStringPtr("������� ������ ������� ����� > ");
		break;
	case SEARCH_BY_GENRE:
		searchBook->genre = getUserStringPtr("������� ���� ��� ������ > ");
		break;
	case SEARCH_BY_TITLE:
		searchBook->title = getUserStringPtr("������� �������� ������� ����� > ");
		break;
	case SEARCH_BY_PUBLISHING_HOUSE:
		searchBook->publishingHouse = getUserStringPtr("������� �������� ������������ ��� ������ > ");
		break;
	case SEARCH_BY_PRICE: {
		searchBook->price = (unsigned)getUserNumber("������� ���� ����� � ������ ��� ������ (�����, ��� �������������� �����������) > ");
		break;
	}
	case SEARCH_BY_SCORE: {
		searchBook->readersScore = getUserNumber("������� ������ ����� ���������� ��� ������ (�� ����� �� 0 �� 10, ������� ������ ���������) > ");
		break;
	}
	case SEARCH_BY_YEAR: {
		searchBook->issueYear = (int)getUserNumber("������� ��� ��������� ����� ��� ������ (����� �����, ���� �� ����� ��� - ������� �� ������ �����) > ");
		break;
	}
	default:
		puts("����������� ������ ��� ������");
		return NULL;
	}

	// ���� �� ���������� ������ � � �������� ������ �������� ������ �� ������� ����� � ����������� ��� ��� ������ �������
	bookStackNode* searchResultStack = search(trees[searchType], searchBook, searchType);

	// ������� ����� ������ ����� ���������� ������
	if (searchBook->author != NULL) free(searchBook->author);
	else if (searchBook->genre != NULL) free(searchBook->genre);
	else if (searchBook->title != NULL) free(searchBook->title);
	else if (searchBook->publishingHouse != NULL) free(searchBook->publishingHouse);

	// ���� � ��������� ���������� ����������� �� ����, ������� ����� ������� ��� ��������� ����� (���������)
	free(searchBook);

	// ���� ����� �� �������, ������ ��������� �� ������
	if (searchResultStack == NULL) {
		puts("� ���������, ������ �� ������� (");
		return NULL;
	}

	// ���� ������� ������ ���� �����, ����� �� ���������� ��
	if (searchResultStack->nextNode == NULL) {
		return searchResultStack->currentBook;
	}

	// �� ���� ���� ������� � ����� ������ ��� ���� �����, � ������������� ����� ������� ���
	puts("\n������ ����, ��������� �� ������ �������: \n");
	for (bookStackNode* currentStackElement = searchResultStack; currentStackElement != NULL; currentStackElement = currentStackElement->nextNode) {
		printBook(currentStackElement->currentBook);
	}

	unsigned searchedID = getUserNumber("������� id ������� �����(�� ��������������� ���� ������ ����) > ");
	for (bookStackNode* currentStackElement = searchResultStack; ; currentStackElement = currentStackElement->nextNode) {
		if (currentStackElement == NULL) {
			puts("�� ������� �������������� id �����\n");
			return NULL;
		}

		// ���� �� ����� �� ����� � ��������� ������������� id, ���������� ��
		if (currentStackElement->currentBook->id == searchedID) {
			return currentStackElement->currentBook;
		}
	};
}

/* �������, �����������, ��������� �� ������������� ������������ ���� �����. 
��������� � �������� ���������� ������, � ������� ������� ������ ������������� �����, � ����������� ���� */
bool needToEditField(char* stringWithFieldsToEdit, enum searchTypes fieldType) {
	return strchr(stringWithFieldsToEdit, intToChar(fieldType + 1)) != NULL;
}

DWORD editBook(void) {
	puts("\t\t\t\t  ������� ������ �����, ������� ����� �������������.  \t\t\t\t \n");
	Book* bookToEdit = searchBook();

	// ���� ����� �� �������, ���������� ������
	if (bookToEdit == NULL) {
		puts("� ���������, ��������������� �������������� ����� ������.\n");
		return ERROR_NODE_NOT_AVAILABLE;
	}
	else printBook(bookToEdit);

	// ������� �������� �����, ������� ����� �������������, � �������� � �� ���������� ���� ����� ������������� �����
	Book* editedBook = (Book*)malloc(sizeof(Book));
	if (editedBook == NULL) {
		puts("������������ ������ ��� �������� ������ ��������� �����");
		return ERROR_NOT_ENOUGH_MEMORY;
	}
	memcpy(editedBook, bookToEdit, sizeof(Book));
	char* authorCopy = (char*) malloc((strlen(bookToEdit->author) + 1) * sizeof(char));
	char* titleCopy = (char*) malloc((strlen(bookToEdit->title) + 1) * sizeof(char));
	char* genreCopy = (char*) malloc((strlen(bookToEdit->genre) + 1) * sizeof(char));
	char* publishingHouseCopy = (char*) malloc((strlen(bookToEdit->publishingHouse) + 1) * sizeof(char));
	char* shortDescriptionCopy = (char*)malloc((strlen(bookToEdit->shortDescription) + 1) * sizeof(char));
	if (authorCopy == NULL || titleCopy == NULL || publishingHouseCopy == NULL || shortDescriptionCopy == NULL || genreCopy == NULL) {
		puts("������������ ������ ��� ���������� ����� ����������������� �����");
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

	// ������� ���������� �����, ��� ��� � ��� ��� ���� �����, ������� �� ����� �������������
	deleteFromAllTrees(bookToEdit);

	puts("\n������ �������� � �������������� ��������� �����.\n");

	printf("\t----------------------------------------------------------------------------------------------------\n"
		"\t|                     ������� ���������, ����� ���� ��������� �������������.                       |\n"
		"\t|     ��� �������������� ���������� ����� ������������ ������� ��������� ����� (��������, 145)     |\n"
		"\t|                                                                                                  |\n"
		"\t| 1. ��������������� �������� �����                                                                |\n"
		"\t| 2. ��������������� ������ �����                                                                  |\n"
		"\t| 3. ��������������� ��� ��������� �����                                                           |\n"
		"\t| 4. ��������������� ���� �����                                                                    |\n"
		"\t| 5. ��������������� ���� �����                                                                    |\n"
		"\t| 6. ��������������� ������ ����� ����������                                                       |\n"
		"\t| 7. ��������������� �������� ������������ �����                                                   |\n"
		"\t| 8. ��������������� ������� �������� �����                                                        |\n"
		"\t|                                                                                                  |\n"
		"\t----------------------------------------------------------------------------------------------------\n\n");

	char* fieldsToEdit = getUserStringPtr("������� �����, ��������������� �����, ������� �� ����������� ������������� > ");

	if (needToEditField(fieldsToEdit, SEARCH_BY_TITLE)) {
		char* newTitle = getUserStringPtr("������� ����������������� (�����������) �������� ����� > ");
		free(editedBook->title);
		editedBook->title = newTitle;
	}

	if (needToEditField(fieldsToEdit, SEARCH_BY_AUTHOR)) {
		char* newAuthor = getUserStringPtr("��������� ������� ������ ������ ����� > ");
		free(editedBook->author);
		editedBook->author = newAuthor;
	}

	if (needToEditField(fieldsToEdit, SEARCH_BY_GENRE)) {
		char* newGenre = getUserStringPtr("������ ������������ (�����) ���� ����� > ");
		free(editedBook->genre);
		editedBook->genre = newGenre;
	}

	if (needToEditField(fieldsToEdit, SEARCH_BY_PUBLISHING_HOUSE)) {
		char* newPublishingHouse = getUserStringPtr("������� ���������� (�����) �������� ������������ > ");
		free(editedBook->publishingHouse);
		editedBook->publishingHouse = newPublishingHouse;
	}

	if (needToEditField(fieldsToEdit, SEARCH_BY_SHORT_DESCRIPTION)) {
		char* newShortDescription = getUserStringPtr("������� ����� ������� �������� ����� > ");
		free(editedBook->shortDescription);
		editedBook->shortDescription = newShortDescription;
	}

	if (needToEditField(fieldsToEdit, SEARCH_BY_PRICE)) 
		editedBook->price = (unsigned) getUserNumber("������� ����� ���� ����� > ");

	if (needToEditField(fieldsToEdit, SEARCH_BY_YEAR)) 
		editedBook->issueYear = (int) getUserNumber("������� ������������ ��� ������ ����� > ");

	if (needToEditField(fieldsToEdit, SEARCH_BY_SCORE)) 
		editedBook->readersScore = (float) getUserNumber("������� ����� ������ ����� ���������� (�� 0 �� 10) > ");

	free(fieldsToEdit);

	// ��������� ����������������� ����� � ������
	insertInAllTrees(editedBook);

	return ERROR_SUCCESS;
}
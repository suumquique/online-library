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

char* getUserStringPtr(char* inputString) {
	char* temp = (char*)malloc(MAX_FIELD_LENGTH * sizeof(char));
	printf(inputString);
	gets_s(temp, MAX_FIELD_LENGTH);
	// ������� ������� ������ ����� ����������
	putchar('\n');

	// �������� ����� ��������� ������������� ������ � ������� ��� ��� ������ �������� � ��� ����� �������� (��� �������� ������)
	size_t sLen = strlen(temp);
	char* resultString = (char*)malloc((sLen + 1) * sizeof(char));
	strcpy(resultString, temp);
	// �� �������� �������� ������, ���������� ��� �������� ������
	free(temp);

	return resultString;
}

double getUserNumber(char* inputString) {
	double number;
	printf(inputString);
	scanf("%lf", &number);
	getchar();
	putchar('\n');

	return number;
}

void printBook(Book* bookPtr) {
	if (bookPtr == NULL) return;

	printf("\n�������� �����: %s                                                                                 \n"
		"����� �����: %s                                                                                    \n"
		"���� ������������: %s                                                                              \n"
		"��� ���������: %d                                                                                  \n"
		"������������: %s                                                                                   \n"
		"������ ���������: %.1f                                                                               \n"
		"���� �����: %u                                                                                     \n"
		"������� �������� ������: %s                                                                        \n\n"
		"ID �����: %u                                                                                       \n\n"
		"----------------------------------------------------------------------------------------------------\n\n",
		bookPtr->title, bookPtr->author, bookPtr->genre, bookPtr->issueYear, bookPtr->publishingHouse, bookPtr->readersScore,
		bookPtr->price, bookPtr->shortDescription, bookPtr->id);
}
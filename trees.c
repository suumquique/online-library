#include "main.h"

/*�������, ��������������� �� ��������� ����� ���������� � ����� (�����, ��������, ����� � ��� �����). ����� ����������
��� ���������� 7 (�� �������� ���������� � id ���������� ���), �������������, ����� ���� ��������.
��� ���������� ��� �������� ����� ��� ����������� ��� ��������� �������������� ��� ���� ���� ��������, �� �
���������� ����� ������������ ������������� ������ ���������� ������ ����������� �����.*/
treeNode* trees[7] = { 0 };

void insertInAllTrees(Book* bookPtr) {
	for (size_t searchType = 0; searchType < SEARCH_FIELDS_NUMBER; searchType++) {
		trees[searchType] = insert(trees[searchType], bookPtr, searchType);
	}
}

void deleteFromAllTrees(Book* bookPtr) {
	for (size_t searchType = 0; searchType < SEARCH_FIELDS_NUMBER; searchType++) {
		trees[searchType] = delete(trees[searchType], bookPtr, searchType);
	}

	free(bookPtr->author);
	free(bookPtr->title);
	free(bookPtr->genre);
	free(bookPtr->publishingHouse);
	free(bookPtr->shortDescription);
	free(bookPtr);
}

// ���������� �����, ���� ��� �����, ���������� 0, ���� ������ ������ ������� - �������, ���� �������� - "-1"
int compareNumbers(double first, double second) {
	return first == second ? 0 : first > second ? 1 : -1;
}

/*���������� ����� �� ������������� ��������� (��������, ������, ���� � �. �.). 
* ���� ������ ����� "������", ���������� �������;
* ���� ����� "�����" (��������, ���� � ��� �� ����� ��� ���������� ����), ���������� ����;
* ���� ������ ����� "������", ���������� �������� -1. */
enum compareResults compareBooks(Book* firstBook, Book* secondBook, enum searchTypes compareType) {
	switch (compareType) {
		case SEARCH_BY_TITLE:
			return strcmp(firstBook->title, secondBook->title);
		case SEARCH_BY_AUTHOR:
			return strcmp(firstBook->author, secondBook->author);
		case SEARCH_BY_GENRE:
			return strcmp(firstBook->genre, secondBook->genre);
		case SEARCH_BY_PUBLISHING_HOUSE:
			return strcmp(firstBook->publishingHouse, secondBook->publishingHouse);
		case SEARCH_BY_PRICE:
			return compareNumbers(firstBook->price, secondBook->price);
		case SEARCH_BY_YEAR:
			return compareNumbers(firstBook->issueYear, secondBook->issueYear);
		case SEARCH_BY_SCORE:
			return compareNumbers(firstBook->readersScore, secondBook->readersScore);
		default:
			return 0;
	}
}

unsigned char getHeight(treeNode* p)
{
	return p ? p->height : 0;
}

int getBalanceFactor(treeNode* p)
{
	return getHeight(p->right) - getHeight(p->left);
}

void fixHeight(treeNode* p)
{
	unsigned char hl = getHeight(p->left);
	unsigned char hr = getHeight(p->right);
	p->height = (hl > hr ? hl : hr) + 1;
}

treeNode* rotateRight(treeNode* p) // ������ ������� ������ p
{
	treeNode* q = p->left;
	p->left = q->right;
	q->right = p;
	fixHeight(p);
	fixHeight(q);
	return q;
}

treeNode* rotateLeft(treeNode* q) // ����� ������� ������ q
{
	treeNode* p = q->right;
	q->right = p->left;
	p->left = q;
	fixHeight(q);
	fixHeight(p);
	return p;
}

treeNode* balance(treeNode* p) // ������������ ���� p
{
	fixHeight(p);
	if (getBalanceFactor(p) == 2)
	{
		if (getBalanceFactor(p->right) < 0)
			p->right = rotateRight(p->right);
		return rotateLeft(p);
	}
	if (getBalanceFactor(p) == -2)
	{
		if (getBalanceFactor(p->left) > 0)
			p->left = rotateLeft(p->left);
		return rotateRight(p);
	}
	return p; // ������������ �� �����
}

treeNode* insert(treeNode* p, Book* bookPtr, enum searchTypes compareType) {
	if (!p) {
		// ������� ����� ���� ��� ����, ���� ��������� ���� �����, ��������� �� ��������� ������� ����� ������
		bookStackNode* newStackTop = (bookStackNode*)calloc(1, sizeof(bookStackNode));
		if (newStackTop == NULL) {
			puts("\n���������� �������� ����� � ���� ����� ����: ������������ ������\n");
			return NULL;
		}

		newStackTop->currentBook = bookPtr;
		newStackTop->nextNode = NULL;

		// ������� ������� (����) ������, ���� ��������� ��������� �� ��������� ���� ����� ����. ������������� ������ ������ ������� �� ���������
		treeNode* newNode = (treeNode*)calloc(1, sizeof(treeNode));
		if (newNode == NULL) {
			puts("\n������ ��� ���������� ���� � ������: ������������ ������ ��� ���������\n");
			return NULL;
		}

		newNode->height = 1;
		newNode->stackTopPtr = newStackTop;
		newNode->left = newNode->right = NULL;
		return newNode;
	}

	/* ���������� ����� �� ������� ������� ������ � ����������� ���� ����� �� ������������ ��������� (��������, �� ���� ��� �� ������).
	* �������� ��������� - compareType.*/
	switch (compareBooks(p->stackTopPtr->currentBook, bookPtr, compareType)) {
		// ����, ��������, �� ���������� �� ������� � ��� �������, �� ��������� ����� � ���� ������� ���� ������
		case IS_EQUAL:
		{
			bookStackNode* newStackTop = (bookStackNode*)calloc(1, sizeof(bookStackNode));
			if (newStackTop == NULL) {
				puts("\n���������� �������� ����� � ����: ������������ ������\n");
				break;
			}

			newStackTop->currentBook = bookPtr;
			newStackTop->nextNode = p->stackTopPtr;
			p->stackTopPtr = newStackTop;
		}
		break;
		/* ���� �����, ������� �� ���������, �� ������������ �������������� "������" ����� � ������� �������,
		* �� �������� �������� �� � ����� ��������� ������� ������� */
		case FIRST_IS_SUPERIOR:
			p->left = insert(p->left, bookPtr, compareType);
			break;
		// ���� �� �����, ��������, �� ��������� ��������� ������ ����� �� ������� �������, �� ��������� � ������ ��������� ��� ������� ����
		case SECOND_IS_SUPERIOR:
			p->right = insert(p->right, bookPtr, compareType);
			break;
	}

	// ���������, �� ������� �� ������ ���-������, ���� ������� - �����������
	return balance(p);
}

treeNode* findMin(treeNode* p) // ����� ���� � ����������� ������ � ������ p 
{
	return p->left ? findMin(p->left) : p;
}

treeNode* removeMin(treeNode* p) // �������� ���� � ����������� ������ �� ������ p
{
	if (p->left == 0)
		return p->right;
	p->left = removeMin(p->left);
	return balance(p);
}

treeNode* delete(treeNode* p, Book* bookPtr, enum searchTypes compareType) {
	if (!p) return NULL;

	switch (compareBooks(p->stackTopPtr->currentBook, bookPtr, compareType)) {
		case IS_EQUAL: {
			/* ���� � ����� ���� ����� �� ���� ������� (�� ����, ��������, ���� ��������� ���� ����� ������),
			* �� ���� ������ �� id � ������ ������� �� �� �����*/
			if (p->stackTopPtr->nextNode != NULL) {
				bookStackNode* currentStackNode = p->stackTopPtr;
				bookStackNode* previousStackNode = p->stackTopPtr;
				// ���� �� ����� ���� � ���� ������ ����� �� id. ����� ���������� ����� � currentNode ����� �������� �����, ���� ���� ����� ������
				while (currentStackNode->currentBook->id != bookPtr->id && currentStackNode != NULL) {
					previousStackNode = currentStackNode;
					currentStackNode = previousStackNode->nextNode;
				}

				// ���� �� �������� ������� �����, ������� ���, �������������, ������ ������ �� ����
				if (currentStackNode == NULL) return p;

				/* ������� ������ ������� �� �����, "���������" ���: � ������ �� ��������� ������� �� ����������� �������� ������
				* ������ �� �������, ������� ������� �� �������. ���� ���� ������� ������� �����, ����� �������� ��*/
				if (currentStackNode == p->stackTopPtr) p->stackTopPtr = currentStackNode->nextNode;
				else previousStackNode->nextNode = currentStackNode->nextNode;

				// ������� ������� ����
				free(currentStackNode);
			}
			// ���� � ����� ���� ������� (���� �����), �� ���� ������� ������� ������ ��������� ������ �� ������
			else {
				free(p->stackTopPtr);

				/* ���������� ����� q � r ������ � ������� ����������� ���� p; ������� ���� p; ���� ������ ��������� ������,
				�� ���������� ��������� �� ����� ���������; ���� ������ ��������� �� ������, �� ������� ��� ����������� ������� min,
				����� ��� ��������� ������, ����� � min ����������� q, ������ � ��, ��� ���������� �� r, ���������� min ����� ��� ������������.*/
				treeNode* l = p->left;
				treeNode* r = p->right;
				free(p);

				if (!r) return l;
				treeNode* min = findMin(r);
				min->right = removeMin(r);
				min->left = l;
				return balance(min);
			}
			break;
		}
		case FIRST_IS_SUPERIOR:
			p->left = delete(p->left, bookPtr, compareType);
			break;
		case SECOND_IS_SUPERIOR:
			p->right = delete(p->right, bookPtr, compareType);
			break;
	}
	
	return balance(p);
}

bookStackNode* search(treeNode* p, Book* bookPtr, enum searchTypes searchType) {
	if (!p) return NULL;

	// ������������ ������� ����� �� ������ ������� ������ � ��������� ���������, �� �������� �� ����
	switch (compareBooks(p->stackTopPtr->currentBook, bookPtr, searchType)) {
		case IS_EQUAL:
			return p->stackTopPtr;
		case FIRST_IS_SUPERIOR:
			return search(p->left, bookPtr, searchType);
		case SECOND_IS_SUPERIOR:
			return search(p->right, bookPtr, searchType);
	}
}
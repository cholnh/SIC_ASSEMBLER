#include <stdio.h>
#define MAX_CHAR_SIZE 10
#define MAX_OPTAB_SIZE 58

/****************************************
*              [ 구조체 ]               *
*                                       *
****************************************/
typedef struct {
	unsigned char index;
	char key[MAX_CHAR_SIZE];
	char value[MAX_CHAR_SIZE];
}Node;

typedef struct {
	Node node;
	struct ListNode *link;
}ListNode;

typedef struct ListHeader {
	char name[MAX_CHAR_SIZE];
	char length[MAX_CHAR_SIZE];
	ListNode *head;
	ListNode *tail;
}ListHeader;

typedef struct {
	char label[MAX_CHAR_SIZE];
	char opcode[MAX_CHAR_SIZE];
	char operand[MAX_CHAR_SIZE];
	unsigned char isIndex;
}Data;

/****************************************
*             [ 전방 선언 ]             *
*                                       *
****************************************/
/* Main function */
void init_OPTAB(Node *node);
void init_SYMTAB(ListHeader *plist);
int modify_Node_Value(ListHeader *plist, char *nodename, char *mod_value);
char *getValue(char *query);
void setError(char *errormsg);
char *searchNode(ListHeader *plist, char *temp);
char *searchIndex(ListHeader *plist, char index);
int modify_Node_Value(ListHeader *plist, char *nodename, char *mod_value);
void printNode(ListHeader *plist);
FILE *openFile(char *filename);

/* Tokenized function */
Data *parse(char *str);
int isCharacter(char input);
int isSpace(char input);

/* Pass1 function */
void pass_1(ListHeader *plist, FILE *source);
Data *readline(FILE *source);

/* Pass2 funtion */
void pass_2(ListHeader *plist, FILE *source);
void writer(FILE *result, char *str);

/* Global var */
ListHeader *SYMTAB;
Node OPTAB[MAX_OPTAB_SIZE];

/****************************************
*               [ Main ]                *
*                                       *
****************************************/
int main() {
	/* Define */
	Data *line_data;
	FILE *sourceFile;
	FILE *sourceFile2;
	int i = 0;
	/* Initialize */
	init_OPTAB(&OPTAB);
	init_SYMTAB(&SYMTAB);

	/* Pass 1 */
	printf("----------- pass 1 start --------------\n\n");
	pass_1(&SYMTAB, openFile("source.txt"));
	printNode(&SYMTAB);

	/* Pass 2 */
	printf("\n----------- pass 2 start --------------\n\n");
	pass_2(&SYMTAB, openFile("source.txt"));

}

/****************************************
*              [ SYMTAB ]               *
*                                       *
* 초기화,노드삽입 연결리스트 관련함수   *
****************************************/
void init_SYMTAB(ListHeader *plist) {
	plist->head = NULL, plist->tail = NULL;
}
void insert_node_last(ListHeader *plist, char *keydata, char *valuedata, unsigned char index) {
	ListNode *temp = (ListNode*)malloc(sizeof(ListNode));
	if (temp == NULL) {
		setError("Memory allocation failed");
		return;
	}
	strcpy(temp->node.key, keydata);
	strcpy(temp->node.value, valuedata);
	temp->node.index = index;
	temp->link = NULL;
	if (plist->tail == NULL)
		plist->head = plist->tail = temp;
	else {
		plist->tail->link = temp;
		plist->tail = temp;
	}
}
int modify_Node_Value(ListHeader *plist, char *nodename, char *mod_value) {
	ListNode *p = plist->head;
	while (p) {
		if (strstr(p->node.key, nodename)) {
			strcpy(p->node.value, mod_value);
			return 1;
		}	
		if (p->link == NULL) break;
		p = p->link;
	}
	return 0;
}
char *searchIndex(ListHeader *plist, char index) {
	ListNode *p = plist->head;
	while (p) {
		if (p->node.index == index)
			return p->node.value;
		if (p->link == NULL) break;
		p = p->link;
	}
	return NULL;
}
char *searchNode(ListHeader *plist, char *temp) {
	ListNode *p = plist->head;
	while (p) {	
		if (strstr(p->node.key, temp)) 
			return p->node.value;
		if (p->link == NULL) break;
		p = p->link;
	}
	return NULL;
}
void printNode(ListHeader *plist) {
	ListNode *p = plist->head;
	printf("Program [%s]\n\n-SYMTAB-\n-------------------------\n", plist->name);
	printf("[INDEX]\t[KEY]\t[VALUE]\n");
	while (p) {
		if(!strstr(p->node.key, "-"))
			printf("%d.\t%s\t%s\n", p->node.index, p->node.key, p->node.value);
		/*
		printf("[%d] %s", p->node.index, p->node.key);
		if (strstr(p->node.key, "-"))
			printf("\t");
		printf("\t%s\n", p->node.value);
		*/
		if (p->link == NULL) break;
		p = p->link;
	}
	printf("total size : %s\n", plist->length);
}

/****************************************
*              [ OPTAB ]                *
*                                       *
* 초기화,노드삽입 연결리스트 관련함수   *
****************************************/
void init_OPTAB(Node *node) {
	Node *OPTAB = node;
	int i = 0;
	char tmp[MAX_CHAR_SIZE];
	char label[MAX_CHAR_SIZE];
	char opcode[MAX_CHAR_SIZE];
	char operand[MAX_CHAR_SIZE];

	/* optab 읽기 */
	FILE *opFile = openFile("optab.txt");
	
	for (i = 0; i < MAX_OPTAB_SIZE; i++) {
		if (feof(opFile)) break;
		fscanf(opFile, "%s", OPTAB[i].key);
		fscanf(opFile, "%s", OPTAB[i].value);
	}
}

/****************************************
*           [ OPTAB - Search ]          *
*                                       *
*               순차탐색                *
****************************************/
char *getValue(char* query) {
	int i = 0;
	for (i = 0; i < MAX_OPTAB_SIZE; i++) {
		if (strstr(OPTAB[i].key, query))
			return OPTAB[i].value;
	}
	return NULL;
}

/****************************************
*          [ Open source File ]         *
****************************************/
FILE *openFile(char *filename) {
	FILE *afile = fopen(filename, "r");
	if (afile == NULL) {
		setError("File has not been found - ");
		printf("%s", filename);
		return NULL;
	}
	return afile;
}

/****************************************
*             [ 에러 표기 ]             *
****************************************/
void setError(char *errormsg) {
	printf("Error - %s", errormsg);
	// 종료구문
}


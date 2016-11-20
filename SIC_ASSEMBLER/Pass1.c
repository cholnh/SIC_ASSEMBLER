#include < stdio.h>
#include <string.h>
#define MAX_CHAR_SIZE 10
#define INSTRUCTION_SIZE 3

/****************************************
*              [ 구조체 ]               *
*                                       *
****************************************/
typedef struct {
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
*             [ Read Line ]             *
****************************************/
Data *readline(FILE *source) {
	char line[100];
	fgets(line, 100, source);
	line[strlen(line) - 1] = '\0';
	return parse(line);
}

/****************************************
*              [ Pass 1 ]               *
*                                       *
****************************************/
void pass_1(ListHeader *plist, FILE *source) {

	ListHeader *symtab = plist;
	FILE *sourceFile = source;
	Data *linedata;

	unsigned short startingAddr = 0;
	unsigned short LOCCTR = 0;
	unsigned char index = 0;
	int len = 0;
	char *startP, *endP;
	char temp[MAX_CHAR_SIZE];
	
	/* Read first line */
	linedata = readline(sourceFile);
	
	/* Initialize addr */
	if (strstr(linedata->opcode, "START")) {
		strcpy(plist->name, linedata->label);
		startingAddr = (int)strtol(linedata->operand, NULL, 16);
		LOCCTR = startingAddr;
	}
	linedata = linedata = readline(sourceFile);
	while (!strstr(linedata->opcode, "END") && !feof(sourceFile)) {

		/* 주석 처리 */
		if (!strstr(linedata->label, ".")) {
			/* 레이블이 없는 경우 
			if (strstr(linedata->label, "-")) {
				LOCCTR += INSTRUCTION_SIZE;
				continue;
			}
			*/
			
			/* symbol 중복 검사 */
			if (!strstr(linedata->label, "-") && searchNode(symtab, linedata->label)) {
				setError("Duplicated Symbol");
				printf("\nerror code : %s\n", linedata->label);
			}
			/* SYMTAB 추가 */
			else {
				sprintf(temp, "%X", LOCCTR);
				insert_node_last(plist, linedata->label, temp, index++);
			}

			/* LOCCTR 증가 */
			if (getValue(linedata->opcode))
				LOCCTR += INSTRUCTION_SIZE;
			else if (strstr(linedata->opcode, "RESW"))
				LOCCTR += atoi(linedata->operand) * INSTRUCTION_SIZE;
			else if (strstr(linedata->opcode, "RESB"))
				LOCCTR += atoi(linedata->operand);
			else if (strstr(linedata->opcode, "WORD")) {
				//modify_Node_Value(symtab, linedata->label, linedata->operand);
				LOCCTR += INSTRUCTION_SIZE;
			}
			else if (strstr(linedata->opcode, "BYTE")) {
				if (startP = strchr(linedata->operand, '\'')) {
					endP = strchr(startP + 1, '\'');
					len = endP - startP - 1;
					endP = strchr(startP, '\'');
					
					if ((linedata->operand)[0] == 'X')
						LOCCTR += (len + 1) / 2;
					else if ((linedata->operand)[0] == 'C')
						LOCCTR += len;	

					//memcpy(linedata->operand, endP + 1, len);
					//(linedata->operand)[len] = '\0';
					//modify_Node_Value(symtab, linedata->label, linedata->operand);
				}
			}
			else {
				setError("Invalid Operator code");
				printf("\nerror code : %s [ %s ] %s\n", linedata->label, linedata->opcode, linedata->operand);
				return;
			}
		}
		/* Read line */
		linedata = linedata = readline(sourceFile);
	}
	/* 프로그램 길이 */
	sprintf(plist->length, "%X", LOCCTR - startingAddr);
	fclose(sourceFile);
}

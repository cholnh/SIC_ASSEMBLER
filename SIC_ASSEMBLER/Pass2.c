#include <stdio.h>
#define MAX_CHAR_SIZE 10
#define MAX_RECORD_LINE_SIZE 59
#define HEADER_RECORD 'H'
#define TEXT_RECORD 'T'
#define END_RECORD 'E'

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
*             [ Formatter ]             *
****************************************/
char *formatter(char *str, int max_length) {
	int i = 0;
	int len = strlen(str);
	char *temp;

	if (len > max_length) {
		setError("check : ");
		printf("%s", str);
		return NULL;
	}
	temp = (char*)malloc(sizeof(MAX_CHAR_SIZE));
	temp[0] = '\0';
	for (i = 0; i < max_length - len; i++)
		temp[i] = '0';
	temp[i] = '\0';
	strcat(temp, str);
	return temp;
}

/****************************************
*              [ Writer ]               *
****************************************/
void writer(FILE *result, char *str) {
	fprintf(result, "%s ", formatter(str, 6));
	printf("%s ", formatter(str, 6));
}

/****************************************
*              [ Pass 2 ]               *
*                                       *
****************************************/
void pass_2(ListHeader *plist, FILE *source) {

	ListHeader *symtab = plist;
	FILE *sourceFile = source;
	FILE *result;
	Data *linedata;

	unsigned char i = 0;
	unsigned char index = 0;
	unsigned char count = 0;
	unsigned char isEnd = 0;
	int ascii;
	int length = 0;
	char *startP, *endP;
	char buffer[128] = {'\0',};
	char *startingAddr;
	char *Origin_StartAddr;
	char *opcode;
	char *operand;
	char len[MAX_CHAR_SIZE];
	char temp[MAX_CHAR_SIZE] = { '\0', };
	char temp2[MAX_CHAR_SIZE] = { '\0', };

	result = fopen("result.txt", "w+");

	//printf("%s", linedata->opcode);

	/* Read first line */
	linedata = readline(source);
	if (strstr(linedata->opcode, "START")) {
		startingAddr = linedata->operand;
		Origin_StartAddr = startingAddr;
		linedata = readline(source);
	}
	/* Write Header Record */
	fprintf(result, "%c ", HEADER_RECORD);
	
	printf("%c ", HEADER_RECORD);
	for (i = 0; i < 6; i++) {
		fprintf(result, "%c", symtab->name[i]);
		printf("%c", symtab->name[i]);
	}
	for (i; i < 6; i++) {
		fprintf(result, " ");
		printf(" ");
	}
	printf(" ");
	writer(result, startingAddr);
	writer(result, symtab->length);

	/////////////////////////////////////////////////////////////////////
	// 텍스트 레코드 추가 알고리즘

	while (1) {
		
		/* 주석 처리 */
		if (!strstr(linedata->label, ".")) {
			
			/* Re-Write text record */
			if (strstr(linedata->opcode, "END") || count > MAX_RECORD_LINE_SIZE || (isEnd == 1 && getValue(linedata->opcode))) {

				fprintf(result, "\n%c ", TEXT_RECORD);
				printf("\n%c ", TEXT_RECORD);
				writer(result, startingAddr);
				sprintf(len, "%X ", count/2);
				fprintf(result, "%s", len);
				printf("%s", len);
				fprintf(result, "%s", buffer);
				printf("%s", buffer);
				
				//printf("%s ", startingAddr);
				//printf("%X ", count/2);
				if (strstr(linedata->opcode, "END")) break;
				
				// 초기화
				count = 0;
				isEnd = 0;
				buffer[0] = '\0';
				startingAddr = searchIndex(symtab, index);
			}

			/* opcode 검사 */
			if (opcode = getValue(linedata->opcode)) {
				strcat(buffer, opcode);

				/* operand 검사 */
				if(linedata->operand == NULL || strstr(linedata->operand, "-"))
					strcat(buffer, "0000");
				else {

					/* symtab 검사 */
					if (operand = searchNode(symtab, linedata->operand)) {
						
						if (linedata->isIndex) {
							sprintf(operand, "%X", _strtoi64(operand, NULL, 16) + (0x4000));
						}
						
						strcat(buffer, operand);
					}
					else {
						setError("Undefined symbol ");
						printf("- %s", linedata->operand);

						strcat(buffer, "0000");
					}
				}
				

				count += 6;
				//startingAddr += 3;
			}
			/* 상수 처리 */
			else if (strstr(linedata->opcode, "BYTE")) {
				if (startP = strchr(linedata->operand, '\'')) {
					
					endP = strchr(startP + 1, '\'');
					length = endP - startP - 1;
					endP = strchr(startP, '\'');

					if ((linedata->operand)[0] == 'C') {
						memcpy(linedata->operand, endP + 1, length);
						(linedata->operand)[length] = '\0';

						
						for (i = 0; i < length; i++) {
							sprintf(temp, "%X", linedata->operand[i]);
							strcat(temp2, temp);
							//sprintf((linedata->operand)[i], "%c", (linedata->operand)[i]);
						}
						
						strcpy(linedata->operand, temp2);
					}
					else if ((linedata->operand)[0] == 'X') {
						memcpy(linedata->operand, endP + 1, length);
						(linedata->operand)[length] = '\0';
					}
				}
				//printf(" >> Byte : %s << ", linedata->operand);
				strcat(buffer, linedata->operand);
				//fprintf(result, "%s", linedata->operand);
				//printf("%s", linedata->operand);
				count += strlen(linedata->operand);
				isEnd = 1;
			}
			else if (strstr(linedata->opcode, "WORD")) {
				//printf(" >> Word : %s << ", linedata->operand);	
				sprintf(linedata->operand, "%X", atoi(linedata->operand));
				strcat(buffer, formatter(linedata->operand, 6));
				count += 6;
				//count += strlen(linedata->operand);
				isEnd = 1;
			}
			
			if (strstr(linedata->opcode, "RESW") || strstr(linedata->opcode, "RESB")) {
				isEnd = 1;
			}
			strcat(buffer, " ");
			index++;
		}
	if(!feof(sourceFile))
		/* Read line */
		linedata = readline(sourceFile);
	}

	/* Write End Record */
	fprintf(result, "\n%c ", END_RECORD);
	printf("\n%c ", END_RECORD);
	writer(result, Origin_StartAddr);

	printf("\n\n");

	fclose(sourceFile);
	fclose(result);
}

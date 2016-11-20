#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_CHAR_SIZE 10

/****************************************
*              [ ����ü ]               *
*                                       *
****************************************/
typedef struct {
	char label[MAX_CHAR_SIZE];
	char opcode[MAX_CHAR_SIZE];
	char operand[MAX_CHAR_SIZE];
	unsigned char isIndex;
}Data;

/****************************************
*              [ �˻繮 ]               *
*                                       *
****************************************/
int isCharacter(char input) {
	if ('a' <= input && input <= 'z') return 1;
	if ('A' <= input && input <= 'Z') return 1;
	if ('0' <= input && input <= '9') return 1;
	return 0;
}
int isSpace(char input) {
	if (input == ' ' || input == '\t')  return 1;
	else return 0;
}

/****************************************
*              [ Parse ]                *
*                                       *
*       String ���� token ����          *
****************************************/
Data *parse(char *str) {

	Data *data = (Data*)malloc(sizeof(Data));

	unsigned char i = 0;
	unsigned char j, k;
	unsigned char index = 0;

	int len = 0;

	char line[512];
	char temp[MAX_CHAR_SIZE] = "";
	char *startP, *endP;

	/* �ּ�ó�� */
	if (strstr(str, ".")) {
		strcpy(data->label, ".");
		return data;
	}

	memcpy(line, str, 512);
	strcpy(data->label, "-");
	data->isIndex = 0;

	while (1) {
		k = 0;

		// ���� ���ڿ����� ����˻�
		for (; line[i] != '\0'; i++) {
			if (isCharacter(line[i])) break;
			if (isSpace(line[i]))
				k++;
		}

		// ���� �߰ߵ��� ���� >> ���ڿ� ����
		if (k == 0) {
			for (j = 0; line[i] != '\0' && j < MAX_CHAR_SIZE; i++, j++) {
				if (isSpace(line[i])) break;
				temp[j] = line[i];
			}
			if (j >= MAX_CHAR_SIZE) 
				setError("Overflowed MAX CHARACTER SIZE");
			
			temp[j] = '\0';
		}
		// ������ '-' �� ǥ��
		else strcpy(temp, "-");

		// ���� ��ū���� �̵�
		while(1) {
			if (isCharacter(line[i])) break;
			if (line[i-1] == '\0' || line[i] == '\0') break;
			i++;
		}
		
		
		// index�� ���� label -> opcode -> operand �� ����
		if (index == 0) {
			strcpy(data->label, temp);
			strcpy(data->opcode, "-");
			strcpy(data->operand, "-");
		}
		else if (index == 1) {
			strcpy(data->opcode, temp);
		}
		else if (index == 2) {
			/* Indexing mode */
			if (strchr(temp, ',')) {
				data->isIndex = 1;
				temp[strlen(temp) - 2] = '\0';
			}
			strcpy(data->operand, temp);
			break;
		}
		if (line[i - 1] == '\0' || line[i] == '\0') return data;
		index++;
	}
	return data;
}
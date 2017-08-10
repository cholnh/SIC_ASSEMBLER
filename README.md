# SIC ASSEMBLER

## Design process

1. Set Architecture  
Used Two-Pass
2. Data-Structure of Micro-Instruction Set Table  
Used Array list
3. The Algorithm of searching Operator Table  
Used Sequential Search
4. Notation of Addressing Mode of Assembler  
Index mode
5. Directive Instructions  
START, END, RESW, REWB, BYTE, WORD
6. Structure of Object Codes  
- Cut off in terms of sub routine
- Maximum line : 60  


## Flow Chart
![ flow1 ]({{ site.url }}/assets/flow1.bmp)
![ flow2 ]({{ site.url }}/assets/flow2.bmp)
![ flow3 ]({{ site.url }}/assets/flow3.bmp)


## Data Flow Diagram

###Modularized Assembler Architecture  
![ Data_Flow_Diagram ]({{ site.url }}/assets/Data_Flow_Diagram.bmp)


## Source Feature

### Main Source  

**Data Structures** : Structure array list(OPTAB), Structure simple linked list(SYTAB)
- Data structure : (String) label, opcode, operand
- Node structure : (String) key, value / (unsigned char) index

**Main methods** : Application Starting
	1. OPTAB , SYMTAB initialize
	2. Open source file
	3. Operate pass1
	4. Operate pass2

**Other methods**
	- init_SYMTAB : Initialize Symbol Table (Initialize head and tail of Structure)
	- insert_node_last : Add Symbol TableNode
	- modify_Node_Value : Find right node and modify value in symbol table
	- searchIndex : Return index of right node in symbol table
	- searchNode : Return right node in symbol table
	- printNode : Print all of symbol table
	- init_OPTAB : Initialize command table (add "optab.txt" in structure array list) 
	- getValue : Return right node's commend number(value) by using sequential search of commend table
	- openFile : Return text file replaced in relative address to file format
	- setError : Print error message
  
### Tokenized

**Data Structure** : Use three token structure
- Data Structure : (String) label, opcode, operand (unsigned char) inIndex  

**parse method** : Divide one line to three tokens (Lable, Operator, Operand)
	1. Get Parameter(char* str) value of command line by "fgets" and temporarliy put in variable data"line" and use memcpy( + (commands)
	2. Examine string's space to know it is empty
	3. If empty, store '-' in temp
	4. If not empty, store string in temp 
	5. And ignore other space until next string
	6. Determine one data struct  of three data struct(Data (label, opcode, operand)) by index ( first is 0)
	7. increase index 1 and loop from twice over (When index is 2, all Data (label, opcode, operand)is full => quit)
	8. Return Data

**Other method**
	- isCharacter : If a ~ z,  A ~ Z, 0 ~ 9 => Return 1, other 0
	- isSpace : If space or tab => Return 1 , other 0


### Pass 1

**Data structure is same as Main source**

**pass_1 method** : Operation SIC Assembler Pass1 algorithm 
	1. Tokenize first line by use readline() method
	2. Initialize first address, next name
	3. Tokenize one more time by use readline() method
	4. Examine duplication of read data's label by SYMTAP( + command)
	5. If is not duplicated, add in SYMTAP
	6. Location counter increases different values by opcode
	7. Tokenize using readline() method
	8. Repeat four time until END
	9. Add program length and quit.

**Other method**
	- readline : Read file by "fgets" and get one line. Use parse method of "Tokenized.c" to get data structure of divided three tokens.

### Pass 2

**Data structure is same as Main source**

**pass_1 method** : Operation SIC Assembler Pass2 algorithm
	1. Tokenize first line by use readline() method
	2. Initialize first address, next name
	3. Write H(Header Record), name, Frist starting address
	4. Examine Data's opcode read by using OPTAB
	5. If command is nomal, add opcode in buffer
	6. Examine Data's operand read by using SYMTAB
	7. If address is normal, add operand in buffer
	8. If address is missed or abnormal, add 0000 in buffer
	9. Increase 6 in count  (when writing Text Record, using when the necessary size of the line is writing)
	10. If is not in case OPTAB treat WORD, BYTE, RESW, RESB
	11. (conditional)write Text Record (if END or over maximum size of writing or the end of one block of source) and initialize count and state 
	13. Repeat four time until END
	14. Add program length and quit.

**Other method**
	- formatter : When writing recode, change each one token to match the spaces up to 6.
	- writer : Write string in each right text file.  


# Usage And Assembler Implementation Process
[ source.txt ]({{ site.url }}/assets/source.txt)  

```
COPY	START	1000
FIRST	STL		RETADR
CLOOP	JSUB	RDREC
		LDA		LENGTH
		COMP	ZERO
		JEQ		ENDFIL
		JSUB	WRREC
		J		CLOOP
ENDFIL	LDA		EOF
		STA		BUFFER
		LDA		THREE
		STA		LENGTH
		JSUB	WRREC
		LDL		RETADR
		RSUB		
EOF		BYTE	C'EOF'
THREE	WORD	3
ZERO	WORD	0
RETADR	RESW	1
LENGTH	RESW	1
BUFFER	RESB	4096
.
.	SUBROUTINE TO READ RECORD INTO BUFFER
.
RDREC	LDX		ZERO
		LDA		ZERO
RLOOP	TD		INPUT
		JEQ		RLOOP
		RD		INPUT
		COMP	ZERO
		JEQ		EXIT
		STCH	BUFFER,X
		TIX		MAXLEN
		JLT		RLOOP
EXIT	STX		LENGTH
		RSUB		
INPUT	BYTE	X'F1'
MAXLEN	WORD	4096
.
.	SUBROUTINE TO READ RECORD INTO BUFFER
.
WRREC	LDX		ZERO
WLOOP	TD		OUTPUT
		JEQ		WLOOP
		LDCH	BUFFER,X
		WD		OUTPUT
		TIX		LENGTH
		JLT		WLOOP
		RSUB		
OUTPUT	BYTE	X'05'
		END		FIRST
```

### Test in Linux
**arguments**
![ args ]({{ site.url }}/assets/args.bmp)  

**usage**
![ usage ]({{ site.url }}/assets/usage.bmp)  

**makefile**
![ makefile ]({{ site.url }}/assets/makefile.bmp)  

**cat**
![ cat ]({{ site.url }}/assets/cat.bmp)  

**script**
![ script ]({{ site.url }}/assets/script.bmp)  

## Result
![ result ]({{ site.url }}/assets/result.bmp)  

## Cautions
1. Write the order of (first)LABEL, (second)OPCODE, (third)OPERAND
2. Use tab to express nothing is used (Not need to tab twice to maintain each token's space)
3. Use tab to express each token's space
4. express command by .(point)
5. If you want to WORD to Character literal, write C in OPERAND and wrap literal by quotation mark(' ')  
If you want to express Hexa, use X and same as up line
6. Using Index register, write comma(,) + X side of OPERAND (ex : operand, x)
7. All word's max size is 10.
8. Text Record is divided by Block of source code, and write max up to 60
9. Do not use comment, top of source code.
10. argument Usage : program name +　source file name (argument)  
			( Ex : ./sic source.txt )

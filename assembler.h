/*
General header file for the assembly. 
Contains macros, data structures and methods declaration.

=======================
By Lotem Ben Yaakov
ID 206663932
=======================
*/

#ifndef ASSEMBLER_H
	#define ASSEMBLER_H

#include <stdio.h>
#include <string.h>

/* ======== Macros ======== */
/* Utilities */
#define FOREVER				for(;;)
#define BYTE_SIZE			8
#define FALSE				0
#define TRUE				1

/* Given Constants */
#define MAX_DATA_NUM		1000
#define FIRST_ADDRESS		100 
#define MAX_LINE_LENGTH		80
#define MAX_LABEL_LENGTH	30
#define MEMORY_WORD_LENGTH	15
#define MAX_REGISTER_DIGIT	7

/* Defining Constants */
#define MAX_LINES_NUM		700
#define MAX_LABELS_NUM		MAX_LINES_NUM 

/* ======== Data Structures ======== */
typedef unsigned int bool; /* Only get TRUE or FALSE values */

/* === First Read  === */

/* Labels Management */
typedef struct
{
	int address;					/* The address it contains */
	char name[MAX_LABEL_LENGTH];	/* The name of the label */					
	bool isExtern;					/* Extern flag */
	bool isData;					/* Data flag (.data or .string) */
} labelInfo;

/* Directive And Commands */
typedef struct 
{
	char *name;
	void (*parseFunc)();
} directive;

typedef struct
{
	char *name;
	unsigned int opcode : 4;
	int numOfParams;
} command;

/* Operands */
typedef enum { NUMBER = 0, LABEL = 1, /* Random = 2 (Isn't a real type by itself), */ REGISTER = 3, INVALID = -1 } opType;

typedef struct
{
	int value;				/* Value */
	char *str;				/* String */
	opType type;			/* Type */
	bool isRandom;			/* 2nd addressing method flag */
	int address;			/* The adress of the operand in the memory */
} operandInfo;

/* Line */
typedef struct
{
	int lineNum;				/* The number of the line in the file */
	int address;				/* The address of the first word in the line */
	char *originalString;		/* The original pointer, allocated by malloc */
	char *lineStr;				/* The text it contains (changed while using parseLine) */
	bool isError;				/* Represent whether there is an error or not */
	labelInfo *label;			/* A poniter to the lines label in labelArr */

	char *commandStr;			/* The string of the command or directive */

	/* Command line */
	const command *cmd;			/* A pointer to the command in g_cmdArr */
	operandInfo op1;			/* The 1st operand */
	operandInfo op2;			/* The 2nd operand */
} lineInfo;

/* === Second Read  === */

typedef enum { ABSOLUTE = 0, EXTENAL = 1, RELOCATABLE = 2 } eraType;

/* Memory Word */

typedef struct /* 15 bits */
{
	unsigned int era : 2;

	union /* 13 bits */
	{
		/* Commands (only 13 bits) */
		struct
		{
			unsigned int dest : 2;		/* Destination op addressing method ID */
			unsigned int src : 2;		/* Source op addressing method ID */
			unsigned int opcode : 4;	/* Command ID */
			unsigned int group : 2;		/* Number of params */
			unsigned int rnd : 2;		/* Number of '*' in the random op */
			unsigned int : 1;			/* Unused Bit */
		} cmdBits;

		/* Registers (only 13 bits) */
		struct
		{
			unsigned int destBits : 6;
			unsigned int srcBits : 6;
			unsigned int : 1;			/* Unused Bit */
		} regBits;

		/* Other operands */
		int value : 13; /* (13 bits) */

	} valueBits; /* End of 13 bits union */

} memoryWord;


/* ======== Methods Declaration ======== */
/* utility.c methods */
int randomInRange(int min, int max);
int getCmdId(char *cmdName);
labelInfo *getLabel(char *labelName);
void trimLeftStr(char **ptStr);
void trimStr(char **ptStr);
char *getFirstTok(char *str, char **endOfTok);
bool isOneWord(char *str);
bool isWhiteSpaces(char *str);
bool isLegalLabel(char *label, int lineNum, bool printErrors);
bool isExistingLabel(char *label);
bool isExistingEntryLabel(char *labelName);
bool isRegister(char *str, int *value);
bool isCommentOrEmpty(lineInfo *line);
char *getFirstOperand(char *line, char **endOfOp, bool *foundComma);
bool isDirective(char *cmd);
bool isLegalStringParam(char **strParam, int lineNum);
bool isLegalNum(char *numStr, int numOfBits, int lineNum, int *value);

/* firstRead.c methods */
int firstFileRead(FILE *file, lineInfo *linesArr, int *linesFound, int *IC, int *DC);

/* secondRead.c methods */
int secondFileRead(int *memoryArr, lineInfo *linesArr, int lineNum, int IC, int DC);

/* main.c methods */
void printError(int lineNum, const char *format, ...);

#endif
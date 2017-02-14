#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>
#include <limits.h>
 #include <assert.h>
#include <stddef.h>
#include <inttypes.h>



/* declarations*/

#define memsize 2048
#define numReg 16
#define boot_mem_base 0
#define instr_mem_base 512
#define data_mem_base 1024

/* Opcodes */
	// store word indirect
    char * STORE_MEMORY= (char *) "00001";
	// load word indirect
    char * LOAD_MEMORY= (char *) "00010";

    char * ALU_Op= (char *) "00011";

	// store word immediate
    char * STORE_MEMORY_imm= (char *) "00100";
	// load word immediate
    char * LOAD_MEMORY_imm= (char *) "00101";

	// load word immediate
    char * LOAD_MEMORY_hlf= (char *) "00110";

	//Jump Operation
	char *JUMP = (char *) "00111"; 

	//Compare
	char *CMPQ = (char *) "01000";

	//Move immediate value into Register
	char *MOVE = (char *) "01001";
	
	//Jump Operation
	char *SET = (char *) "01010"; 

	//Move immediate value into Register
	char *MOVER = (char *) "01011";

	//Test Condition
	char *TESTQ = (char *) "01100";

	char *LEA = (char *) "01101";

	//User Inputs
	char *DATA = (char *) "11111";

//ALU functions

    char * Add= (char *) "00001";
    char * Sub= (char *) "00010";
    char * Mul= (char *) "00011";
    char * Div= (char *) "00100";
    char * Mod= (char *) "00101";


//Different Jump Instructions

	char * JMP=(char *) "00001";
	char * JE=(char *) "00010";
	char * JNE=(char *) "00011";
	char * JS=(char *) "00100";
	char * JNS=(char *) "00101";
	char * JG=(char *) "00110";
	char * JGE=(char *) "00111";
	char * JL=(char *) "01000";
	char * JLE=(char *) "01001";
	
//Different Set Instructions

	char * SETE=(char *) "00001";
	char * SETNE=(char *) "00010";
	char * SETS=(char *) "00011";
	char * SETNS=(char *) "00100";
	char * SETG=(char *) "00101";
	char * SETGE=(char *) "00110";
	char * SETL=(char *) "00111";
	char * SETLE=(char *) "01000";

/* CPU regiseters */

/* general purpose Registers */
	char * R[numReg];
/* Program Counter */
	int pc;
/* Stack Pointer */
	int sp;
/* frame Pointer */
	int framePointer;
/* Memory data Register */
    	char  *mdr[4];
/* Memory address Register */
	char ** mar;
/* Flag Register */
	int flag;
/* access individual bits of flag */
	int zf=0;
	int sf=0;
	int cf=0;
	int of=0;

	//char *flag;
/* Current instruction register */
	 char *cir;
/* Memory block */
	char *mem[memsize];

/* Total Instructions */
int totalInst=0;
int line=0;
/* function declarations */

// Initilize values of registers and memory
void initialize();

// write value to a register at secified location
void wGReg(char *inst,int loc);

//load specified instructions into instruction memory
void loadInstructions(char *inst[], int size);

//load default instructions into instruction memory for jump simulation
void loadJumpInstructions(char *instsjmp[], int jmpSize);

// store value from register to data memory location by decoding instruction
// bool flag represent direct and indirect
// for immediate it is true
void storeToMemory(bool flg);

// load value from data memory to register by decoding the instruction
// bool flag represent direct and indirect
// for immediate it is true
//hlfg true loads half word
void LoadFromMemory(bool flg,bool hlfflg);

// perform computation by decoding the instruction
void Alu();

// function pointer to ALU operations
int (*AluFunPtr)(int,int);

//perform move instruction execution
void MoveInstruction();
void MoveRInstruction();
//perform compare instruction execution
void Compare();

void SetOperation();

void LoadEffectiveAddress();

//perform compare instruction execution
void TestOperation();
// function pointer to JUMP operations
void (*JmpPtr)(int);

//Jump Operations
void JumpOperation();

//convert from decimal to binary
char *deciBinary(int n);

// convert binary to decimal
int bToInt( const char* binstr ) ;

// dump values of CPU
void dumpCpu();

//execute Instructions
void executeInstr();

// for label index
int indexLabel=0;;

// For storing Memory location of label
//char *indexMem[8];	
int exe[128];

//For mapping of index for instruction exectuion	
int loopexe[128];

//For execution of instructions
int loopIndex=0;


// Concatenate function

char* scat(char *s, char *t)
{

	char *p=malloc(strlen(s)+strlen(t)+1);    /* reserve memory to hold the copy. */
    	int ptr =0, temp = 0;                   /* initialise some helpers */

    	while(s[temp]!='\0'){                  /* use the temp to "walk" over string 1 */
        p[ptr++] = s[temp++];
    	}
    	temp=0;
    	while(t[temp]!='\0'){                   /* and string two */
        p[ptr++]=t[temp++];
    	}
	p[ptr++]='\0';
    	return p;

}

char *dest;

// Bit representation of an instruction
void alu_instruction()
{

	char *buff1 = NULL;
	char *buff2 = NULL;
	char *buff3 = NULL;
	buff1=malloc(32*sizeof(char));
	buff2=malloc(32*sizeof(char));	
	buff3=malloc(32*sizeof(char));	
	memset(buff1,'\0',32);	
	memset(buff2,'\0',32);	
	memset(buff3,'\0',32);	
	buff1=ALU_Op;
	
	
	buff2=strtok(NULL," ");
	
	if(strcasecmp(buff2,"Add")==0)
	{	
	
		char *final=malloc(32*sizeof(char));	
		memset(final,'\0',32);		
		final=scat(buff1,Add);
		dest = final;
	//	printf("Code is \n%s\n\n",final);
	}
	if(strcasecmp(buff2,"Sub")==0)
	{	
	
		char *final=malloc(32*sizeof(char));	
		memset(final,'\0',32);		
		final=scat(buff1,Sub);
		//char *final=scat(buff1,Sub);
		dest = final;
	//	printf("Code is \n%s\n\n",final);
	}
	if(strcasecmp(buff2,"mul")==0)
	{	
	
		char *final=malloc(32*sizeof(char));	
		memset(final,'\0',32);		
		final=scat(buff1,Mul);
		dest = final;
	//	printf("Code is \n%s\n\n",final);
	}
	if(strcasecmp(buff2,"div")==0)
	{	
	
		char *final=malloc(32*sizeof(char));	
		memset(final,'\0',32);		
		final=scat(buff1,Div);
		dest = final;
	//	printf("Code is \n%s\n\n",final);
	}
	if(strcasecmp(buff2,"mod")==0)
	{	
	
		char *final=malloc(32*sizeof(char));	
		memset(final,'\0',32);		
		final=scat(buff1,Mod);
		dest = final;
	//	printf("Code is \n%s\n\n",final);
	}

	/*** Source Register 1 for ALU ***/
	buff3=strtok(NULL," ");
	if(buff3[0]==82 || buff3[0]==114)
	{
		if(buff3[2])
		{
		
			char temp[3];
			memcpy(&temp,&buff3[1],2);
			temp[3]='\0';
			//printf("%d\n\n",atoi(temp));
			if(atoi(temp)>18)
			{
				printf("ERROR: NO SUCH REGISTER\n");
				exit(0);
			}
			char *src1=malloc(32*sizeof(char));	
			memset(src1,'\0',32);		
			src1=deciBinary(atoi(temp));
			char subbuff[6];
			memcpy(subbuff,&src1[27],5);
			subbuff[5]='\0';
			//printf("%s\n\n",subbuff);
			char *final=malloc(32*sizeof(char));	
			memset(final,'\0',32);		
			final=scat(dest,subbuff);
			dest = final;
			//printf("%s\n\n",dest);
		}else if(!buff3[2])
		{
	
		//printf("yes it is R\n");
		int m = buff3[1]-48;
		char *src1=malloc(32*sizeof(char));	
		memset(src1,'\0',32);		
		src1=deciBinary(m);
		char subbuff[6];
		memcpy(subbuff,&src1[27],5);
		subbuff[5]='\0';
		//printf("%s\n\n",subbuff);
		char *final=malloc(32*sizeof(char));	
		memset(final,'\0',32);		
		final=scat(dest,subbuff);
		dest = final;
	//	printf("%s\n\n",dest);
		}
	}
	else
	{

		printf("ERROR: Wrong Instruction provided..!");

	}
	
	/*** Source Register 2 for ALU ***/
	buff3=strtok(NULL," ");
	if(buff3[0]==82 || buff3[0]==114)
	{
		if(buff3[2])
		{
		
			char temp[3];
			memcpy(&temp,&buff3[1],2);
			temp[3]='\0';
			//printf("%d\n\n",atoi(temp));
			if(atoi(temp)>18)
			{
				printf("ERROR: NO SUCH REGISTER\n");
				exit(0);
			}
			char *src1=malloc(32*sizeof(char));	
			memset(src1,'\0',32);		
			src1=deciBinary(atoi(temp));
			char subbuff[6];
			memcpy(subbuff,&src1[27],5);
			subbuff[5]='\0';
			//printf("%s\n\n",subbuff);
			char *final=malloc(32*sizeof(char));	
			memset(final,'\0',32);		
			final=scat(dest,subbuff);
			dest = final;
		//	printf("%s\n\n",dest);
		}else if(!buff3[2])
		{
	
		//printf("yes it is R\n");
		int m = buff3[1]-48;
		char *src1=malloc(32*sizeof(char));	
		memset(src1,'\0',32);		
		src1=deciBinary(m);
		char subbuff[6];
		memcpy(subbuff,&src1[27],5);
		subbuff[5]='\0';
		//printf("%s\n\n",subbuff);
		char *final=malloc(32*sizeof(char));	
		memset(final,'\0',32);		
		final=scat(dest,subbuff);
		dest = final;
		//printf("%s\n\n",dest);
		}
	}
	else
	{

		printf("ERROR: Wrong Instruction provided..!");

	}

	/*** Source Register 3 for ALU ***/
	buff3=strtok(NULL," ");
	if(buff3[0]==82 || buff3[0]==114)
	{
		if(buff3[2])
		{
		
			char temp[3];
			memcpy(&temp,&buff3[1],2);
			temp[3]='\0';
			//printf("%d\n\n",atoi(temp));
			if(atoi(temp)>18)
			{
				printf("ERROR: NO SUCH REGISTER\n");
				exit(0);
			}
			char *src1=malloc(32*sizeof(char));	
			memset(src1,'\0',32);		
			src1=deciBinary(atoi(temp));
			char subbuff[6];
			memcpy(subbuff,&src1[27],5);
			subbuff[5]='\0';
			//printf("%s\n\n",subbuff);
			char *final=malloc(32*sizeof(char));	
			memset(final,'\0',32);		
			final=scat(dest,subbuff);
			dest = final;
		//	printf("%s\n\n",dest);
		}else if(!buff3[2])
		{
	
		//printf("yes it is R\n");
		int m = buff3[1]-48;
		char *src1=malloc(32*sizeof(char));	
		memset(src1,'\0',32);		
		src1=deciBinary(m);
		char subbuff[6];
		memcpy(subbuff,&src1[27],5);
		subbuff[5]='\0';
		//printf("%s\n\n",subbuff);
		char *final=malloc(32*sizeof(char));	
		memset(final,'\0',32);		
		final=scat(dest,subbuff);
		dest = final;
		//printf("%s\n\n",dest);
		}
	}
	else
	{

		printf("ERROR: Wrong Instruction provided..!");

	}

		
	//printf("\n\n%s\n\n",dest);

}

//Bit Representation for Load and Store

void load_store_instruction(char *select)
{
	char *buff1 = NULL;
	char *buff2 = NULL;
	char *buff3 = NULL;
	buff1=malloc(32*sizeof(char));
	buff2=malloc(32*sizeof(char));	
	buff3=malloc(32*sizeof(char));	
	memset(buff1,'\0',32);	
	memset(buff2,'\0',32);	
	memset(buff3,'\0',32);	
	if(strcasecmp(select,"Store")==0)
		buff1=STORE_MEMORY;
	else if(strcasecmp(select,"Load")==0)
		buff1=LOAD_MEMORY;	
	//char *dest = NULL;
	
	/*** Register 1 for Load ***/
	buff2=strtok(NULL," ");
	if(buff2[0]==82 || buff2[0]==114)
	{
		if(buff2[2])
		{
		
			char temp[3];
			memcpy(&temp,&buff2[1],2);
			temp[3]='\0';
			//printf("%d\n\n",atoi(temp));
			if(atoi(temp)>18)
			{
				printf("ERROR: NO SUCH REGISTER\n");
				exit(0);
			}
			char *src1=malloc(32*sizeof(char));	
			memset(src1,'\0',32);		
			src1=deciBinary(atoi(temp));
			char subbuff[6];
			memcpy(subbuff,&src1[27],5);
			subbuff[5]='\0';
			//printf("%s\n\n",subbuff);
			char *final=malloc(32*sizeof(char));	
			memset(final,'\0',32);		
			final=scat(buff1,subbuff);
			dest = final;
			//printf("%s\n\n",dest);
		}else if(!buff2[2])
		{
	
		//printf("yes it is R\n");
		int m = buff2[1]-48;
		char *src1=malloc(32*sizeof(char));	
		memset(src1,'\0',32);		
		src1=deciBinary(m);
		char subbuff[6];
		memcpy(subbuff,&src1[27],5);
		subbuff[5]='\0';
		//printf("%s\n\n",subbuff);
		char *final=malloc(32*sizeof(char));	
		memset(final,'\0',32);		
		final=scat(buff1,subbuff);
		dest = final;
		//printf("%s\n\n",dest);
		}
	}
	else
	{

		printf("ERROR: Wrong Instruction provided..!");

	}

	/*** Offset Value ***/
	buff2=strtok(NULL," ");
	int cn=atoi(buff2);
	char *src1=malloc(32*sizeof(char));	
	memset(src1,'\0',32);		
	src1=deciBinary(cn);
	//printf("%s\n\n",src1);
	char subbuff[6];
	memcpy(subbuff,&src1[27],5);
	subbuff[5]='\0';
	//printf("%s\n\n",subbuff);
	char *final=malloc(32*sizeof(char));	
	memset(final,'\0',32);		
	final=scat(dest,subbuff);
	dest = final;
	//printf("LOL is%s\n\n",dest);
		
	
	/*** Another Register ***/
	buff3=strtok(NULL," ");
	if(buff3[0]==82 || buff3[0]==114)
	{
		if(buff3[2])
		{
		
			char temp[3];
			memcpy(&temp,&buff3[1],2);
			temp[3]='\0';
			//printf("%d\n\n",atoi(temp));
			if(atoi(temp)>18)
			{
				printf("ERROR: NO SUCH REGISTER\n");
				exit(0);
			}
			char *src1=malloc(32*sizeof(char));	
			memset(src1,'\0',32);		
			src1=deciBinary(atoi(temp));
			char subbuff[6];
			memcpy(subbuff,&src1[27],5);
			subbuff[5]='\0';
			//printf("%s\n\n",subbuff);
			char *final=malloc(32*sizeof(char));	
			memset(final,'\0',32);		
			final=scat(dest,subbuff);
			dest = final;
	//		printf("%s\n\n",dest);
		}else if(!buff3[2])
		{
	
		//printf("yes it is R\n");
		int m = buff3[1]-48;
		char *src1=malloc(32*sizeof(char));	
		memset(src1,'\0',32);		
		src1=deciBinary(m);
		char subbuff[6];
		memcpy(subbuff,&src1[27],5);
		subbuff[5]='\0';
		//printf("%s\n\n",subbuff);
		char *final=malloc(32*sizeof(char));	
		memset(final,'\0',32);		
		final=scat(dest,subbuff);
		dest = final;
	//	printf("%s\n\n",dest);
		}
	}
	else
	{

		printf("ERROR: Wrong Instruction provided..!");

	}

}

// Bit Representation for load immediate and store immediate
void immediate_instruction(char *select)
{

	char *buff1 = NULL;
	char *buff2 = NULL;
	char *buff3 = NULL;
	buff1=malloc(32*sizeof(char));
	buff2=malloc(32*sizeof(char));	
	buff3=malloc(32*sizeof(char));	
	memset(buff1,'\0',32);	
	memset(buff2,'\0',32);	
	memset(buff3,'\0',32);	
	if(strcasecmp(select,"StoreImm")==0)
		buff1=STORE_MEMORY_imm;
	else if(strcasecmp(select,"LoadImm")==0)
		buff1=LOAD_MEMORY_imm;	
	
	buff2=strtok(NULL," ");

	if(buff2[0]==82 || buff2[0]==114)
	{
		if(buff2[2])
		{
		
			char temp[3];
			memcpy(&temp,&buff2[1],2);
			temp[3]='\0';
			//printf("%d\n\n",atoi(temp));
			if(atoi(temp)>18)
			{
				printf("ERROR: NO SUCH REGISTER\n");
				exit(0);
			}
			char *src1=malloc(32*sizeof(char));	
			memset(src1,'\0',32);		
			src1=deciBinary(atoi(temp));
			char subbuff[6];
			memcpy(subbuff,&src1[27],5);
			subbuff[5]='\0';
			//printf("%s\n\n",subbuff);
			char *final=malloc(32*sizeof(char));	
			memset(final,'\0',32);		
			final=scat(buff1,subbuff);
			dest = final;
			//printf("%s\n\n",dest);
		}else if(!buff2[2])
		{
	
		int m = buff2[1]-48;
		char *src1=malloc(32*sizeof(char));	
		memset(src1,'\0',32);		
		src1=deciBinary(m);
		char subbuff[6];
		memcpy(subbuff,&src1[27],5);
		subbuff[5]='\0';
		//printf("%s\n\n",subbuff);
		char *final=malloc(32*sizeof(char));	
		memset(final,'\0',32);		
		final=scat(buff1,subbuff);
		dest = final;
	//	printf("%s\n\n",dest);
		}
	}
	else
	{

		printf("ERROR: Wrong Instruction provided..!");

	}

	/*** Offset Value ***/

	buff2=strtok(NULL," ");
	int cn=atoi(buff2);
	char *src1=malloc(32*sizeof(char));	
	memset(src1,'\0',32);		
	src1=deciBinary(cn);
	char subbuff[11];
	memcpy(subbuff,&src1[22],10);
	subbuff[10]='\0';
	//printf("%s\n\n",subbuff);
	char *final=malloc(32*sizeof(char));	
	memset(final,'\0',32);		
	final=scat(dest,subbuff);
	dest = final;
	

	//printf("\n\n%s\n",dest);



}

// Bit Representation for Compare
void compare_instruction()
{

	char *buff1 = NULL;
	char *buff2 = NULL;
	char *buff3 = NULL;
	buff1=malloc(32*sizeof(char));
	buff2=malloc(32*sizeof(char));	
	buff3=malloc(32*sizeof(char));	
	memset(buff1,'\0',32);	
	memset(buff2,'\0',32);	
	memset(buff3,'\0',32);	
	buff1=CMPQ;	
	
	buff2=strtok(NULL," ");
	//printf("buffer is %s\n",buff2);
	if(buff2[0]==82 || buff2[0]==114)
	{
		if(buff2[2])
		{
		
			char temp[3];
			memcpy(&temp,&buff2[1],2);
			temp[3]='\0';
			printf("%d\n\n",atoi(temp));
			if(atoi(temp)>18)
			{
				printf("ERROR: NO SUCH REGISTER\n");
				exit(0);
			}
			char *src1=malloc(32*sizeof(char));	
			memset(src1,'\0',32);		
			src1=deciBinary(atoi(temp));
			char subbuff[6];
			memcpy(subbuff,&src1[27],5);
			subbuff[5]='\0';
			//printf("%s\n\n",subbuff);
			char *final=malloc(32*sizeof(char));	
			memset(final,'\0',32);		
			final=scat(buff1,subbuff);
			dest = final;
			//printf("%s\n\n",dest);
		}else if(!buff2[2])
		{
	
		int m = buff2[1]-48;
		char *src1=malloc(32*sizeof(char));	
		memset(src1,'\0',32);		
		src1=deciBinary(m);
		char subbuff[6];
		memcpy(subbuff,&src1[27],5);
		subbuff[5]='\0';
		//printf("%s\n\n",subbuff);
		char *final=malloc(32*sizeof(char));	
		memset(final,'\0',32);		
		final=scat(buff1,subbuff);
		dest = final;
		//printf("%s\n\n",dest);
		}
	}
	else
	{

		printf("ERROR: Wrong Instruction provided..!");

	}

	buff3=strtok(NULL," ");
	if(buff3[0]==82 || buff3[0]==114)
	{
		if(buff3[2])
		{
		
			char temp[3];
			memcpy(&temp,&buff3[1],2);
			temp[3]='\0';
			//printf("%d\n\n",atoi(temp));
			if(atoi(temp)>18)
			{
				printf("ERROR: NO SUCH REGISTER\n");
				exit(0);
			}
			char *src1=malloc(32*sizeof(char));	
			memset(src1,'\0',32);		
			src1=deciBinary(atoi(temp));
			char subbuff[6];
			memcpy(subbuff,&src1[27],5);
			subbuff[5]='\0';
			//printf("%s\n\n",subbuff);
			char *final=malloc(32*sizeof(char));	
			memset(final,'\0',32);		
			final=scat(dest,subbuff);
			dest = final;
	//		printf("%s\n\n",dest);
		}else if(!buff3[2])
		{
	
		//printf("yes it is R\n");
		int m = buff3[1]-48;
		char *src1=malloc(32*sizeof(char));	
		memset(src1,'\0',32);		
		src1=deciBinary(m);
		char subbuff[6];
		memcpy(subbuff,&src1[27],5);
		subbuff[5]='\0';
		//printf("%s\n\n",subbuff);
		char *final=malloc(32*sizeof(char));	
		memset(final,'\0',32);		
		final=scat(dest,subbuff);
		dest = final;
		printf("%s\n\n",dest);
		}
	}
	else
	{
		int cn = atoi(buff3);
		char *src1=malloc(32*sizeof(char));	
		memset(src1,'\0',32);		
		src1=deciBinary(cn);
		char subbuff[11];
		memcpy(subbuff,&src1[22],10);
		subbuff[10]='\0';
		printf("%s\n\n",subbuff);
		char *final=malloc(32*sizeof(char));	
		memset(final,'\0',32);		
		final=scat(dest,subbuff);
		dest = final;
		printf("%s\n\n",dest);
		
		//printf("ERROR: Wrong Instruction provided..!");

	}



}

// Bit Representation for Move Instruction
void move_instruction()
{

	char *buff1 = NULL;
	char *buff2 = NULL;
	char *buff3 = NULL;
	buff1=malloc(32*sizeof(char));
	buff2=malloc(32*sizeof(char));	
	buff3=malloc(32*sizeof(char));	
	memset(buff1,'\0',32);	
	memset(buff2,'\0',32);	
	memset(buff3,'\0',32);	
	buff1=MOVE;	
	
	buff2=strtok(NULL," ");
	//printf("buffer is %s\n",buff2);
	if(buff2[0]==82 || buff2[0]==114)
	{
		if(buff2[2])
		{
		
			char temp[3];
			memcpy(&temp,&buff2[1],2);
			temp[3]='\0';
			printf("%d\n\n",atoi(temp));
			if(atoi(temp)>18)
			{
				printf("ERROR: NO SUCH REGISTER\n");
				exit(0);
			}
			char *src1=malloc(32*sizeof(char));	
			memset(src1,'\0',32);		
			src1=deciBinary(atoi(temp));
			char subbuff[6];
			memcpy(subbuff,&src1[27],5);
			subbuff[5]='\0';
			
			char *final=malloc(32*sizeof(char));	
			memset(final,'\0',32);		
			final=scat(buff1,subbuff);
			dest = final;
			//printf("%s\n\n",dest);
		}else if(!buff2[2])
		{
	
		int m = buff2[1]-48;
		char *src1=malloc(32*sizeof(char));	
		memset(src1,'\0',32);		
		src1=deciBinary(m);
		char subbuff[6];
		memcpy(subbuff,&src1[27],5);
		subbuff[5]='\0';
		//printf("%s\n\n",subbuff);
		char *final=malloc(32*sizeof(char));	
		memset(final,'\0',32);		
		final=scat(buff1,subbuff);
		dest = final;
		//printf("%s\n\n",dest);
		}
	}
	else
	{

		printf("ERROR: Wrong Instruction provided..!");

	}

	buff3=strtok(NULL," ");
	if(buff3[0]==82 || buff3[0]==114)
	{
		printf("ERROR: Wrong Instruction provided..!");
	}
	else
	{
		int cn = atoi(buff3);
		char *src1=malloc(32*sizeof(char));	
		memset(src1,'\0',32);		
		src1=deciBinary(cn);
		char subbuff[11];
		memcpy(subbuff,&src1[22],10);
		subbuff[10]='\0';
		printf("ggggggggggggggggggggggggg %s\n\n",subbuff);
		char *final=malloc(32*sizeof(char));	
		memset(final,'\0',32);		
		final=scat(dest,subbuff);
		dest = final;
		//printf("%s\n\n",dest);
	}



}



// Bit Representation for Move Instruction
void moveR_instruction()
{
char *buff1 = NULL;
	char *buff2 = NULL;
	char *buff3 = NULL;
	buff1=malloc(32*sizeof(char));
	buff2=malloc(32*sizeof(char));	
	buff3=malloc(32*sizeof(char));	
	memset(buff1,'\0',32);	
	memset(buff2,'\0',32);	
	memset(buff3,'\0',32);	
	buff1=MOVER;	
	
	buff2=strtok(NULL," ");
	//printf("buffer is %s\n",buff2);
	if(buff2[0]==82 || buff2[0]==114)
	{
		if(buff2[2])
		{
		
			char temp[3];
			memcpy(&temp,&buff2[1],2);
			temp[3]='\0';
			printf("%d\n\n",atoi(temp));
			if(atoi(temp)>18)
			{
				printf("ERROR: NO SUCH REGISTER\n");
				exit(0);
			}
			char *src1=malloc(32*sizeof(char));	
			memset(src1,'\0',32);		
			src1=deciBinary(atoi(temp));
			char subbuff[6];
			memcpy(subbuff,&src1[27],5);
			subbuff[5]='\0';
			//printf("%s\n\n",subbuff);
			char *final=malloc(32*sizeof(char));	
			memset(final,'\0',32);		
			final=scat(buff1,subbuff);
			dest = final;
			//printf("%s\n\n",dest);
		}else if(!buff2[2])
		{
	
		int m = buff2[1]-48;
		char *src1=malloc(32*sizeof(char));	
		memset(src1,'\0',32);		
		src1=deciBinary(m);
		char subbuff[6];
		memcpy(subbuff,&src1[27],5);
		subbuff[5]='\0';
		//printf("%s\n\n",subbuff);
		char *final=malloc(32*sizeof(char));	
		memset(final,'\0',32);		
		final=scat(buff1,subbuff);
		dest = final;
		//printf("%s\n\n",dest);
		}
	}
	else
	{

		printf("ERROR: Wrong Instruction provided..!");

	}

	buff3=strtok(NULL," ");
	if(buff3[0]==82 || buff3[0]==114)
	{
		if(buff3[2])
		{
		
			char temp[3];
			memcpy(&temp,&buff3[1],2);
			temp[3]='\0';
			//printf("%d\n\n",atoi(temp));
			if(atoi(temp)>18)
			{

				printf("ERROR: NO SUCH REGISTER\n");
				exit(0);
			}
			char *src1=malloc(32*sizeof(char));	
			memset(src1,'\0',32);		
			src1=deciBinary(atoi(temp));
			char subbuff[6];
			memcpy(subbuff,&src1[27],5);
			subbuff[5]='\0';
			//printf("%s\n\n",subbuff);
			char *final=malloc(32*sizeof(char));	
			memset(final,'\0',32);		
			final=scat(dest,subbuff);
			dest = final;
	//		printf("%s\n\n",dest);
		}else if(!buff3[2])
		{
	
		//printf("yes it is R\n");
		int m = buff3[1]-48;
		char *src1=malloc(32*sizeof(char));	
		memset(src1,'\0',32);		
		src1=deciBinary(m);
		char subbuff[6];
		memcpy(subbuff,&src1[27],5);
		subbuff[5]='\0';
		//printf("%s\n\n",subbuff);
		char *final=malloc(32*sizeof(char));	
		memset(final,'\0',32);		
		final=scat(dest,subbuff);
		dest = final;
		printf("%s\n\n",dest);
		}
	}
	else
	{
		int cn = atoi(buff3);
		char *src1=malloc(32*sizeof(char));	
		memset(src1,'\0',32);		
		src1=deciBinary(cn);
		char subbuff[11];
		memcpy(subbuff,&src1[22],10);
		subbuff[10]='\0';
		printf("%s\n\n",subbuff);
		char *final=malloc(32*sizeof(char));	
		memset(final,'\0',32);		
		final=scat(dest,subbuff);
		dest = final;
		printf("%s\n\n",dest);
		
		//printf("ERROR: Wrong Instruction provided..!");

	}


}

// Bit Representation for Test
void test_instruction()
{

	char *buff1 = NULL;
	char *buff2 = NULL;
	char *buff3 = NULL;
	buff1=malloc(32*sizeof(char));
	buff2=malloc(32*sizeof(char));	
	buff3=malloc(32*sizeof(char));	
	memset(buff1,'\0',32);	
	memset(buff2,'\0',32);	
	memset(buff3,'\0',32);	
	buff1=TESTQ;	
	
	buff2=strtok(NULL," ");
	//printf("buffer is %s\n",buff2);
	if(buff2[0]==82 || buff2[0]==114)
	{
		if(buff2[2])
		{
		
			char temp[3];
			memcpy(&temp,&buff2[1],2);
			temp[3]='\0';
			printf("%d\n\n",atoi(temp));
			if(atoi(temp)>17)
			{
				printf("ERROR: NO SUCH REGISTER\n");
				exit(0);
			}
			char *src1=malloc(32*sizeof(char));	
			memset(src1,'\0',32);		
			src1=deciBinary(atoi(temp));
			char subbuff[6];
			memcpy(subbuff,&src1[27],5);
			subbuff[5]='\0';
			//printf("%s\n\n",subbuff);
			char *final=malloc(32*sizeof(char));	
			memset(final,'\0',32);		
			final=scat(buff1,subbuff);
			dest = final;
			//printf("%s\n\n",dest);
		}else if(!buff2[2])
		{
	
		int m = buff2[1]-48;
		char *src1=malloc(32*sizeof(char));	
		memset(src1,'\0',32);		
		src1=deciBinary(m);
		char subbuff[6];
		memcpy(subbuff,&src1[27],5);
		subbuff[5]='\0';
		//printf("%s\n\n",subbuff);
		char *final=malloc(32*sizeof(char));	
		memset(final,'\0',32);		
		final=scat(buff1,subbuff);
		dest = final;
		//printf("%s\n\n",dest);
		}
	}
	else
	{

		printf("ERROR: Wrong Instruction provided..!");

	}

	buff3=strtok(NULL," ");
	if(buff3[0]==82 || buff3[0]==114)
	{
		if(buff3[2])
		{
		
			char temp[3];
			memcpy(&temp,&buff3[1],2);
			temp[3]='\0';
			//printf("%d\n\n",atoi(temp));
			if(atoi(temp)>17)
			{
				printf("ERROR: NO SUCH REGISTER\n");
				exit(0);
			}
			char *src1=malloc(32*sizeof(char));	
			memset(src1,'\0',32);		
			src1=deciBinary(atoi(temp));
			char subbuff[6];
			memcpy(subbuff,&src1[27],5);
			subbuff[5]='\0';
			//printf("%s\n\n",subbuff);
			char *final=malloc(32*sizeof(char));	
			memset(final,'\0',32);		
			final=scat(dest,subbuff);
			dest = final;
	//		printf("%s\n\n",dest);
		}else if(!buff3[2])
		{
	
		//printf("yes it is R\n");
		int m = buff3[1]-48;
		char *src1=malloc(32*sizeof(char));	
		memset(src1,'\0',32);		
		src1=deciBinary(m);
		char subbuff[6];
		memcpy(subbuff,&src1[27],5);
		subbuff[5]='\0';
		//printf("%s\n\n",subbuff);
		char *final=malloc(32*sizeof(char));	
		memset(final,'\0',32);		
		final=scat(dest,subbuff);
		dest = final;
		printf("%s\n\n",dest);
		}
	}
	else
	{
		int cn = atoi(buff3);
		char *src1=malloc(32*sizeof(char));	
		memset(src1,'\0',32);		
		src1=deciBinary(cn);
		char subbuff[11];
		memcpy(subbuff,&src1[22],10);
		subbuff[10]='\0';
		printf("%s\n\n",subbuff);
		char *final=malloc(32*sizeof(char));	
		memset(final,'\0',32);		
		final=scat(dest,subbuff);
		dest = final;
		printf("%s\n\n",dest);
		
		//printf("ERROR: Wrong Instruction provided..!");

	}



}


// Bit Representation for JUMP Instruction
void jump_instruction(char *select)
{

	char *buff1 = NULL;
	char *buff2 = NULL;
	char *buff3 = NULL;
	char *buffs1 = NULL;
	buff1=malloc(32*sizeof(char));
	buff2=malloc(32*sizeof(char));	
	buff3=malloc(32*sizeof(char));	
	buffs1=malloc(32*sizeof(char));	
	memset(buff1,'\0',32);	
	memset(buff2,'\0',32);	
	memset(buff3,'\0',32);	
	memset(buffs1,'\0',32);	
	buff1=JUMP;	
	
	if(strcasecmp(select,"JMP")==0)
		buff2=JMP;
	else if(strcasecmp(select,"JLE")==0)
		buff2=JLE;
	else if(strcasecmp(select,"JE")==0)
		buff2=JE;
	else if(strcasecmp(select,"JG")==0)
		buff2=JG;
	else if(strcasecmp(select,"JGE")==0)
		buff2=JGE;
	else if(strcasecmp(select,"JNE")==0)
		buff2=JNE;
	else if(strcasecmp(select,"JS")==0)
		buff2=JS;
	else if(strcasecmp(select,"JNS")==0)
		buff2=JNS;
	else if(strcasecmp(select,"JL")==0)
		buff2=JL;
	
	buffs1=scat(buff1,buff2);
	buff1=buffs1;

	buff2=strtok(NULL," ");	
	char subbuff[6];
	memcpy(subbuff,buff2,5);
	subbuff[5]='\0';
	
	if(strcasecmp(subbuff,"Label")==0)
	{
		int x=buff2[5]-48;
		if(x>0 && x<=128)
		{
			char *src1=malloc(32*sizeof(char));	
			memset(src1,'\0',32);		
			src1=deciBinary(x);
			char subbuff[9];
			memcpy(subbuff,&src1[24],8);
			subbuff[8]='\0';
			//printf("%s\n\n",subbuff);
			char *final=malloc(32*sizeof(char));	
			memset(final,'\0',32);		
			final=scat(buff1,subbuff);
			dest = final;
			
		}
		else
		{
			printf("Invalid Label: only Label0 to Label8 allowed");
			exit(-1);		
		}
	}

}

// Bit Representation for Set Instructions

void Set_instruction(char *select){

	char *buff1 = NULL;
	char *buff2 = NULL;
	char *buff3 = NULL;
	buff1=malloc(32*sizeof(char));
	buff2=malloc(32*sizeof(char));	
	buff3=malloc(32*sizeof(char));	
	memset(buff1,'\0',32);	
	memset(buff2,'\0',32);	
	memset(buff3,'\0',32);	
	buff1=SET;	
	
	if(strcasecmp(select,"SETE")==0)
		buff2=SETE;
	else if(strcasecmp(select,"SETNE")==0)
		buff2=SETNE;
	else if(strcasecmp(select,"SETS")==0)
		buff2=SETS;
	else if(strcasecmp(select,"SETNS")==0)
		buff2=SETNS;
	else if(strcasecmp(select,"SETG")==0)
		buff2=SETG;
	else if(strcasecmp(select,"SETGE")==0)
		buff2=SETGE;
	else if(strcasecmp(select,"SETL")==0)
		buff2=SETL;
	else if(strcasecmp(select,"SETLE")==0)
		buff2=SETLE;
	
	dest=scat(buff1,buff2);
	buff3=strtok(NULL," ");
	if(buff3[0]==82 || buff3[0]==114)
	{
		if(buff3[2])
		{
		
			char temp[3];
			memcpy(&temp,&buff3[1],2);
			temp[3]='\0';
			//printf("%d\n\n",atoi(temp));
			if(atoi(temp)>18)
			{
				printf("ERROR: NO SUCH REGISTER\n");
				exit(0);
			}
			char *src1=malloc(32*sizeof(char));	
			memset(src1,'\0',32);		
			src1=deciBinary(atoi(temp));
			char subbuff[6];
			memcpy(subbuff,&src1[27],5);
			subbuff[5]='\0';
			//printf("%s\n\n",subbuff);
			char *final=malloc(32*sizeof(char));	
			memset(final,'\0',32);		
			final=scat(dest,subbuff);
			dest = final;
	//		printf("%s\n\n",dest);
		}else if(!buff3[2])
		{
	
		//printf("yes it is R\n");
		int m = buff3[1]-48;
		char *src1=malloc(32*sizeof(char));	
		memset(src1,'\0',32);		
		src1=deciBinary(m);
		char subbuff[6];
		memcpy(subbuff,&src1[27],5);
		subbuff[5]='\0';
		//printf("%s\n\n",subbuff);
		char *final=malloc(32*sizeof(char));	
		memset(final,'\0',32);		
		final=scat(dest,subbuff);
		dest = final;
		//printf("%s\n\n",dest);
		}
	}

	//printf("Dest is %s\n\n",dest);


}

/* Parse user input for binary search */

int data[1000];
int length=0;
int search=0;
int low=0;
int high=0;
void binary_data(){

	char *buff1 = NULL;
	char *buff2 = NULL;
	char *buff3 = NULL;
	buff1=malloc(32*sizeof(char));
	buff2=malloc(32*sizeof(char));	
	buff3=malloc(32*sizeof(char));	
	memset(buff1,'\0',32);	
	memset(buff2,'\0',32);	
	memset(buff3,'\0',32);
	dest=DATA;
	int i=0;
	for(i=0;i<length;i++)
	{	
		buff1 = strtok(NULL," ");	
		data[i] = atoi(buff1);
	}	

	
	high=length-1;
	wGReg(deciBinary(low),10);
	wGReg(deciBinary(high),9);
	sp=sp-high;

	int temp=sp;

	for(i=0; i<length;i++){
    	mar= mem;
	// calculating memory address based on OFFSet - and indirect addressing
    	mar=mar+temp *4 + data_mem_base;
	*mar = deciBinary(data[i]);
	temp++;
	}
	wGReg(deciBinary(sp),11);	

	

}

// Load Effective Address

void loadeffective(){

	char *buff1 = NULL;
	char *buff2 = NULL;
	char *buff3 = NULL;
	buff1=malloc(32*sizeof(char));
	buff2=malloc(32*sizeof(char));	
	buff3=malloc(32*sizeof(char));	
	memset(buff1,'\0',32);	
	memset(buff2,'\0',32);	
	memset(buff3,'\0',32);	
	buff1=LEA;	
	//char *dest = NULL;
	
	/*** Register 1 for Load ***/
	buff2=strtok(NULL," ");
	if(buff2[0]==82 || buff2[0]==114)
	{
		if(buff2[2])
		{
		
			char temp[3];
			memcpy(&temp,&buff2[1],2);
			temp[3]='\0';
			//printf("%d\n\n",atoi(temp));
			if(atoi(temp)>18)
			{
				printf("ERROR: NO SUCH REGISTER\n");
				exit(0);
			}
			char *src1=malloc(32*sizeof(char));	
			memset(src1,'\0',32);		
			src1=deciBinary(atoi(temp));
			char subbuff[6];
			memcpy(subbuff,&src1[27],5);
			subbuff[5]='\0';
			//printf("%s\n\n",subbuff);
			char *final=malloc(32*sizeof(char));	
			memset(final,'\0',32);		
			final=scat(buff1,subbuff);
			dest = final;
			//printf("%s\n\n",dest);
		}else if(!buff2[2])
		{
	
		//printf("yes it is R\n");
		int m = buff2[1]-48;
		char *src1=malloc(32*sizeof(char));	
		memset(src1,'\0',32);		
		src1=deciBinary(m);
		char subbuff[6];
		memcpy(subbuff,&src1[27],5);
		subbuff[5]='\0';
		//printf("%s\n\n",subbuff);
		char *final=malloc(32*sizeof(char));	
		memset(final,'\0',32);		
		final=scat(buff1,subbuff);
		dest = final;
		//printf("%s\n\n",dest);
		}
	}
	else
	{

		printf("ERROR: Wrong Instruction provided..!");

	}

	/*** Offset Value ***/
	buff2=strtok(NULL," ");
	int cn=atoi(buff2);
	char *src1=malloc(32*sizeof(char));	
	memset(src1,'\0',32);		
	src1=deciBinary(cn);
	//printf("%s\n\n",src1);
	char subbuff[6];
	memcpy(subbuff,&src1[27],5);
	subbuff[5]='\0';
	//printf("%s\n\n",subbuff);
	char *final=malloc(32*sizeof(char));	
	memset(final,'\0',32);		
	final=scat(dest,subbuff);
	dest = final;
	//printf("LOL is%s\n\n",dest);
		
	
	/*** Another Register ***/
	buff3=strtok(NULL," ");
	if(buff3[0]==82 || buff3[0]==114)
	{
		if(buff3[2])
		{
		
			char temp[3];
			memcpy(&temp,&buff3[1],2);
			temp[3]='\0';
			//printf("%d\n\n",atoi(temp));
			if(atoi(temp)>18)
			{
				printf("ERROR: NO SUCH REGISTER\n");
				exit(0);
			}
			char *src1=malloc(32*sizeof(char));	
			memset(src1,'\0',32);		
			src1=deciBinary(atoi(temp));
			char subbuff[6];
			memcpy(subbuff,&src1[27],5);
			subbuff[5]='\0';
			//printf("%s\n\n",subbuff);
			char *final=malloc(32*sizeof(char));	
			memset(final,'\0',32);		
			final=scat(dest,subbuff);
			dest = final;
	//		printf("%s\n\n",dest);
		}else if(!buff3[2])
		{
	
		//printf("yes it is R\n");
		int m = buff3[1]-48;
		char *src1=malloc(32*sizeof(char));	
		memset(src1,'\0',32);		
		src1=deciBinary(m);
		char subbuff[6];
		memcpy(subbuff,&src1[27],5);
		subbuff[5]='\0';
		//printf("%s\n\n",subbuff);
		char *final=malloc(32*sizeof(char));	
		memset(final,'\0',32);		
		final=scat(dest,subbuff);
		dest = final;
	//	printf("%s\n\n",dest);
		}
	}
	else
	{

		printf("ERROR: Wrong Instruction provided..!");

	}

	




}

// Selection of Instruction

void selectInstruction(char *buff1,int lineNumber)
{
		char *buffs=malloc(sizeof(char)*32);
		memcpy(buffs,buff1,5);
		if(!(strcasecmp(buff1,"ALU_OP"))){

			alu_instruction();

		}
		if((!(strcasecmp(buff1,"Load"))) || (!(strcasecmp(buff1,"Store")))){

			load_store_instruction(buff1);

		}
		if((!(strcasecmp(buff1,"LoadImm"))) || (!(strcasecmp(buff1,"StoreImm")))){

			immediate_instruction(buff1);

		}
		if(!strcasecmp(buff1,"CMPQ")){

			compare_instruction();			

		}
		if(!strcasecmp(buff1,"MOVE")){

			move_instruction();			

		}

		if(!strcasecmp(buff1,"MOVER")){

			moveR_instruction();			

		}
		if(!strcasecmp(buffs, "Label")){

			indexLabel=(int)buff1[5]-48;
			buff1=strtok(NULL," ");
			line=lineNumber+1;
			selectInstruction(buff1,line);
			
		}
		if(!strcasecmp(buff1,"JMP") || !strcasecmp(buff1,"JLE") || !strcasecmp(buff1,"JE") || !strcasecmp(buff1,"JG") || !strcasecmp(buff1,"JGE") || !strcasecmp(buff1,"JNE") || !strcasecmp(buff1,"JS") || !strcasecmp(buff1,"JNS") || !strcasecmp(buff1,"JL")){
		
			jump_instruction(buff1);
		}
		if(!strcasecmp(buff1,"SETE") || !strcasecmp(buff1,"SETNE") || !strcasecmp(buff1,"SETS") || !strcasecmp(buff1,"SETNS") || !strcasecmp(buff1,"SETG") || !strcasecmp(buff1,"SETGE") || !strcasecmp(buff1,"SETL") || !strcasecmp(buff1,"SETLE")){
		
			Set_instruction(buff1);
		}
		if(!strcasecmp(buff1,"TESTQ")){

			test_instruction();			

		}
		if(!strcasecmp(buff1,"length")){
			
			dest=DATA;			
			buffs=strtok(NULL," ");
			length = atoi(buffs);
			//printf("Length is %d\n",length);			
			wGReg(deciBinary(length),12);
		}
		if(!strcasecmp(buff1,"data")){

			binary_data();			

		}
		if(!strcasecmp(buff1,"search")){

			dest=DATA;			
			buffs=strtok(NULL," ");
			search = atoi(buffs);			
			//printf("Search is %d\n",search);			
			wGReg(deciBinary(search),13);
		}if(!strcasecmp(buff1,"LEA")){

			loadeffective();
		}	
	
}
int main()
{

	
	FILE *fp=0;	//File Pointer
	char buffer[2000];
	char *buff1=malloc(sizeof(buffer));
	memset(buff1,'\0',sizeof(buffer));		
			
	initialize();

	// loading register R1 with dummy value 10
	char *num=deciBinary(7);
	wGReg(num,1);

	// loading register R1 with dummy value 10
	num=deciBinary(8);
	wGReg(num,15);

	// loading register R13 with dummy value for indirect addressing
	num=deciBinary(6);
	wGReg(num,13);
	
	// loading register R14 with dummy value for indirect addressing
	num=deciBinary(-1);
	wGReg(num,14);

	//Reading instructions from file 

	fp=fopen("instruction.txt","r");
	
	if(fp==NULL)
	{

		printf("Error Opening file instruction.txt\n");
		exit(-1);
	}
	while(fgets(buffer,2000,fp))
	{
		//printf("%s\n\n",buffer);
		if(buffer[0]=='\n' || buffer[0]=='/')
		{
			continue;
		}
			totalInst++;
	}	
		printf("Total Instructions to be executed: %d",totalInst);
	
	fclose(fp);
	fp=fopen("instruction.txt","r");

	char *insts[totalInst];
	totalInst=0;
	while(fgets(buffer,2000,fp))
	{
		if(buffer[0]=='\n' || buffer[0]=='/')
		{
			continue;
		}		
		//printf("Instruction is: \n%s\n\n",buffer);
		buff1=strtok(buffer," ");
	
		selectInstruction(buff1,totalInst);

		char *append=malloc(32*sizeof(char));
		memset(append,'0',32-strlen(dest));
		dest = scat(dest,append);		
		int tempdest=bToInt(dest);
		if(line==totalInst+1)
		{
			tempdest=tempdest | indexLabel;
			dest = deciBinary(tempdest);
		}	
	
		insts[totalInst]=dest;
		printf("\n%s\n",insts[totalInst]);		
		totalInst++;
		
	}

	fclose(fp);
	// load instructions in to instuctions memory
	int size=sizeof(insts)/sizeof(char *);
	loadInstructions(insts, size);
	
	
	// load default instructions at some location in to instuctions memory for jump simulation
	
	dumpCpu();

	
	while(loopIndex<=totalInst){
		executeInstr();
		loopIndex++;

	    }
	   return 0;
}


void initialize()
{	int i=0;

	for(i=0;i<16;i++){
		R[i]=malloc(sizeof(char)*32+1);
		memset(R[i],0,sizeof(char)*32+1);
		R[i]="00000000000000000000000000000000";
	}
	sp=255;
	pc=0;
	//flag="00000000000000000000000000000000";
	flag=0;        
	mdr[0] = malloc(sizeof(char)*8+1);
	memset(mdr[0],0,sizeof(char)*8+1);
        mdr[1] = malloc(sizeof(char)*8+1);
	memset(mdr[1],0,sizeof(char)*8+1);
        mdr[2] = malloc(sizeof(char)*8+1);
	memset(mdr[2],0,sizeof(char)*8+1);
        mdr[3] = malloc(sizeof(char)*8+1);
	memset(mdr[3],0,sizeof(char)*8+1);
	for(i=0;i<(memsize);i++){
		mem[i]= malloc(sizeof(char)*8+1);
		memset(mem[i],0,sizeof(char)*8+1);
		mem[i]="00000000";
	}
}

void loadInstructions(char *inst[],int size){

	//memset(indexMem,'\0',32);	
	//printf("Here index is %d and Line is %d\n\n",indexLabel,line);
	pc=instr_mem_base;     
	int i;
	for( i=0 ; i < size; i++ ){
	char * p1= malloc(sizeof(char)*8+1);
	memset(p1,0,sizeof(char)*8+1);
	char * p2= malloc(sizeof(char)*8+1);
	memset(p2,0,sizeof(char)*8+1);
	char * p3= malloc(sizeof(char)*8+1);
	memset(p3,0,sizeof(char)*8+1);
	char * p4= malloc(sizeof(char)*8+1);
	memset(p4,0,sizeof(char)*8+1);
	char * p5= malloc(sizeof(char)*8+1);
	memset(p5,0,sizeof(char)*8+1);

        mar= mem;
	mar=mar+pc+i*4;
	char **s;
	s=mar;
        strncpy(p1, inst[i], 8);
	*(p1+8)='\0';
        *mar=mdr[0]=p1;

        mar = mar+1;
        strncpy(p2, inst[i]+8, 8);
 	*(p2+8)='\0';
        *mar=mdr[1]=p2;
  
        mar = mar+1;
        strncpy(p3, inst[i]+16, 8);
	*(p3+8)='\0'; 
        *mar=mdr[2]=p3;

        mar = mar+1;
        strncpy(p4, inst[i]+24, 8);
	*(p4+8)='\0';
         *mar=mdr[3]=p4;
	
	int cm=0;
	
	strncpy(p5, inst[i]+25, 7);
	*(p5+7)='\0';
	int templabel=0;
	templabel=bToInt(p5);
	//printf("Conversion is %d\n",templabel);
	
	if(templabel>0)
	{
	
		exe[templabel]=pc+i*4;//indexMem[q]=s;			
		loopexe[templabel]=i;
	}
	
 	}

}


void executeInstr()
{

	char *cirDummy=malloc(sizeof(char)*32+1);
		memset(cirDummy,0,sizeof(char)*32+1);

		strcat(cirDummy, mem[pc]);
		strcat(cirDummy, mem[pc+1]);
		strcat(cirDummy, mem[pc+2]);
		strcat(cirDummy, mem[pc+3]); 
		*(cirDummy+32)='\0';  
  		cir=cirDummy;
		printf("%s \n",cir);   
	       	// load opcode
	       	pc+=4;
	       	char subbuff[6];
		memcpy( subbuff, &cir[0],5  );
		subbuff[5] = '\0';
		if(strcmp(subbuff, STORE_MEMORY)==0) {
			storeToMemory(false);
			printf("\n");
			printf("\n");
			printf("***** Cpu dump after Store Instruction execution ***** \n");
			printf("*****  Data from R1 to third data memory location *****\n");
			printf("\n");
			printf("\n");
			dumpCpu();
		}
		
		else if(strcmp(subbuff, LOAD_MEMORY)==0){
			LoadFromMemory(false,false);
			printf("\n");
			printf("\n");
			printf("***** Cpu dump after Load Instruction execution ***** \n");
			printf("***** Data from third data memory location to register R2*****\n");
			printf("\n");
			printf("\n");
			dumpCpu();
		}
		else if(strcmp(subbuff, STORE_MEMORY_imm)==0){
			storeToMemory(true);
			printf("\n");
			printf("\n");
			printf("***** Store Immediate Operation ***** \n");
			printf("\n");
			printf("\n");
			dumpCpu();
		}
		else if(strcmp(subbuff, LOAD_MEMORY_imm)==0){
			LoadFromMemory(true,false);
			printf("\n");
			printf("\n");
			printf("***** Load Immediate Operation ***** \n");
			printf("\n");
			printf("\n");
			dumpCpu();
		}
		else if(strcmp(subbuff, ALU_Op)==0){
			printf("\n");
			printf("\n");
			printf("***** ALU Operation ***** \n");
			Alu();
			printf("\n");
			printf("\n");
			dumpCpu();
			
		}
		else if(strcmp(subbuff, JUMP)==0){
			printf("\n");
			printf("\n");
			printf("***** JUMP Operation *****\n");
			JumpOperation();
			printf("\n");
			printf("\n");
			//dumpCpu();
		}else if(strcmp(subbuff, CMPQ)==0){
			printf("\n");
			printf("\n");
			printf("***** COMPARE Operation *****\n");
			Compare();
			printf("\n");
			printf("\n");
			dumpCpu();
		}else if(strcmp(subbuff, MOVE)==0){
			printf("\n");
			printf("\n");
			printf("***** MOVE Operation *****\n");
			MoveInstruction();
			printf("\n");
			printf("\n");
			dumpCpu();
		}else if(strcmp(subbuff, MOVER)==0){
			printf("\n");
			printf("\n");
			printf("***** MOVE R Operation *****\n");
			MoveRInstruction();
			printf("\n");
			printf("\n");
			dumpCpu();
		}else if(strcmp(subbuff, SET)==0){
			printf("\n");
			printf("\n");
			printf("***** SET Operation *****\n");
			SetOperation();
			printf("\n");
			printf("\n");
			dumpCpu();
		}else if(strcmp(subbuff, TESTQ)==0){
			printf("\n");
			printf("\n");
			printf("***** TEST Operation *****\n");
			TestOperation();
			printf("\n");
			printf("\n");
			dumpCpu();
		}else if(strcmp(subbuff, DATA)==0){
			/*printf("\n");
			printf("\n");
			printf("***** No Operation *****\n");
			printf("\n");
			printf("\n");*/
		}else if(strcmp(subbuff, LEA)==0){
			printf("\n");
			printf("\n");
			printf("***** Load Effective Address Operation ***** \n");
			LoadEffectiveAddress();
			printf("\n");
			printf("\n");
			dumpCpu();
		}
}

void LoadEffectiveAddress(){

	char regUsed[6];
       	memcpy( regUsed, &cir[5],5  );
       	regUsed[5] = '\0';
	// register index 	
       	int regIndex=bToInt( regUsed) ;


	// offset
       	char offsetUsed[6];
       	memcpy( offsetUsed, &cir[10],5  );
        offsetUsed[5] = '\0';
    	// offset index 
    	int offsetBytes=bToInt( offsetUsed) ;    

	// calculate memory to be fetched from    
       char memUsed[6];
       memcpy( memUsed, &cir[15],5  );
       memUsed[5] = '\0';
       // memory index 
       // memory index 
    	int memBytes=bToInt( R[bToInt(memUsed)]) ;	
    	
    
	
    	// fill mar
    	mar= mem;

	// calculating memory address based on OFFSet - and indirect addressing
    	mar=mar+memBytes *4 + data_mem_base+ offsetBytes;    

	R[regIndex]= deciBinary(memBytes + offsetBytes);
  
	printf("Address %p loaded into Register\n",mar);


}
void storeToMemory(bool flg){

    	char regUsed[6];
        memcpy( regUsed, &cir[5],5 );
        regUsed[5] = '\0';
    	// register index 
    	int regIndex=bToInt( regUsed) ;
      
	if(flg){
        char memUsedImmi[11];
        memcpy( memUsedImmi, &cir[10],10  );
        memUsedImmi[10] = '\0';	


    	// fill mar
    	mar= mem;
	// calculating memory address -direct addressing
    	mar=mar+bToInt( memUsedImmi )*4+ data_mem_base;

	}

	else{

        char offsetUsed[6];
        memcpy( offsetUsed, &cir[10],5 );
        offsetUsed[5] = '\0';
        // offset index 
    	int offsetBytes=bToInt( offsetUsed) ;


        char memUsed[6];
        memcpy( memUsed, &cir[15],5  );
        memUsed[5] = '\0';
       // memory index 

	int temp=bToInt(memUsed);
	int memBytes;


	if(temp==17){
	memBytes=framePointer ;

    	// fill mar
    	mar= mem;
	// calculating memory address based on OFFSet - and indirect addressing
    	mar=mar+memBytes *4 + data_mem_base+ offsetBytes; 
printf("ddddddddd temp %p hhhh\n\n",(void *)mar);
	}
	if(temp==16){
	memBytes=sp ;

    	// fill mar
    	mar= mem;
	// calculating memory address based on OFFSet - and indirect addressing
    	mar=mar+memBytes *4 + data_mem_base+ offsetBytes; 
printf("ddddddddd temp %p hhhh\n\n",(void *)mar);
	}
	else{
	memBytes=bToInt( R[temp]) ;

    	// fill mar
    	mar= mem;
	// calculating memory address based on OFFSet - and indirect addressing
    	mar=mar+memBytes *4 + data_mem_base+ offsetBytes;  
	}





	}

	char * p1= malloc(sizeof(char)*8+1);
	char * p2= malloc(sizeof(char)*8+1);
	char * p3= malloc(sizeof(char)*8+1);
	char * p4= malloc(sizeof(char)*8+1);
	memset(p1,0,sizeof(char)*8+1);
	memset(p2,0,sizeof(char)*8+1);
	memset(p3,0,sizeof(char)*8+1);
	memset(p4,0,sizeof(char)*8+1);

        strncpy(p1, R[regIndex], 8);
       *(p1+8)='\0';
        *mar= mdr[0]=p1;

        mar = mar+1;
        strncpy(p2, R[regIndex]+8, 8);
       *(p2+8)='\0';
        *mar= mdr[1]=p2;
  
        mar = mar+1;
        strncpy(p3, R[regIndex]+16, 8);
        *(p3+8)='\0';
        *mar= mdr[2]=p3;

        mar = mar+1;
        strncpy(p4, R[regIndex]+24, 8);
       *(p4+8)='\0';
        *mar= mdr[3]=p4;
    	printf("ddddddddd p4p4p4p4 %s hhhh",p4);
}
void LoadFromMemory( bool flg,bool hlfflg){
       char regUsed[6];
       memcpy( regUsed, &cir[5],5  );
       regUsed[5] = '\0';
	// register index 	
       	int regIndex=bToInt( regUsed) ;

	if(flg){
        char memUsedImmi[11];
        memcpy( memUsedImmi, &cir[10],10  );
        memUsedImmi[10] = '\0';	


    	// fill mar
    	mar= mem;
	// calculating memory address -direct addressing
    	mar=mar+bToInt( memUsedImmi )*4+ data_mem_base;

	}

	else{


	// offset
       char offsetUsed[6];
       memcpy( offsetUsed, &cir[10],5  );
        offsetUsed[5] = '\0';
    	// offset index 
    	int offsetBytes=bToInt( offsetUsed) ;    

	// calculate memory to be fetched from    
       char memUsed[6];
       memcpy( memUsed, &cir[15],5  );
       memUsed[5] = '\0';
       // memory index 
       // memory index 
    	int memBytes=bToInt( R[bToInt(memUsed)]) ;	
    	
    
	
    	// fill mar
    	mar= mem;

	// calculating memory address based on OFFSet - and indirect addressing
    	mar=mar+memBytes *4 + data_mem_base+ offsetBytes;    

	}

	char * p1= malloc(sizeof(char)*8+1);
	char * p2= malloc(sizeof(char)*8+1);
	char * p3= malloc(sizeof(char)*8+1);
	char * p4= malloc(sizeof(char)*8+1);

	if(hlfflg){
		memset(mdr[0],0, 8);
		memset(mdr[1], 0, 8);
		mdr[0][9]='\0';
		strncpy(mdr[2], *mar, 8);
		mdr[0][9]='\0';
	       mar = mar+1;
		strncpy(mdr[3], *mar, 8);
		mdr[0][9]='\0';

	}
	else{

		strncpy(p1, *mar, 8);
		*(p1+8)='\0';
		mdr[0]=p1;

		mar = mar+1;
		strncpy(p2, *mar, 8);
		*(p2+8)='\0';
		mdr[1]=p2;		


		mar = mar+1;
		strncpy(p3, *mar, 8);
		*(p3+8)='\0';
		mdr[2]=p3;
	       	
		mar = mar+1;
		strncpy(p4, *mar, 8);
		*(p4+8)='\0';
		mdr[3]=p4;


	}

	 char *regVal=malloc(sizeof(char)*32+1);
         strcat(regVal, mdr[0]);
         strcat(regVal, mdr[1]);
         strcat(regVal, mdr[2]);
         strcat(regVal, mdr[3]); 

	 R[regIndex]=regVal;
  
}



/* performs binary addition for the given values */
  int binaryAddition(int n1, int n2) {
        int carry;
	cf=0;
        while (n2 != 0) {
                /* calculating the carry and do a left shift*/
                carry = (n1 & n2) << 1;
                /* calculating the sum */
                n1 = n1 ^ n2;
                n2 = carry;
        }
	if(n2>0){cf=1;}

        return n1;
  }

  /* performs binary subtraction for the given values */
  int binarySubtracton(int n1, int n2) {
        int carry;
	cf=0;
        /* finding two's complement for n2 and add the o/p with n1 */
        n2 = binaryAddition(~n2, 1);

        while (n2 != 0) {
                /* calculating the carry and do a left shift*/
                carry = (n1 & n2) << 1;
                /* calculating the sum */
                n1 = n1 ^ n2;
                n2 = carry;
        }
	if(n2>0){cf=1;}
        return n1;
  }

  /* performs binary multiplication for the given values */
  int binaryMultiplication(int n1, int n2) {
        int i, res = 0;
    	if(n2<0){
    		n1=-n1;
    		n2=abs(n2);
    	}

        for (i = 0; i < n2; i++) {
                res = binaryAddition(res, n1);
        }
        return res;
  }

  /* performs binary division for the given values */
  int binaryDivision(int n1, int n2) {
        int i, res, count = 0, twosComplement;
    printf("ttttttttttttttttttttt %d", n1);
printf("ttttttttttttttttttttt %d", n2);    
	bool sig=false;

	if( n1<0 && n2>0 ){sig=true;}
	else if(n1>0 && n2<0){sig=true;}
	else if(n1<0 && n2<0){sig=false;}
	
	n1=abs(n1);
	n2=abs(n2);

	if (n2==0) {
		printf("Wrong division values instruction!! Can not be divided by zero.\n");
                exit(-1);
        }
	if (n1 < n2) {
                return count;
        }

        res = n1;
        twosComplement = binaryAddition(~n2, 1);
        /*
         * Add n1 with 2's complement of n2 continuosly
         * until n1 becomes lesser value than n2.
         * Division of two numbers without using arithmetic
         * operator.
         */
        for (i = 0; res > 0; i++) {
                res = binaryAddition(res, twosComplement);
                if (res <= 0) {
                        if (res == 0)
                                count = binaryAddition(count, 1);
                        break;
                } else {
                        count = binaryAddition(count, 1);
                }
        }
	if(sig){
		count=-count;
	}
printf("ttttttttttttttttttttt %d", count);
        return count;
  }

  /* performs binary division for the given values */
  int binaryMod(int n1, int n2) {
	return binarySubtracton(n1,binaryMultiplication(binaryDivision(n1,n2), n2));
  }

void Alu(){

      char fun[6];
       memcpy( fun, &cir[5],5 );
       fun[5] = '\0';
	// function to be performed 	
       	int operation=bToInt( fun) ;



      char src1[6];
       memcpy( src1, &cir[10],5  );
       src1[5] = '\0';
	// register val	
	int src11=bToInt( src1);
	int int_src1;
		if(src11==16){int_src1= sp; }
		else if(src11==17){int_src1=framePointer; }
		else{int_src1= bToInt( R[src11]) ;}


      char src2[6];
       memcpy( src2, &cir[15],5  );
       src2[5] = '\0';
	// register val	
	int src22=bToInt( src2);
	int int_src2;
		if(src22==16){int_src2= sp; }
		else if(src22==17){int_src2=framePointer; }
		else{int_src2= bToInt( R[src22]) ;}


      char dst[6];
       memcpy( dst, &cir[20],5  );
       dst[5] = '\0';
	// register index 	
       	int int_dst_ind=bToInt( dst) ;
	
	int result;
	of=0;
	sf=0;
	cf=0;
	zf=0;

      switch(operation) {
                case 1:
			printf("\n");
			printf("\n");
			printf("***** Addition Function ***** \n");
                        // perform binary addition by decoding current instruction
			  if (((int_src2 > 0) && (int_src1 > (INT_MAX - int_src2))) ||
			      ((int_src2 < 0) && (int_src1 < (INT_MIN - int_src2)))) {
				of=1;
			}

			AluFunPtr = &binaryAddition;
	                break;
                case 2:
			printf("\n");
			printf("\n");
			printf("***** Subtraction Function ***** \n");
                        // perform binary subtraction by decoding current instruction
			  if ((int_src2 > 0 && int_src1 < INT_MIN + int_src2) ||
			      (int_src2 < 0 && int_src1 > INT_MAX + int_src2)) {
				of=1;	
				}

			AluFunPtr = &binarySubtracton;                        
			break;
                case 3:
			printf("\n");
			printf("\n");
			printf("***** Multiplication Function ***** \n");
                        // perform binary multiplication by decoding current instruction
			  signed long long tmp;
			  assert(ULLONG_MAX >= 2 * UINT_MAX);
			  tmp = (signed long long)int_src1 * (signed long long)int_src2;
			  if ((tmp > INT_MAX) || (tmp < INT_MIN)) {
			    of=1;
			  }

			AluFunPtr = &binaryMultiplication;
                        break;
                case 4:
			printf("\n");
			printf("\n");
			printf("***** Division Function ***** \n");
                        // perform binary division by decoding current instruction
			  if ((int_src2 == 0) || ((int_src1 == LONG_MIN) && (int_src2 == -1))) {
			    of=1;
			  }

			AluFunPtr = &binaryDivision;
                        break;
                case 5:
                        // perform binary mod by decoding current instruction
			printf("\n");
			printf("\n");
			printf("***** Remainder -modulus  Function ***** \n");
			  if ((int_src2 == 0) || ((int_src1 == LONG_MIN) && (int_src2 == -1))) {
			    of=1;
			  }
			AluFunPtr = &binaryMod;
                        break;		

                default:
                        printf("Wrong instruction!!\n");
                        break;
        }
	result=(*AluFunPtr)(int_src1, int_src2);

	if(result== 0){ zf =1;}
	if(result < 0){sf =1;}

	// writing to register value
	wGReg(deciBinary(result) ,int_dst_ind);

}

/***MOVE FUNCTION***/
void MoveInstruction()
{

	
	int cmp_result=0;
 	
	char src1[6];
       	memcpy( src1, &cir[5],5  );
       	src1[5] = '\0';

	// register val	
       	int int_src1= bToInt( src1);

        char value[11];
	//memset(value,'0',sizeof(value));
       	memcpy(value, &cir[10],10);
        value[10] = '\0';
        char value2[33];
	memset( value2, '0',32  );	
       	memset( value2, value[0],22  );	
	value2[32] = '\0';

    	// offset index 
    	int valueUsed=bToInt(value2) | bToInt(value) ;        
	
	char *num=deciBinary(valueUsed);
		if(int_src1==16){sp= valueUsed; }
		else if(int_src1==17){framePointer=valueUsed; }
		else{wGReg(num,int_src1);}


}

void MoveRInstruction()
{

	
	int cmp_result=0;
 	
	char src1[6];
       	memcpy( src1, &cir[5],5  );
       	src1[5] = '\0';

	// register val	
       	int int_src1= bToInt( src1);



        char value[6];
	//memset(value,'0',sizeof(value));
       	memcpy(value, &cir[10],5);
        value[5] = '\0';
    	// offset index 
    	int valueUsed=bToInt(value) ;  
	int copyVal;



		if(int_src1==16){
			if(valueUsed==16){sp=sp ;}
			else if(valueUsed==17){sp=framePointer; }
			else{ sp= bToInt( R[valueUsed]); }

		 }
		else if(int_src1==17){

			if(valueUsed==16){framePointer=sp ;}
			else if(valueUsed==17){framePointer=framePointer; }
			else{ framePointer= bToInt( R[valueUsed]); }

		 }
		else{
			if(valueUsed==16){wGReg(deciBinary(sp),int_src1); }
			else if(valueUsed==17){wGReg(deciBinary(framePointer),int_src1); }
			else if(valueUsed==18){wGReg(deciBinary(pc),int_src1); }
			else{ wGReg(R[valueUsed],int_src1);}

		}

}
/***COMPARE FUNCTION***/

void Compare()
{

	zf=0;
	sf=0;
	of=0;
	cf=0;
	int cmp_result=0;
 	
      	char src1[6];
       	memcpy( src1, &cir[5],5  );
       	src1[5] = '\0';

	// register val	
       	int int_src1= bToInt( R[bToInt( src1)]) ;

      	char src2[6];
       memcpy( src2, &cir[10],5  );
       src2[5] = '\0';
	// register val	

       	int int_src2=bToInt( R[bToInt( src2)]) ;
	
	AluFunPtr = &binarySubtracton;

	cmp_result=(*AluFunPtr)(int_src1, int_src2);

	printf("Result is %d\n",cmp_result);
	if(cmp_result == 0)
		zf=1;

	if(cmp_result < 0)
		sf=1;

//	printf("Flag is %d   \t %s\n",flag,deciBinary(flag));
	
}

/***TEST FUNCTION***/

void TestOperation(){

	zf=0;
	sf=0;
	of=0;
	cf=0;

	char src1[6];
       	memcpy( src1, &cir[5],5  );
       	src1[5] = '\0';

	// register val	
       	int int_src1= bToInt( R[bToInt( src1)]) ;

      	char src2[6];
       	memcpy( src2, &cir[10],5  );
       	src2[5] = '\0';
	// register val	

       	int int_src2=bToInt( R[bToInt( src2)]) ;
	
	if((int_src1 & int_src2)==0)
		zf=1;
	else if((int_src1 & int_src2)<0)
		sf=1;
	
}

/***Unconditional Jump***/

void UncondJump(int loc)
{

	pc=exe[loc];		
	loopIndex=loopexe[loc];	
	
}

	int cnt=0;	//to print the number of iterations in a loop

// JUMP IF EQUAL
void jmpequal(int loc)
{
	
	if(zf==1)
	{
	pc=exe[loc];		
	loopIndex=loopexe[loc];	
	}else
	{
		printf("Out of the Loop\n");
	}
	
}

// JUMP IF NOT EQUAL

void jmpntequal(int loc)
{

	
	if((~zf)==1)
	{
	pc=exe[loc];		
	loopIndex=loopexe[loc];	
	}else
	{
		printf("Out of the Loop\n");
	}
	
	
}

// JUMP IF NEGATIVE

void jmpsgnd(int loc)
{

	
	if(sf==1)
	{
	pc=exe[loc];		
	loopIndex=loopexe[loc];	
	}else
	{
		printf("Out of the Loop\n");
	}
	
}

// JUMP IF NONNEGATIVE

void jmpnonsgnd(int loc)
{

	
	if((~sf)==1)
	{
	pc=exe[loc];		
	loopIndex=loopexe[loc];	
	}else
	{
		printf("Out of the Loop\n");
	}
	
}
// JUMP IF GREATER THAN

void jmpgreater(int loc)
{
	if((~(sf^of) & ~zf)==1)
	{
		pc=exe[loc];		
	//printf("\nInstruction to be executed at location %d is\n",pc);
		loopIndex=loopexe[loc];	
	//printf("\nInstruction to be executed at index %d is\n",loopIndex);
	}else
	{
		printf("Out of the Loop\n");
	}
}

// JUMP IF GREATER THAN OR EQUAL

void jmpgreatequal(int loc)
{
	if((~(sf^of))==1)
	{
		pc=exe[loc];		
	//printf("\nInstruction to be executed at location %d is\n",pc);
		loopIndex=loopexe[loc];	
	//printf("\nInstruction to be executed at index %d is\n",loopIndex);
	}else
	{
		printf("Out of the Loop\n");
	}
}

// JUMP IF LESS THAN

void jmpless(int loc)
{
	if((sf^of)==1)
	{
		pc=exe[loc];		
		loopIndex=loopexe[loc];	
printf("tetetetetetetete\n");
	}else
	{
		printf("Out of the Loop\n");
	}
}

// JUMP IF LESS THAN OR EQUAL

void jmplessequal(int loc)
{
	if(((sf^of)|zf)==1)
	{
	pc=exe[loc];		
	//printf("\nInstruction to be executed at location %d is\n",pc);
	loopIndex=loopexe[loc];	
	printf("\nInstruction to be executed at index %d is\n",loopIndex);
	}else
	{
		printf("Out of the Loop\n");
	}
}

// Function Calls for different jump instruction

void JumpOperation(){

	char fun[6];
      	memcpy( fun, &cir[5],5 );
       	fun[5] = '\0';
	      	
	char labels[9];
      	memcpy( labels, &cir[10],8 );
       	labels[8] = '\0';
	// function to be performed 	
       	int operation=bToInt( fun) ;
       	int labelIndex=bToInt( labels) ;

      	switch(operation) {
                case 1:
			printf("\n");
			printf("\n");
			printf("***** Unconditional Jump Function ***** \n");
                        JmpPtr = &UncondJump;
	                break;
                case 2:
			printf("\n");
			printf("\n");
			printf("***** Jump if equal Function ***** \n");
                        JmpPtr = &jmpequal;   
			//cnt++;
			//printf("Final Count is %d\n\n",cnt);
			break;
             	case 3:
			printf("\n");
			printf("\n");
			printf("***** Jump if Not Equal Function ***** \n");
                       	JmpPtr = &jmpntequal;                        
			break;
		case 4:
			printf("\n");
			printf("\n");
			printf("***** Jump if Negative ***** \n");
                       	JmpPtr = &jmpsgnd;                        
			break;
		case 5:
			printf("\n");
			printf("\n");
			printf("***** Jump if Non-Negative ***** \n");
                       	JmpPtr = &jmpnonsgnd;                        
			break;
		case 6:
			printf("\n");
			printf("\n");
			printf("***** Jump if Greater than Function ***** \n");
                       	JmpPtr = &jmpgreater;                        
			break;
		case 7:
			printf("\n");
			printf("\n");
			printf("***** Jump if Greater than or equal Function ***** \n");
                       	JmpPtr = &jmpgreatequal;                        
			break;
		case 8:
			printf("\n");
			printf("\n");
			printf("***** Jump if Less than Function ***** \n");
                       	JmpPtr = &jmpless;                        
			break;
		case 9:
			printf("\n");
			printf("\n");
			printf("***** Jump if Less than or equal Function ***** \n");
                       	JmpPtr = &jmplessequal;                        
			break;
                default:
                        printf("Wrong instruction!!\n");
                        break;
        }
	(*JmpPtr)(labelIndex);
}

// Function Calls for different Set instruction

void SetOperation(){

	char fun[6];
      	memcpy( fun, &cir[5],5 );
       	fun[5] = '\0';
	      	
	char dst[6];
       	memcpy( dst, &cir[10],5  );
       	dst[5] = '\0';
	// register index 	
       	int int_dst_ind=bToInt( dst) ;
	// function to be performed 	
       	int operation=bToInt( fun) ;
       	int result=1;
      	switch(operation) {
                case 1:
			printf("\n");
			printf("\n");
			printf("***** Set if Equal Function ***** \n");
			//JmpPtr = &UncondJump;
			if(zf==1)
				wGReg(deciBinary(result) ,int_dst_ind);
	                break;
                case 2:
			printf("\n");
			printf("\n");
			printf("***** Set if Not Equal Function ***** \n");
                        //JmpPtr = &UncondJump;
			if((~zf)==1)
				wGReg(deciBinary(result) ,int_dst_ind);
	                break;
             	case 3:
			printf("\n");
			printf("\n");
			printf("***** Set if Negative Function ***** \n");
                        //JmpPtr = &UncondJump;
			if(sf==1)
				wGReg(deciBinary(result) ,int_dst_ind);
	                break;
		case 4:
			printf("\n");
			printf("\n");
			printf("***** Set if Not Negative Function ***** \n");
                        //JmpPtr = &UncondJump;
			if((~sf)==1)
				wGReg(deciBinary(result) ,int_dst_ind);
	                break;
		case 5:
			printf("\n");
			printf("\n");
			printf("***** Set if Greater than Function ***** \n");
                        //JmpPtr = &UncondJump;
			if((~(sf^of) & ~zf)==1)
				wGReg(deciBinary(result) ,int_dst_ind);
	                break;
		case 6:
			printf("\n");
			printf("\n");
			printf("***** Set if Greater than or Equal Function ***** \n");
                        //JmpPtr = &UncondJump;
			if((~(sf^of))==1)
				wGReg(deciBinary(result) ,int_dst_ind);
	                break;
		case 7:
			printf("\n");
			printf("\n");
			printf("***** Set if Less than Function ***** \n");
                        //JmpPtr = &UncondJump;
			if((sf^of)==1)
				wGReg(deciBinary(result) ,int_dst_ind);
	                break;
		case 8:
			printf("\n");
			printf("\n");
			printf("***** Set if Less than or Equal Function ***** \n");
                        //JmpPtr = &UncondJump;
			if(((sf^of)|zf)==1)
				wGReg(deciBinary(result) ,int_dst_ind);
	                break;
		default:
                        printf("Wrong instruction!!\n");
                        break;
        }
	
}

void wGReg(char *val,int loc)
{

		if(loc==16){ sp=bToInt(val); }
		else if(loc==17){framePointer=bToInt(val); }
		else{R[loc] = val;}

}

char *deciBinary(int n)
{
   int a, d, cnt;
   char *ptr;
 
   cnt = 0;
   ptr = (char*)malloc(32+1);
 
   if ( ptr == NULL )
      exit(EXIT_FAILURE);
 
   for ( a = 31 ; a >= 0 ; a-- )
   {
      d = n >> a;
 
      if ( d & 1 )
         *(ptr+cnt) = 1 + '0';
      else
         *(ptr+cnt) = 0 + '0';
 
      cnt++;
   }
   *(ptr+cnt) = '\0';
 
   return  ptr;
}


int bToInt( const char* instr ) 
{          
    int result = 0 ;
    int b = 0; 
    while( instr[b] != '\0' )
    {
        if( instr[b] == '1' )
        {
            result |= 1 ;
        }

        b++ ;
        if( instr[b] != '\0' )
        {
            result <<= 1 ;
        }
    }
    return result ;
} 

void dumpCpu(){
	int i;
	// display general register values
	printf(" ----- General Purpose Registers -----  \n");
	for(i=0;i<16;i++)
	{
		printf(" R[%d] = %s (%d) \n",i,R[i],bToInt( R[i] ));
	}
	printf("  ----- Special Purpose Registers -----  \n");
	// display special register values

	/* Program Counter */
	printf(" pc = %p (%d) \t", (void *) &mem[pc], pc);
	/* Stack Pointer */
	printf(" sp = %p (%d)  \t",(void *) &mem[sp],sp);
	/* frame Pointer */
	printf(" fp = %d \t", framePointer);
	printf("  \n");
	/* Flag Register */
	printf(" flag = %d  \t",flag);
 	/* Memory address Register */
	printf(" mar = %p \n",(void *)mar);
	/* Memory data Register */
        char *strMdr= malloc(sizeof(char)*32+1);
	memset(strMdr,0,sizeof(char)*32+1);
         strcat(strMdr, mdr[0]);
         strcat(strMdr, mdr[1]);
         strcat(strMdr, mdr[2]);
         strcat(strMdr, mdr[3]); 	
	
	printf(" mdr = %s (%d) \n",strMdr,bToInt( strMdr ) );
	/* Current instruction register */
	//printf(" cir = %s  ",cir);
	printf(" \n ----- Focused Instruction Memory Dump -----  \n");
	printf(" Memory Address \t Instruction Data \n");
		
	for(i=512;i<549;i=i+4)
	{	char *str=malloc(sizeof(char)*32+1);
		memset(str,0,sizeof(char)*32+1);

		strcat(str, mem[i]);
		strcat(str, mem[i+1]);
		strcat(str, mem[i+2]);
		strcat(str, mem[i+3]);  
		*(str+32)='\0';
		printf(" %p  \t %s \n",(void *) &mem[i], str);
		//free(str);
	}
	printf(" \n ----- Focused stack Memory Dump -----  \n");

/*	printf(" Memory Address \t Data \n");
	for(i=1024;i<1061;i=i+4)
	{
		
		char *str=malloc(sizeof(char)*32+1);
		memset(str,0,sizeof(char)*32+1);
		strcat(str, mem[i]);
		strcat(str, mem[i+1]);
		strcat(str, mem[i+2]);
		strcat(str, mem[i+3]);  
		*(str+32)='\0';

	    printf(" %p  \t %s (%d) \n",(void *) &mem[i], str,bToInt( str ));
	}

	printf(" \n----------------------------------------------------------------------- \n");
*/
	printf(" Memory Address \t Data \n");
	for(i=1944;i<2045;i=i+4)
	{
		
		char *str=malloc(sizeof(char)*32+1);
		memset(str,0,sizeof(char)*32+1);
		strcat(str, mem[i]);
		strcat(str, mem[i+1]);
		strcat(str, mem[i+2]);
		strcat(str, mem[i+3]);  
		*(str+32)='\0';

	    printf(" %p  \t %s (%d) \n",(void *) &mem[i], str,bToInt( str ));
	}

	printf(" \n----------------------------------------------------------------------- \n");
printf("Flags: zf - %d, sf - %d, cf - %d, of -%d\n ", zf,sf,cf,of);

printf("Incase of binary search, the index value found in the array matching the key (%d) is %d\n ", bToInt( R[13]),bToInt( R[14] ));



}


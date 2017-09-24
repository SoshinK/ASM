#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <cstring>


#define DEFAULT_SOURCE_NAME "source.txt"
#define DEFAULT_ASM_NAME "asm.txt"

enum 
	{
	MAX_CMD_SIZE = 16,
	MAX_Q_FLAGS = 2048,
	MAX_FLAGNAME_SIZE = 32
	};

enum ASM_CMDS
	{
		ASM_default,
	#define ASMCMDS
	#define COMMAND(cmd_name, q_args) \
		ASM_##cmd_name,
	#include "ASMCMDS.txt"
	#undef COMMAND
	#undef ASMCMDS
	};
enum ASM_REGS
	{
	#define ASMREGS
	#define REGISTER(reg_name, rnumber)  \
		ASM_##reg_name,
	#include "ASMREGS.txt"
	#undef REGISTER
	#undef ASMREGS
	};
/*!
default 0
COMMAND(mov, 2) 1
COMMAND(movr, 2) 2 

COMMAND(push, 1) 3
COMMAND(pushr, 1) 4
COMMAND(pop, 1) 5
COMMAND(adds, 0) 6
COMMAND(subs, 0) 7
COMMAND(muls, 0) 8
COMMAND(divs, 0) 9
  
COMMAND(addr, 2) 10
COMMAND(subr, 2) 11
COMMAND(mulr, 2) 12
COMMAND(divr, 2) 13

COMMAND(addn, 2) 14
COMMAND(subn, 2) 15
COMMAND(muln, 2) 16
COMMAND(divn, 2) 17

COMMAND(inr, 1) 18
COMMAND(out, 1) 19

COMMAND(cmp, 2) 20 
COMMAND(jmp, 1) 21
COMMAND(ja, 1) 22
COMMAND(jae, 1) 23 
COMMAND(jb, 1) 24
COMMAND(jbe, 1) 25
COMMAND(je, 1) 26
COMMAND(jne, 1) 27

COMMAND(hlt, 0) 28

ax 29
bx 30
cx 31
dx 32
flag 33
sp 34
ip 35

*/

class ASM
	{
	public:
		ASM();
		~ASM();
		void compile();
		void compile(char* asm_name);
		bool load_source(char* source_name);
		void dump();
	private:
		void precompile();
		ASM_CMDS get_cmd();
		int get_q_args();
		size_t get_source_size();

		FILE* source_;
		FILE* asm_;

		char* prgrm_;
		char* cur_cmd_;
		int q_cmds_;

		//char* flags_[MAX_Q_FLAGS];
	};

ASM::ASM():
	source_(NULL),
	asm_(NULL),
	prgrm_(NULL),
	cur_cmd_((char*)malloc(sizeof(char) * MAX_CMD_SIZE)),
	q_cmds_(0)
	{
	/*for(int i = 0; i < MAX_Q_FLAGS; i++)
		{
		flags_[i] = NULL;
		}*/
	}

ASM::~ASM()
	{
	fclose(source_);
	fclose(asm_);
	free(prgrm_);
	free(cur_cmd_);
	}

void ASM::dump()
	{
	printf("\n***Dump has been called***\n");
	printf("source_ = %p\n", source_);
	printf("asm_ = %p\n", asm_);
	printf("prgrm_ = %p\n", prgrm_);
	printf("cur_cmd_ = %p\n", cur_cmd_);
	printf("q_cmds_ = %d\n", q_cmds_);
	//if(*flags_ == NULL) printf("null lol omg\n");
	}
/*
bool ASM::load_source(char* source_name)
	{
	FILE* f= fopen(source_name, "rt");
	if(f == NULL) return false;
	source_ = f;
	return true;
	}

size_t ASM::get_source_size()
	{
	if(source_ != NULL)
		{
		size_t filesize;
		fseek(source_, 0, SEEK_END);
		filesize = ftell(source_);
		rewind(source_);
		return filesize;
		}
	return 0;
	}

void ASM::precompile()
	{
	if(source_ != NULL)
		{
		size_t source_size = get_source_size();
		prgrm_ = (char*)malloc(sizeof(char) * source_size);
		fread(prgrm_, sizeof(char), source_size, source_);
		rewind(source_);
		for(int i = 0; i < source_size; i++)
			{
			if(prgrm_[i] == '\n')q_cmds_++;
			if(prgrm_[i] == ':')
				{
				jumps_[prgrm_[++i] - '0'] = q_cmds_;
				}
			}
		}
	else
		{
		printf("\nError:source doesnt exist\n");
		assert(0);
		}
	}

ASM_CMDS ASM::get_cmd()
	{
	#define ASMCMDS
	#define COMMAND(cmd_name, q_args) \
		if(!strcmp(cur_cmd_, #cmd_name)) \
			return ASM_##cmd_name;
	#include "ASM_CMDS.txt"
	#undef COMMAND
	#undef ASMCMDS
		else
			{
			return ASM_default;
			}
	}

int ASM::get_q_args()
	{
	#define ASMCMDS
	#define COMMAND(cmd_name, q_args) \
		if(!strcmp(cur_cmd_, #cmd_name)) \
			return q_args;
	#include "ASM_CMDS.txt"
	#undef COMMAND
	#undef ASMCMDS
		else
			{
			return 0;
			}
	}

#define CASE_NOT_JMP(cmd) \
			cmd: \
				for(int j = 0; j < qargs; j++) \
					{ \
					fscanf(source_, "%s", cur_cmd_); \
					fprintf(asm_, "%s", cur_cmd_); \
					} \
				fprintf(asm_, " ");  \
				break;

#define CASE_JMP(cmd)\
			cmd: \
			fscanf(source_, "%s", cur_cmd_); \
			fprintf(asm_, "%d", jumps_[cur_cmd_[1] - '0']); \
			fprintf(asm_, " "); \
			break;

void ASM::compile()
	{
	precompile();
	asm_ = fopen(DEFAULT_ASM_NAME, "wt");
	for(int i = 0; i < q_cmds_; i++)
		{
		fscanf(source_, "%s", cur_cmd_);
		ASM_CMDS command = get_cmd();
		int qargs = get_q_args();
		fprintf(asm_, "%d", command);
		fprintf(asm_," ");
		if(qargs)
			{
			switch(command)
				{
				case CASE_NOT_JMP(ASM_PUSH);
				case CASE_NOT_JMP(ASM_PUSH_R);
				case CASE_NOT_JMP(ASM);
				case CASE_JMP(ASM_JMP);
				case CASE_JMP(ASM_JA);
				case CASE_JMP(ASM_JAE);
				case CASE_JMP(ASM_JB);
				case CASE_JMP(ASM_JBE);
				case CASE_JMP(ASM_JE);
				case CASE_JMP(ASM_JNE);
				default:
					break;
				}
			}
		}
	}

void ASM::compile(char* const asm_name)
	{
	precompile();
	asm_ = fopen(asm_name, "wt");
	for(int i = 0; i < q_cmds_; i++)
		{
		fscanf(source_, "%s", cur_cmd_);
		ASM_CMDS command = get_cmd();
		int qargs = get_q_args();
		fprintf(asm_, "%d", command);
		fprintf(asm_," ");
		if(qargs)
			{
			for(int j = 0; j < qargs; j++)
				{
				fscanf(source_, "%s", cur_cmd_);
				fprintf(asm_, "%d", atoi(cur_cmd_));
				}
			fprintf(asm_, " ");
			}
		}
	//fclose(asm_);
	//fclose(source_);
	}
*/

int main()
	{
	ASM a;
	a.dump();
	return 0;
	}

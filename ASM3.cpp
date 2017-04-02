#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <cstring>
#include <cctype>

#define DEFAULT_SOURCE_NAME "source.txt"
#define DEFAULT_ASM_NAME "asm.txt"

enum CONSTANTS
    {
    MAX_CMD_SIZE = 16,
    MAX_Q_LABELS = 2048,
    MAX_LABELNAME_SIZE = 32,
    MAX_LINE_SIZE = 256,
    SOURCENAME_LEN = 32,
    };



enum ASM_ERR
    {
    ERR_DEFAULT = 0,//err = 00000000
    ERR_BADSOURCE = 2,//err = 00000010
    ERR_BADCURLINE = 4,//err = 00000100
    ERR_BADCURCMD = 8,//err = 00001000 
    };




enum ASM_CMDS
    {
                ASM_default,
    #define ASMCMDS
    #define COMMAND(cmd_name, q_r_rgs, q_n_args, q_s_args) \
                ASM_##cmd_name,
    #include "ASMCMDS.txt"
    #undef COMMAND
    #undef ASMCMDS
    };


enum ASM_REGS
    {
    #define ASMREGS
    #define REGISTER(reg_name, rnumber)  \
            ASM_##reg_name = rnumber,
    #include "ASMREGS.txt"
    #undef REGISTER
    #undef ASMREGS
            ASM_badreg = -666, //if input is incorrect
    };




class ASM
    {
    public:
        ASM(const char* asm_name);
        ~ASM();
        void compile();
        void compile(const char* asm_name);
        int ok();
        void dump();
    private:
        void precompile();
        ASM_CMDS getcmd();
        ASM_REGS getreg();
        int getqargs();
        void printlabels();
        int findlabel();
        void compile_err(const char* message, int nline);
        int qargs(const char argtype); //!argtype = r (if register), n (if numeric constant), l (if label)


        FILE* source_;
        FILE* asm_;

        char* cur_line_;
        char* cur_cmd_;
        int q_cmds_;

        char* labels_[MAX_Q_LABELS];
               
        char* sourcename_;

        };




#define IS_CORRECT  \
         \
    if(ok() & ERR_BADSOURCE || ok() & ERR_BADCURLINE || ok() & ERR_BADCURCMD) \
        { \
        dump(); \
        assert(0) ; \
        }





ASM::ASM(const char* sourcename):
    source_(fopen(sourcename,"rt")),
    asm_(NULL),
    cur_line_((char*)malloc(sizeof(char) * MAX_LINE_SIZE)),
    cur_cmd_((char*)malloc(sizeof(char) * MAX_CMD_SIZE)),
    q_cmds_(0),
    sourcename_((char*)malloc(sizeof(char) * SOURCENAME_LEN))
        {
        sscanf(sourcename, "%20s", sourcename_);
        for(int i = 0; i < MAX_Q_LABELS; i++)
            {
            labels_[i] = NULL;
            }
    IS_CORRECT;
    }

ASM::~ASM()
    {
    if(!source_)fclose(source_);
    if(!asm_)fclose(asm_);
    free(cur_line_);
    free(cur_cmd_);
    free(sourcename_);
    for(int i = 0; i < MAX_Q_LABELS; i++)
        {
        if(!labels_[i])free(labels_[i]);
        } 
    }

#define PUTERR(err_name, condition) \
    if(condition) \
        { \
        error |= ERR_##err_name; \
        } 

int ASM::ok()
    {
    int error = 0;
    PUTERR(BADSOURCE, !source_);
    PUTERR(BADCURLINE, !cur_line_);
    PUTERR(BADCURCMD, !cur_cmd_);
    return error;
    }
#undef PUTERR

#define PRINTERR(err_name, message) \
    { \
    if(err & ERR_##err_name) printf(message); \
    }

void ASM::dump()
    {
    printf("\n***Dump has been called***\n");
    printf("sourcename_ = %s\n", sourcename_);
    printf("source_ = %p\n", source_);
    printf("asm_ = %p\n", asm_);
    printf("cur_line_ = %p\n", cur_line_);
    printf("cur_line_ = \"%s\"\n", cur_line_);
    printf("cur_cmd_ = %p\n", cur_cmd_);
    printf("cur_cmd = \"%s\"\n", cur_cmd_);
    printf("q_cmds_ = %d\n", q_cmds_);
    int err = ok();
    printlabels();
    PRINTERR(BADSOURCE, "Error: cant load source file\n");
    PRINTERR(BADCURLINE, "Error: current line has been spoiled\n");
    PRINTERR(BADCURCMD, "Error: current command has been spoiled\n");
    if(err == 0) printf("Any errors hasnt been detected\n");
    }
#undef PRINTERR

void ASM::printlabels()
    {
    printf("Existing labels:\n-------\n");
    for(int i = 0; i < MAX_Q_LABELS; i++)
        {
        if(labels_[i] != NULL) printf("[%d]%s\n", i, labels_[i]);
        } 
    printf("-------\n");
    }

void  ASM::precompile()
    {
    IS_CORRECT;
    char* linepointer;
    while(fgets(cur_line_, MAX_LINE_SIZE, source_))
        {
        int sline = strlen(cur_line_);
        linepointer = cur_line_;
        while(sscanf(linepointer, "%s", cur_cmd_) > 0)
            {
            int scmd = strlen(cur_cmd_);
            if(cur_cmd_[scmd - 1] == ':')//label has been found
                {
                labels_[q_cmds_ + 1] = (char*)malloc(sizeof(char) * MAX_LABELNAME_SIZE);
                cur_cmd_[scmd - 1] = '\0';//now cur_cmd_ contain only  label name without ':'
                sscanf(cur_cmd_, "%s", labels_[q_cmds_ + 1]);
                }
            else ++q_cmds_;// count commands
            while(isspace(*linepointer))
                {
                 ++linepointer; //move through spaces
                }
            linepointer += scmd;//move through read command
            }
        }
    IS_CORRECT;
    }
    
ASM_CMDS ASM::getcmd()
    {
        IS_CORRECT;
    #define ASMCMDS
    #define COMMAND(cmd_name, q_r_args, q_n_args, q_s_args) \
        if(!strcmp(cur_cmd_, #cmd_name)) return ASM_##cmd_name;
    #include "ASMCMDS.txt"
    #undef COMMAND
    #undef ASMCMDS
        else return ASM_default;
    }

ASM_REGS ASM::getreg()
    {
        IS_CORRECT;
    #define ASMREGS
    #define REGISTER(reg_name, rnumber) \
        if(!strcmp(cur_cmd_, #reg_name))return ASM_##reg_name;
    #include "ASMREGS.txt"
    #undef REGISTER
    #undef ASMREGS             
        else return ASM_badreg;
    }
    
int ASM::findlabel()
    {
    for(int i = 0; i < MAX_Q_LABELS; ++i)
        {
        if(labels_[i] != NULL && !strcmp(labels_[i], cur_cmd_)) return i;
        }
    return -1;
    }

int ASM::qargs(const char argtype) 
    { 
    #define ASMCMDS 
    #define COMMAND(cmd_name, q_r_args, q_n_args, q_l_args) \
        if(!strcmp(cur_cmd_, #cmd_name) && argtype == 'r') return q_r_args; \
        if(!strcmp(cur_cmd_, #cmd_name) && argtype == 'n') return q_n_args; \
        if(!strcmp(cur_cmd_, #cmd_name) && argtype == 'l') return q_l_args;  
    #include "ASMCMDS.txt"  
        else return -1; 
    #undef COMMAND 
    #undef ASMCMDS 
    }

void ASM::compile_err(const char* message, int nline)
    {
    printf("\n***Compilation failed***\n");
    printf("ERROR:In line %d : %s '%s'\n", nline, message, cur_cmd_);
    printf("Enter d to print dump, or another key to continue...\n");
    if(getchar() == 'd') dump();
    assert(0);
    }
 


#define MOVEPOINTER \
    while(isspace(*linepointer)) ++linepointer; \
    linepointer += scmd; \
    scmd = 0;
    
#define GETARGS(qarg, val, code, cond) \
    for(int i = 0; i < qarg; i++) \
        { \
        if(sscanf(linepointer, "%s", cur_cmd_) < 1) compile_err("Too few arguments", ncurline); \
        else \
            { \
            scmd = strlen(cur_cmd_); \
            val = code; \
            cond; \
            fprintf(asm_, "%d ", val); \
            MOVEPOINTER; \
            } \
        }


void ASM::compile(const char* asm_name)
    {
    IS_CORRECT;
    asm_ = fopen(asm_name, "wt");
    if(asm_ == NULL)
        {
        printf("\nNo source file\n");
        dump();
        assert(0);
        }
    precompile();
    IS_CORRECT;
    int ncurline = 0;
    char* linepointer;
    rewind(source_);
    while(fgets(cur_line_, MAX_LINE_SIZE, source_))
        {
        ++ncurline;
        int sline = strlen(cur_line_);
        linepointer = cur_line_;
        if(sscanf(linepointer, "%s", cur_cmd_) > 0)
            {
            int scmd = strlen(cur_cmd_);
            if(cur_cmd_[scmd - 1] !=  ':')//if not a label
                {
                ASM_CMDS cmd = getcmd();
                if(cmd == ASM_default) compile_err("Not declared", ncurline);
                fprintf(asm_, "%d ", cmd);
                int q_r_args = qargs('r'), q_n_args = qargs('n'), q_l_args = qargs('l');
                int larg = 0, narg = 0;
                ASM_REGS reg;
                MOVEPOINTER;
                GETARGS(q_r_args, reg, getreg(), if(reg == ASM_badreg) compile_err("Wrong register", ncurline)); 
                GETARGS(q_n_args, narg, atoi(cur_cmd_), if(!((scmd == 1) && (*cur_cmd_ == '0')) && (narg == 0)) compile_err("Number expected", ncurline));
                GETARGS(q_l_args, larg, findlabel(), if(larg == -1) compile_err("Wrong label", ncurline));
                if(sscanf(linepointer, "%s", cur_cmd_) > 0) compile_err("Not declared", ncurline);
                }
            else  //line with label must contain only label`s name
                {
                MOVEPOINTER;
                if(sscanf(linepointer,"%s",cur_cmd_) > 0) compile_err("Wrong syntax in label definition", ncurline);
                }
            }
        }
    fclose(asm_);
    IS_CORRECT;
    }

void ASM::compile()
    {
    compile(DEFAULT_SOURCE_NAME);
    }

int main(int argc, char* argv[])
    {
    if(argc > 2)
        {
        ASM a(argv[1]);
        a.compile(argv[2]);
        printf("\nCompilation successful\n");
        }
    }

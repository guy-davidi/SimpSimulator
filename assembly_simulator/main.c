#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define LEN_OF_HW_REG 23
#define LEN_OF_SIMP_REG 16
#define LEN_OF_RAM 4096
#define LEN_48_BITS 12
#define BUFFER_RAM_LEN 8+1 /*string is 8 +1*/
#define BUFFER_INST_LEN 12+1
#define OPCODE 11
#define FALSE 0
#define TRUE 1

#define add 0
#define sub 1
#define mac 2
#define and 3
#define or 4
#define xor 5
#define sll 6
#define sra 7
#define srl 8
#define beq 9
#define bne 10
#define blt 11
#define bgt 12
#define ble 13   
#define bge 14
#define jal 15   
#define lw 16
#define sw 17
#define reti 18 
#define in 19
#define out 20
#define halt 21

unsigned createMask(unsigned a, unsigned b)
{
    unsigned r = 0;
    for (unsigned i = a; i <= b; i++)
        r |= 1 << i;

    return r;
}

void remove_all_chars(char* str, char c) {
    char* pr = str, * pw = str;
    while (*pr) {
        *pw = *pr++;
        pw += (*pw != c);
    }
    *pw = '\0';
}

int init_list(int hardware_registers[]) {
    int i = 0;
    for (i = 0; i < LEN_OF_HW_REG; i++) {
        hardware_registers[i] = 0;
    }
    return 0;
}

int print_list(int *list) {
    int i = 0;
    for (i = 0; i < LEN_OF_RAM; i++)
    {
        printf("%c\n", list[i]);
    }
    return 0;
 }

void myprintf(char* s) {
    for (int i = 0; i < LEN_48_BITS; i++) {
        printf("%c", s[i]);
    }
}

void get_imemin_inst_from_file(FILE * file_imemin, char imemin_inst[LEN_OF_RAM][LEN_48_BITS]) {
    char buffer_inst[BUFFER_INST_LEN] = { 0 };
    char buffer_inst_inst_real_len[LEN_48_BITS] = { 0 };
    int i = 0;
    int len = 0;

    while (fgets(buffer_inst, BUFFER_INST_LEN, file_imemin)){
        //remove the '/n' from last char in buffer_inst
        len = strlen(buffer_inst);
        if (len > 0 && buffer_inst[len - 1] == '\n') buffer_inst[len - 1] = '\0';
        //copy the ral value of inst to dentination
        strcpy(imemin_inst[i], buffer_inst);
        //myprintf(imemin_inst[i]);// debage line
        i++;
    }
}

void get_dmemin_ins_from_file(FILE* file_dmemin, int dmemin_list_RAM[LEN_OF_RAM])
{
    char buffer[BUFFER_RAM_LEN] = { 0 };
    int ret;
    int i = 0;
    while (fgets(buffer, BUFFER_RAM_LEN, file_dmemin))
    {
        ret = strtol(buffer, NULL, 16);
        dmemin_list_RAM[i++] = ret;
       // printf("%d", dmemin_list_RAM[i-1]); //debage line
    }
}

void zero_holder(char* list) {
    for (int i = 0; i < 4; i++)
        list[i] = 0;
}

// this function gets an OPCODE
// returns True if OPCODE = <HALT_OP_CODE> = 21
int Decode_execute(int opcode,int rd,int rs, int rt,int rm,int immediate1,int immediate2, int * MEM,int pc,int * R,int  * IORegister) {
    unsigned mask = createMask(0, 11);
    switch (opcode)
    {
    case add:
        R[rd] = R[rs] + R[rt] + R[rm];
        break;
    case sub:
        R[rd] = R[rs] - R[rt] - R[rm];
        break;
    case mac:
        R[rd] = R[rs] * R[rt] + R[rm];
        break;
    case and:
        R[rd] = R[rs] & R[rt] & R[rm];
        break;
    case or:
        R[rd] = R[rs] | R[rt] | R[rm];
        break;
    case xor:
        R[rd] = R[rs] ^ R[rt] ^ R[rm];
        break;
    case sll:
        R[rd] = R[rs] << R[rt];
        break;
    case sra:
        R[rd] = R[rs] >> R[rt];
        break;
    case srl:
        R[rd] = R[rs] >> R[rt];
        break;
    case beq:
        if (R[rs] == R[rt]) 
            pc = R[rm] & mask;
        break;
    case bne: 
        if (R[rs] != R[rt]) 
            pc = R[rm] & mask;
        break;
    case blt:
        if (R[rs] < R[rt]) 
            pc = R[rm] & mask;
        break;
    case bgt:
        if (R[rs] > R[rt])
            pc = R[rm] & mask;
        break;
    case ble:
        if (R[rs] <= R[rt])
            pc = R[rm] & mask;
        break;
    case bge:
        if (R[rs] >= R[rt])
            pc = R[rm] & mask;
        break;
    case jal:
        R[rd] = pc + 1;
        pc = R[rm] & mask;
        break;
    case lw:
        R[rd] = MEM[R[rs] + R[rt]] + R[rm];
        break;
    case sw:
        MEM[R[rs] + R[rt]] = R[rm] + R[rd];
        break;
    case reti:
        pc = IORegister[7];
        break;
    case in:
        R[rd] = IORegister[R[rs] + R[rt]];
        break;
    case out:
        IORegister[R[rs] + R[rt]] = R[rm];
        break;
    case halt:
        printf("############ halt ############");
        exit(1);
        break;
    }

    return TRUE;
}

void mystrcpy(char* dest , char* source) {
    for (int i = 0; i < 12; i++)
        dest[i] = source[i];
}

int main(int argc, char** argv)
{ 
    
    /*SIMP has 16 registers from 0-15, when 0,1,2 unchangeable*/ 
    int simp_reg[LEN_OF_SIMP_REG] = { 0 };                                          
    simp_reg[0] = 0; /*Init the registers*/
    int IORegister[LEN_OF_HW_REG] = { 0 };
    init_list(IORegister);
    /*get simp_reg[1]=immidiate1 and simp_reg[2]= immidiate2 */                                                
    /*Read from file - imemin*/
   

    /* init 48 bits for each instruction In imemin.txt */
    char* const fileName_imemin = argv[1];
    char imemin_inst[LEN_OF_RAM][LEN_48_BITS] = { 0 };  
    FILE* file_imemin = fopen(fileName_imemin, "r"); 
    if (file_imemin == 0)
       exit(1);
    //update instruction to imemin_inst matrix
    get_imemin_inst_from_file(file_imemin, imemin_inst);
    
    
    
    /*read from dmemin*/
    char * const fileName_dmemin = argv[2]; 
    FILE* file_dmemin = fopen(fileName_dmemin, "r");
    int dmemin_list_RAM[LEN_OF_RAM] = { 0 };/* init 32 bits for each instruction In dmemin.txt*/
    if (file_dmemin == 0)
        exit(1);
    get_dmemin_ins_from_file(file_dmemin, dmemin_list_RAM);
   
    
    
    /*PC register need to cange to 12bits*/
    int program_counter = 0;
    /*fetch*/
    char current_inst[LEN_48_BITS] = { 0 };
    char holder[4] = { 0 };
    
    // init registers
    int OP_Code = 0;
    int register_RD = 0;
    int register_RS = 0;
    int register_RT = 0;
    int register_RM = 0;
    int immediate1 = 0;
    int immediate2 = 0;


    /*
    while(running){

    running = decoder(...);
    decoder returns 1 if not halt
    decoder returns 0 if current instruction = 21 - halt_code
    
    */
    //Halted(current_inst) == FALSE
    
    
    int running = 1;
    while (running)
    {
        // fetch
        mystrcpy(current_inst, imemin_inst[program_counter]);
        myprintf(current_inst);
        
        
        //get opcode
        holder[0] = current_inst[0];
        holder[1] = current_inst[1];
        OP_Code = strtol(holder, NULL, 16);
        zero_holder(holder);
        
        //get RD
        holder[0] = current_inst[2];
        register_RD = strtol(holder, NULL, 16);
        zero_holder(holder);

        //get RS
        holder[0] = current_inst[3];
        register_RS = strtol(holder, NULL, 16);
        zero_holder(holder);

        //get RT
        holder[0] = current_inst[4];
        register_RT = strtol(holder, NULL, 16);
        zero_holder(holder);

        //get RM
        holder[0] = current_inst[5];
        register_RM = strtol(holder, NULL, 16);
        zero_holder(holder);

        //get imm1
        holder[0] = current_inst[6];
        holder[1] = current_inst[7];
        holder[2] = current_inst[8];
        immediate1 = strtol(holder, NULL, 16);
        zero_holder(holder);

        //get imm2
        holder[0] = current_inst[9];
        holder[1] = current_inst[10];
        holder[2] = current_inst[11];
        immediate2 = strtol(holder, NULL, 16);

        // Switch Halted to Decoder function that returns 1 if good and 0 if inst was halt
        running = Decode_execute(OP_Code,register_RD,register_RS,register_RT,register_RM,immediate1,immediate2,dmemin_list_RAM,program_counter, simp_reg, IORegister); 
       
        program_counter++;
        printf("\n");
    }


 

	return 0;
}
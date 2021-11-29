#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define LEN_OF_HW_REG 23
#define LEN_OF_SIMP_REG 16
#define LEN_OF_RAM 4096
#define LEN_48_BITS 12
#define BUFFER_RAM_LEN 8+1 /*string is 8 +1*/
#define BUFFER_INST_LEN 12+1


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


int main(int argc, char** argv)
{ 
    /*SIMP has 16 registers from 0-15, when 0,1,2 unchangeable*/
    int simp_reg[LEN_OF_SIMP_REG] = { 0 };
   /*Init the registers*/
    simp_reg[0] = 0;
    int hardware_registers[LEN_OF_HW_REG] = { 0 };
    init_list(hardware_registers);
    /*get simp_reg[1]=immidiate1 and simp_reg[2]= immidiate2 */                                                
    /*PC register need to cange to 12bits*/
    int pc = 0;
    /*##################################################################*/
    /*imemin############################################################*/
    /*##################################################################*/
    /* init 48 bits for each instruction In imemin.txt */
    char const* const fileName_imemin = argv[1]; 
    FILE* file_imemin = fopen(fileName_imemin, "r"); 
    char imemin_inst[LEN_OF_RAM][LEN_48_BITS] = { 0 }; 
    if (file_imemin == 0) {
        exit(1);
    }


    char buffer_inst[BUFFER_INST_LEN] = { 0 };
    char buffer_inst_inst_real_len[LEN_48_BITS] = { 0 };
    int i = 0;
    int len = 0;
    
    while (fgets(buffer_inst, BUFFER_INST_LEN, file_imemin))
    {
        //remove the '/n' from last char in buffer_inst
        len = strlen(buffer_inst);
        if (len > 0 && buffer_inst[len - 1] == '\n') buffer_inst[len - 1] = '\0';
        //copy the ral value of inst to dentination
        strcpy(imemin_inst[i], buffer_inst);
        myprintf(imemin_inst[i]);
        i++;

    }

    /*##################################################################*/
     /*dmemin############################################################*/
    /*##################################################################*/
    /* init 32 bits for each instruction In dmemin.txt*/
    char const* const fileName_dmemin = argv[2]; 
    FILE* file_dmemin = fopen(fileName_dmemin, "r");
    int dmemin_list_RAM[LEN_OF_RAM] = { 0 };
    if (file_dmemin == 0){
        exit(1);
    }
    char buffer[BUFFER_RAM_LEN] = { 0 };
    int ret;
    char* ptr;
    i = 0;
    while (fgets(buffer,BUFFER_RAM_LEN, file_dmemin))
    {
        ret = strtol(buffer, &ptr, 16);
        dmemin_list_RAM[i++] = ret;
    }
    /*read instarcation from imemin*/


	return 0;
}
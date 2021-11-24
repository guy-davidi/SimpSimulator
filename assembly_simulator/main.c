#include <stdio.h>
#include <stdint.h>
#define LEN_OF_HW_REG 23
#define LEN_OF_SIMP_REG 16
#define LEN_OF_RAM 4096



int init_list(int hardware_registers[]) {

    int i = 0;
    for (i = 0; i < LEN_OF_HW_REG; i++) {
        hardware_registers[i] = 0;
    }
    return 0;
}





int main(int argc, char** argv)
{

    uint32_t ram_arr[LEN_OF_RAM] = { 0 };               /*RAM list contain the RAM DATA during the program*/
    uint32_t simp_reg[LEN_OF_SIMP_REG] = { 0 };         /*SIMP has 16 registers from 0-15, when 0,1,2 unchangeable*/
    simp_reg[0] = 0;                                /*Init the registers*/
    uint32_t hardware_registers[LEN_OF_HW_REG] = { 0 };
    init_list(hardware_registers);

                                                    /*get simp_reg[1]=immidiate1 and simp_reg[2]= immidiate2 */
    int pc = 0;                                     /*PC register need to cange to 12bits*/


    char const* const fileName_imemin = argv[1]; 
    FILE* file_imemin = fopen(fileName_imemin, "r"); 
    char line_in_imemin[6];                         /* init 48 bits for each instruction In imemin.txt */
    
    char const* const fileName_dmemin = argv[2]; 
    FILE* file_dmemin = fopen(fileName_dmemin, "r");
    uint32_t line_in_dmemin =  0;                         /* init 32 bits for each instruction In dmemin.txt, we could define it as int but easier to control the bits*/


    
     /*read instarcation from imemin*/

    /*Init to memory from dmemin*/
    int i = 0;
    while (fgets(line_in_dmemin, sizeof(line_in_dmemin), file_dmemin)) {
        ram_arr[i] = line_in_dmemin;
        printf("%d\n", ram_arr[i]);
        i++;
    }

    fclose(file_imemin);

	return 0;
}
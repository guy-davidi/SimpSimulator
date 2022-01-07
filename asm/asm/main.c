#define  _CRT_SECURE_NO_WARNINGS
#define _CRT_NONSTDC_NO_DEPRECATE
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<ctype.h>

#define MAX_LINE_LEN 500
#define MAX_WORD_LEN  10
#define MAX_LABEL_LEN  50
#define MAX_LABEL_NUM  500

typedef struct label
{
    char name[50];
    int line_num;
    struct label* next;
}label;

void opcode_sel(char* word, char m_code[], char* line, char* line2, FILE* dmemin); // identify opcode and update m_code
void reg_sel(char* word, char m_code[], int i); // identify word reg type and put it in m_code index i
int label_to_line_num(label* head, char* label_name);
int imm_sel(char* word, char m_code[], label* head, int j);
label* new_label(char* word, int line_num, label* head);
void clean_list(label* head);
void word_func(char* line, FILE* dmemin);




int main(int argc, char* argv[])          //int argc, char *argv[]
{

    char line[MAX_LINE_LEN];
    char line2[MAX_LINE_LEN];
    char* word = NULL;
    char* trimmed_word = NULL;
    int line_num = 0;
    label* head = NULL;
    FILE* file_name, * imemin, * dmemin;
    if (argc != 4)
    {
        printf("Invalid number of ARG");
        exit(1);
    }

    file_name = fopen(argv[1], "r");
    imemin = fopen(argv[2], "wt");

    dmemin = fopen(argv[3], "r+");

    //file_name = fopen("assembler command tests.txt", "r");
    //imemin = fopen("imemin.txt", "wt");
    //dmemin = fopen("dmemin.txt", "r+");
    while (!feof(file_name))
    {
        char delimit[] = " ,\t\r\n\v\f#";
        int flagcount = 1;
        fgets(line, MAX_LINE_LEN, file_name);
        if (line[0] == '\n')        // if blank line don't count it
        {
            continue;
        }
        word = strtok(line, delimit);
        //printf("word is: %s\n", word);
        if (word[strlen(word) - 1] == ':')        //if its a label
        {
            word[strlen(word) - 1] = '\0';
            head = new_label(word, line_num, head);
            //printf("lebel %s line %d added\n", head, line_num);
            word = strtok(NULL, delimit);
            //printf("second word is: %s\n", word);
        }

        if (word == NULL)      //if afterlabel there is no command don't count it
        {
            flagcount = 0;
        }

        if (flagcount)
        {
            line_num += 1;
        }

    }
    rewind(file_name);          // set in file pointer back to begining
    while (!feof(file_name))
    {
        char delimit[] = " ,\t\r\n\v\f#";
        char m_code[] = "000000000000";
        fgets(line, MAX_LINE_LEN, file_name);
        strcpy(line2, line);
        if (line[0] == '\n')        //if blank line skip it
        {
            continue;
        }
        word = strtok(line, delimit);
        //printf("word is: |%s|\n", word);
        if (word[strlen(word) - 1] == ':') //if its a label
        {
            //printf("found label : %s\n", word);
            word = strtok(NULL, delimit);
            if (word == NULL || word[0] == '\n')
            {
                //printf("only label in row\n");
                continue;
            }
            //printf("after label next word is: |%s|\n", word);
        }

        if (word == NULL || word[0] == '\n')
        {
            continue;
        }
        opcode_sel(word, m_code, line, line2, dmemin);
        if (!strcmp(word, ".word"))
            continue;
        //printf("opcode:%s\n", word);
        if (!strcmp(word, "halt") || !strcmp(word, "halt\n") || !strcmp(word, "reti") || !strcmp(word, "reti\n"))    // if halt or reti continue
        {
            fprintf(imemin, "%s\n", m_code);
            fflush(imemin);
            continue;
        }



        word = strtok(NULL, delimit);
        //printf("rd:%s\n", word);
        reg_sel(word, m_code, 2);
        //printf("m_code is:%s\n", m_code);


        word = strtok(NULL, delimit);
        //printf("rs:%s\n", word);
        reg_sel(word, m_code, 3);
        //printf("m_code is:%s\n", m_code);


        word = strtok(NULL, delimit);
        //printf("rt:%s\n", word);
        reg_sel(word, m_code, 4);
        //printf("m_code is:%s\n", m_code);


        word = strtok(NULL, delimit);
        //printf("rm:%s\n", word);
        reg_sel(word, m_code, 5);
        //printf("m_code is:%s\n", m_code);


        word = strtok(NULL, delimit);
        //printf("imm1:%s\n", word);
        imm_sel(word, m_code, head, 0);
        //printf("m_code is:%s\n", m_code);


        word = strtok(NULL, delimit);
        //printf("imm2:%s\n", word);
        imm_sel(word, m_code, head, 3);
        //printf("m_code is:%s\n\n", m_code);

        fprintf(imemin, "%s\n", m_code);
        fflush(imemin);
    }

    fclose(file_name);
    fclose(imemin);
    fclose(dmemin);
    clean_list(head);          // free labels linked list
    return 0;
}
int label_to_line_num(label* head, char* label_name)    //searchs for label in label list
{
    label* temp = head;
    while (temp != NULL)
    {
        if (!strcmp(temp->name, label_name))
        {
            return temp->line_num;
            break;
        }
        temp = temp->next;
    }

    printf("error could not find label\n");
}

label* new_label(char* word, int line_num, label* head)     //add new label to list
{
    label* lbl = malloc(sizeof(label));
    if (lbl == NULL)
    {
        printf("memory alocation error\n");
        exit(1);
    }
    strcpy(lbl->name, word);
    lbl->line_num = line_num;
    lbl->next = head;
    return lbl;
}

void reg_sel(char* word, char m_code[], int i)       // identify and update machine code register number
{
    if (word != NULL)
    {
        if (!strcmp(word, "$imm1"))
        {
            m_code[i] = '1';
        }
        else if (!strcmp(word, "$imm2"))
        {
            m_code[i] = '2';
        }
        else if (!strcmp(word, "$v0"))
        {
            m_code[i] = '3';
        }
        else if (!strcmp(word, "$a0"))
        {
            m_code[i] = '4';
        }
        else if (!strcmp(word, "$a1"))
        {
            m_code[i] = '5';
        }
        else if (!strcmp(word, "$a2"))
        {
            m_code[i] = '6';
        }
        else if (!strcmp(word, "$t0"))
        {
            m_code[i] = '7';
        }
        else if (!strcmp(word, "$t1"))
        {
            m_code[i] = '8';
        }
        else if (!strcmp(word, "$t2"))
        {
            m_code[i] = '9';
        }
        else if (!strcmp(word, "$s0"))
        {
            m_code[i] = 'A';
        }
        else if (!strcmp(word, "$s1"))
        {
            m_code[i] = 'B';
        }
        else if (!strcmp(word, "$s2"))
        {
            m_code[i] = 'C';
        }
        else if (!strcmp(word, "$gp"))
        {
            m_code[i] = 'D';
        }
        else if (!strcmp(word, "$sp"))
        {
            m_code[i] = 'E';
        }
        else if (!strcmp(word, "$ra"))
        {
            m_code[i] = 'F';
        }
    }

}

void opcode_sel(char* word, char m_code[], char* line, char* line2, FILE* dmemin) // identify and update machine code opcode number
{
    //printf("inside opcode sel with %s\n", word);
    if (!strcmp(word, "add"))
    {
        strcpy(m_code, "000000000000");
    }
    else if (!strcmp(word, "sub"))
    {
        strcpy(m_code, "010000000000");
    }
    else if (!strcmp(word, "mac"))
    {
        strcpy(m_code, "020000000000");
    }
    else if (!strcmp(word, "and"))
    {
        strcpy(m_code, "030000000000");
    }
    else if (!strcmp(word, "or"))
    {
        strcpy(m_code, "040000000000");
    }
    else if (!strcmp(word, "xor"))
    {
        strcpy(m_code, "050000000000");
    }
    else if (!strcmp(word, "sll"))
    {
        strcpy(m_code, "060000000000");
    }
    else if (!strcmp(word, "sra"))
    {
        strcpy(m_code, "070000000000");
    }
    else if (!strcmp(word, "srl"))
    {
        strcpy(m_code, "080000000000");
    }
    else if (!strcmp(word, "beq"))
    {
        strcpy(m_code, "090000000000");
    }
    else if (!strcmp(word, "bne"))
    {
        strcpy(m_code, "0A0000000000");
    }
    else if (!strcmp(word, "blt"))
    {
        strcpy(m_code, "0B0000000000");
    }
    else if (!strcmp(word, "bgt"))
    {
        strcpy(m_code, "0C0000000000");
    }
    else if (!strcmp(word, "ble"))
    {
        strcpy(m_code, "0D0000000000");
    }
    else if (!strcmp(word, "bge"))
    {
        strcpy(m_code, "0E0000000000");
    }
    else if (!strcmp(word, "jal"))
    {
        strcpy(m_code, "0F0000000000");
    }
    else if (!strcmp(word, "lw"))
    {
        strcpy(m_code, "100000000000");
    }
    else if (!strcmp(word, "sw"))
    {
        strcpy(m_code, "110000000000");
    }
    else if (!strcmp(word, "reti"))
    {
        strcpy(m_code, "120000000000");
    }
    else if (!strcmp(word, "in"))
    {
        strcpy(m_code, "130000000000");
    }
    else if (!strcmp(word, "out"))
    {
        strcpy(m_code, "140000000000");
    }
    else if (!strcmp(word, "halt") || !strcmp(word, "halt\n"))
    {
        strcpy(m_code, "150000000000");
    }
    else if (!strcmp(word, ".word"))
    {
        word_func(line2, dmemin);
    }
}

int imm_sel(char* word, char m_code[], label* head, int j)  // identify and update machine code immediate number

{

    int sum = 0;
    char hextmp[9];

    if (word[1] == 'x' && word[0] == '0') // if hexa
    {
        //printf("inside hexa\n");
        //printf("imm is hexa\n");
        if (strlen(word) == 1)             // if hexa is one digit set others to 0
        {
            m_code[8 + j] = word[0];
            m_code[7 + j] = '0';
            m_code[6 + j] = '0';
        }
        else if (strlen(word) == 2)        // if hexa is two digit set last to 0
        {
            m_code[7 + j] = hextmp[0];
            m_code[8 + j] = hextmp[1];
            m_code[6 + j] = '0';
        }
        else
        {
            m_code[6 + j] = word[2];
            m_code[7 + j] = word[3];
            m_code[8 + j] = word[4];
        }

    }
    else if (isdigit(word[0]) || (word[0] == '-' && isdigit(word[1]))) // if decimel
    {

        //printf("imm is decimal\n");
        sum = atoi(word);
        //printf("sum in decimal: %d\n", sum);
        itoa(sum, hextmp, 16);
        //printf("sum in hexa: %s\n", hextmp);


        if (strlen(hextmp) == 1)             // if hexa is one digit set others to 0
        {
            m_code[8 + j] = hextmp[0];
            m_code[7 + j] = '0';
            m_code[6 + j] = '0';
        }
        else if (strlen(hextmp) == 2)        // if hexa is two digit set last to 0
        {
            m_code[8 + j] = hextmp[1];
            m_code[7 + j] = hextmp[0];
            m_code[6 + j] = '0';
        }
        else
        {
            m_code[8 + j] = hextmp[2];
            m_code[7 + j] = hextmp[1];
            m_code[6 + j] = hextmp[0];
        }
        if (word[0] == '-')                 //if negitive number
        {
            m_code[8 + j] = hextmp[7];
            m_code[7 + j] = hextmp[6];
            m_code[6 + j] = hextmp[5];
        }

    }
    else   // if label
    {
        //printf("imm is label\n");
        sum = label_to_line_num(head, word);
        //printf("imm2 cought is: |%s| line is: %d\n", word, sum);
        //printf("sum in decimal: %d\n", sum);
        itoa(sum, hextmp, 16);
        //printf("sum in hexa: %s\n", hextmp);
        if (strlen(hextmp) == 1)             // if hexa is one digit set others to 0
        {
            m_code[8 + j] = hextmp[0];
            m_code[7 + j] = '0';
            m_code[6 + j] = '0';
        }
        else if (strlen(hextmp) == 2)        // if hexa is two digit set last to 0
        {
            m_code[7 + j] = hextmp[0];
            m_code[8 + j] = hextmp[1];
            m_code[6 + j] = '0';
        }
        else
        {
            m_code[8 + j] = hextmp[0];
            m_code[7 + j] = hextmp[1];
            m_code[6 + j] = hextmp[2];
        }
    }

    return sum;
}

void clean_list(label* head)        // free label list memory
{
    label* tmp = NULL;
    while (head != NULL)
    {
        tmp = head->next;
        free(head);
        head = tmp;
    }
}

void word_func(char* line, FILE* dmemin)            // special function to handle .word
{

    char* word = NULL, * ptr;
    char data[13] = "000000000000";
    char tmp[13] = { 0 };
    int i = 0;
    long address = 0, sum = 0;
    word = strtok(line, " ,\0\n");
    word = strtok(NULL, " ,\0\n");

    //address
    if (word[1] == 'x' && word[0] == '0')    //if adrsress is hexa
    {
        address = strtol(word, &ptr, 0);
    }
    else                                    //if address is decimal
    {
        address = atoi(word);
    }
    word = strtok(NULL, " ,\0 \n");

    //Data
    if (word[1] == 'x' && word[0] == '0')    //if Data is hexa
    {
        for (i = 2; i < strlen(word) - 1; i++)
        {
            data[13 - strlen(word) + i] = word[i];
        }
    }
    else                                    //if Data is decimal
    {

        sum = atoi(word);                   // sum = int data
        itoa(sum, tmp, 16);                   // tmp = 32 bits data hexa string
        for (i = 0; i < strlen(tmp); i++)
        {
            data[12 - strlen(tmp) + i] = tmp[i];
        }
        if (sum < 0)
        {
            data[0] = 'f';
            data[1] = 'f';
            data[2] = 'f';
            data[3] = 'f';
        }
    }
    int count = 0;
    char line2[MAX_LINE_LEN];
    while ((fgets(line2, MAX_LINE_LEN, dmemin)) != NULL)
    {
        count++;
        if (count == address)
        {

            fseek(dmemin, -((int)strlen(line2)) - 1, SEEK_CUR);       // set file pointer to beginig of line
            fprintf(dmemin, "%s", data);                    // print data
            fprintf(dmemin, " ");                            // get rid of original text in line that stayed at the end after printing data
            //printf("data printed in line %d is %s\n", count, data);

            break;
        }

    }
}


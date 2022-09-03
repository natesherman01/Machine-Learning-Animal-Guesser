#include "guess.h"

/* prototype */
void rewriteDB(node *rootNode);

/* MAIN */
int main(int argc, char *argv[])
{
    FILE *file = fopen("qa.db", "r");
    stringArray *sa;
    char *la;
    node *rootNode;
    node *cur;
    char *input;
    char answer;

    char *printData;
    int j;

    char *newAnm;
    char *newAns;
    char ansPass;
    char *newQ;

    /* PERROR IF FILE IS NULL */
    if (file == NULL)
    {
        perror("open file");
        exit(-1);
    }

    /* COLLECT Q/A LINES FROM DATABASE */
    sa = collectLines(file);
    if (sa == NULL)
    {
        /* CASE OF EMPTY DATABASE */
        printf("Unable to read database qa.db. Starting fresh.\n");

        printf("What is it (with article)? ");
        newAnm = read_long_line(stdin);
        
        rootNode = createNode(newAnm);
        rewriteDB(rootNode);

        fclose(file);
    }
    else
    {
        /* CASE OF NON-EMPTY DATABASE */
        /* COLLECT LEAF NODE DATA INFORMATION */
        la = collectLeafInfo(file, sa->len);

        /* CREATE ROOT NODE */
        rootNode = constructTree(sa->lines, sa->len, la);

        cur = rootNode;
        /* LOOP - ASK QUESTION NODE UNTIL BASE NODES == NULL */
        while (!(cur->left == NULL && cur->right == NULL))
        {
            /* print question */
            printData = cur->questionData;
            j = 0;
            while (printData[j] != '\0')
            {
                printf("%c", printData[j++]);
            }
            printf(" ");
            /* traverse binary tree */
            input = read_long_line(stdin);
            if (input == NULL)
            {
                printf("\n");
                continue;
            }
            answer = input[0];
            free(input);
            if (answer == 'y' || answer == 'Y')
            {
                /* traverse yes by going to left node */
                cur = cur->left;
            }
            else if (answer == '\0')
            {
                /* RE ASK QUESTION */
            }
            else
            {
                /* traverse no by going to right node */
                cur = cur->right;
            }
        }

        /* ASK USER IF GUESS IS CORRECT */
        printData = cur->questionData;
        j = 0;
        printf("Is it %s? ", printData);

        /* IF GUESSED CORRECTLY */
        input = read_long_line(stdin);
        while (1)
        {
            if (input != NULL && input[0] != '\0')
            {
                break;
            }
            else if (input != NULL && input[0] == '\0')
            {
                free(input);
            }
            printf("Is it %s? ", printData);
            input = read_long_line(stdin);
        }
        answer = input[0];
        free(input);
        if (answer == 'y' || answer == 'Y')
        {
            /* PRINT VICTORY */
            printf("My, am I clever. :) Thanks for playing.");
            freeTree(rootNode);
            freeSA(sa);
            freeLA(la);
            fclose(file);
            exit(0);
        }
        else if (answer == '\0')
        {
            /* RE ASK QUESTION */
            j = 0;
            while (printData[j] != '\0')
            {
                printf("%c", printData[j++]);
            }
        }
        /* ELSE GUESSED INCORRECTLY */
        else
        {
            printf("How disappointing.\n");

            /* ask for animal */
            printf("What is it (with article)? ");
            newAnm = read_long_line(stdin);
            printf("\n");

            /* ask for question */
            printf("What is a yes/no question that will distinguish %s from ", newAnm);
            j = 0;
            while (j > -1 && printData[j] != '\0')
            {
                printf("%c", printData[j++]);
            }
            printf("? ");
            newQ = read_long_line(stdin);
            printf("\n");

            /* ask for yes/no to question */
            printf("What is the answer to the question for %s? ", newAnm);
            newAns = read_long_line(stdin);
            ansPass = newAns[0];
            free(newAns);

            printf("\n");
            printf("I'll get it next time, I'm sure. Thanks for playing.\n");

            /* UPDATE BINARY TREE */
            addQuestion(cur, ansPass, newAnm, newQ);

        }
        /* FREE AND CLOSE FILE, AND REWRITE DATABASE */
        freeSA(sa);
        freeLA(la);
        fclose(file);
        rewriteDB(rootNode);
    }

    return 0;
}

/* FUNCTION TO REWRITE DATABASE */
void rewriteDB(node *rootNode)
{
    /* open file with 'w' to wipe previous file contents */
    FILE *file = fopen("qa.db", "w");
    printStrings(rootNode, file);

    fputs("\\end\n", file);

    printLeafs(rootNode, file);
    freeTree(rootNode);

    fclose(file);
} 

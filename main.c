/* ================================================================
      ALGORITHMICS AND DYNAMIC DATA STRUCTURES LAB WORK   
   ================================================================
   Data struct used is singly linked list of paragraphs, each
   paragraph is a singly linked list of words.
 
   Realised by Selmane Nada & Houchati Abdelmoumene 
   ================================================================
   Submitted May 10th 2026
   ================================================================ */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

// CONSTANT DEFINITONS: 
#define MaxWord 64
#define MaxLine BUFSIZ 
#define MaxFileName 256
#define MaxDocuments 10 


//DATA STRUCTURE 
// 1- WORD : ELEMENT IN A PARAGRAPH 

typedef struct WordCell {
    char Word[MaxWord];
    struct WordCell *NextWord;
} WordCell;

//2- PARAGRAPH : ELEMENT IN A TEXT DOCUMENT

typedef struct ParagraphCell {
    int paraName; 
    WordCell *words;
    struct ParagraphCell *next; 
    int WordCount;
}ParagraphCell;

//3-DOCUMENT : SET OF PARAGRAPHS

typedef struct DocumentCell {
    char filename[MaxFileName];
    struct ParagraphCell *Paragraph; 
    int numberof_para;
} DocumentCell; 

// ================================================================
// Words ABSTRACT MACHINE 
// CHECK IF THE WORD IS EXISTANT IN A SET 
int ExistentWord(WordCell *head , const char *word) {
    for (; head; head=head->NextWord);
    if (strcmp(head->Word,word)==0) return 1;
    return 0;
}
 // INSERT A WORD IN THE LIST 
int InsertWord(WordCell **head, const char *word ) {
    //if the word is already existant, no need to insert it
    if (ExistentWord(*head,word)) return 0;
    // insert the word in the linked list 
    WordCell *newword= malloc(sizeof(WordCell));
    //copy the word we want to insert into the word field 
    strcmp(newword->Word, word);
    // link it to the list 
    newword->NextWord = *head;
    *head = newword;
    return 1;
}
// DELETE SET OF WORDS 
void DeleteSet(WordCell *head) { 
    while (head !=NULL)    {
    WordCell *temp = head;
    head=head->NextWord;
    free(temp); 
    }
}

// PRINT A SET OF WORDS 
void PrintSet(WordCell *head) {
    int cpt = 0; 
    if (!head) {
        printf("The set is empty");
        return;
    }
    while (head != NULL) {
        printf("%s", head->Word);
        if (++cpt % 10 == 0) {
            printf("\n");
        }
    }
  if (cpt % 10 !=10 ){ printf("\n"); }
     
}


//THE ABSTRACT MACHINE OF PARAGRAPHS 
// 1 - Create A paragraph 
ParagraphCell *CreateParagraph(int id) {
    ParagraphCell *p = malloc(sizeof(ParagraphCell)); 
    p->paraName = id;
    p->words = NULL;
    p->next = NULL;
    p->WordCount = 0; 
    return p; 
}

// 2 - Insert A Paragraph At the Queue: 
void InsertParagraph(ParagraphCell **head , ParagraphCell *paragraph){
if (*head = NULL) { 
    *head= paragraph ;
    return; 
}
ParagraphCell *currentpara = *head; 
while (currentpara!=NULL) { 
    currentpara = currentpara->next;
}
currentpara->next = paragraph; 
}

// 3 - Extract A Paragraph : 
ParagraphCell *ExtractParagraph(ParagraphCell *head, int index) {
    int i = 1; 
    while (i !=index) {
        if (head == NULL) return NULL;  // index too big — stop safely
        head = head->next; 
        i++;
    }
    return head; 
}

// 4 - Delete A Paragraph : 
void DeleteParagraph(ParagraphCell *head) { 
while (head != NULL) {
    ParagraphCell *temp = head;
    DeleteSet(head->words); 
    head= head->next;
    free(temp);
}
}

// ================================================================
// Text PreProcessing 
//1 - normalize text : Removes Upper case and punctuation 
void normalize_word(char *word) {
    int len = strlen(word);
    while (len > 0 && !isalnum(word[len - 1])) {
        word[len - 1] = '\0';   // replace punctuation with end of string
        len--;
    }
    int start = 0;
    while (word[start] && !isalnum(word[start])) {
        start++;                // skip punctuation characters
    }
    if (start > 0)
        memmove(word, word + start, len - start + 1);  // shift left
    for (int i = 0; word[i]; i++) {
        word[i] = tolower(word[i]);
    }
}

//2- Split a line into seperated Words and insert them in a paragraph: 

void add_words(const char *line, ParagraphCell *para) {
    char buf[MaxLine];
    strcpy(buf, line);

    char *word = strtok(buf, " \t\r\n");
    int count = 0;
 while (word != NULL && count < MaxLine) {
    normalize_word(word);
    if (strlen(word) > 1)
        if (InsertWord(&para->words, word))
            para->WordCount ++;
    word = strtok(NULL, " \t\r\n");
    count++;
 }
}

/*================================================================
FILE LOADING : load a txt format file 
================================================================*/

int load_document(DocumentCell *doc, const char *filename)
{
    FILE *fp = fopen(filename, "r");
    char line[200];

    if (fp == NULL)
        return 0;
    doc->Paragraph = NULL;
    doc->numberof_para = 0;

    ParagraphCell *p = CreateParagraph(1);
    while (fgets(line, sizeof(line), fp))
    {
        if (strcmp(line, "\n") == 0)
        {
            InsertParagraph(&doc->Paragraph, p);
            doc->numberof_para++;
            p = CreateParagraph(doc->numberof_para + 1);
        }
        else
        {
            add_words(line, p);
        }
    }
    InsertParagraph(&doc->Paragraph, p);
    doc->numberof_para ++;
    fclose(fp);
    return 1;
}

//SET OPERATIONS 
//SET OPERATIONS 
//1- Intersection of two sets of words
WordCell *Intersection(WordCell *set1, WordCell *set2) {
    WordCell *result = NULL;
    for (WordCell *w1 = set1; w1 != NULL; w1 = w1->NextWord) {
        if (ExistentWord(set2, w1->Word)) {
            InsertWord(&result, w1->Word);
        }
    }
    return result;
}
//2- Union of two sets of words
WordCell *Union(WordCell *set1, WordCell *set2) {   
    WordCell *result = NULL;
    for (WordCell *w1 = set1; w1 != NULL; w1 = w1->NextWord) {
        InsertWord(&result, w1->Word);
    }
    for (WordCell *w2 = set2; w2 != NULL; w2 = w2->NextWord) {
        InsertWord(&result, w2->Word);
    }
    return result;
}
//3- Difference of two sets of words
WordCell *Difference(WordCell *set1, WordCell *set2) {  
    WordCell *result = NULL;
    for (WordCell *w1 = set1; w1 != NULL; w1 = w1->NextWord) {
        if (!ExistentWord(set2, w1->Word)) {
            InsertWord(&result, w1->Word);
        }
    }
    return result;
}
//4- check if a is a subset of b
int IsSubset(WordCell *set1, WordCell *set2) {
    for (WordCell *w1 = set1; w1 != NULL; w1 = w1->NextWord) {
        if (!ExistentWord(set2, w1->Word)) {
            return 0;
        }
    }
    return 1; 
}

//set operations accross multiple documents
void setopdoc(DocumentCell *docs, int numDocs) {
    for (int i = 0; i < numDocs; i++) {
        for (int j = i + 1; j < numDocs; j++) {
            printf("Comparing Document %s and Document %s:\n", docs[i].filename, docs[j].filename);
            for (ParagraphCell *p1 = docs[i].Paragraph; p1 != NULL; p1 = p1->next) {
                for (ParagraphCell *p2 = docs[j].Paragraph; p2 != NULL; p2 = p2->next) {
                    printf("Comparing Paragraph %d of Document %s with Paragraph %d of Document %s:\n", 
                           p1->paraName, docs[i].filename, p2->paraName, docs[j].filename);
                    WordCell *intersection = Intersection(p1->words, p2->words);
                    WordCell *unionSet = Union(p1->words, p2->words);
                    WordCell *difference = Difference(p1->words, p2->words);
                    printf("Intersection:\n");
                    PrintSet(intersection);
                    printf("Union:\n");
                    PrintSet(unionSet);
                    printf("Difference (Paragraph %d - Paragraph %d):\n", p1->paraName, p2->paraName);
                    PrintSet(difference);
                    printf("Difference (Paragraph %d - Paragraph %d):\n", p2->paraName, p1->paraName);
                    PrintSet(Difference(p2->words, p1->words));
                    printf("Is Paragraph %d a subset of Paragraph %d? %s\n", 
                           p1->paraName, p2->paraName, IsSubset(p1->words, p2->words) ? "Yes" : "No");
                    printf("Is Paragraph %d a subset of Paragraph %d? %s\n", 
                           p2->paraName, p1->paraName, IsSubset(p2->words, p1->words) ? "Yes" : "No");
                    DeleteSet(intersection);
                    DeleteSet(unionSet);
                    DeleteSet(difference);
                }
            }
        }
    }
}
void menu() {
    printf("Menu:\n");
    printf("1. perform set operations in one document\n");
    printf("2. Perform Set Operations Across Documents\n");
    printf("3. Exit\n");
}
int main() {
    DocumentCell docs[MaxDocuments];
    int numDocs = 0;
    char filename[MaxFileName];

    while (1) {
        menu();
        int choice;
        scanf("%d", &choice);
        switch (choice) {
            case 1:{
                // load the document
                printf("Enter the filename to load: ");
                scanf("%s", filename);
                if (!load_document(&docs[numDocs], filename)) {
                    printf("Failed to load document.\n");
                    break;
                }
                printf("Document loaded successfully (%d paragraphs).\n", docs[numDocs].numberof_para);
                numDocs++;

                // show paragraphs
                printf("Paragraphs in document:\n");
                ParagraphCell *cur = docs[numDocs-1].Paragraph;
                while (cur != NULL) {
                    printf("  Paragraph %d (%d words)\n", cur->paraName, cur->WordCount);
                    cur = cur->next;
                }

                // pick two paragraphs
                int p1idx, p2idx;
                printf("Select first paragraph: ");
                scanf("%d", &p1idx);
                printf("Select second paragraph: ");
                scanf("%d", &p2idx);

                ParagraphCell *p1 = ExtractParagraph(docs[numDocs-1].Paragraph, p1idx);
                ParagraphCell *p2 = ExtractParagraph(docs[numDocs-1].Paragraph, p2idx);
                if (p1 == NULL || p2 == NULL) {
                    printf("Invalid paragraph index.\n");
                    break;
                }

                // pick operation
                printf("Select operation:\n");
                printf("1. Union\n");
                printf("2. Intersection\n");
                printf("3. Difference (P%d - P%d)\n", p1idx, p2idx);
                printf("4. Is P%d a subset of P%d?\n", p1idx, p2idx);
                int op;
                scanf("%d", &op);

                WordCell *result = NULL;
                switch (op) {
                    case 1:
                        result = Union(p1->words, p2->words);
                        printf("Union of P%d and P%d:\n", p1idx, p2idx);
                        PrintSet(result);
                        DeleteSet(result);
                        break;
                    case 2:
                        result = Intersection(p1->words, p2->words);
                        printf("Intersection of P%d and P%d:\n", p1idx, p2idx);
                        PrintSet(result);
                        DeleteSet(result);
                        break;
                    case 3:
                        result = Difference(p1->words, p2->words);
                        printf("Difference (P%d - P%d):\n", p1idx, p2idx);
                        PrintSet(result);
                        DeleteSet(result);
                        break;
                    case 4:
                        printf("Is P%d a subset of P%d? %s\n", p1idx, p2idx,
                            IsSubset(p1->words, p2->words) ? "Yes" : "No");
                        break;
                    default:
                        printf("Invalid operation.\n");
                }
                break;
              
              }          



            case 2:
                if (numDocs < 2) {
                    printf("Please load at least two documents first.\n");
                    break;
                }
                setopdoc(docs, numDocs);
                break;
            case 3:
                printf("Thanksfor using the program\n");
                return 0;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
}
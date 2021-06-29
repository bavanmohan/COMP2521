
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#include "invertedIndex.h"

//Part 1 Functions
void trim_lead_sp(char *str);

void trim_trail_sp(char *str);

InvertedIndexBST BSTreeInsert(InvertedIndexBST t, char *word, char *filename);

InvertedIndexBST BSTreeNewNode(char *word);

FileList fileListInsert (FileList fileNode, char *filename);

FileList newFileNode(char *filename);

double fileWordCount(char *filename);

void printInvertedIndex(InvertedIndexBST t);

void BSTreeInfix(InvertedIndexBST t, FILE *file);

void printList(FileList node, FILE *file);

//Part 2 Functions
TfIdfList calculateTfIdf(InvertedIndexBST tree, char *searchWord, int D);

InvertedIndexBST findWord(InvertedIndexBST tree, char *searchWord);

TfIdfList createTfList(FileList node, int D);

TfIdfList createTfNode(char *filename, double sum);

double findIdf(FileList node, int D);

TfIdfList insertTfNode(TfIdfList root, char *filename, double tfIdf);

TfIdfList retrieve(InvertedIndexBST tree, char *searchWords[], int D);



/*_______________PART 1 FUNCTIONS_______________*/
/*
   This function opens the collection file with the given name, and then
   generates an inverted index from those files listed in the collection
   file,  as  discussed  in  the spec. It returns the generated inverted
   index.
 */
InvertedIndexBST generateInvertedIndex(char *collectionFilename) {

    char file_name[100];
    char word[50];    
    InvertedIndexBST tree = NULL;

    FILE *collect_file = fopen(collectionFilename, "r");
    
    while (fscanf (collect_file, "%s", file_name) == 1) {

        FILE *word_file = fopen(file_name, "r");

        while (fscanf (word_file, "%s", word)==1){
            //After reading in the word, this loop calls the insert
            //function to insert into the tree alphabetically
            normaliseWord(word);            

            tree = BSTreeInsert(tree, word, file_name);
        }
        fclose(word_file); 
    }
    fclose(collect_file);

    return tree;
}

/*
   This function opens the collection file with the given name, and then
   generates an inverted index from those files listed in the collection
   file,  as  discussed  in  the spec. It returns the generated inverted
   index.
 */
char *normaliseWord(char *str) {

    int i = 0;

    while (str[i]) {
        str[i]=(tolower(str[i]));
        if (str[i+1] == '\0'){
            //Checks if the last char in the string is a punctuation mark,
            //if so, brings the end of the string forwards by 1 space
            if ((str[i] == '.') || (str[i] == ',') || (str[i] == ';') || (str[i] == '?')) {
                str[i] = '\0';
            }
        }
        i++;
    }

    trim_lead_sp(str);
    trim_trail_sp(str);
    return str;
}


//Checks if the first char is a space, if so, moves the string forwards
//This is done on the loop until the first char is not a space
void trim_lead_sp(char *str) {

    int j = 0;

    while (str[0] == ' ') {
        if (isspace(str[0]) != 0) {
            j = 0;
            while (str[j] != '\0') {
                str[j] = str[j+1];
                j++;
            }
        }
    }
}

//Checks if the last char is a space, if so, moves the end of the string
//forwards by one until the last char is not a space
void trim_trail_sp(char *str) {

    while (str[strlen(str)-1] == ' ') {
        str[strlen(str)-1] = '\0';
    }

}

//By comparing the words using strcmp through recursion, this function creates and inserts
//a node alphabetically into the BST - this function was influenced by the 
//BSTree BSTreeInsert(BSTree t, int v) function in COMP2521 - Lab03
InvertedIndexBST BSTreeInsert(InvertedIndexBST tree, char *word, char *filename){
    
    //If the node is empty, a new node is created to insert into the tree
    if (tree == NULL) {
        InvertedIndexBST newNode = BSTreeNewNode(word);
        newNode->fileList = fileListInsert(newNode->fileList,filename);
        return newNode;
    }
    //If the target word falls before the current node's word in the alphabet
    //It is moved to the left and recursively given to the function
    else if (strcmp(word, tree->word) < 0) {
        tree->left = BSTreeInsert(tree->left, word, filename);    
    }
    else if (strcmp(word, tree->word) > 0) {
        tree->right = BSTreeInsert(tree->right, word, filename);              
    }
    //The else statement is triggered when the two words are the same
    //Instead of inputting the same word twice, the filename is passed
    //to be inserted into the FileList
    else {
        tree->fileList = fileListInsert(tree->fileList,filename);
    }
    return tree;
}
//This function creates a new node for when a unique word is scanned for
//entry into the BST - This function was influenced by the 
//static BSTLink newBSTNode(int v) function in COMP2521 - lab03
InvertedIndexBST BSTreeNewNode(char *word) {

    InvertedIndexBST newNode = malloc(sizeof(*newNode));
    newNode->word = malloc(sizeof(char)*strlen(word)+1);

    strcpy(newNode->word,word);

	newNode->fileList = NULL;
	newNode->left = NULL;
	newNode->right = NULL;

	return newNode;
}

//This function is called in the insert function in order
//to insert files alphabetically into the FileList linked list
FileList fileListInsert (FileList node, char *filename){

    //If head node doesn't exist, a new one is created
    if (node == NULL) {
        FileList new = newFileNode(filename);
        return new;
    }
    //If a node with the same filename already exists
    //The tf value is updated
    else if (strcmp(node->filename,filename)==0) {
        node->tf += 1/fileWordCount(filename);

        return node;
    }
    //if filename is earlier in the alphabet a node is created
    //and the currrent node will be used as the next node
    else if (strcmp(node->filename,filename) > 0) {
        FileList new = newFileNode(filename);
        new->next = node;

        return new;
    }
    //else if filename is bigger than it is appended
    else {
        node->next = fileListInsert(node->next,filename);
    }
    return node;
}

//This function creates a new node for the FileList Linked List
FileList newFileNode(char *filename) {

    FileList fileNode = malloc(sizeof(struct FileListNode));
    fileNode->filename = malloc(sizeof(char)*strlen(filename)+1);

    strcpy(fileNode->filename,filename);
    fileNode->next = NULL;
    fileNode->tf = 1/fileWordCount(filename);

    return fileNode;
}

//This function calculates the wordcount of a file by opening the
//relevant file and iterating until it reachs the end of file
double fileWordCount(char *filename) {

    char word[50];    
    double word_count = 0;

    FILE *file = fopen(filename, "r");

    while (fscanf (file, "%s", word)==1){
        word_count ++;
    }

    fclose(file);
    return word_count;
}

/**
 * Outputs  the  given inverted index to a file named invertedIndex.txt.
 * The output should contain one line per word, with the  words  ordered
 * alphabetically  in ascending order. Each list of filenames for a word
 * should be ordered alphabetically in ascending order.
*/
void printInvertedIndex(InvertedIndexBST tree) {
    
    //To ensure the helper functions can succesfully print to the file
    //it is opened in this function and passed through as an argument   
    FILE *output = fopen("invertedIndex.txt","w+");

    if (output == NULL) {
        printf("ERROR\n");
        return;
    }

    //The tree is printed in an infix method as seen in lab01
    BSTreeInfix(tree, output);
    
    fclose(output);
}
//This function was influenced by the void BSTreeInfix(BSTree t) in 
//COMP2521 - lab03
void BSTreeInfix(InvertedIndexBST tree, FILE *file) {
    
    if (tree == NULL) {
        return;
    }

    if (file == NULL) {
        printf("ERROR\n");
        return;
    }
    //The BST is printed starting at the leftmost leaf and will finish
    // at the rightmost leaf
    BSTreeInfix(tree->left,file);

    fprintf(file, "%s ", tree->word);

    //A seperate function is required to print the linked lists
    printList(tree->fileList,file);
    fprintf(file, "\n");

    BSTreeInfix(tree->right,file);
}

//This function iterates through the linked list to print all files and 
//their tf in order
void printList(FileList node, FILE *file){

    if (file == NULL) {
        printf("ERROR\n");
        return;
    }

	if (node != NULL){
	    fprintf(file, "%s (%f) ", node->filename, node->tf);
        printList(node->next, file);
    }
}


/*_______________PART 2 FUNCTIONS_______________*/
/**
 * Returns  an  ordered list where each node contains a filename and the 
 * corresponding tf-idf value for a given searchWord. You only  need  to
 * include documents (files) that contain the given searchWord. The list
 * must  be  in  descending order of tf-idf value. If there are multiple
 * files with same tf-idf, order them by  their  filename  in  ascending
 * order. D is the total number of documents in the collection.
 */
TfIdfList calculateTfIdf(InvertedIndexBST tree, char *searchWord, int D) {

    if (findWord(tree, searchWord)!= 0) {
        InvertedIndexBST new = BSTreeNewNode(searchWord);
        new = findWord(tree, searchWord);
        TfIdfList list = createTfList(new->fileList, D);

        return list;
    }

    return 0;
}

//This function checks if the word is present in the tree, and if it is,
//returns the relevant node
InvertedIndexBST findWord(InvertedIndexBST tree, char *searchWord) {
    if (tree->word == NULL) {
        return 0; 
    }
    else if (strcmp(searchWord, tree->word) < 0) {
        return findWord(tree->left,searchWord);
    }
    else if (strcmp(searchWord, tree->word) > 0) {
        return findWord(tree->right,searchWord);
    }
    else {
        return tree;
    }
    return 0;
}

//This function prepares each node for insertion into the TfIdf linked list
// by calculating the idf for each file and calling the insert function
TfIdfList createTfList(FileList head, int D) {

    TfIdfList fin = NULL;
    double idf = findIdf(head, D);;
    double tfIdf = 0;

    //This loop iterates through each node in the given
    //linked list and feeds each node to the insert function
    while (head != NULL) {  
        tfIdf = idf * head->tf;
        fin = insertTfNode(fin,head->filename,tfIdf);
        head = head->next;        
    }

    return fin;
}

//Using math.h library, log base 10 is used to calculate the idf
//D is passed to be used as the numerator (no. of files)
double findIdf(FileList node, int D) {
        
        double filecount =0;

        while (node != NULL) {
            filecount++;            
            node = node->next;
        }

        return log10(D/filecount);
}

//This function is used to insert each file into the tfIdf linked list
//This is done based in descending order of tfIdf
TfIdfList insertTfNode(TfIdfList root, char *filename, double tfIdf) {

    if (root == NULL) {
        TfIdfList new = createTfNode(filename, tfIdf);
        return new;
    }
    //If a file already exists, its tfIdf value is updated
    else if (strcmp(root->filename,filename)==0) {
        root->tfIdfSum += tfIdf;
    }
    //if the target tfIdf value is smaller than the current node
    //it is moved to be compared with the next node. The files are
    //given in alphabetical order, so if tfIdf values are the same
    //ascending filename order is maintained by inserting intot the next node
    else if (tfIdf <= root->tfIdfSum) {
        root->next = insertTfNode(root->next,filename,tfIdf);
    }
    else if (tfIdf > root->tfIdfSum) {
        TfIdfList new = createTfNode(filename, tfIdf);
        new->next = root;
        return new;
    }
    return root;
}

//The node for the tfIdf linked list is created in this function
TfIdfList createTfNode(char *filename, double sum) {
    
    TfIdfList newNode = malloc(sizeof(struct TfIdfNode));
    newNode->filename = malloc(sizeof(char)*strlen(filename)+1);
    strcpy(newNode->filename,filename);

    newNode->next = NULL;
    newNode->tfIdfSum = sum;

    return newNode;
}

/**
 * Returns  an  ordered list where each node contains a filename and the
 * summation of tf-idf values of all the matching search words for  that
 * file.  You only need to include documents (files) that contain one or
 * more of the given search words. The list must be in descending  order
 * of summation of tf-idf values (tfIdfSum). If there are multiple files
 * with  the  same tf-idf sum, order them by their filename in ascending
 * order. D is the total number of documents in the collection.
 */
TfIdfList retrieve(InvertedIndexBST tree, char *searchWords[], int D) {
    
    TfIdfList gather = NULL;
    int i = 0;    

    //This iterates through each of the given words
    while (searchWords[i]!= NULL) {
        //A list is then returned containing each file and its tfidf value
        TfIdfList list = calculateTfIdf(tree, searchWords[i], D);

        while (list!=NULL) {   
            //Files across the linked lists of each word are then united in the
            //linked list below    
            gather = insertTfNode(gather, list->filename, list->tfIdfSum);
            list=list->next;            
        }

        i++;
    }
    //The first linked list in this function will have duplicates of filenames due
    //to the manner in which the insertTfNode function is called, therefore another
    // linked list is created to get rid of duplicates
    TfIdfList fin = NULL;

    while (gather!=NULL) {
        fin = insertTfNode(fin, gather->filename,gather->tfIdfSum);
        gather = gather->next;
    }

    //When getting rid of duplicates, the tfIdf values of recurring files will be added
    //This means the list will not be in order of tfIdf values, therefore a final list is
    //Created and then returned
    TfIdfList ordered = NULL;

    while (fin!=NULL) {
        ordered = insertTfNode(ordered, fin->filename,fin->tfIdfSum);
        fin = fin->next;
    }
    return ordered;
}

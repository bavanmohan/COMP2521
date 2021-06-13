# COMP2521 (20T3): Assignment 1
# Information Retrieval

### Objectives
To implement an information retrieval system using well known tf-idf measures
To give you further practice with C and data structures (Tree ADT)

### Aim
In this assignment, your task is to implement an information retrieval system using a well known term-weighting scheme called "tf-idf". You should start by reading the Wikipedia entries on these topics. The following Wikipedia page describes how to calculate tf-idf values. Later I will also discuss these topics in the lecture.

## tf-idf
For this assignment,

calculate relative term frequency tf(t,d) adjusted for document (d) length,
**tf(t, d) = (frequency of term t in d) / (number of words in d).**

calculate inverse document frequency idf(t, D) by dividing the total number of documents by the number of documents containing the term, and then taking the logarithm of that quotient:

calculate tfidf(t, d) = tf(t,d) * idf(t,D).

For clarifications, see the example below.

## Part-1: Inverted Index, using BST
You need to implement the required functions in the file invertedIndex.c that reads data from a given collection of files in collection.txt (see simple example files) and generates an "inverted index" that provides a sorted list (set) of filenames for every word in a given collection of files. You need to use binary search ADT to implement your inverted index. For more information on this, please see the following hints:

"How to Implement Ass1"
We will also discuss the above hints in the lecture. Please note that each list of filenames (for a single word) in your inverted index should be alphabetically ordered, using ascending order, and importantly duplicate filenames are not allowed.

  Hints: You should use fscanf to read words from a file, you do not need to impose any restriction on a line length. You need to use a dynamic data structure(s) to handle words in a file and across files, no need to know max words beforehand. You can assume a maximum word length of 100.

**Normalise words**: Before inserting words in your inverted index, you need to "normalise" words by,

removing leading and trailing spaces,
converting all characters to lowercase,
remove the following punctuation marks, if they appear at the end of a word.
- '.' (dot),
- ',' (comma),
- ';' (semicolon),
- ? (question mark)
Please note that if you have multiple punctuation marks at the end of a word, you need to remove only the last punctuation mark. You also don't need to remove the above punctuation marks if they appear in the middle or at the start of a word.

Importantly, you need to modify a given string, do NOT create another copy. You can use the functions tolower and strlen. You may find the following links useful:

tolower example
C Strings

For example,

| word        | normalised word |
|-------------|-----------------|
| .Net        | .net            |
| smh.com.au  | smh.com.au      |
| Sydney!     | sydney!         |
| why?        | why             |
| order.      | order           |
| text;       | text            |
| abc.net.au. | abc.net.au      |
| sydney???   | sydney??        |

You need to implement the following functions in the file invertedIndex.c. The API file invertedIndex.h is provided, you must implement the required functions in your invertedIndex.c. We will individually test these functions for awarding marks.

`char *normaliseWord(char *str);`

Follow the instructions provided earlier in the specs to normalise a given string. You need to modify a given string, do NOT create another copy.

`InvertedIndexBST generateInvertedIndex(char *collectionFilename);`

The function needs to read a given file with collection of file names, read each of these files, generate inverted index as discussed earlier in the specs and return the inverted index. Do not modify invertedIndex.h file.

`void printInvertedIndex(InvertedIndexBST tree);`

Your program should output a give inverted index tree to a file named invertedIndex.txt. One line per word, words should be alphabetically ordered, using ascending order. Each list of filenames (for a single word) should be alphabetically ordered, using ascending order. You also need to print the tf value of the relevant word in that file, as shown below.

For example, invertedIndex.txt may look like the following. The example below offers formatting information. The expected invertedIndex.txt for exmp1 is available here.

`and file11.txt (0.030303)
apparent file11.txt (0.030303)
ascribed file11.txt (0.030303) file21.txt (0.062500)
attributed file11.txt (0.030303) file21.txt (0.062500)
been file11.txt (0.030303)
changes file11.txt (0.030303) file21.txt (0.062500)
circle mixed.txt (0.111111) planets.txt (0.222222)
color file11.txt (0.030303) file21.txt (0.062500)`

## Part-2: Information Retrieval Engine
In this part, you need to implement an information retrieval function that finds files (documents) with one or more query terms, and uses the summation of tf-idf values of all matching query terms (words) for ranking such files (documents). You need to calculate the tf-idf value for each matching query term in a file (document), and rank files (documents) based on the summation of tf-idf values for all matching query terms present in that file. Use the "inverted index" you created in Part 1 to locate files with one or more query terms and calculate the required tf-idf values for such files.

Implement the following information retrieval function retrieve in the file invertedIndex.c that given search terms (words), returns an ordered list of type TfIdfList, where each node contains a filename and the corresponding summation of tf-idf values for the given searchWords. The list must be in descending order of summation of tf-idf values. See invertedIndex.h for the type definition of TfIdfList. You also need to implement another function calculateTfIdf.

You need to implement the following two functions in the file invertedIndex.c. Total number of documents D is provided as an argument in both the functions.

`TfIdfList calculateTfIdf(InvertedIndexBST tree, char *searchWord, int D);`

This function returns an ordered list where each node contains a filename and the corresponding tf-idf value for a given searchWord. You only need to include documents (files) that contain the given searchWord. The list must be in descending order of tf-idf value. If there are multple files with the same tf-idf, order them by their filename in ascending order. D is the total number of documents in the collection.

`TfIdfList retrieve(InvertedIndexBST tree, char *searchWords[], int D);`

This function returns an ordered list where each node contains a filename and the summation of tf-idf values of all the matching searchWords for that file. You only need to include documents (files) that contain one or more of the given searchWords. The list must be in descending order of summation of tf-idf values (tfIdfSum). If there are multiple files with the same tf-idf sum, order them by their filename in ascending order. D is the total number of documents in the collection.

The searchWords array will be terminated with a NULL pointer. Here's an example:
`char *words[] = { "nasa", "mars", "earth", NULL };`

`TfIdfList list = retrieve(index, words, 7);`

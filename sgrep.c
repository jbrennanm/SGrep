#include <stdio.h>
#include <string.h>

void basic(char*, int, int);
char toLower(char);
int isUpper(char);
int isLetter(char);
int isValidPattern(char*);
void patternSearch(char*, int, int);
/*
Author: Brennan Mitchell
Class: CSc 352
Program: sgrep.c
Description: This program mimics the grep command in BASH. It can take up to
4 command line arguments. These are -o for only the matching string, -i for
case-insensitive search, -e to specifiy a pattern, and the final parameter
is the string (or pattern) to search for.
*/
int main(int argc, char * argv[]) {
	int o = 0;
	int i = 0;
	int e = 0;
	int specials = 0;
	int index = 0;
	// if argc < 2, not enough command arguments given
	if (argc < 2) {
		printf("Requires more command-line arguments.\n");
                return 1;
	}
	// this loop finds which flag arguments are called
	for (int x = 1; x < argc; x++) {
		if (argv[x][0] == '-') {
			if (argv[x][1] == 'o') {
				o = 1;
			}
			else if (argv[x][1] == 'i') {
                                i = 1;
                        }
			else if (argv[x][1] == 'e') {
                                e = 1;
                        } else {
				printf("Invalid search term.\n");
				return 2;
			}
		}
		if (isLetter(argv[x][0]) == 0 || argv[x][0] == '.') {
			index = x;
		}		
	}
	// this loop dictates if we're using pattern searching
	for (int x = 0; x < strlen(argv[index]); x++) {
		if (argv[index][x] == '.' || argv[index][x] == '#') {
			specials = 1;
			break;
		}
	}
	if (specials == 1) {
		if (isValidPattern(argv[index]) == 1) {
			printf("Invalid search term.\n");
			return 2;
		}
	}
	if (e == 1) {
		patternSearch(argv[index], i, o);
	} else {	
		basic(argv[index], i, o);
	}
	return 0;
}

/*
This function acts as the basic search. It takes in a string, and searches case
insensitive if i is 1, and only returns the word if o is 1.
*/
void basic(char args[], int i, int o) {
	if (i == 1) {
		for (int x = 0; x < strlen(args); x++) {
			if (isUpper(args[x]) == 0) {
				args[x] = toLower(args[x]);
			}
		}
	}
	int length = strlen(args);
	char buffer[202];
	while(fgets(buffer, 202, stdin) != NULL) {
		for (int x = 0; x < (strlen(buffer) - length); x++) {
			char curWord[length + 1];
			char lower[length + 1];
			int lcv = x;
			int pointer = 0;
			// this loop builds the comparison string from
			// the given index for comparison to arg string
			while (pointer < length) {
				char current = buffer[lcv];
				curWord[pointer] = current;
				if (isUpper(current) == 0) {
					char lowerCur = toLower(current);
					lower[pointer] = lowerCur;
				} else {
					lower[pointer] = current;
				}
				lcv++;
				pointer++;
			}
			curWord[pointer] = '\0';
			lower[pointer] = '\0';
			// if i == 1, then we have to search regardless
			// of case
			if (i == 1) {
				if (strcmp(lower, args) == 0) {
					if (o == 1) {	
                                		printf("%s\n", curWord);
					} else {
						printf("%s", buffer);
						break;
					}
                        }
			} else {
				if (strcmp(curWord, args) == 0) {
					if (o == 1) {
                                		printf("%s\n", curWord);
                                	} else {
                                  		printf("%s", buffer);
						break;
                                	}	
				}
			}
		}
	}
}

/*
This functions provides the pattern searching in the case that
the user inputs the -e flag. Takes in 3 arguments, which are the pattern
to be searched, the i if it is case insensitive, and the o to indicate whether
or not to return the full line, or only the matching string.
*/
void patternSearch(char args[], int i, int o) {
        if (i == 1) {
                for (int x = 0; x < strlen(args); x++) {
                        if (isUpper(args[x]) == 0) {
                                args[x] = toLower(args[x]);
                        }
                }
        }
        int length = strlen(args);
        char buffer[202];
        while(fgets(buffer, 202, stdin) != NULL) {
                for (int x = 0; x < (strlen(buffer) - length); x++) {
                	int lcv = x;
			int pointer = 0;
			int notMatch = 0;
	 		char curWord[200];
			// this loop searches through the string from
			// a start index, checking the pattern as we go.
			while (pointer < length && lcv < (strlen(buffer - length)) && notMatch == 0) {
				// if index is '.', then put it in the comparison
				// string and increment index.
				if (args[pointer] == '.') {
					curWord[pointer] = buffer[lcv];	
				}
				// if index is a letter, it must match the given
				// char in the argument string.
				else if (isLetter(args[pointer]) == 0) {
					if (buffer[lcv] == args[pointer]) {
						curWord[pointer] = buffer[lcv];
					} else {
						notMatch = 1;
						break;
					}	
				}
				lcv++;
				pointer++;
			}
			// notMatch is set to 1 if at any point the strings
			// do not match.
			if (notMatch == 0) {
				printf("%s\n", curWord);
			}
                }
        }
}

/*
This function takes in a char as an argument and returns the lowercase
equivalent of the char.
*/
char toLower(char c) {
	char retChar = c + 32;
	return retChar;
}

/*
This function determines whether or not a char is uppercase by using its
ASCII value. Returns 1 if false, 0 if true.
*/
int isUpper(char c) {
	if (c < 91) {
		return 0;
	}	
	return 1;
}

/*
This function determines if a char is a letter a-z, A-Z, by using its
ASCII value. Returns 1 if false, 0 if true.
*/
int isLetter(char c) {
	if (c > 64 && c < 91) {
		return 0;
	}
	else if (c > 96 && c < 123) {
		return 0;
	}
	return 1;
}	

/*
This function analyzes the pattern input by the user and determines whether
or not it is a valid pattern. Returns 1 if false, 0 if the pattern is valid.
*/
int isValidPattern(char pattern[]) {
	int length = strlen(pattern);
	if (pattern[0] == '#') {
		return 1;
	}
	if (pattern[length - 1] == '#') {
                return 1;
        }
	for (int i = 0; i < length; i++) {
		if (pattern[i] == '.') {
			if (pattern[i + 1] == '#')
				return 1;
		}
		if (pattern[i] == '#') {
			if (pattern[i - 1] == '.' || pattern[i + 1] == '.')
				return 1;
			if (pattern[i - 1] == '#' || pattern[i + 1] == '#')
                                return 1;
		}
	}
	return 0;
}





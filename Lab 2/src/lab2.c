/* File: lab2.c
 * Author: Youwei Zhang
 * School: Dalhousie University
 * Dept: Electrical and Computer Engineering
 * Course: ECED 3403 Computer Architecture
 *
 * Purpose： This file is for lab 2
 * Run Command: ./lab2 Dimension-A.Munro.txt SortMe.txt Checksum.txt
 * Last Modified: 2020.6.8
 */
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RECORDLENGTH 255

int main(int argc, char **argv) {
  if (argc != 4) {
    printf("Invlid Input, please try again\n");
    return 0;
  }

  FILE *f_dimension = fopen(argv[1], "r");
  FILE *f_output = fopen("Output.txt", "w+");
  char record[RECORDLENGTH];
  char delimit[] = " \t\r\n\v\f-.,:/";
  int comma_count = 0;
  int she_count = 0;

  while (fgets(record, sizeof record, f_dimension) != NULL) {
    // check comma in the record
    char *pch = strchr(record, ',');
    while (pch != NULL) {
      comma_count++;
      pch = strchr(pch + 1, ',');
    }
    // Check "she" token and printf into output file
    char *token = strtok(record, delimit);
    while (token != NULL) {
      int length = strlen(token) + 1;
      int i;
      char temp[length];
      for (i = 0; i < length; i++) {
        temp[i] = toupper(token[i]);
      }
      if (strcmp(temp, "SHE") == 0)
        she_count++;
      fprintf(f_output, "%s %lu\n", token, strlen(token));
      token = strtok(NULL, delimit);
    }
  }
  fclose(f_output);
  printf("The total occurance of comma is: %d\n", comma_count);
  printf("The total occurance of she is: %d\n", she_count);
  fclose(f_dimension);

  // Sort the SortMe.txt
  FILE *f_sortme = fopen(argv[2], "r");
  char first_word[RECORDLENGTH];
  fgets(first_word, sizeof first_word, f_sortme);
  char temp_word[RECORDLENGTH];
  while (fgets(temp_word, sizeof temp_word, f_sortme) != NULL) {
    if (strcmp(first_word, temp_word) > 0) {
      strcpy(first_word, temp_word);
    }
  }
  printf("After sorting, the first word is: %s\n", first_word);
  fclose(f_sortme);

  // checksum: Parity byte & Sum compliment
  FILE *f_checksum = fopen(argv[3], "a+");
  bool parity_sum = 0;
  int compliment_sum = 0;
  char asc;
  while ((asc = fgetc(f_checksum)) != EOF) {
    int value = (int)asc;
    compliment_sum += value;
    while (value != 0) {
      if (value % 2 == 1) {
        parity_sum = !parity_sum;
        value--;
      }
      value = value / 2;
    }
  }
  printf("Parity Sum is: %d\n", parity_sum);
  fputc(parity_sum, f_checksum);
  printf("The sum comliment checksum for Checksum.txt is: %d\n",
         compliment_sum);

  fclose(f_checksum);

  return 0;
}
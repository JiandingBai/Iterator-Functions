#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "list.h"

// Copy list.h and your completed list.c implementation into the q5 directory 
// and use the provided Makefile to compile and test your implementation of trunc
void string_to_number(element_t* out, element_t in) {
  char* str = (char*)in;
  int* num = malloc(sizeof(int));
  char* endptr;
  long val = strtol(str, &endptr, 10);
  if (*endptr == '\0' && strlen(str) > 0) {
      *num = (int)val;
  } else {
      *num = -1;
  }
  *out = num;
}

void nullify_number_strings(element_t* out, element_t num_elem, element_t str_elem) {
  int* num = (int*)num_elem;
  *out = (*num != -1) ? NULL : str_elem;
}

int is_non_negative(element_t elem) {
  int* num = (int*)elem;
  return *num >= 0;
}

int is_not_null(element_t elem) {
  return elem != NULL;
}

void truncate_string(element_t* out, element_t num_elem, element_t str_elem) {
  int* num = (int*)num_elem;
  char* input_str = (char*)str_elem;
  int trunc_len = *num;
  int input_len = strlen(input_str);
  int result_len = (trunc_len < input_len) ? trunc_len : input_len;
  
  char* result = malloc(result_len + 1);
  strncpy(result, input_str, result_len);
  result[result_len] = '\0';
  *out = result;
}

void print_string(element_t elem) {
  printf("%s\n", (char*)elem);
}

void print_with_space(element_t elem) {
  static int first = 1;
  if (first) {
      printf("%s", (char*)elem);
      first = 0;
  } else {
      printf(" %s", (char*)elem);
  }
}

void find_max(element_t* out, element_t acc, element_t elem) {
  int* max_so_far = (int*)acc;
  int* current = (int*)elem;
  int* max_ptr = (int*)*out;
  
  if (max_ptr == NULL) {
      max_ptr = malloc(sizeof(int));
      *out = max_ptr;
  }
  *max_ptr = (*current > *max_so_far) ? *current : *max_so_far;
}

int main (int argc, char** argv) {
  // TODO: your implementation of the trunc program
 // Step 1: Read command line arguments
 struct list* list = list_create();
 for (int i = 1; i < argc; i++) {
     list_append(list, argv[i]);
 }

 // Step 2: Map to create list of numbers (int*)
 struct list* numbers = list_create();
 list_map1(string_to_number, numbers, list);

 // Step 3: Map2 to create list with NULLs for number strings
 struct list* strings_with_nulls = list_create();
 list_map2(nullify_number_strings, strings_with_nulls, numbers, list);

 // Step 4: Filter numbers to remove negatives
 struct list* filtered_numbers = list_create();
 list_filter(is_non_negative, filtered_numbers, numbers);

 // Step 5: Filter strings to remove NULLs
 struct list* filtered_strings = list_create();
 list_filter(is_not_null, filtered_strings, strings_with_nulls);

 // Step 6: Truncate strings using the filtered lists
 struct list* truncated = list_create();
 list_map2(truncate_string, truncated, filtered_numbers, filtered_strings);

 // Step 7: Print truncated strings, one per line
 list_foreach(print_string, truncated);

 // Step 8: Concatenate and print
 list_foreach(print_with_space, truncated);
 printf("\n");

 // Step 9: Find and print max number
 int max = 0;
 int* max_ptr = &max;
 list_foldl(find_max, (element_t*)&max_ptr, filtered_numbers);
 printf("%d\n", max);

 // Step 10: Free all memory
 list_foreach(free, numbers);      // Free int* elements
 list_foreach(free, truncated);    // Free truncated string elements
 list_destroy(list);
 list_destroy(numbers);
 list_destroy(strings_with_nulls);
 list_destroy(filtered_numbers);
 list_destroy(filtered_strings);
 list_destroy(truncated);

 return 0;
}



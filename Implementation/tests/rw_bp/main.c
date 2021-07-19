/******************************************************************************
 * main.c
 *
 * Read and write balanced parenthesis sequences
 * 
 * Compile : gcc -o bp main.c bit_array.c
 * Usage   : ./bp <input BP sequence> <output BP sequence> 
 *
 * This code uses the bit array C library of Isaac Turner
 * url: https://github.com/noporpoise/BitArray/
 *
 ******************************************************************************
 * Copyright (C) 2016 José Fuentes Sepúlveda <jfuentess@dcc.uchile.cl>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 *****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "bit_array.h"

/* 
 * Read the parentheses sequence in 'fn' as the bit array 'B'. An opening 
 * parenthesis is represented as a 1 and a closing parentheses is represented as
 * a 0
 */
BIT_ARRAY* parentheses_to_bits(const char* fn, long* n) {
  
  char parenthesis;
  long counter = 0L;
  
  FILE* fp = fopen(fn, "r");
  if (!fp) {
    fprintf(stderr, "Error opening file \"%s\".\n", fn);
    exit(EXIT_FAILURE);
  }

  fseek(fp, 0L, SEEK_END);
  *n = ftell(fp);

  BIT_ARRAY* B = bit_array_create(*n);
  
  fseek(fp, 0L, SEEK_SET);

  while (fread(&parenthesis,sizeof(char),1,fp)==1) {
    if(parenthesis == '(')
      bit_array_set_bit(B, counter);
    counter++;
  }
  
  fclose(fp);
  
  return B;

}

/*
 * Convert the bit array B in a parentheses sequence
 */
void bits_to_parentheses(BIT_ARRAY *B, FILE* f) {
  long i = 0;
  for(i = 0; i < B->num_of_bits; i++)
    if(bit_array_get_bit(B, i) == 1)
      fprintf(f, "(");
    else
      fprintf(f, ")");
}


int main(int argc, char *argv[]) {  
  if(argc < 3){
    fprintf(stderr, "Usage: %s <input BP sequence> <output BP sequence>\n",
	    argv[0]); 
    exit(EXIT_FAILURE);
  }

  long n;

  /* Read a parentheses sequence as a bit array */
  BIT_ARRAY *B = parentheses_to_bits(argv[1], &n);

  printf("%ld parentheses read\n", n);
  
  /* Your code */

  /* Output file */
  FILE *f = fopen(argv[2], "w");

  /* Convert a bit array in a parentheses sequence */
  bits_to_parentheses(B, f);

  bit_array_free(B);
  fclose(f);

  return EXIT_SUCCESS;
}

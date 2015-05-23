/*
 * testmain.c
 */

#include <stdlib.h>
#include <stdio.h>

#include "avenida.h"
#include "commands.h"

static void try(const char *str);

static void
try(const char *str)
{
  static int counter = 1;
  struct avnop *op;

  if ((op = avnop_from_json(str)) == NULL) {
    printf("%d\tNOTOK\n", counter);
  } else {
    printf("%d\t%s\n", counter,
      cJSON_PrintUnformatted(avnop_to_json(op)));
    avnop_free(op);
  }

  printf("==========\n");
  counter++;
}


int
main(void)
{
  try("");
  try("{}");
  try("{\"name\":}");
  try("{\"name\":0}");
  try("{\"name\":\"a\"}");
  try("{\"name\":\"a\", \"args\":}");
  try("{\"name\":\"a\", \"args\":\"a\"}");
  try("{\"name\":\"a\", \"args\":[]}");
  try("{\"name\":\"negate\", \"args\":[]}");
  try("{\"name\":\"rotate\", \"args\":[5,6,7,8]}");
  return EXIT_SUCCESS;
}

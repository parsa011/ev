#include "prompt.h"
#include <stdio.h>

bool prompt_bool(char *message)
{
	puts(message);
	return true;
}

char *prompt_string(char *message)
{
	puts(message);
	char *string = "test";
	return string;
}

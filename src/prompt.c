#include "prompt.h"
#include <stdio.h>

bool prompt_bool(char *message)
{
	printf("%s\n", message);
	return true;
}

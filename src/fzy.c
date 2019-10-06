#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <limits.h>
#include <unistd.h>

#include "match.h"
#include "choices.h"
#include "options.h"

#include "../config.h"

int main(int argc, char *argv[]) {
	int ret = 0;

	options_t options;
	options_parse(&options, argc, argv);

	choices_t choices;
	choices_init(&choices, &options);

	if (options.benchmark) {
		if (!options.filter) {
			fprintf(stderr, "Must specify -e/--show-matches with --benchmark\n");
			exit(EXIT_FAILURE);
		}
		choices_fread(&choices, stdin, options.input_delimiter);
		for (int i = 0; i < options.benchmark; i++)
			choices_search(&choices, options.filter);
	} else if (options.filter) {
		choices_fread(&choices, stdin, options.input_delimiter);
		choices_search(&choices, options.filter);
		for (size_t i = 0; i < choices_available(&choices); i++) {
			if (options.show_scores)
				printf("%f\t", choices_getscore(&choices, i));
			printf("%s\n", choices_get(&choices, i));
		}
	}

	choices_destroy(&choices);

	return ret;
}

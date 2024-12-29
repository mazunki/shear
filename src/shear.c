#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

typedef struct {
	char *text;
} field;

int split(field **out_fields, const char *separator) {
	size_t field_count = 0;
	size_t field_capacity = 8;
	*out_fields = malloc(field_capacity*sizeof(field));
	if (!*out_fields) {
		fprintf(stderr, "memory error\n");
		return -1;
	}

	char *current_field = malloc(BUFFER_SIZE);
	if (!current_field) {
		fprintf(stderr, "memory error\n");
		free(*out_fields);
		return -1;
	}
	size_t current_length = 0;

	const size_t sep_len = strlen(separator);
	size_t sep_idx = 0;
	char c;

	while ((c = getc(stdin)) != EOF) {
		if (c == separator[sep_idx]) {
			sep_idx++;
			if (sep_idx == sep_len) {
				current_field[current_length-sep_len+1] = '\0';
				if (field_count >= field_capacity) {
					field_capacity *= 2;
					*out_fields = realloc(*out_fields, field_capacity*sizeof(field));
					if (!*out_fields) {
						fprintf(stderr, "realloc failed\n");
						free(current_field);
						return -1;
					}
				}
				(*out_fields)[field_count].text = strdup(current_field);
				if (!(*out_fields)[field_count].text) {
					fprintf(stderr, "memory error\n");
					free(current_field);
					return -1;
				}
				field_count++;
				current_length = 0;
				sep_idx = 0;
				continue;
			}
		} else {
			sep_idx = 0;
		}

		if (current_length < BUFFER_SIZE - 1) {
			current_field[current_length++] = c;
		} else {
			fprintf(stderr, "field size too long\n");
			free(current_field);
			return -1;
		}
	}

	if (current_length > 0) {
		current_field[current_length] = '\0';
		if (field_count >= field_capacity) {
			field_capacity *= 2;
			*out_fields = realloc(*out_fields, field_capacity * sizeof(field));
			if (!*out_fields) {
				fprintf(stderr, "realloc failed\n");
				free(current_field);
				return -1;
			}
		}
		(*out_fields)[field_count].text = strdup(current_field);
		if (!(*out_fields)[field_count].text) {
			fprintf(stderr, "memory error\n");
			free(current_field);
			return -1;
		}
		field_count++;
	}

	free(current_field);
	return field_count;
}

int main(int argc, char *argv[]) {
	const char *separator = " - ";
	char *desired_fields = "1,2";

	int opt;
	while((opt = getopt(argc, argv, "d:f:")) != -1) {
		switch (opt) {
			case 'd':
				separator = optarg;
			break;
			case 'f':
				desired_fields = optarg;
			break;
			default:
				fprintf(stderr, "usage: %s [-d delimiter] [-f fields]\n", argv[0]);
			return 1;
		}
	}


	field *fields = NULL;
	int field_count = split(&fields, separator);

	if (field_count < 0) {
		fprintf(stderr, "couldn't split input\n");
		return 1;
	}

	const char *field_list_sep = ",";
	for (char *p = strtok(desired_fields, field_list_sep); p != NULL; p = strtok(NULL, field_list_sep)) {
		char *c = strstr(p, "..");
		if (c == NULL) {
			int f = strtol(p, NULL, 10);
			printf("field %d: «%s»\n", f, fields[f-1].text);
		} else if (c == p) {
			int end = strtol(p+2, NULL, 10);
			for (int f=1; f <= field_count && f <= end; f++) {
				printf("field %d: «%s»\n", f, fields[f-1].text);
			}
		} else if (c == p + strlen(p) - 2) {
			int start = strtol(p, NULL, 10);
			for (int f=start; f <= field_count; f++) {
				printf("field %d: «%s»\n", f, fields[f-1].text);
			}
		} else {
			int start = strtol(p, NULL, 10);
			int end = strtol(c + 2, NULL, 10);
			for (int f=start; f <= field_count && f <= end; f++) {
				printf("field %d: «%s»\n", f, fields[f-1].text);
			}
		}
	}

	free(fields);

	return 0;
}

// vim: set ts=4 sw=4 noet


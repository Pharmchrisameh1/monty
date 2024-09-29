#include "monty.h"

bus_t bus = {NULL, NULL, NULL, 0};

/**
 * getline - Reads an entire line from a stream.
 * @lineptr: Pointer to the buffer containing the line read.
 * @n: Pointer to the size of the buffer.
 * @stream: The stream to read from.
 *
 * Return: The number of characters read, including the delimiter.
 */
ssize_t getline(char **lineptr, size_t *n, FILE *stream)
{
	char *buffer;
	int c;
	size_t len = 0;

	if (!lineptr || !n || !stream)
		return (-1);

	if (*lineptr == NULL)
	{
		*n = 128;
		*lineptr = malloc(*n);
		if (!*lineptr)
			return (-1);
	}

	buffer = *lineptr;

	while ((c = fgetc(stream)) != EOF)
	{
		if (len + 1 >= *n)
		{
			*n *= 2;
			buffer = realloc(buffer, *n);
			if (!buffer)
				return (-1);
			*lineptr = buffer;
		}
		buffer[len++] = c;
		if (c == '\n')
			break;
	}

	if (len == 0)
		return (-1);

	buffer[len] = '\0';
	return (len);
}

/**
 * main - Monty code interpreter.
 * @argc: Number of arguments.
 * @argv: Monty file location.
 *
 * Return: 0 on success.
 */
int main(int argc, char *argv[])
{
	char *content;
	FILE *file;
	size_t size = 0;
	ssize_t read_line = 1;
	stack_t *stack = NULL;
	unsigned int counter = 0;

	if (argc != 2)
	{
		fprintf(stderr, "USAGE: monty file\n");
		exit(EXIT_FAILURE);
	}

	file = fopen(argv[1], "r");
	bus.file = file;
	if (!file)
	{
		fprintf(stderr, "Error: Can't open file %s\n", argv[1]);
		exit(EXIT_FAILURE);
	}

	while (read_line > 0)
	{
		content = NULL;
		read_line = getline(&content, &size, file);
		bus.content = content;
		counter++;
		if (read_line > 0)
			execute(content, &stack, counter, file);
		free(content);
	}

	free_stack(stack);
	fclose(file);
	return (0);
}


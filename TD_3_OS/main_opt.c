#include <stdio.h>
#include <fcntl.h>
#include "io_file.h"

void check_string() {
	IO_FILE file;
	char buffer[32];

	printf("String test:\nbase: Hello world!\nfile: ");

	file = IO_open("test.txt", O_CREAT | O_WRONLY);

	IO_string_write(file, "Hello world!\n", 13);

	IO_close(file);

	file = IO_open("test.txt", O_RDONLY);

	IO_string_read(file, buffer, 31);

	IO_close(file);

	IO_remove("test.txt");

	printf("%s", buffer);
}

/*void check_int() {
	IO_FILE file;
	int i;

	printf("Integer test:\nbase: %d %d %d %d\nfile: ", 42, -17, 0, 42359428);

	file = IO_open("test.txt", O_CREAT | O_WRONLY);

	IO_char_write(file, '-');
	IO_char_write(file, ' ');
	IO_int_write(file, 42);
	IO_char_write(file, ' ');
	IO_int_write(file, -17);
	IO_char_write(file, ' ');
	IO_int_write(file, 0);
	IO_char_write(file, ' ');
	IO_int_write(file, 42359428);
	IO_char_write(file, ' ');

	IO_close(file);

	file = IO_open("test.txt", O_RDONLY);

	IO_int_read(file, &i);
	printf("%d ", i);
	IO_int_read(file, &i);
	printf("%d ", i);
	IO_int_read(file, &i);
	printf("%d ", i);
	IO_int_read(file, &i);
	printf("%d\n", i);

	IO_close(file);

	IO_remove("test.txt");
}
*/
int main(int argc, char **argv) {
	check_string(); printf("\n");
	//check_int();

	return 0;
}

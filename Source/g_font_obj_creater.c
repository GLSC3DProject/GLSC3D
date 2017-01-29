#include <stdio.h>
#include <string.h>

int main(int argc, char **argv){
	FILE *font_object = fopen("g_font_obj.c", "w");
	FILE *font_header = fopen("g_font_obj.symb", "w");

	unsigned font_file_size[argc - 1];
	char basenames[argc][256];

	for(int i = 1; i < argc; ++i){
		FILE *font_file = fopen(argv[i], "rb");
		char basename[strlen(argv[i])];
		char *s = argv[i], *d = basename;

		while(*s != '.')
			*d++ = *s++;
		*d = '\0';
		unsigned size = 0;
		unsigned long long buffer;
		fprintf(font_object, "unsigned long long g_font_%s_data[] = {\n", basename);
		while(fread(&buffer, sizeof(buffer), 1, font_file) > 0)
			fprintf(font_object, "0x%0*llx, ", (int)sizeof(buffer)*2, buffer), size++;
		fprintf(font_object, "};\n");
		font_file_size[i - 1] = size;
		fprintf(font_header, "extern unsigned long long g_font_%s_data[];\n", basename);
		strcpy(basenames[i - 1], basename);
		fclose(font_file);
	}
	fprintf(font_header, "enum {\n");
	for(int i = 1; i < argc; ++i){
		fprintf(font_header, "\tg_font_%s_size = %d,\n", basenames[i -1], (int)(font_file_size[i - 1]*sizeof(unsigned long long)));
	}
	fprintf(font_header, "};\n\n");

	return 0;
}

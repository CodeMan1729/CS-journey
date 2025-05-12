// Modifies the volume of an audio file

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// Number of bytes in .wav header
const int HEADER_SIZE = 44;

void copy_header(FILE *input, FILE *output);
void modify(FILE *input, FILE *output, float factor);

int main(int argc, char *argv[])
{
    // Check command-line arguments
    if (argc != 4)
    {
        printf("Usage: ./volume input.wav output.wav factor\n");
        return 1;
    }

    // Open files and determine scaling factor
    FILE *input = fopen(argv[1], "rb");
    if (input == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    FILE *output = fopen(argv[2], "wb");
    if (output == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    float factor = atof(argv[3]);

    // Copy header from input file to output file
    copy_header(input, output);

    // Read samples from input file and write updated data to output file
    modify(input, output, factor);

    // Close files
    fclose(input);
    fclose(output);
}

void copy_header(FILE *input, FILE *output)
{
    uint8_t header[HEADER_SIZE];

    fread(header, sizeof(header), 1, input);
    fwrite(header, sizeof(header), 1, output);
}

void modify(FILE *input, FILE *output, float factor) // manipulate samples
{
    int16_t buffer;
    float sample;

    while (fread(&buffer, sizeof(buffer), 1, input) == 1)
    {
        sample = (float) buffer * factor;
        buffer = (int16_t) sample;
        fwrite(&buffer, sizeof(buffer), 1, output);
    }
}

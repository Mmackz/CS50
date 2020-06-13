#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    // Check for valid command-line arguments
    if (argc != 2)
    {
        printf("Usage: ./recover image\n");
        return 1;
    }

    // Open file and assign it to a pointer named file
    FILE *file = fopen(argv[1], "r");

    // Check if file could not be opened/equals NULL
    if (!file)
    {
        printf("File could not be opened\n");
        return 1;
    }

    // Allocate memory for the 512-byte read buffer
    BYTE *buffer = calloc(512, sizeof(BYTE));

    // Check if memory allocation was successful
    if (!buffer)
    {
        printf("Insufficient Memory available.\nProgram exiting.\n");
        return 1;
    }

    int jpeg_count = 0;
    FILE *jpeg = NULL;

    // Fill buffer 512 bytes at a time from file until end of file
    while (fread(buffer, 512, sizeof(BYTE), file))
    {
        // Look for JPEG headers
        if (buffer[0] == 0xFF && buffer[1] == 0xD8 && buffer[2] == 0xFF && (buffer[3] & 0xF0) == 0xE0)
        {
            // If a JPEG has previously been found, close previous file
            if (jpeg_count > 0)
            {
                fclose(jpeg);
            }

            // Create filename
            char *filename = calloc(8, sizeof(char));

            // Check if memory allocation was successful
            if (!filename)
            {
                printf("Insufficient Memory available.\nProgram exiting.\n");
                return 1;
            }

            // Assign a string "xxx.jpg" to filename
            sprintf(filename, "%03i.jpg", jpeg_count);

            // Start writing the new JPEG file
            jpeg = fopen(filename, "w");
            fwrite(buffer, 512, sizeof(BYTE), jpeg);

            // Increase jpeg_count after each header is found
            jpeg_count++;

            // Free memory after each loop, it will re-initialize on next iteration
            free(filename);
        }
        // Write the rest of the file until a new header is found
        else if (jpeg_count > 0)
        {
            fwrite(buffer, 512, sizeof(BYTE), jpeg);
        }

    }

    // Close files and free memory
    fclose(file);
    fclose(jpeg);
    free(buffer);

    return 0;

}

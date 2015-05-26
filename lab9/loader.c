#include "loader.h"

#include "memory.h"

#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

/**
 * Parses command line arguments to find and validate the input file name.
 * @param argc the number of command line arguments.  Must always be 2.
 * @param argv the strings which make up the command line arguments.  The
 * filename must be in argv[1].
 * @return A pointer to the filename if found, otherwise null.
 */
static char* parseFilename(int argc, char** argv);

/**
 * Verifies if a string is a memory address, matching the format "0xFFF:",
 * where F represents a hex digit.
 * @param addr the string to be tested.
 * @return true if addr contains an address as described.
 */
static bool isAddress(char* addr);

/**
 * Verifies if a string represents a comment line, with 21 spaces 
 * followed by a |.
 * @param line the string to be tested.
 * @return true if line is a comment line.
 */
static bool isComment(char* line);

/**
 * Verifies if a string contains data, represented by up to 6 hexadecimal bytes.
 * @param data the string to be tested.
 * @return true if data is a data block.
 */
static bool isData(char* data);

/**
 * Verifies if an input line is a data record in the format,
 * " 0xfff: ffffffffffff |"
 * @param str the string to be verified.
 * @return true if str is a data record.
 */
static bool isDataRecord(char* str);

/**
 * Parses a data record and inserts the bytes into memory.
 * @param str the data record string.
 * @return true if the record was successfully inserted.
 */
static bool insertDataRecord(char* str);

/**
 * Verifies that a line read from the file contains a newline.  if it does
 * not, reads from the file until a newline is found.
 * @param line the string read from the file.
 * @param file the file being read.
 */
static void discardRestOfLine(char* line, FILE* file);

char* parseFilename(int argc, char** argv)
{
  char* errorMsg = "Usage: yess <filename>.yo";
  
  if (argc != 2)
  {
    printf("%s\n", errorMsg);
    return NULL;
  }
  
  char* temp = argv[1];
  while(*temp)
  {
    // find and verify the file extension
    if(*temp == '.')
    {
      if(temp[1] == 'y' && temp[2] == 'o')
      {
        return argv[1];
      }
    }
    temp++;
  }
  return NULL;
}

bool isDataRecord(char* str)
{
  // verify the spacing of the record
  if(str[0] == ' ' && str[1] == ' ' && str[8] == ' ' && str[21] == ' ' &&
      str[22] == '|')
  {
    // verify the address and data info
    if(isAddress(str + 2) && isData(str + 9))
    {
      return TRUE;
    }
  }
  return FALSE;
}

bool isAddress(char* addr)
{
  // verify that the address begins with 0x or 0X and ends with :
  if (addr[0] != '0' || (addr[1] != 'x' && addr[1] != 'X') || addr[5] != ':')
  {
    return FALSE;
  }
  
  // verify all digits of the address
  for (int i = 2; i < 5; ++i)
  {
    if (!isxdigit((int)addr[i]))
    {
      return FALSE;
    }
  }

  return TRUE;
}

bool isData(char* data)
{
  // verify that the data contains groups of 2 hex digits (1 byte)
  for (int i = 0; i < 12;)
  {
    if (isxdigit((int)data[i]) && isxdigit((int)data[i + 1]))
    {
      i += 2;
    }
    else if (data[i] == ' ')
    {
      ++i;
    }
    else
    {
      return FALSE;
    }
  }

  return TRUE;
}

bool isComment(char* line)
{
  // verify that the record is all spaces with a | in column 22
  for(int i = 0; i < 23; ++i)
  {
    if(i == 22 && line[22] == '|')
    {
      return TRUE;
    }
    else if(line[i] != ' ')
    {
      return FALSE;
    }
  }
  return FALSE;
}

bool insertDataRecord(char* str)
{
  // tracks the last address inserted into memory
  static int lastAddr = -1;  
  char buffer[4];

  // read the address from the record
  buffer[0] = str[4];
  buffer[1] = str[5];
  buffer[2] = str[6];
  buffer[3] = 0;
  int address = strtol(buffer, NULL, 16);
  if (address <= lastAddr)
  {
    return FALSE;
  }

  // read the data record, picking out pairs of hex digits (bytes)
  char* data = str + 9;
  for (int i = 0; i < 13;)
  {
    if (!isxdigit((int)data[i]))
    {
      ++i;
    }
    else
    {
      // read the byte
      buffer[0] = data[i];
      buffer[1] = data[i + 1];
      buffer[2] = 0;
      unsigned int byte = strtol(buffer, NULL, 16);

      // insert it into memory
      bool memErr;
      putByte(address, byte, &memErr);
      if (memErr)
      {
        printf("Error writing data to %x\n", address);
        return FALSE;
      }

      lastAddr = address++;
      i += 2;
    }
  }

  return TRUE;
}

void discardRestOfLine(char* line, FILE* file)
{
  bool newline = FALSE;

  // search for the newline
  while (*line)
  {
    if (*line++ == '\n')
    {
      newline = TRUE;
      break;
    }
  }

  if (!newline)
  {
    // strip chars from the file until newline or EOF is found
    int lastChar = 0;
    while (lastChar != '\n' && lastChar != EOF)
    {
      lastChar = fgetc(file);
    }
  }
}

bool load(int argc, char** argv)
{
  char* filename = parseFilename(argc, argv);
  
  if (!filename)
  {
    return FALSE;
  }

  FILE * file = fopen(filename, "r");
  if(!file)
  {
    printf("File %s does not exist\n", filename);
    return FALSE;
  }

  const int bufSize = 80;
  char line[bufSize];
  int lineNumber = 1;
  bool error = FALSE;
  while(fgets(line, bufSize, file))
  { 
    discardRestOfLine(line, file);

    if(isDataRecord(line))
    {
      if (!insertDataRecord(line))
      {
        error = TRUE;
      }
    }
    else if (!isComment(line))
    {
      error = TRUE;
    }

    if (error)
    {
      printf("Error on line %d\n%s\n", lineNumber, line);
      break;
    }

    ++lineNumber;
  }

  fclose(file);
  return !error;
}

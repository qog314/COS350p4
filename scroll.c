#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

// Boolean type definition:
typedef int bool;
#define true 1
#define false 0

// Constants:
#define TAB_LEN 8

// Globals:
int colWidth;
int numRows;
char* srcBuf;
int srcBufLen;
char** lines;
int numLines;

// Prototypes:
bool loadFile(char*);
bool parseLines();
void freeLines();


main(int argc, char* argv[]) {
   // Get window column width:
   struct winsize winsizestruct;
   ioctl(1, TIOCGWINSZ, &winsizestruct);
   colWidth = winsizestruct.ws_col;
   numRows = winsizestruct.ws_row - 1;

// Load file:
   if (argc > 1){
      if (loadFile(argv[1]) == false){
         // TODO Report error.
      }
   } else{
      // Load from stdin.
   }

   // Parse into lines.
   if (parseLines()){
      free(srcBuf);
      srcBufLen = 0;
   }
   else {
      // TODO Report error.
   }

   int i;
   for (i = 0; i < numLines; i++) {
      printf("%s\n", lines[i]);
   }

   // TODO free lines array.
}

void printPrompt(int lineNum){
   int percent = (lineNum *100)/numLines;
   printf("\033[7m--scroll--(%d%%)\033[0m",percent);
   fflush(stdout);
}

void printLine(int lineNum){
   
}

/* loadFile:
 * Takes the given filename and attempts to load it into a buffer.
 * On failure, for any reason, returns 0. Otherwise returns 1.*/
bool loadFile(char* filename){
   // Attempt to open file.
   int fd = open(filename, O_RDONLY);
   if (fd == -1) perror("Open error");

   int fileLen = lseek(fd, 0, SEEK_END);
   if (fileLen == -1) perror("lseek error");

   srcBufLen = sizeof(char) * fileLen;
   srcBuf = malloc(srcBufLen);
   if (srcBuf == NULL) return false;

   // Attempt to load file.
   lseek(fd, 0, SEEK_SET);
   if (read(fd, srcBuf, srcBufLen) != srcBufLen) return false;

   // Succeeded!
   close(fd);
   return true;
}

/* parseLines:
 * Takes the input from the srcBuffer and parses it into discrete lines.
 * These lines can then be printed directly.
 * Note: Lines in the resulting array are null-terminated strings with no
 * new-line character.
 * Additionally, the lines array should be freed from memory once no
 * longer needed. This can be done with freeLines().*/
bool parseLines(){
   // Count lines:
   int currCol = 0;
   numLines = 0;
   int i;
   for (i = 0; i < srcBufLen; i++){
      char currChar = srcBuf[i];
      if (currChar == '\t'){
         do{
            if (currCol == colWidth - 1){
               numLines++;
               currCol = 0;
            }
            else currCol++;
         } while (currCol % TAB_LEN);
      }
      else if(currChar == '\n'){
         numLines++;
         currCol = 0;
      }
      else currCol++;

      if (currCol >= colWidth){
         numLines++;
         currCol = 0;
      }
   }

   // Create line array.
   lines = malloc(sizeof(char*) * numLines);
   if (lines == NULL) return false;
   int lineWidth = sizeof(char) * (colWidth + 1);
   for (i = 0; i < numLines; i++){
      lines[i] = malloc(lineWidth);
      if (lines[i] == NULL) return false;
   }

   // Parse lines.
   if (lines[0] == NULL) return false;
   int currLine = 0;
  currCol = 0;
   for (i = 0; i < srcBufLen; i++){
      char currChar = srcBuf[i];

      // Handle tab characters.
      if (currChar == '\t'){
         do{
            if (currCol == colWidth - 1){
               lines[currLine][currCol] = 0;
               currLine++;
               currCol = 0;
            }
            else {
               lines[currLine][currCol] = ' ';
               currCol++;
            }
         } while(currCol % TAB_LEN);
      }

      // Handle newlines.
      else if(currChar == '\n'){
         lines[currLine][currCol] = 0;
         currLine++;
         currCol = 0;
      }

      // Everything else.
      else{
         lines[currLine][currCol] = currChar;
         currCol++;
         if (currCol == colWidth){
            lines[currLine][currCol] = 0;
            currLine++;
            currCol = 0;
         }
      }
   }

   // Success!
   return true;
}

/* freeLines:
 * Frees all of the malloced memory associated with the lines array. */
void freeLines(){
   int i;
   for (i = 0; i < numLines; i++) free(lines[i]);
   free(lines);
}

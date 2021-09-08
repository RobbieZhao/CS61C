#include <stdio.h>
#include <string.h>

#include <unistd.h>
#include <sys/stat.h>

#include "beargit.h"
#include "util.h"

/* Implementation Notes:
 *
 * - Functions return 0 if successful, 1 if there is an error.
 * - All error conditions in the function description need to be implemented
 *   and written to stderr. We catch some additional errors for you in main.c.
 * - Output to stdout needs to be exactly as specified in the function description.
 * - Only edit this file (beargit.c)
 * - You are given the following helper functions:
 *   * fs_mkdir(dirname): create directory <dirname>
 *   * fs_rm(filename): delete file <filename>
 *   * fs_mv(src,dst): move file <src> to <dst>, overwriting <dst> if it exists
 *   * fs_cp(src,dst): copy file <src> to <dst>, overwriting <dst> if it exists
 *   * write_string_to_file(filename,str): write <str> to filename (overwriting contents)
 *   * read_string_from_file(filename,str,size): read a string of at most <size> (incl.
 *     NULL character) from file <filename> and store it into <str>. Note that <str>
 *     needs to be large enough to hold that string.
 *  - You NEED to test your code. The autograder we provide does not contain the
 *    full set of tests that we will run on your code. See "Step 5" in the homework spec.
 */

/* beargit init
 *
 * - Create .beargit directory
 * - Create empty .beargit/.index file
 * - Create .beargit/.prev file containing 0..0 commit id
 *
 * Output (to stdout):
 * - None if successful
 */

int beargit_init(void)
{
  fs_mkdir(".beargit");

  FILE *findex = fopen(".beargit/.index", "w");
  fclose(findex);

  write_string_to_file(".beargit/.prev", "0000000000000000000000000000000000000000");

  return 0;
}

/* beargit add <filename>
 * 
 * - Append filename to list in .beargit/.index if it isn't in there yet
 *
 * Possible errors (to stderr):
 * >> ERROR: File <filename> already added
 *
 * Output (to stdout):
 * - None if successful
 */

int beargit_add(const char *filename)
{
  FILE *findex = fopen(".beargit/.index", "r");
  FILE *fnewindex = fopen(".beargit/.newindex", "w");

  char line[FILENAME_SIZE];
  while (fgets(line, sizeof(line), findex))
  {
    strtok(line, "\n");
    if (strcmp(line, filename) == 0)
    {
      fprintf(stderr, "ERROR: File %s already added\n", filename);
      fclose(findex);
      fclose(fnewindex);
      fs_rm(".beargit/.newindex");
      return 3;
    }

    fprintf(fnewindex, "%s\n", line);
  }

  fprintf(fnewindex, "%s\n", filename);
  fclose(findex);
  fclose(fnewindex);

  fs_mv(".beargit/.newindex", ".beargit/.index");

  return 0;
}

/* beargit rm <filename>
 * 
 * See "Step 2" in the homework 1 spec.
 *
 */

int beargit_rm(const char *filename)
{
  FILE *findex = fopen(".beargit/.index", "r");
  FILE *fnewindex = fopen(".beargit/.newindex", "w");

  char line[FILENAME_SIZE];
  int contains = 0;
  while (fgets(line, sizeof(line), findex))
  {
    strtok(line, "\n");
    if (strcmp(line, filename) != 0)
    {
      fprintf(fnewindex, "%s\n", line);
    }
    else
    {
      contains = 1;
    }
  }

  fclose(findex);
  fclose(fnewindex);

  if (contains)
  {
    fs_mv(".beargit/.newindex", ".beargit/.index");

    return 0;
  }

  fprintf(stderr, "ERROR: File %s not tracked\n", filename);

  return 1;
}

/* beargit commit -m <msg>
 *
 * See "Step 3" in the homework 1 spec.
 *
 */

const char *go_bears = "GO BEARS!";

int is_commit_msg_ok(const char *msg)
{
  for (int i = 0; msg[i] != '\0'; i++)
  {
    int j;
    for (j = 0; msg[i + j] != '\0' && go_bears[j] != '\0'; j++)
    {
      if (msg[i + j] != go_bears[j])
      {
        break;
      }
    }

    if (go_bears[j] == '\0')
    {
      return 1;
    }
  }

  return 0;
}

void next_commit_id(char *commit_id)
{
  for (int i = 0; commit_id[i] != '\0'; i++)
  {
    char c = commit_id[i];

    if (c == '1')
    {
      commit_id[i] = '6';
      break;
    }
    else if (c == '6')
    {
      commit_id[i] = 'c';
      break;
    }
    else
    {
      commit_id[i] = '1';
    }
  }
}

int beargit_commit(const char *msg)
{
  if (!is_commit_msg_ok(msg))
  {
    fprintf(stderr, "ERROR: Message must contain \"%s\"\n", go_bears);
    return 1;
  }

  char commit_id[COMMIT_ID_SIZE];
  read_string_from_file(".beargit/.prev", commit_id, COMMIT_ID_SIZE);
  next_commit_id(commit_id);

  /* COMPLETE THE REST */

  // Create a folder .beargit/<newid>
  char folder[10 + COMMIT_ID_SIZE];
  sprintf(folder, ".beargit/%s", commit_id);
  fs_mkdir(folder);

  // Copy all tracked files into .beargit/<newid>
  FILE *findex = fopen(".beargit/.index", "r");
  char line[FILENAME_SIZE];
  char filepath[20 + COMMIT_ID_SIZE + FILENAME_SIZE];

  while (fgets(line, sizeof(line), findex))
  {
    strtok(line, "\n");
    sprintf(filepath, ".beargit/%s/%s", commit_id, line);

    fs_cp(line, filepath);
  }

  fclose(findex);

  // Copy .beargit/.index into .beargit/<newid>/.index
  char indexfile[FILENAME_SIZE];
  sprintf(indexfile, ".beargit/%s/.index", commit_id);
  fs_cp(".beargit/.index", indexfile);

  // Copy .beargit/.prev into .beargit/<newid>/.prev
  char prevfile[FILENAME_SIZE];
  sprintf(prevfile, ".beargit/%s/.prev", commit_id);
  fs_cp(".beargit/.prev", prevfile);

  // Store the commit message into .beargit/<newid>/.msg
  char msgfile[FILENAME_SIZE];
  sprintf(msgfile, ".beargit/%s/.msg", commit_id);
  write_string_to_file(msgfile, msg);

  write_string_to_file(".beargit/.prev", commit_id);

  return 0;
}

/* beargit status
 *
 * See "Step 1" in the homework 1 spec.
 *
 */

int beargit_status()
{
  fprintf(stdout, "Tracked files:\n\n");

  FILE *findex = fopen(".beargit/.index", "r");
  char line[FILENAME_SIZE];

  int count = 0;
  while (fgets(line, sizeof(line), findex))
  {
    strtok(line, "\n");

    fprintf(stdout, "  ");
    fprintf(stdout, "%s\n", line);

    count++;
  }

  fprintf(stdout, "\n");
  fprintf(stdout, "%d files total\n", count);

  return 0;
}

/* beargit log
 *
 * See "Step 4" in the homework 1 spec.
 *
 */

int beargit_log()
{
  /* COMPLETE THE REST */
  char commit_id[COMMIT_ID_SIZE];
  read_string_from_file(".beargit/.prev", commit_id, COMMIT_ID_BYTES);
  // commit_id[COMMIT_ID_SIZE] = '\0';

  if (strcmp(commit_id, "0000000000000000000000000000000000000000") == 0)
  {
    fprintf(stderr, "ERROR: There are no commits!\n");
    return 1;
  }

  fprintf(stdout, "\n");
  while (strcmp(commit_id, "0000000000000000000000000000000000000000") != 0)
  {
    char prevfile[FILENAME_SIZE];
    sprintf(prevfile, ".beargit/%s/.prev", commit_id);

    char msgfile[FILENAME_SIZE];
    sprintf(msgfile, ".beargit/%s/.msg", commit_id);

    char commit_msg[MSG_SIZE];
    read_string_from_file(msgfile, commit_msg, MSG_SIZE);

    fprintf(stdout, "commit %s\n", commit_id);
    fprintf(stdout, "    %s\n\n", commit_msg);

    read_string_from_file(prevfile, commit_id, COMMIT_ID_SIZE);
  }

  return 0;
}

// 208561357 Bar Katz

#include <stdio.h>
#include <string.h>

#define UNIXNL 10
#define MACNL 14
#define WINNL 24
#define SWAP 1
#define KEEP 0

static void switch_OS_NL(FILE *f1, FILE *f2, short os_src_NL, short
os_dest_NL, char swap);
static void swap_endian(unsigned short *c);

int main(int argc, char **argv) {

  FILE *f1, *f2;
  unsigned short c = 0;

  if ((f1 = fopen(argv[1], "rb")) == 0) {
    printf("First file not found.");
  }
  if ((f2 = fopen(argv[2], "wb")) == 0) {
    printf("Second file not found.");
  }

  switch (argc) {
    case 3: {
      while (fread(&c, sizeof(short), 1, f1) == 1) {

        fwrite(&c, sizeof(short), 1, f2);
      }

      break;
    }

    case 5:
    case 6: {
      if (argc == 5 || strcmp(argv[5], "-swap") != 0) {
        if (strcmp(argv[3], "-unix") == 0 && strcmp(argv[4], "-mac") == 0) {
          switch_OS_NL(f1, f2, UNIXNL, MACNL, KEEP);
        } else if (strcmp(argv[3], "-unix") == 0
            && strcmp(argv[4], "-win") == 0) {
          switch_OS_NL(f1, f2, UNIXNL, WINNL, KEEP);
        } else if (strcmp(argv[3], "-mac") == 0
            && strcmp(argv[4], "-unix") == 0) {
          switch_OS_NL(f1, f2, MACNL, UNIXNL, KEEP);
        } else if (strcmp(argv[3], "-mac") == 0
            && strcmp(argv[4], "-win") == 0) {
          switch_OS_NL(f1, f2, MACNL, WINNL, KEEP);
        } else if (strcmp(argv[3], "-win") == 0
            && strcmp(argv[4], "-unix") == 0) {
          switch_OS_NL(f1, f2, WINNL, UNIXNL, KEEP);
        } else if (strcmp(argv[3], "-win") == 0
            && strcmp(argv[4], "-mac") == 0) {
          switch_OS_NL(f1, f2, WINNL, MACNL, KEEP);
        }
      } else {
        if (strcmp(argv[3], "-unix") == 0 && strcmp(argv[4], "-mac") == 0) {
          switch_OS_NL(f1, f2, UNIXNL, MACNL, SWAP);
        } else if (strcmp(argv[3], "-unix") == 0
            && strcmp(argv[4], "-win") == 0) {
          switch_OS_NL(f1, f2, UNIXNL, WINNL, SWAP);
        } else if (strcmp(argv[3], "-mac") == 0
            && strcmp(argv[4], "-unix") == 0) {
          switch_OS_NL(f1, f2, MACNL, UNIXNL, SWAP);
        } else if (strcmp(argv[3], "-mac") == 0
            && strcmp(argv[4], "-win") == 0) {
          switch_OS_NL(f1, f2, MACNL, WINNL, SWAP);
        } else if (strcmp(argv[3], "-win") == 0
            && strcmp(argv[4], "-unix") == 0) {
          switch_OS_NL(f1, f2, WINNL, UNIXNL, SWAP);
        } else if (strcmp(argv[3], "-win") == 0
            && strcmp(argv[4], "-mac") == 0) {
          switch_OS_NL(f1, f2, WINNL, MACNL, SWAP);
        }
      }

      break;
    }
  }

  fclose(f1);
  fclose(f2);

  return 0;
}

static void switch_OS_NL(FILE *f1, FILE *f2, short os_src_NL, short
os_dest_NL, char swap) {
  unsigned short c = 0;
  unsigned short temp;

  while (fread(&c, sizeof(short), 1, f1) == 1) {
    if (os_src_NL == WINNL && c == 14) {
      temp = c;
      fread(&c, sizeof(short), 1, f1);
      if (c == 10) {
        c = 24;
      } else {
        if (swap) {
          swap_endian(&temp);
        }
        fwrite(&temp, sizeof(short), 1, f2);
      }
    }

    if (c != os_src_NL) {
      if (swap) {
        swap_endian(&c);
      }
      fwrite(&c, sizeof(short), 1, f2);
    } else {
      if (os_dest_NL != WINNL) {
        c = os_dest_NL;
        if (swap) {
          swap_endian(&c);
        }
        fwrite(&c, sizeof(short), 1, f2);
      } else {
        c = 14;
        if (swap) {
          swap_endian(&c);
        }
        fwrite(&c, sizeof(short), 1, f2);

        c = 10;
        if (swap) {
          swap_endian(&c);
        }
        fwrite(&c, sizeof(short), 1, f2);
      }
    }
  }
}

static void swap_endian(unsigned short *c) {
  unsigned char temp1 = *((unsigned char *) c);
  unsigned char temp2 = *(((unsigned char *) c) + 1);

  *((char *) c) = temp2;
  *(((char *) c) + 1) = temp1;
}
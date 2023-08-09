/*

  wn.c - Command line interface to WordNet

*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "wn.h"

static char *Id = "$Id: wn.c,v 1.13 2005/01/31 19:19:09 wn Rel $";

static struct
{
  char *option;   /* user's search request */
  int search;     /* search to pass findtheinfo() */
  int pos;        /* part-of-speech to pass findtheinfo() */
  int helpmsgidx; /* index into help message table */
  char *label;    /* text for search header message */
} *optptr, optlist[] = {
               {"-synsa", SIMPTR, ADJ, 0, "Similarity"},
               {"-antsa", ANTPTR, ADJ, 1, "Antonyms"},
               {"-perta", PERTPTR, ADJ, 0, "Pertainyms"},
               {"-attra", ATTRIBUTE, ADJ, 2, "Attributes"},
               {"-domna", CLASSIFICATION, ADJ, 3, "Domain"},
               {"-domta", CLASS, ADJ, 4, "Domain Terms"},
               {"-famla", FREQ, ADJ, 5, "Familiarity"},
               {"-grepa", WNGREP, ADJ, 6, "Grep"},

               {"-synsn", HYPERPTR, NOUN, 0, "Synonyms/Hypernyms (Ordered by Estimated Frequency)"},
               {"-antsn", ANTPTR, NOUN, 2, "Antonyms"},
               {"-coorn", COORDS, NOUN, 3, "Coordinate Terms (sisters)"},
               {"-hypen", -HYPERPTR, NOUN, 4, "Synonyms/Hypernyms (Ordered by Estimated Frequency)"},
               {"-hypon", HYPOPTR, NOUN, 5, "Hyponyms"},
               {"-treen", -HYPOPTR, NOUN, 6, "Hyponyms"},
               {"-holon", HOLONYM, NOUN, 7, "Holonyms"},
               {"-sprtn", ISPARTPTR, NOUN, 7, "Part Holonyms"},
               {"-smemn", ISMEMBERPTR, NOUN, 7, "Member Holonyms"},
               {"-ssubn", ISSTUFFPTR, NOUN, 7, "Substance Holonyms"},
               {"-hholn", -HHOLONYM, NOUN, 8, "Holonyms"},
               {"-meron", MERONYM, NOUN, 9, "Meronyms"},
               {"-subsn", HASSTUFFPTR, NOUN, 9, "Substance Meronyms"},
               {"-partn", HASPARTPTR, NOUN, 9, "Part Meronyms"},
               {"-membn", HASMEMBERPTR, NOUN, 9, "Member Meronyms"},
               {"-hmern", -HMERONYM, NOUN, 10, "Meronyms"},
               {"-nomnn", DERIVATION, NOUN, 11, "Derived Forms"},
               {"-derin", DERIVATION, NOUN, 11, "Derived Forms"},
               {"-domnn", CLASSIFICATION, NOUN, 13, "Domain"},
               {"-domtn", CLASS, NOUN, 14, "Domain Terms"},
               {"-attrn", ATTRIBUTE, NOUN, 12, "Attributes"},
               {"-famln", FREQ, NOUN, 15, "Familiarity"},
               {"-grepn", WNGREP, NOUN, 16, "Grep"},

               {"-synsv", HYPERPTR, VERB, 0, "Synonyms/Hypernyms (Ordered by Estimated Frequency)"},
               {"-simsv", RELATIVES, VERB, 1, "Synonyms (Grouped by Similarity of Meaning)"},
               {"-antsv", ANTPTR, VERB, 2, "Antonyms"},
               {"-coorv", COORDS, VERB, 3, "Coordinate Terms (sisters)"},
               {"-hypev", -HYPERPTR, VERB, 4, "Synonyms/Hypernyms (Ordered by Estimated Frequency)"},
               {"-hypov", HYPOPTR, VERB, 5, "Troponyms (hyponyms)"},
               {"-treev", -HYPOPTR, VERB, 5, "Troponyms (hyponyms)"},
               {"-tropv", -HYPOPTR, VERB, 5, "Troponyms (hyponyms)"},
               {"-entav", ENTAILPTR, VERB, 6, "Entailment"},
               {"-causv", CAUSETO, VERB, 7, "\'Cause To\'"},
               {"-nomnv", DERIVATION, VERB, 8, "Derived Forms"},
               {"-deriv", DERIVATION, VERB, 8, "Derived Forms"},
               {"-domnv", CLASSIFICATION, VERB, 10, "Domain"},
               {"-domtv", CLASS, VERB, 11, "Domain Terms"},
               {"-framv", FRAMES, VERB, 9, "Sample Sentences"},
               {"-famlv", FREQ, VERB, 12, "Familiarity"},
               {"-grepv", WNGREP, VERB, 13, "Grep"},

               {"-synsr", SYNS, ADV, 0, "Synonyms"},
               {"-antsr", ANTPTR, ADV, 1, "Antonyms"},
               {"-pertr", PERTPTR, ADV, 0, "Pertainyms"},
               {"-domnr", CLASSIFICATION, ADV, 2, "Domain"},
               {"-domtr", CLASS, ADV, 3, "Domain Terms"},
               {"-famlr", FREQ, ADV, 4, "Familiarity"},
               {"-grepr", WNGREP, ADV, 5, "Grep"},

               {"-over", OVERVIEW, ALL_POS, -1, "Overview"},
               {NULL, 0, 0, 0, NULL}};

struct
{
  char *template; /* template for generic search message */
  char *option;   /* text for help message */
  char *helpstr;
} searchstr[] = {
    /* index by search type type */
    {NULL, NULL, NULL},
    {
        "-ants%c",
        "-ants{n|v|a|r}",
        "\t\tAntonyms",
    },
    {
        "-hype%c",
        "-hype{n|v}",
        "\t\tHypernyms",
    },
    {
        "-hypo%c, -tree%c",
        "-hypo{n|v}, -tree{n|v}",
        "\tHyponyms & Hyponym Tree",
    },
    {
        "-enta%c",
        "-entav\t",
        "\t\tVerb Entailment",
    },
    {
        "-syns%c",
        "-syns{n|v|a|r}",
        "\t\tSynonyms (ordered by estimated frequency)",
    },
    {
        "-smem%c",
        "-smemn\t",
        "\t\tMember of Holonyms",
    },
    {
        "-ssub%c",
        "-ssubn\t",
        "\t\tSubstance of Holonyms",
    },
    {
        "-sprt%c",
        "-sprtn\t",
        "\t\tPart of Holonyms",
    },
    {
        "-memb%c",
        "-membn\t",
        "\t\tHas Member Meronyms",
    },
    {
        "-subs%c",
        "-subsn\t",
        "\t\tHas Substance Meronyms",
    },
    {
        "-part%c",
        "-partn\t",
        "\t\tHas Part Meronyms",
    },
    {
        "-mero%c",
        "-meron\t",
        "\t\tAll Meronyms",
    },
    {
        "-holo%c",
        "-holon\t",
        "\t\tAll Holonyms",
    },
    {
        "-caus%c",
        "-causv\t",
        "\t\tCause to",
    },
    {NULL, NULL, NULL}, /* PPLPTR - no specific search */
    {NULL, NULL, NULL}, /* SEEALSOPTR - no specific search */
    {
        "-pert%c",
        "-pert{a|r}",
        "\t\tPertainyms",
    },
    {
        "-attr%c",
        "-attr{n|a}",
        "\t\tAttributes",
    },
    {NULL, NULL, NULL}, /* verb groups - no specific pointer */
    {
        "-deri%c",
        "-deri{n|v}",
        "\t\tDerived Forms",
    },
    {"-domn%c", "-domn{n|v|a|r}", "\t\tDomain"},
    {"-domt%c", "-domt{n|v|a|r}", "\t\tDomain Terms"},
    {NULL, NULL, NULL}, /* SYNS - taken care of with SIMPTR */
    {
        "-faml%c",
        "-faml{n|v|a|r}",
        "\t\tFamiliarity & Polysemy Count",
    },
    {
        "-fram%c",
        "-framv\t",
        "\t\tVerb Frames",
    },
    {
        "-coor%c",
        "-coor{n|v}",
        "\t\tCoordinate Terms (sisters)",
    },
    {
        "-sims%c",
        "-simsv\t",
        "\t\tSynonyms (grouped by similarity of meaning)",
    },
    {
        "-hmer%c",
        "-hmern\t",
        "\t\tHierarchical Meronyms",
    },
    {"-hhol%c", "-hholn\t", "\t\tHierarchical Holonyms"},
    {"-grep%c", "-grep{n|v|a|r}", "\t\tList of Compound Words"},
    {"-over", "-over\t", "\t\tOverview of Senses"},
};

// 函数声明
static int getoptidx(char *), cmdopt(char *);
static int searchwn(int, char *[]);
static int do_search(char *, int, int, int, char *);
static int do_is_defined(char *);
static void printusage(), printlicense(), printsearches(char *, int, unsigned long);
static int error_message(char *);

int main(int argc, char *argv[])
{
  display_message = error_message;

  // 参数数量，如果只有一个，则为只输入程序名，输出使用说明
  if (argc < 2)
  {
    printusage();
    exit(-1);
  }

  // 第二个参数是-l，打印license
  else if (argc == 2 && !strcmp("-l", argv[1]))
  {
    printlicense();
    exit(-1);
  }

  // 初始化数据，判断初始化结果，
  // C语言规范，返回0则为成功，非0为出错
  if (wninit())
  { /* open database */
    display_message("wn: Fatal error - cannot open WordNet database\n");
    exit(-1);
  }

  // 执行搜索，然后退出
  exit(searchwn(argc, argv));
}

static int searchwn(int ac, char *av[])
{
  int i, j = 1, pos;
  int whichsense = ALLSENSES, help = 0;
  int errcount = 0, outsenses = 0;
  char tmpbuf[256]; /* buffer for constuction error messages */

  if (ac == 2) /* print available searches for word */
    exit(do_is_defined(av[1]));

  /* Parse command line options once and set flags */

  dflag = fileinfoflag = offsetflag = wnsnsflag = 0;

  // 遍历每一个参数，解析其含义，因为参数的顺序是随机的，所以二重循环
  for (i = 1; i < ac; i++)
  {
    if (!strcmp("-g", av[i]))
      // 注释
      dflag++;
    else if (!strcmp("-h", av[i]))
      // 帮助信息
      help++;
    else if (!strcmp("-l", av[i]))
      printlicense();
    else if (!strncmp("-n", av[i], 2) && strncmp("-nomn", av[i], 5))
      // 只显示某个意思
      whichsense = atoi(av[i] + 2); 
    else if (!strcmp("-a", av[i]))
      // 显示文件信息
      fileinfoflag = 1;
    else if (!strcmp("-o", av[i]))
      // 显示同义词
      offsetflag = 1;
    else if (!strcmp("-s", av[i]))
      // 在同义词中显示序号
      wnsnsflag = 1;
  }

  /* Replace spaces with underscores before looking in database */
  // 把单词中的空格替换为下划线，便于处理
  // 所以支持词组
  // 输入的方式是需要用到转义字符\\后面接空格
  strtolower(strsubst(av[1], ' ', '_'));

  /* Look at each option in turn.  If it's not a command line option
     (which was processed earlier), perform the search requested. */

  while (av[++j])
  {
    if (!cmdopt(av[j]))
    { /* not a command line option */
      if ((i = getoptidx(av[j])) != -1)
      {
        optptr = &optlist[i];

        /* print help text before search output */
        if (help && optptr->helpmsgidx >= 0)
          printf("%s\n", helptext[optptr->pos][optptr->helpmsgidx]);

        if (optptr->pos == ALL_POS)
          for (pos = 1; pos <= NUMPARTS; pos++)
            outsenses += do_search(av[1], pos, optptr->search,
                                   whichsense, optptr->label);
        else
          outsenses += do_search(av[1], optptr->pos, optptr->search,
                                 whichsense, optptr->label);
      }
      else
      {
        sprintf(tmpbuf, "wn: invalid search option: %s\n", av[j]);
        display_message(tmpbuf);
        errcount++;
      }
    }
  }
  return (errcount ? -errcount : outsenses);
}

static int do_search(char *searchword, int pos, int search, int whichsense,
                     char *label)
{
  int totsenses = 0;
  char *morphword, *outbuf;

  outbuf = findtheinfo(searchword, pos, search, whichsense);
  totsenses += wnresults.printcnt;
  if (strlen(outbuf) > 0)
    printf("\n%s of %s %s\n%s",
           label, partnames[pos], searchword, outbuf);

  if (morphword = morphstr(searchword, pos))
    do
    {
      outbuf = findtheinfo(morphword, pos, search, whichsense);
      totsenses += wnresults.printcnt;
      if (strlen(outbuf) > 0)
        printf("\n%s of %s %s\n%s",
               label, partnames[pos], morphword, outbuf);
    } while (morphword = morphstr(NULL, pos));

  return (totsenses);
}

static int do_is_defined(char *searchword)
{
  int i, found = 0;
  unsigned int search;
  char *morphword;

  if (searchword[0] == '-')
  {
    display_message("wn: invalid search word\n");
    return (-1);
  }

  /* Print all valid searches for word in all parts of speech */

  strtolower(strsubst(searchword, ' ', '_'));

  for (i = 1; i <= NUMPARTS; i++)
  {
    if ((search = is_defined(searchword, i)) != 0)
    {
      printsearches(searchword, i, search);
      found = 1;
    }
    else
      printf("\nNo information available for %s %s\n",
             partnames[i], searchword);

    if ((morphword = morphstr(searchword, i)) != NULL)
      do
      {
        if ((search = is_defined(morphword, i)) != 0)
        {
          printsearches(morphword, i, search);
          found = 1;
        }
        else
          printf("\nNo information available for %s %s\n",
                 partnames[i], morphword);
      } while ((morphword = morphstr(NULL, i)) != NULL);
  }
  return (found);
}

static void printsearches(char *word, int dbase, unsigned long search)
{
  int j;

  printf("\nInformation available for %s %s\n", partnames[dbase], word);
  for (j = 1; j <= MAXSEARCH; j++)
    if ((search & bit(j)) && searchstr[j].option)
    {
      printf("\t");
      printf(searchstr[j].template,
             partchars[dbase], partchars[dbase]);
      printf(searchstr[j].helpstr);
      printf("\n");
    }
}

static void printusage()
{
  int i;

  fprintf(stdout,
          "\nusage: wn word [-hgla] [-n#] -searchtype [-searchtype...]\n");
  fprintf(stdout, "       wn [-l]\n\n");
  fprintf(stdout, "\t-h\t\tDisplay help text before search output\n");
  fprintf(stdout, "\t-g\t\tDisplay gloss\n");
  fprintf(stdout, "\t-l\t\tDisplay license and copyright notice\n");
  fprintf(stdout, "\t-a\t\tDisplay lexicographer file information\n");
  fprintf(stdout, "\t-o\t\tDisplay synset offset\n");
  fprintf(stdout, "\t-s\t\tDisplay sense numbers in synsets\n");
  fprintf(stdout, "\t-n#\t\tSearch only sense number #\n");
  fprintf(stdout, "\nsearchtype is at least one of the following:\n");

  for (i = 1; i <= OVERVIEW; i++)
    if (searchstr[i].option)
      fprintf(stdout, "\t%s%s\n",
              searchstr[i].option, searchstr[i].helpstr);
}

static void printlicense()
{
  printf("WordNet Release %s\n\n%s", wnrelease, license);
}

static int cmdopt(char *str)
{

  if (!strcmp("-g", str) ||
      !strcmp("-h", str) ||
      !strcmp("-o", str) ||
      !strcmp("-l", str) ||
      !strcmp("-a", str) ||
      !strcmp("-s", str) ||
      (!strncmp("-n", str, 2) && strncmp("-nomn", str, 5)))

    return (1);
  else
    return (0);
}

static int getoptidx(char *searchtype)
{
  int i;

  for (i = 0; optlist[i].option; i++)
    if (!strcmp(optlist[i].option, searchtype))
      return (i);

  return (-1);
}

static int error_message(char *msg)
{
  fprintf(stderr, msg);
  return (0);
}

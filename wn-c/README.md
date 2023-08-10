# WordNet-WN

此项目来源于 [普林斯顿大学 WordNet 项目](https://wordnet.princeton.edu/) 一个英语词典数据库

原项目包含两个程序，wn 和 wnb, wn是命令行中的程序，wnb是wn browser的意思，是视窗程序。

本仓库中只保留了wn程序的C语言代码，去掉了wnb和项目构建的基础代码，因为那些代码不太掌握，只保留核心程序便于学习。

wnb需要tcl/tk，依赖8.5版本的，新版的8.6不兼容，8.5版本的又找不到。

## 本项目构建方法

```shell
$ clang *.c


$ ./a.out

usage: wn word [-hgla] [-n#] -searchtype [-searchtype...]
       wn [-l]

      -h                        Display help text before search output
      -g                        Display gloss
      -l                        Display license and copyright notice
      -a                        Display lexicographer file information
      -o                        Display synset offset
      -s                        Display sense numbers in synsets
      -n#                       Search only sense number #

searchtype is at least one of the following:
      -ants{n|v|a|r}            Antonyms
      -hype{n|v}                Hypernyms
      -hypo{n|v}, -tree{n|v}    Hyponyms & Hyponym Tree
      -entav                    Verb Entailment
      -syns{n|v|a|r}            Synonyms (ordered by estimated frequency)
      -smemn                    Member of Holonyms
      -ssubn                    Substance of Holonyms
      -sprtn                    Part of Holonyms
      -membn                    Has Member Meronyms
      -subsn                    Has Substance Meronyms
      -partn                    Has Part Meronyms
      -meron                    All Meronyms
      -holon                    All Holonyms
      -causv                    Cause to
      -pert{a|r}                Pertainyms
      -attr{n|a}                Attributes
      -deri{n|v}                Derived Forms
      -domn{n|v|a|r}            Domain
      -domt{n|v|a|r}            Domain Terms
      -faml{n|v|a|r}            Familiarity & Polysemy Count
      -framv                    Verb Frames
      -coor{n|v}                Coordinate Terms (sisters)
      -simsv                    Synonyms (grouped by similarity of meaning)
      -hmern                    Hierarchical Meronyms
      -hholn                    Hierarchical Holonyms
      -grep{n|v|a|r}            List of Compound Words
      -over                     Overview of Senses

```

## 普林斯顿大学 WordNet 项目

源码来自 https://wordnet.princeton.edu/download/current-version

### WordNet 3.0 原项目代码

WordNet 3.0 for UNIX-like systems (including Linux, Mac OS X, Solaris)

https://wordnetcode.princeton.edu/3.0/WordNet-3.0.tar.gz


### WordNet 3.1 DATABASE FILES ONLY

https://wordnetcode.princeton.edu/wn3.1.dict.tar.gz

You can download the WordNet 3.1 database files. Note that this is not a full package as those above, nor does it contain any code for running WordNet. However, you can replace the files in the database directory of your 3.0 local installation with these files and the WordNet interface will run, returning entries from the 3.1 database. This is simply a compressed tar file of the WordNet 3.1 database files.

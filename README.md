ROOTmacros
==========

directory contains all useful ROOT based macros


File description:
graphDraw.C - macro which reads in the ASCII data from file. Data format of the file should be Xvalues<tab>Yvalues. One    can give several files separated by the comma sign. 
  Usage: 
  root -l
  .L graphDraw.C++
  grapher("/path1/file1,/path2/file2,...")

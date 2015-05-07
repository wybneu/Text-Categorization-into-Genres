this programm is developed for nText Categorisation into Genres

running envirnment : UNIX system

linked files : 1 words.txt 
2 anytext sets need to be search include train set and development sets which are in the package by their name
3 class files with the same name of the data sets
This files needs to be in a same catalogue

To compile this files . copy all the file into a catalogue type
                 make
or type following command
                 gcc genres.c -o genres
When it compile, there should be some warning , no problem for that.

To run the program .
         ./genres ./train ./dev 

if will generate two aiff files in the current directionary which is used for weka computing.

all the books in the report could be download from http://www.gutenberg.org/ or /home/subjects/comp90049/local/project2-2012/


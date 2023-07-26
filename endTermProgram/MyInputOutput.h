#pragma once
#define BUFFSIZE 1024

struct Age
{
    int age;
    int population;
    double percentage;
    char name[BUFFSIZE];
    Age* nextAge;
};

struct Year {
    int year;
    double oldpercentage;
    Age* firstAge;
    Year* nextYear;
    Year* previousYear;
};

void getGurrentDirectory(char* currentDirectory);
void inputFile(Year* year1);
void outputFile(Year* year1);
void loadFileData(char* fileName, Year* year1);
void saveFileData(char* fileName, struct Year* year1);
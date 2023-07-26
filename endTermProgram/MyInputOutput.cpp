#include <iostream>
#include <stdio.h>
#include <curses.h>
#include <Windows.h>
#include <stdlib.h>
#include <string.h>

#define BUFFSIZE 1024
#define CHARBUFF 124

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

void getGurrentDirectory(char* currentDirectory) {
    GetCurrentDirectory(CHARBUFF, currentDirectory);
}

void loadFileData(char* fileName, Year* year1)
{
    FILE* fp;
    char s[BUFFSIZE];
    char* p1;

    errno_t error;
    error = fopen_s(&fp, fileName, "r");
    if (error != 0)
        fprintf_s(stderr, "failed to open");
    else {
        fgets(s, BUFFSIZE, fp);
        Year* previousYear = NULL;
        Year* prePreviousYear = NULL;
        for (int i = 0; i < 20; i++) {
            Year* year = new Year();
            year->year = 1920 + (i * 5);
            Age* previousAge = NULL;

            for (int j = 0; j < 19; j++) {
                Age* tmp = new Age();
                fgets(s, BUFFSIZE, fp);
                char delim[] = ", ";//�f���~�^�i�����n����j�����ł̓J���}�Ƌ�
                char* ctx;
                //== �ϊ������i�X�y�[�X��؂�ŕ�������������̐��l�ϊ��j==
                p1 = strtok_s(s, delim, &ctx); // 1�ڂ̕���������擾
                if (j == 0) {
                    tmp->age = -1;
                }
                else {
                    tmp->age = ((j - 1) * 5);
                }

                for (int k = 0; k < 5; k++) {

                    switch (k)
                    {
                    case 4: tmp->population = atoi(p1);
                    }
                    p1 = strtok_s(NULL, delim, &ctx); // 2�ڈȍ~�̕���������擾
                }
                if (j == 0) {
                    year->firstAge = tmp;
                }
                else {
                    previousAge->nextAge = tmp;
                }
                previousAge = tmp;
            }
            if (i == 0) {
                year1->nextYear = year;
                prePreviousYear = year;
            }
            else {
                previousYear->nextYear = year;
                previousYear->previousYear = prePreviousYear;
                prePreviousYear = previousYear;
            }
            previousYear = year;
            if (i == 19) {
                year->previousYear = prePreviousYear;
            }
        }

        fclose(fp);
    }
}

void saveFileData(char* fileName, struct Year* year1) {
    FILE* fp;

    errno_t error;
    error = fopen_s(&fp, fileName, "w");
    if (error != 0) {
        fprintf_s(stderr, "Failed to open file.\n");
        return;
    }
    fprintf(fp, "%s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s\n", "�N", "�����(65�ȏ�)��", "�l���i���l�j", "0��", "10��", "20��", "30��", "40��", "50��", "60��", "70��", "80��ȏ�");

    struct Year* currentYear = year1->nextYear;
    while (currentYear != NULL) {
        fprintf(fp, "%d, %f", currentYear->year, currentYear->oldpercentage);
        struct Age* currentAge = currentYear->firstAge;
        while (currentAge != NULL) {
            fprintf(fp, ",%d", currentAge->population);
            currentAge = currentAge->nextAge;
        }
        fprintf(fp, "\n");
        currentYear = currentYear->nextYear;
    }

    fclose(fp);
}

void inputFile(Year* year1) {
    char currentDirectory[CHARBUFF];
    getGurrentDirectory(currentDirectory);
    char section[CHARBUFF];
    sprintf_s(section, "section1");
    char keyWord[CHARBUFF];
    sprintf_s(keyWord, "file");
    char settingFile[CHARBUFF];
    sprintf_s(settingFile, "%s\\setting.ini", currentDirectory);
    char keyValue[CHARBUFF];
    GetPrivateProfileString(section, keyWord, "none", keyValue, CHARBUFF, settingFile);
    loadFileData(keyValue, year1);
}

void outputFile(Year* year1) {
    char currentDirectory[CHARBUFF];
    getGurrentDirectory(currentDirectory);
    char section[CHARBUFF];
    sprintf_s(section, "section2");
    char keyWord[CHARBUFF];
    sprintf_s(keyWord, "file");
    char settingFile[CHARBUFF];
    sprintf_s(settingFile, "%s\\setting.ini", currentDirectory);
    char keyValue[CHARBUFF];
    GetPrivateProfileString(section, keyWord, "none", keyValue, CHARBUFF, settingFile);
    saveFileData(keyValue, year1);
}


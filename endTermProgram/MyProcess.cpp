#include <iostream>
#include <curses.h>
#include <algorithm>
#include <string>
#include <Windows.h>
#include "MyInputOutput.h"

// �l���̒l�𑫂�
void addPopulation(Age* firstAge)
{
    Age* current = firstAge;
    if (current != nullptr) {
        current = current->nextAge;
    }

    while (current != nullptr && current->nextAge != nullptr) {
        int sum = current->population + current->nextAge->population;
        current->population = sum;

        Age* toDelete = current->nextAge;
        current->nextAge = current->nextAge->nextAge;
        delete toDelete;

        current = current->nextAge;
    }
}

// 5�Ζ��̓��v�̒l��10�Ζ��ɓ�������
void integrateGenerations(Year* firstYear) {

    Year* currentYear = firstYear;

    while (currentYear != nullptr) {
        Age* currentAge = currentYear->firstAge;
        addPopulation(currentAge);
        currentYear = currentYear->nextYear;
    }
}

// ���ʂ��o�͂��� // �f�o�b�N�p
void outputPopulation(Year* firstYear) {

    Year* currentYear = firstYear;
    while (currentYear != nullptr) {
        std::cout << "Year: " << currentYear->year << std::endl;
        Age* currentAge = currentYear->firstAge;
        while (currentAge != nullptr) {
            std::cout << "Age: " << currentAge->age << ", Population: " << currentAge->population << ", Percentage: " << currentAge->percentage << std::endl;
            currentAge = currentAge->nextAge;
        }
        currentYear = currentYear->nextYear;
    }
}

// �e����̐l���������i�[�����Q�����̔z����쐬����
void calculatePercentage(Year* firstYear) {
    Year* currentYear = firstYear;

    while (currentYear != nullptr) {

        Age* currentAge = currentYear->firstAge;
        double sum;
        if (currentAge != nullptr) {

            sum = currentAge->population;

            currentAge = currentAge->nextAge;
        }


        while (currentAge != nullptr) {

            currentAge->percentage = double(currentAge->population / sum);
            currentAge = currentAge->nextAge;

        }
        currentYear = currentYear->nextYear;

    }

}

// ��������v�Z����
void calculateOldPercentage(Year* firstYear) {
    Year* currentYear = firstYear;

    while (currentYear != nullptr) {

        Age* currentAge = currentYear->firstAge;
        double sum;
        if (currentAge != nullptr) {

            sum = currentAge->population;

            currentAge = currentAge->nextAge;
        }

        if (currentAge != nullptr) {

            for (int i = 0; i < 14; i++) {
                currentAge = currentAge->nextAge;
            }

            long add = currentAge->population + currentAge->nextAge->population + currentAge->nextAge->nextAge->population;
            currentYear->oldpercentage = double(add / sum) * 100;
        }

        currentYear = currentYear->nextYear;
    }
}

// �P�ʂ��i���l/%�j�ɕύX 
void changeUnit(Year* firstYear) {

    Year* currentYear = firstYear;
    while (currentYear != nullptr) {

        Age* currentAge = currentYear->firstAge;
        while (currentAge != nullptr) {

            currentAge->population = int(currentAge->population / 10000);
            int temp = int(currentAge->percentage * 1000);
            currentAge->percentage = temp / 10.0;

            currentAge = currentAge->nextAge;
        }
        currentYear = currentYear->nextYear;
    }
}

enum Color {
    COLOR_DEFAULT,
    COLOR_SELECTED1,
    COLOR_SELECTED2
};

// ���̊֐��ȍ~�̏o�͂ɐF���Z�b�g����
void setColor(WINDOW* win, int color) {
    if (color == 1) {
        wattron(win, COLOR_PAIR(COLOR_SELECTED1));
    }
    else if (color == 2) {
        wattron(win, COLOR_PAIR(COLOR_SELECTED2));
    }
}

// �F�̃Z�b�g���~�߂�
void offColor(WINDOW* win, int color) {
    if (color == 1) {
        wattroff(win, COLOR_PAIR(COLOR_SELECTED1));
    }
    else if (color == 2) {
        wattroff(win, COLOR_PAIR(COLOR_SELECTED2));
    }
}
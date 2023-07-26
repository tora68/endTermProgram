#include <iostream>
#include <curses.h>
#include <algorithm>
#include <string>
#include <Windows.h>
#include "MyInputOutput.h"

// 人口の値を足す
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

// 5歳毎の統計の値を10歳毎に統合する
void integrateGenerations(Year* firstYear) {

    Year* currentYear = firstYear;

    while (currentYear != nullptr) {
        Age* currentAge = currentYear->firstAge;
        addPopulation(currentAge);
        currentYear = currentYear->nextYear;
    }
}

// 結果を出力する // デバック用
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

// 各世代の人口割合を格納した２次元の配列を作成する
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

// 高齢化率を計算する
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

// 単位を（万人/%）に変更 
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

// この関数以降の出力に色をセットする
void setColor(WINDOW* win, int color) {
    if (color == 1) {
        wattron(win, COLOR_PAIR(COLOR_SELECTED1));
    }
    else if (color == 2) {
        wattron(win, COLOR_PAIR(COLOR_SELECTED2));
    }
}

// 色のセットを止める
void offColor(WINDOW* win, int color) {
    if (color == 1) {
        wattroff(win, COLOR_PAIR(COLOR_SELECTED1));
    }
    else if (color == 2) {
        wattroff(win, COLOR_PAIR(COLOR_SELECTED2));
    }
}
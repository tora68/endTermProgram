#include <iostream>
#include <curses.h>
#include <algorithm>
#include <string>
#include <Windows.h>
#include "MyInputOutput.h"
#include "MyProcess.h"

void drawBarGraph(WINDOW* win, Year* year, int x, int y) {

    int maxData = 2200;
    int graphHeight = getmaxy(win) - y - 3;
    int barWidth = 4;
    int spaceWidth = 3;
    x -= 20;

    // �g���
    box(win, '|', '-');
    mvwprintw(win, 1, 1, "%3d(���l)", maxData);
    mvwprintw(win, 15, 1, "1000(���l)");
    mvwprintw(win, graphHeight, 8, "0");
    mvwvline(win, 1, 11, '|', graphHeight - 1);
    mvwhline(win, graphHeight, 11, '-', x + getmaxx(win) - 30);
    mvwprintw(win, 0, 30, "%4d�N-����ʐl��", year->year);

    Age* age = year->firstAge;
    age = age->nextAge;
    for (int i = 0; i < 9; ++i) {
        int barHeight = (age->population * graphHeight - 1) / (maxData + 10);

        // �_�̕`��
        for (int j = 0; j < barHeight; ++j) {
            for (int k = 0; k < barWidth; ++k) {
                mvwaddch(win, y + graphHeight - j - 1, x + (barWidth + spaceWidth) * i + k, ACS_BLOCK);
            }

        }

        // �l�̕`��
        mvwprintw(win, y + graphHeight - barHeight - 2, x + (barWidth + spaceWidth) * i, "%4d", age->population);
        mvwprintw(win, y + graphHeight - barHeight - 1, x + (barWidth + spaceWidth) * i, "%4.1f", age->percentage);
        mvwprintw(win, y + graphHeight - barHeight - 1, x + (barWidth + spaceWidth) * i + 4, "��");

        // �ڐ���̕`��
        mvwprintw(win, y + graphHeight + 1, x + (barWidth + spaceWidth) * i + 1, "%i", age->age);
        mvwprintw(win, y + graphHeight + 1, x + (barWidth + spaceWidth) * i + 3, "��");
        age = age->nextAge;
    }
}

void showTable(WINDOW* win, Year* year, int year_num) {

    int cellWidth = 7;
    int cellHeight = 1;

    // �\�̃w�b�_��\��
    mvwprintw(win, 0, 2, "%s", "�N");
    mvwprintw(win, 0, 5, "%s", "�l��(���l)");
    mvwprintw(win, 0, 15, "%s", "�����(65~)");


    Year* currentYear = year->nextYear;
    int row = 0;
    while (currentYear != nullptr) {

        Age* currentAge = currentYear->firstAge;
        int y = row + 1;

        if (y == year_num) {
            setColor(win, 1);
        }
        // �\�̓��e��\��
        mvwprintw(win, y, 0, "%-*i", cellWidth, currentYear->year);
        mvwprintw(win, y, 1 * (cellWidth + 1), "%*5d", cellWidth, currentAge->population);
        mvwprintw(win, y, 2 * (cellWidth + 1), "%*.1f", cellWidth, currentYear->oldpercentage);
        if (y == year_num) {
            offColor(win, 1);
        }

        currentYear = currentYear->nextYear;
        row++;
    }

}

void showGraph(Year* year) {
    int mode = 2; // �\�����[�h�i1�F�O���t�𓮂����A�Q�F�Î~�O���t�j
    int year_num = 1; // �\�Ɩ_�O���t�ŎQ�Ƃ��Ă���N����v�����邽�߂̕ϐ�

    initscr();
    cbreak();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);

    start_color();
    init_pair(COLOR_SELECTED1, COLOR_GREEN, COLOR_BLACK);

    // window�̃T�C�Y���w��
    int startX = 35;
    int startY = 0;
    int height = 30;
    int width = 83;

    int startX2 = 0;
    int startY2 = 0;
    int height2 = 30;
    int width2 = 30;

    WINDOW* win = newwin(height, width, startY, startX);
    WINDOW* win2 = newwin(height2, width2, startY2, startX2);

    refresh();

    Year* currentYear = year->nextYear;

    while (true) {

        // �O���t�𓮂���
        if (mode == 1) 
        {

            if (currentYear->nextYear != NULL) {
                werase(win);  // �E�B���h�E���N���A
                werase(win2);  // �E�B���h�E���N���A
                drawBarGraph(win, currentYear, startX, startY); // �_�O���t�̕`��
                showTable(win2, year, year_num); // �\�̕`��
                wrefresh(win);
                wrefresh(win2);
                napms(500);
                currentYear = currentYear->nextYear;
                year_num++;
            }
            else {
                werase(win);  // �E�B���h�E���N���A
                werase(win2);  // �E�B���h�E���N���A
                drawBarGraph(win, currentYear, startX, startY);
                showTable(win2, year, year_num);
                wrefresh(win);
                wrefresh(win2);
                mode = 2;
            }

            // �L�[���͏���
            int key = getch();
            if (key == 'a') {
                mode = 2;
            }
            else if (key == 'b') {
                currentYear = year->nextYear;
                year_num = 1;

            }
        }
        else if (mode == 2) // �Î~�O���t
        { 
            werase(win);  // �E�B���h�E���N���A
            werase(win2);  // �E�B���h�E���N���A
            drawBarGraph(win, currentYear, startX, startY);
            showTable(win2, year, year_num);
            wrefresh(win);
            wrefresh(win2);


            // �L�[���͏���
            int key = getch();
            if (key == KEY_UP) {
                if (currentYear->previousYear != NULL) {
                    currentYear = currentYear->previousYear;
                    year_num--;

                }
            }
            else if (key == KEY_DOWN) {
                if (currentYear->nextYear != NULL) {
                    currentYear = currentYear->nextYear;
                    year_num++;
                }
            }
            else if (key == 'a') {
                mode = 1;
            }
            else if (key == 'b') {
                currentYear = year->nextYear;
                year_num = 1;
            }

        }

    }

    endwin();
}


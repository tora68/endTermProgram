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

    // 枠作り
    box(win, '|', '-');
    mvwprintw(win, 1, 1, "%3d(万人)", maxData);
    mvwprintw(win, 15, 1, "1000(万人)");
    mvwprintw(win, graphHeight, 8, "0");
    mvwvline(win, 1, 11, '|', graphHeight - 1);
    mvwhline(win, graphHeight, 11, '-', x + getmaxx(win) - 30);
    mvwprintw(win, 0, 30, "%4d年-世代別人口", year->year);

    Age* age = year->firstAge;
    age = age->nextAge;
    for (int i = 0; i < 9; ++i) {
        int barHeight = (age->population * graphHeight - 1) / (maxData + 10);

        // 棒の描画
        for (int j = 0; j < barHeight; ++j) {
            for (int k = 0; k < barWidth; ++k) {
                mvwaddch(win, y + graphHeight - j - 1, x + (barWidth + spaceWidth) * i + k, ACS_BLOCK);
            }

        }

        // 値の描画
        mvwprintw(win, y + graphHeight - barHeight - 2, x + (barWidth + spaceWidth) * i, "%4d", age->population);
        mvwprintw(win, y + graphHeight - barHeight - 1, x + (barWidth + spaceWidth) * i, "%4.1f", age->percentage);
        mvwprintw(win, y + graphHeight - barHeight - 1, x + (barWidth + spaceWidth) * i + 4, "％");

        // 目盛りの描画
        mvwprintw(win, y + graphHeight + 1, x + (barWidth + spaceWidth) * i + 1, "%i", age->age);
        mvwprintw(win, y + graphHeight + 1, x + (barWidth + spaceWidth) * i + 3, "代");
        age = age->nextAge;
    }
}

void showTable(WINDOW* win, Year* year, int year_num) {

    int cellWidth = 7;
    int cellHeight = 1;

    // 表のヘッダを表示
    mvwprintw(win, 0, 2, "%s", "年");
    mvwprintw(win, 0, 5, "%s", "人口(万人)");
    mvwprintw(win, 0, 15, "%s", "高齢化率(65~)");


    Year* currentYear = year->nextYear;
    int row = 0;
    while (currentYear != nullptr) {

        Age* currentAge = currentYear->firstAge;
        int y = row + 1;

        if (y == year_num) {
            setColor(win, 1);
        }
        // 表の内容を表示
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
    int mode = 2; // 表示モード（1：グラフを動かす、２：静止グラフ）
    int year_num = 1; // 表と棒グラフで参照している年を一致させるための変数

    initscr();
    cbreak();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);

    start_color();
    init_pair(COLOR_SELECTED1, COLOR_GREEN, COLOR_BLACK);

    // windowのサイズを指定
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

        // グラフを動かす
        if (mode == 1) 
        {

            if (currentYear->nextYear != NULL) {
                werase(win);  // ウィンドウをクリア
                werase(win2);  // ウィンドウをクリア
                drawBarGraph(win, currentYear, startX, startY); // 棒グラフの描画
                showTable(win2, year, year_num); // 表の描画
                wrefresh(win);
                wrefresh(win2);
                napms(500);
                currentYear = currentYear->nextYear;
                year_num++;
            }
            else {
                werase(win);  // ウィンドウをクリア
                werase(win2);  // ウィンドウをクリア
                drawBarGraph(win, currentYear, startX, startY);
                showTable(win2, year, year_num);
                wrefresh(win);
                wrefresh(win2);
                mode = 2;
            }

            // キー入力処理
            int key = getch();
            if (key == 'a') {
                mode = 2;
            }
            else if (key == 'b') {
                currentYear = year->nextYear;
                year_num = 1;

            }
        }
        else if (mode == 2) // 静止グラフ
        { 
            werase(win);  // ウィンドウをクリア
            werase(win2);  // ウィンドウをクリア
            drawBarGraph(win, currentYear, startX, startY);
            showTable(win2, year, year_num);
            wrefresh(win);
            wrefresh(win2);


            // キー入力処理
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


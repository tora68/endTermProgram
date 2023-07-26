#include <iostream>
#include <curses.h>
#include <algorithm>
#include <string>
#include <vector>
#include <cmath>
#include "MyInputOutput.h"
#include "MyProcess.h"
#include "MyGraph.h"
#include <Windows.h>



int main(int argc, char* argv[])
{
    Year* year1 = new Year();
    inputFile(year1);

    // 高齢化率を計算
    calculateOldPercentage(year1);

    // 構造体の中身を10歳毎の人口に変更
    integrateGenerations(year1);

    Year* currentYear = year1;

    // それぞれの世代の人口割合を計算
    calculatePercentage(currentYear);

    // 単位の変更
    changeUnit(currentYear);

/*
    //デバック
    outputPopulation(currentYear);
*/

    // ファイル出力
    outputFile(year1);

    // グラフの表示
    showGraph(year1);

    return 0;
}

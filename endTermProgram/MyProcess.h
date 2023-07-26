#pragma 
enum Color {
    COLOR_DEFAULT,
    COLOR_SELECTED1,
    COLOR_SELECTED2
};

void setColor(WINDOW* win, int color);
void offColor(WINDOW* win, int color);
void addPopulation(Age* firstAge);
void integrateGenerations(Year* firstYear);
void outputPopulation(Year* firstYear);
void calculatePercentage(Year* firstYear);
void calculateOldPercentage(Year* firstYear);
void changeUnit(Year* firstYear);

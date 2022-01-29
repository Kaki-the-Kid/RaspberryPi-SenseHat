
#include <SenseHat.h>
#include <string>
#include <iostream>
#include <ncurses.h>

void make_vertical_line(WINDOW *win, int x, int yStart, int yStop, char ch)
{
    for(int i = yStart; i <= yStop; i++)
    {
        wmove(win, i, x);
        waddch(win, ch);
    }

}


int main()
{
    SenseHat carte;

    float pression;
    float temperature;
    float humidite;

    initscr();
    cbreak();
    noecho();
    refresh();      // Important to refresh screen before refresh window


    WINDOW* win = newwin(24, 80, 0, 0); //sætter vinduets størelse


    keypad(win, TRUE);	//Giver adgang til f. eks. pil op og ned på tastatur

    start_color(); //valg af farver
    init_pair(2, COLOR_WHITE, COLOR_BLACK);
    wattron(win, COLOR_PAIR(2));



    wborder(win, '+','+','+','+','+','+','+','+');		//window border

    wmove(win, 3, 12);
    waddstr(win, "Get Temp");		//cursor position 0

    wmove(win, 5, 12);
    waddstr(win, "Get Pressure");		//cursor position 1

    wmove(win, 7, 12);
    waddstr(win, "Get moistness");       //cursor position 2






    curs_set(1);
    wmove(win, 3, 10);  // window, y, x

    wrefresh(win);		// Refresh window


    int cursor_position = 0; //curser position
    int menu_entries = 3; //antal meny'er

    while(1)
    {
        //henter barometrisk data
        pression    = carte.ObtenirPression();
        temperature = carte.ObtenirTemperature();
        humidite    = carte.ObtenirHumidite();


        int key = wgetch(win);

        if(key == 'q') break; //her kan du afslutte koden

        switch(key)
        {
            case KEY_UP : //flytter curseren en position op
                if(cursor_position - 1 >= 0) --cursor_position;
                break;


            case KEY_DOWN : //flytter curseren en position ned
                if(cursor_position + 1 < menu_entries) ++cursor_position;
                break;


            case KEY_LEFT : //tilbage til hovedmenu
                wclear(win);
                wborder(win, '+','+','+','+','+','+','+','+');

                wmove(win, 3, 12);
                waddstr(win, "Get Temp");

                wmove(win, 5, 12);
                waddstr(win, "Get Pressure");

                wmove(win, 7, 12);
                waddstr(win, "Get moistness");
            break;


            case KEY_RIGHT ://lader dig gå ind i menu'erne

            	wclear(win);

            	if(cursor_position == 0)
            	{
                    wborder(win, '+','+','+','+','+','+','+','+');
            		wmove(win, 5, 12);//cursor position 0
    				wprintw(win, "Temp %.2f C", temperature);
            	}

            	else if(cursor_position == 1)
            	{
                    wborder(win, '+','+','+','+','+','+','+','+');
            		wmove(win, 5, 12);//cursor position 1
    				wprintw(win, "Tryk %.1f hPa", pression);
            	}

            	else if(cursor_position == 2)
            	{
                    wborder(win, '+','+','+','+','+','+','+','+');
            		wmove(win, 5, 12);//cursor position 2
    				wprintw(win, "Hum %.1f%%", humidite);
            	}

            	wrefresh(win);		// Refresh window
            	break;

            default :
                break;
        }

        wmove(win, 3 + cursor_position * 2, 10); //sætter curser position udenfor valgte menu

    }

    delwin(win);
    endwin();

    return 0;
}

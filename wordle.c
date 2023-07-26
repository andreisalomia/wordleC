#include <ncurses.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

int randomgenerator()
{//calculeaza in variabila value un numar random de la 1 la 30 folosind srand si time
    int value;
    time_t t;
    srand(time(&t));
    value=t%30;
    return value;
}

void culori(char *v, char **cuvinte, int random, int j)
{
    int i,ok1,ok2,ok3;
    int y=j,x=1;
    init_pair(2,COLOR_WHITE,COLOR_GREEN);
    init_pair(3,COLOR_WHITE,COLOR_BLACK); //initializam culorile folosite
    init_pair(4,COLOR_WHITE,COLOR_YELLOW);
    char cuvant[5];
    strcpy(cuvant,cuvinte[random]); //copiem cuvantul cheie ca sa coloram litera cu litera
    for(i=0;i<5;i++)
    {
        ok1=0; ok2=0; ok3=0;
        if(cuvant[i]==v[i])
            ok1=1; //ok1 e responsabil pentru litere verzi
        if(ok1==0)
        {
            for(j=0;j<5;j++)
                if(cuvant[j]==v[i])
                    ok2=1; //ok2 coloreaza cu galben daca gasim litera undeva in cuvant
        }
        if(ok1==0&&ok2==0)
            ok3=1; //daca nu gasim litera in cuvant ok3 coloreaza cu negru
        if(ok1==1)
            {
                attron(COLOR_PAIR(2));
                mvaddch(y, x, v[i]);
                attroff(COLOR_PAIR(2));
                x++;
            }
        if(ok2==1)
            {
                attron(COLOR_PAIR(4));
                mvaddch(y, x, v[i]);
                attroff(COLOR_PAIR(4));
                x++;
            }
        if(ok3==1)
            {
                attron(COLOR_PAIR(3));
                mvaddch(y, x, v[i]);
                attroff(COLOR_PAIR(3));
                x++;
            }                

    }
}

void pierdut(char **cuvinte, int random, WINDOW *tabel)
{ //mesaj de joc pierdut si afisarea cuvantului cautat
    mvprintw(8,0,"Ai pierdut serifule. Cuvantul era: %s.", cuvinte[random]);
}

void castig(int *j, WINDOW *tabel, int *random, int *check)
{
    int ch,x,y;
    init_pair(1, COLOR_WHITE, COLOR_BLUE);
    mvprintw(8,0,"Ai castigat rege. Mai bagi unu? y/n");
    ch=getch();
    if(ch=='n')
        {
            *j=7; //iesim din fereastra pe axa OY pentru a forta jocul sa se opreasca
            *check=0; //variabila in care retinem daca vrem sa iesim chiar la apasarea tastei "n" sau asteptam input
        }
    if(ch=='y')
    {
        (*j)=0; //restartam pozitia
        wrefresh(tabel);
        for(y=1;y<=6;y++)
            for(x=1;x<=5;x++)
            {
                attron(COLOR_PAIR(1));
                mvaddch(y, x, ' ');
                attroff(COLOR_PAIR(1)); //resetam toate caracterele completate cu cate un spatiu
                wrefresh(tabel);
            }
        move(8,0); //stergem mesajul de castig
        clrtoeol();
        refresh();
        move(1,1);
        wrefresh(tabel);
        *random=randomgenerator(); //alegem un nou cuvant
    }


}

void meniu(int *j, WINDOW *tabel, int *random, int *check)
{
    int ch,y,x;
    init_pair(5,COLOR_BLACK,COLOR_MAGENTA);
    WINDOW *panou=newwin(3,20,10,0); //cream un nou window pentru meniu
    box(panou,0,0);
    wbkgd(panou,COLOR_PAIR(5));
    attron(COLOR_PAIR(5));
    mvprintw(11,1,"F1-Restart F2-Quit"); //afisam mesajul in fereastra
    attroff(COLOR_PAIR(5));
    keypad(panou,TRUE);
    wrefresh(panou);
    ch=getch();
    if(ch==265)
    { //daca apasam F1
        *j=1;
        for(y=1;y<=6;y++)
            for(x=1;x<=5;x++)
            { //folosim metoda de mai sus
                attron(COLOR_PAIR(1));
                mvaddch(y, x, ' ');
                attroff(COLOR_PAIR(1));
                wrefresh(tabel);
            }
        move(1,1);
        refresh();
    }
    if(ch==266)
    { //daca apasam F2
        *j=7; //iesim fortat din while-ul jocului
        *check=0;
    }
    move(10,0); //stergem liniile pe care am format fereastra
    clrtoeol();
    move(11,0);
    clrtoeol();
    move(12,0);
    clrtoeol();
    refresh();
}


int main()
{
    int i = 1, j = 1, ok, eroare1, check=1;
    int ch;
    char *cuvinte[]={"baros", "arici" "atent", //29 cuvinte
"baiat", "ceata", "debut", "peste", "fixat", "hamac", "harta", "jalon", "jucam", "lacat", "magie", "nufar", "oaste", "perus",
"rigle", "roman", "sanie", "scris", "sonda", "texte", "tipar", "titan", "zebra", "vapor", "vatra", "sulea"};
    int contor=0;
    char v[5]; //in vectorul asta formez cuvantul scris la fiecare try ca sa il compar
    int random=randomgenerator(); //initializam un numar random de la 1 la 30
    initscr();
    noecho();
    start_color();
    keypad(stdscr, TRUE);
    WINDOW *tabel = newwin(8, 7, 0, 0); //fereastra in care introducem cuvintele
    init_pair(1, COLOR_WHITE, COLOR_BLUE);
    refresh();
    box(tabel, 0, 0); 
    wbkgd(tabel, COLOR_PAIR(1)); //colorez fereastra jocului
    mvwprintw(tabel, 0, 0, "Wordle!"); //afisam numele jocului
    keypad(tabel, TRUE);
    wrefresh(tabel);
    //j-ul va fi axa OY si i va fi axa OX
    while (j <= 6) //cat timp mai avem incercari
    {
        ok = 1;
        ch = getch();

        switch (ch)
        {
        case 10: // (10==enter)
        {
            if (i == 6)
            {
                if (j < 6)
                    move(j + 1, 1); //muta cursorul pentru urmatoarea incercare
                culori(v,cuvinte,random,j); //coloreaza literele
                if(strcmp(cuvinte[random],v)==0) //daca ghicim cuvantul atunci functia castig stabileste play again sau quit
                {
                    castig(&j,tabel,&random,&check);
                }
                j++; //ne mutam pe urmatorul rand
                contor=0; //resetam vectorul in care retinem cuvantul curent
                if(j==7)
                { //daca pe axa OY sarim de 6 inseamna ca nu mai avem incercari si afisam mesajul de pierdere
                    pierdut(cuvinte,random,tabel);
                }
                i = 1;
            }
            else
            { //daca nu avem un cuvant complet afisam un mesaj de eroare in afara ferestrei
                mvprintw(8, 0, "NU sunt destule litere!");
                eroare1 = 1;
                refresh();
            }
            ok = 0;
            break;
        }
        case KEY_BACKSPACE:
        {
            if (i > 1) //conditie ca sa nu stergem outline ul ferestrei
            {
                attron(COLOR_PAIR(1));
                mvaddch(j, i - 1, ' '); //umplu cu un spatiu liber literele sterse si mut cursorul
                move(j, i - 1);
                attroff(COLOR_PAIR(1));
                i--;
                contor--; //scadem cu o pozitie vectorul cuvantului curent ca sa suprascriem ultima litera
            }
            ok = 0;
            break;
        }
        }
        if(ch==':')
        { //meniul de control
            attron(COLOR_PAIR(1));
            mvaddch(j, i , ' '); //sterg ":" din incercarea curenta
            move(j, i);
            attroff(COLOR_PAIR(1));
            refresh();
            wrefresh(tabel);
            meniu(&j,tabel,&random,&check);
            refresh();
            ok=0;

        }
        if (ok == 1)
        {
            if (eroare1 == 1)
            {
                move(8, 0); //sterg mesajul de eroare pentru cuvant incomplet
                clrtoeol();
                refresh();
                eroare1 = 0;
            }
            if (i == 6)
            { //daca cuvantul e complet:
                i = 1;
                contor=0; //mutam cursorul si resetam vectorul de caractere
                j++;
            }
            attron(COLOR_PAIR(1));
            mvaddch(j, i, ch); //adauga cate un caracter
            v[contor]=ch; //formeaza cuvantul curent
            contor++;
            i++;
            attroff(COLOR_PAIR(1));
        }
        refresh();
    }
    if(check==1)
        getch();
    endwin();
    return 0;
}
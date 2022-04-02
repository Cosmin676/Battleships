#include<stdio.h>
#include<ncurses.h>
#include<stdlib.h>
#include<time.h>
#include<unistd.h>

void move_right(int *n, int *highlight_bot_x);
void move_left(int *n, int *highlight_bot_x);
void move_up(int *m, int *highlight_bot_y);
void move_down(int *m, int *highlight_bot_y);
void title();
int **alocare();

int main(int argc, char **argv)
{
	int k=0;
	for(k = 0; k < argc; k++)
		{
			FILE * pFILE;
			pFILE = fopen(argv[k], "r");
			if(pFILE == NULL){
				printf("Fisierul %d nu poate fi deschis]n", k);
			}

		}
	if(argc <= 1)
	{
		printf("Nu s-au dat argumente de comanda!\n");
	}
	else
	{
		initscr();
		noecho();
		cbreak();
		start_color();
		use_default_colors();
		curs_set(false);
		char *meniu_choice[] = {"New Game", "Resume Game", "Quit"};
		char *meniu_choice_resume[] = {"New Game", "Quit"}; 
		int i=0,j=0;

		init_pair(1, COLOR_RED, -1);
		init_pair(2, COLOR_GREEN, -1);
		init_pair(3, COLOR_BLACK, -1);
		init_pair(4, COLOR_CYAN, -1);

		title();

		WINDOW * player_win = newwin(getmaxy(stdscr)-13 ,43, 2 ,0);
		refresh();
		box(player_win,0 ,0);
		wrefresh(player_win);

		WINDOW * bot_win = newwin(getmaxy(stdscr)-13, 43, 2, getmaxx(stdscr)-43);
		refresh();
		box(bot_win, 0,0);
		wrefresh(bot_win);

		WINDOW * meniu_win = newwin(9, getmaxx(stdscr), getmaxy(stdscr)-9, 0);
		refresh();
		box(meniu_win, 0, 0);
		wrefresh(meniu_win);
		keypad(meniu_win, TRUE);
		int highlight = 0 ;
		int ptr = 0;

		int flag = 1;
		int resume_flag = 0;
		int score = 0;
		int win = 20;
		int lost = 20;
		int win_flag = 0;
		int lost_flag = 0;
		int second_flag = 1;
		int another_flag = 0;

		int flag_flag = 0;

		int **mat_map;
		mat_map = alocare();
		int **mat_map_bot;
		mat_map_bot = alocare();
		int **mat_map_resume;
		mat_map_resume = alocare();
		int **mat_map_bot_resume;
		mat_map_bot_resume = alocare();

		while(flag != 0)
		{
			while(second_flag == 1)
			{
				while(1)
				{
					flag_flag = 0;
					wclear(meniu_win);
					box(meniu_win, 0, 0);
					wrefresh(meniu_win);
					for(i = 0; i < 3; i++)
					{
						if(i == highlight)
							wattron(meniu_win,A_STANDOUT);
						mvwprintw(meniu_win, i+1, 1, meniu_choice[i]);
						wattroff(meniu_win, A_STANDOUT);
					}
					int choice = wgetch(meniu_win);
					switch(choice)
					{
						case KEY_UP:
						case 'w':
						case 'W':
							highlight--;
							if(highlight < 0)
								highlight = 2;
							break;
						case KEY_DOWN:
						case 's':
						case 'S':
							highlight++;
							if(highlight > 2)
								highlight = 0;
							break;
					}
					if(choice == 10)
						break;
				}
				second_flag = 0;
			}


			wrefresh(meniu_win);

			if(highlight == 2)
			{
				flag = 0;
			}

			if(highlight == 0)
			{
				resume_flag = 1;
				wclear(meniu_win);
				box(meniu_win, 0, 0);
				wrefresh(meniu_win);
				int highlight_newgame = 1;
				if(another_flag == 0)
				{
					while(1)
					{
						if(win_flag == 1)
						{
							wclear(meniu_win);
							box(meniu_win, 0,0);
							mvwprintw(meniu_win, 1,1, "You Won. Congratulations!");
							mvwprintw(meniu_win, 2,1, "Your score:%d", score);
							mvwprintw(meniu_win, 3,1, "Please press [q] to continue");
							wrefresh(meniu_win);
							win_flag = 0;
							win = 20;
							lost = 20;
							flag_flag = 1;
							break;
						}
						else
							if(lost_flag == 1)
							{
								wclear(meniu_win);
								box(meniu_win, 0,0);
								mvwprintw(meniu_win, 1,1, "You Lost. Game over!");
								mvwprintw(meniu_win, 2,1, "Your score:%d", score);
								mvwprintw(meniu_win, 3,1, "Please press [q] to continue");
								wrefresh(meniu_win);
								lost_flag = 0;
								win = 20;
								lost = 20;
								flag_flag = 1;
								break;
							}
						else
							if(win_flag == 0 && lost_flag == 0)
							{
								wattron(meniu_win, A_BOLD);
								mvwprintw(meniu_win, 1,1,"Please chose a map to play with:");
								wattroff(meniu_win, A_BOLD);
								for(i = 1; i < argc; i++)
								{
									if(i == highlight_newgame)
										wattron(meniu_win,A_STANDOUT);
									mvwprintw(meniu_win, i+1, 1, "Map #%d", i);
									wattroff(meniu_win, A_STANDOUT);
								}			
								int choice_newgame = wgetch(meniu_win);	
								switch(choice_newgame)
								{
									case KEY_UP:
									case 'w':
									case 'W':
										highlight_newgame--;
										if(highlight_newgame < 1)
											highlight_newgame = argc-1;
										break;
									case KEY_DOWN:
									case 's':
									case 'S':
										highlight_newgame++;
										if(highlight_newgame > argc-1)
											highlight_newgame = 1;
										break;
								}
								if(choice_newgame == 10)
									break;
							}
					}
				}
				wrefresh(meniu_win);

				if(ptr == 0)
				{
					FILE * map;
					map = fopen(argv[highlight_newgame], "r");
					for(i = 0; i < 10; i++)
					{
						for(j = 0; j < 10; j++)
						{
							fscanf(map, "%d", &mat_map[i][j]);
							if(mat_map[i][j] == 1)
							{	
								wattron(player_win,COLOR_PAIR(1));
								mvwprintw(player_win,1+i*4 ,1+j*4, "|###|");
								mvwprintw(player_win,2+i*4 ,1+j*4, "|###|");
								mvwprintw(player_win,3+i*4, 1+j*4, "|###|");
								wattroff(player_win,COLOR_PAIR(1));
								mvwprintw(player_win,4+i*4, 1+j*4, "-----");
								wrefresh(player_win);
								
							}

							else
							{
								if(mat_map[i][j] == 0)
								{
									wattron(player_win,COLOR_PAIR(2));
									mvwprintw(player_win,1+i*4, 1+j*4, "|@@@|");
									mvwprintw(player_win,2+i*4, 1+j*4, "|@@@|");							
									mvwprintw(player_win,3+i*4, 1+j*4, "|@@@|");
									wattroff(player_win,COLOR_PAIR(2));
									mvwprintw(player_win,4+i*4, 1+j*4, "-----");
									wrefresh(player_win);
								}
							}
						}
						box(player_win,0 ,0);
						wrefresh(player_win);
					}
					srand(time(NULL));
					int p = rand()%(argc-1) + 1;
					FILE * map_bot;
					map_bot = fopen(argv[p], "r");
					for(i = 0; i < 10; i++)
					{
						for(j = 0; j < 10; j++)
						{
							fscanf(map_bot, "%d", &mat_map_bot[i][j]);
							if(mat_map_bot[i][j] == 1)
							{	
								mvwprintw(bot_win,1+i*4 ,1+j*4, "|???|");
								mvwprintw(bot_win,2+i*4 ,1+j*4, "|???|");
								mvwprintw(bot_win,3+i*4 ,1+j*4, "|???|");
								mvwprintw(bot_win,4+i*4, 1+j*4, "-----");
								wrefresh(bot_win);	
							}

							else
							{
								if(mat_map_bot[i][j] == 0)
								{
									mvwprintw(bot_win,1+i*4 ,1+j*4, "|???|");
									mvwprintw(bot_win,2+i*4 ,1+j*4, "|???|");
									mvwprintw(bot_win,3+i*4 ,1+j*4, "|???|");
									mvwprintw(bot_win,4+i*4, 1+j*4, "-----");
									wrefresh(bot_win);
								}
							}
						}

						box(bot_win,0 ,0);
						wrefresh(bot_win);
					}
				}
				if(ptr == 1)
				{
					another_flag = 0;
					for(i = 0; i < 10; i++)
					{
						for(j = 0; j < 10; j++)
						{
							if(mat_map_resume[i][j] == 1)
							{	
								wattron(player_win,COLOR_PAIR(1));
								mvwprintw(player_win,1+i*4 ,1+j*4, "|###|");
								mvwprintw(player_win,2+i*4 ,1+j*4, "|###|");
								mvwprintw(player_win,3+i*4, 1+j*4, "|###|");
								wattroff(player_win,COLOR_PAIR(1));
								mvwprintw(player_win,4+i*4, 1+j*4, "-----");
								wrefresh(player_win);
								
							}
							if(mat_map_resume[i][j] == 0)
							{
								wattron(player_win,COLOR_PAIR(2));
								mvwprintw(player_win,1+i*4, 1+j*4, "|@@@|");
								mvwprintw(player_win,2+i*4, 1+j*4, "|@@@|");							
								mvwprintw(player_win,3+i*4, 1+j*4, "|@@@|");
								wattroff(player_win,COLOR_PAIR(2));
								mvwprintw(player_win,4+i*4, 1+j*4, "-----");
								wrefresh(player_win);
							}
							if(mat_map_resume[i][j] == 7)
							{
								wattron(player_win,COLOR_PAIR(4));
								mvwprintw(player_win,1+i*4 ,1+j*4, "|###|");
								mvwprintw(player_win,2+i*4 ,1+j*4, "|###|");
								mvwprintw(player_win,3+i*4, 1+j*4, "|###|");
								wattroff(player_win,COLOR_PAIR(4));
								mvwprintw(player_win,4+i*4, 1+j*4, "-----");
								wrefresh(player_win);	
							}
							if(mat_map_resume[i][j] == 8)
							{
								wattron(player_win,COLOR_PAIR(3));
								mvwprintw(player_win,1+i*4 ,1+j*4, "|@@@|");
								mvwprintw(player_win,2+i*4 ,1+j*4, "|@@@|");
								mvwprintw(player_win,3+i*4, 1+j*4, "|@@@|");
								wattroff(player_win,COLOR_PAIR(3));
								mvwprintw(player_win,4+i*4, 1+j*4, "-----");
								wrefresh(player_win);
							}
						}
					}
					box(player_win,0 ,0);
					wrefresh(player_win);

					for(i = 0; i < 10; i++)
					{
						for(j = 0; j < 10; j++)
						{
							if(mat_map_bot_resume[i][j] == 1)
							{	
								mvwprintw(bot_win,1+i*4 ,1+j*4, "|???|");
								mvwprintw(bot_win,2+i*4 ,1+j*4, "|???|");
								mvwprintw(bot_win,3+i*4 ,1+j*4, "|???|");
								wrefresh(bot_win);	
							}
							if(mat_map_bot_resume[i][j] == 0)
							{
								mvwprintw(bot_win,1+i*4 ,1+j*4, "|???|");
								mvwprintw(bot_win,2+i*4 ,1+j*4, "|???|");
								mvwprintw(bot_win,3+i*4 ,1+j*4, "|???|");
								wrefresh(bot_win);
							}
							if(mat_map_bot_resume[i][j] == 2)
							{
								wattron(bot_win,COLOR_PAIR(1));
								mvwprintw(bot_win,1+i*4 ,1+j*4, "|???|");
								mvwprintw(bot_win,2+i*4 ,1+j*4, "|???|");
								mvwprintw(bot_win,3+i*4 ,1+j*4, "|???|");
								wattron(bot_win, COLOR_PAIR(1));
								wrefresh(bot_win);	
							}
							if(mat_map_bot_resume[i][j] == 3)
							{
								wattron(bot_win,COLOR_PAIR(2));
								mvwprintw(bot_win,1+i*4 ,1+j*4, "|???|");
								mvwprintw(bot_win,2+i*4 ,1+j*4, "|???|");
								mvwprintw(bot_win,3+i*4 ,1+j*4, "|???|");
								wattron(bot_win, COLOR_PAIR(2));
								wrefresh(bot_win);		
							}
						}
					}
					box(bot_win,0 ,0);
					wrefresh(bot_win);
					ptr = 0;
				}

				int highlight_bot_x = 0;
				int highlight_bot_y = 0;
				int m=0,n=0;
				int bot_play_flag = 0;

				while(1)
				{
					keypad(bot_win, true);
					while(bot_play_flag == 1)
					{
						srand(time(NULL));
						int l1,l2;
						l1 = rand() % 10;
						l2 = rand() % 10;

						while(mat_map_bot[l1][l2] != 7 && mat_map_bot[l1][l2] != 8)
						{
							if(mat_map[l1][l2] == 1)
							{
								mat_map[l1][l2] = 7;
								lost--;
								score--;
								wattron(player_win, COLOR_PAIR(4));
								mvwprintw(player_win, 1+l1*4, 1+l2*4, "|###|");
								mvwprintw(player_win, 2+l1*4, 1+l2*4, "|###|");
								mvwprintw(player_win, 3+l1*4, 1+l2*4, "|###|");
								wattroff(player_win, COLOR_PAIR(4));
								wrefresh(player_win);
								sleep(3);
							}
							else
								if(mat_map[l1][l2] == 0)
								{
									bot_play_flag = 0;
									mat_map[l1][l2] = 8;
									wattron(player_win, COLOR_PAIR(3));
									mvwprintw(player_win, 1+l1*4, 1+l2*4, "|@@@|");
									mvwprintw(player_win, 2+l1*4, 1+l2*4, "|@@@|");
									mvwprintw(player_win, 3+l1*4, 1+l2*4, "|@@@|");
									wattroff(player_win, COLOR_PAIR(3));
									wrefresh(player_win);
									break;
								}
							l1 = rand() % 10;
							l2 = rand() % 10;
						}
					}
					if(lost == 0)
					{
						lost_flag = 1;
						break;
					}
					
					for(i = 0; i < 10; i++)
					{
						for(j = 0; j < 10; j++)
						{
							if(i == highlight_bot_y)
							{
								if(j == highlight_bot_x)
									wattron(bot_win, A_STANDOUT);
							}
							if(mat_map_bot[i][j] == 2)
							{
								wattron(bot_win, COLOR_PAIR(1));
							}
							else
								if(mat_map_bot[i][j] == 3)
									wattron(bot_win, COLOR_PAIR(2));

							mvwprintw(bot_win, 1+i*4, 1+j*4, "|???|");
							mvwprintw(bot_win, 2+i*4, 1+j*4, "|???|");
							mvwprintw(bot_win, 3+i*4, 1+j*4, "|???|");
							wattroff(bot_win,  A_STANDOUT);
							wattroff(bot_win, COLOR_PAIR(1));
							wattroff(bot_win, COLOR_PAIR(2));
							box(bot_win,0,0);
							wrefresh(bot_win);
						}
					}

					int choice_bot = wgetch(bot_win);
					switch(choice_bot)
					{
						case KEY_RIGHT:
						case 'd':
						case 'D':
							move_right(&n,&highlight_bot_x);
							break;

						case KEY_LEFT:
						case 'a':
						case 'A':
							move_left(&n, &highlight_bot_x);
							break;

						case KEY_DOWN:
						case 's':
						case 'S':
							move_down(&m, &highlight_bot_y);
							break;

						case KEY_UP:
						case 'w':
						case 'W':
							move_up(&m, &highlight_bot_y);
							break;
					}

					if(choice_bot == 10 && flag_flag == 0)
					{
						if(win == 0)
						{
							win_flag = 1;
							break;
						}
						if(win != 0)
						{
							if(mat_map_bot[m][n] == 1)
							{
								win--;
								score += 2;
								mat_map_bot[m][n] = 2;
							}
							if(mat_map_bot[m][n] == 0)
							{
								mat_map_bot[m][n] = 3;
								bot_play_flag = 1;
							}
						}
					}
					else
						if(choice_bot == 113)
						{
							second_flag = 1;
							resume_flag = 1;
							break;
						}
				}
			}
			if(highlight == 1)
			{
				wclear(meniu_win);
				box(meniu_win, 0, 0);
				wrefresh(meniu_win);

				if(resume_flag == 0)
				{
					int highlight_resume = 0;
					while(1)
					{
						wclear(meniu_win);
						box(meniu_win, 0, 0);
						wrefresh(meniu_win);
						mvwprintw(meniu_win, 1,1, "You have no game in progress!");
						wrefresh(meniu_win);
						for(i = 0; i < 2; i++)
						{
							if(i == highlight_resume)
								wattron(meniu_win, A_STANDOUT);
							mvwprintw(meniu_win, i+2, 1, meniu_choice_resume[i]);
							wattroff(meniu_win, A_STANDOUT);
						}

						int choice_resume = wgetch(meniu_win);
						switch(choice_resume)
						{
							case KEY_UP:
								highlight_resume--;
								if(highlight_resume < 0)
									highlight_resume = 1;
								break;
							case KEY_DOWN:
								highlight_resume++;
								if(highlight_resume > 1)
									highlight_resume = 0;
								break;
						}

						if(choice_resume == 10)
							break;
					}
					wrefresh(meniu_win);
					
					if(highlight_resume == 1)
						flag = 0;
					else
						if(highlight_resume == 0)
							highlight = 0;
				}
				if(resume_flag == 1)
				{
					highlight = 0;
					another_flag = 1;
					ptr = 1;

					for(i = 0; i < 10; i++)
					{
						for(j = 0; j < 10; j++)
							mat_map_resume[i][j] = mat_map[i][j]; 
					}
					for(i = 0; i < 10; i++)
					{
						for(j = 0; j < 10; j++)
							mat_map_bot_resume[i][j] = mat_map_bot[i][j];
					}
				}

			}

		}
		endwin();
		return 1;
	}
}


void title()
{
	int height_title, width_title, start_y_title, start_x_title;
	height_title = 2;
	width_title = getmaxx(stdscr);
	start_y_title = start_x_title = 0;
	WINDOW * title_win = newwin(height_title, width_title, start_y_title, start_x_title);
	refresh();
	box(title_win, 0, 0);
	wattron(title_win, A_BOLD);
	mvwprintw(title_win, 0, (getmaxx(stdscr) / 2) - 6, "BATTLESHIPS");
	wattroff(title_win,A_BOLD);
	wrefresh(title_win);
}

void move_up(int *m, int *highlight_bot_y)
{
	(*m)--;
	if(*m < 0)
		*m = 0;
	(*highlight_bot_y)--;
	if(*highlight_bot_y < 0)
		*highlight_bot_y = 0;
}

void move_down(int *m, int *highlight_bot_y)
{
	(*m)++;
	if((*m) > 9)
		*m = 9;
	(*highlight_bot_y)++;
	if(*highlight_bot_y > 9)
		*highlight_bot_y = 9;
}

void move_left(int *n, int *highlight_bot_x)
{
	(*n)--;
	if(*n < 0)
		*n = 0;
	(*highlight_bot_x)--;
	if(*highlight_bot_x < 0)
		*highlight_bot_x = 0;
}

void move_right(int *n, int *highlight_bot_x)
{
	(*n)++;
	if(*n > 9)
		*n = 9;
	(*highlight_bot_x)++;
	if(*highlight_bot_x > 9)
		*highlight_bot_x = 9;
}

int **alocare()
{
	int i;
	int **a = (int**)malloc(10*sizeof(int*));
	for(i = 0; i < 10; i++)
		a[i] = (int*)malloc(10*sizeof(int));
	return a;
}
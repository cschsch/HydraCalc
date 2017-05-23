#include <iostream>
#include <cstdlib>

#ifdef _WIN32
#include <Windows.h>
#include <conio.h>
#define clear system("cls");
#define sleep Sleep(1000);
#define getch() _getch()
#elif defined __unix__
#include <unistd.h>
#include <termios.h>
#include <stdio.h>
#define clear system("clear")
#define sleep sleep(1)
char getch()
{
	char buf = 0;
	struct termios old = { 0 };
	fflush(stdout);

	if (tcgetattr(0, &old) < 0)
		perror("tcsetattr()");

	old.c_lflag &= ~ICANON;
	old.c_lflag &= ~ECHO;
	old.c_cc[VMIN] = 1;
	old.c_cc[VTIME] = 0;

	if (tcsetattr(0, TCSANOW, &old) < 0)
		perror("tcsetattr ICANON");

	if (read(0, &buf, 1) < 0)
		perror("read()");

	old.c_lflag |= ICANON;
	old.c_lflag |= ECHO;

	if (tcsetattr(0, TCSADRAIN, &old) < 0)
		perror("tcsetattr ~ICANON");

	printf("%c\n", buf);
	return buf;
}
#endif

using namespace std;

class hydra;
class weapon;
void is_done(char input[], int n);
void exit_keypress();
void update_weapons(weapon w[8], hydra h);
void use_with_ID(int i);
void step_manager(int steps[6], int ids_for_steps[6][6]);

class hydra
{
public:
	int heads;
	bool exists;

	hydra(int h, bool e)
	{
		heads = h;
		exists = e;
	}
};

class weapon
{
public:
	int cut, grow, cut_grow, s_cut_factor, id;
	bool exists, s_cut, s_grow;

	weapon()
	{
		exists = false;
	}

	void set_Weapon(int i, int c, int g, int c_g, int s_c_f, bool e, bool s_c, bool s_g)
	{
		id = i;
		cut = c;
		grow = g;
		cut_grow = c_g;
		s_cut_factor = s_c_f;
		exists = e;
		s_cut = s_c;
		s_grow = s_g;
	}
};

void is_done(char input[], int n)
{
	cout << endl << input << " has succesfully been ";

	if (n == 0)
		cout << "deleted.";
	else if (n == 1)
		cout << "created.";

	sleep;
	clear;
}

void exit_keypress()
{
	cout << endl << "Press any key to continue.";
	getch();
	clear;
}

void update_weapons(weapon w[8], hydra h)
{
	for (int ID = 0; ID < 8; ++ID)
	{
		if (w[ID].exists)
		{
			if (w[ID].s_cut)
			{
				if (h.heads % w[ID].s_cut_factor == 0)
					w[ID].cut = h.heads - h.heads / w[ID].s_cut_factor;
				else
					w[ID].cut = 0;

				if (w[ID].s_grow)
					w[ID].grow = w[ID].cut * 2;

				w[ID].cut_grow = w[ID].cut - w[ID].grow;
			}
		}
	}
}

void use_with_ID(int i)
{
	if (i != -1)
		cout << "Use weapon with ID " << i << endl;
}

void step_manager(int steps[6], int ids_for_steps[6][6])
{
	bool *is_steps = new bool[6]();

	for (int step = 0; step < 6; ++step)
	{
		if (steps[step] != 0)
			is_steps[step] = true;
		else
			continue;

		if (is_steps[step])
		{
			for (int step1 = 0; step1 < 6; ++step1)
				use_with_ID(ids_for_steps[step][step1]);

			break;
		}
	}

	delete[] is_steps;
	exit_keypress();
}

int main()
{
	int *var_heads_1 = new int;

	while (1)
	{
		cout << "How many heads does the hydra have? ";
		cin >> *var_heads_1;

		if (*var_heads_1 < 0)
		{
			cout << endl << "Invalid input.";
			sleep;
			clear;
		}
		else
			break;
	}

	hydra enemy(*var_heads_1, 1);
	hydra test(*var_heads_1, 1);
	delete var_heads_1;
	is_done("Hydra", 1);
	weapon weapons[8];
	int steps[6];
	int ids_for_steps[6][6];

	for (int n = 0; n < 6; ++n)
	{
		steps[n] = 0;

		for (int m = 0; m < 6; ++m)
			ids_for_steps[n][m] = -1;
	}

	while (1)
	{
		cout << "Heads: " << enemy.heads << endl << endl;
		cout << "0 - Execute the program" << endl;
		cout << "1 - Enter a new weapon" << endl;
		cout << "2 - Delete a weapon by ID" << endl;
		cout << "3 - Print all weapons" << endl;
		cout << "4 - Enter a new hydra" << endl;
		cout << "5 - Exit the program" << endl << endl;
		update_weapons(weapons, enemy);
		int *var_choice_1 = new int;
		cin >> *var_choice_1;
		clear;

		if (*var_choice_1 == 0)
		{
			int ID, ID1, ID2, ID3, ID4, ID5;

			for (ID = 0; ID < 8; ++ID)
			{
				if (weapons[ID].exists)
				{
					if ((weapons[ID].cut > test.heads) | (weapons[ID].cut == 0))
						continue;

					if (test.heads - weapons[ID].cut == 0)
					{
						enemy.heads = test.heads - weapons[ID].cut;
						steps[0] = 1;
						ids_for_steps[0][0] = ID;
						continue;
					}

					test.heads -= weapons[ID].cut_grow;
					update_weapons(weapons, test);

					for (ID1 = 0; ID1 < 8; ID1++)
					{
						if (weapons[ID1].exists)
						{
							if ((weapons[ID1].cut > test.heads) | (weapons[ID1].cut == 0))
								continue;

							if (test.heads - weapons[ID1].cut == 0)
							{
								enemy.heads = test.heads - weapons[ID1].cut;
								steps[1] = 2;
								ids_for_steps[1][0] = ID;
								ids_for_steps[1][1] = ID1;
								continue;
							}

							test.heads -= weapons[ID1].cut_grow;
							update_weapons(weapons, test);

							for (ID2 = 0; ID2 < 8; ++ID2)
							{
								if (weapons[ID2].exists)
								{
									if ((weapons[ID2].cut > test.heads) | (weapons[ID2].cut == 0))
										continue;

									if (test.heads - weapons[ID2].cut == 0)
									{
										enemy.heads = test.heads - weapons[ID2].cut;
										steps[2] = 3;
										ids_for_steps[2][0] = ID;
										ids_for_steps[2][1] = ID1;
										ids_for_steps[2][2] = ID2;
										continue;
									}

									test.heads -= weapons[ID2].cut_grow;
									update_weapons(weapons, test);

									for (ID3 = 0; ID3 < 8; ++ID3)
									{
										if (weapons[ID3].exists)
										{
											if ((weapons[ID3].cut > test.heads) | (weapons[ID3].cut == 0))
												continue;

											if (test.heads - weapons[ID3].cut == 0)
											{
												enemy.heads = test.heads - weapons[ID3].cut;
												steps[3] = 4;
												ids_for_steps[3][0] = ID;
												ids_for_steps[3][1] = ID1;
												ids_for_steps[3][2] = ID2;
												ids_for_steps[3][3] = ID3;
												continue;
											}

											test.heads -= weapons[ID3].cut_grow;
											update_weapons(weapons, test);

											for (ID4 = 0; ID4 < 8; ++ID4)
											{
												if (weapons[ID4].exists)
												{
													if ((weapons[ID4].cut > test.heads) | (weapons[ID4].cut == 0))
														continue;

													if (test.heads - weapons[ID4].cut == 0)
													{
														enemy.heads = test.heads - weapons[ID1].cut;
														steps[4] = 5;
														ids_for_steps[4][0] = ID;
														ids_for_steps[4][1] = ID1;
														ids_for_steps[4][2] = ID2;
														ids_for_steps[4][3] = ID3;
														ids_for_steps[4][4] = ID4;
														continue;
													}

													test.heads -= weapons[ID4].cut_grow;
													update_weapons(weapons, test);

													for (ID5 = 0; ID5 < 8; ++ID5)
													{
														if (weapons[ID5].exists)
														{
															if ((weapons[ID5].cut > test.heads) | (weapons[ID5].cut == 0))
																continue;

															if (test.heads - weapons[ID5].cut == 0)
															{
																enemy.heads = test.heads - weapons[ID5].cut;
																steps[5] = 6;
																ids_for_steps[5][0] = ID;
																ids_for_steps[5][1] = ID1;
																ids_for_steps[5][2] = ID2;
																ids_for_steps[5][3] = ID3;
																ids_for_steps[5][4] = ID4;
																ids_for_steps[5][5] = ID5;
																continue;
															}
														}
													}

													if (weapons[ID4].s_cut)
													{
														if (weapons[ID4].s_grow)
															test.heads /= 2;
														else
															test.heads -= weapons[ID4].grow;

														test.heads *= weapons[ID4].s_cut_factor;
													}
													else
														test.heads += weapons[ID4].cut_grow;

													update_weapons(weapons, test);
												}
											}

											if (weapons[ID3].s_cut)
											{
												if (weapons[ID3].s_grow)
													test.heads /= 2;
												else
													test.heads -= weapons[ID3].grow;

												test.heads *= weapons[ID3].s_cut_factor;
											}
											else
												test.heads += weapons[ID3].cut_grow;

											update_weapons(weapons, test);
										}
									}

									if (weapons[ID2].s_cut)
									{
										if (weapons[ID2].s_grow)
											test.heads /= 2;
										else
											test.heads -= weapons[ID2].grow;

										test.heads *= weapons[ID2].s_cut_factor;
									}
									else
										test.heads += weapons[ID2].cut_grow;

									update_weapons(weapons, test);
								}
							}

							if (weapons[ID1].s_cut)
							{
								if (weapons[ID1].s_grow)
									test.heads /= 2;
								else
									test.heads -= weapons[ID1].grow;

								test.heads *= weapons[ID1].s_cut_factor;
							}
							else
								test.heads += weapons[ID1].cut_grow;

							update_weapons(weapons, test);
						}
					}

					if (weapons[ID].s_cut)
					{
						if (weapons[ID].s_grow)
							test.heads /= 2;
						else
							test.heads -= weapons[ID].grow;

						test.heads *= weapons[ID].s_cut_factor;
					}
					else
						test.heads += weapons[ID].cut_grow;

					update_weapons(weapons, test);
				}
			}

			int check_if_successful;

			for (check_if_successful = 0; check_if_successful < 6; ++check_if_successful)
			{
				if (steps[check_if_successful] != 0)
					break;
			}

			if (check_if_successful < 6)
				step_manager(steps, ids_for_steps);
			else
			{
				cout << "Killing the hydra in six or less hits is impossible.";
				sleep;
				clear;
			}
		}
		else if (*var_choice_1 == 1)
		{
			int ID = 0;

			for (ID; ID < 8; ++ID)
			{
				if (!weapons[ID].exists)
				{
					int *var_weapon_int = new int[4];
					bool *var_weapon_bool = new bool[3];
					var_weapon_bool[0] = true;

					while (1)
					{
						cout << "Is your weapon a divisor? (0/1)" << endl;
						cin >> var_weapon_bool[1];

						if ((var_weapon_bool[1] == 1) | (var_weapon_bool[1] == 0))
							break;
						else
							cout << endl << "Invalid input." << endl << endl;
					}

					if (var_weapon_bool[1])
					{
						while (1)
						{
							cout << endl << "By which amount? ";
							cin >> var_weapon_int[3];

							if (var_weapon_int[3] < 2)
								cout << endl << "Invalid input." << endl << endl;
							else
								break;
						}

						if (enemy.heads % var_weapon_int[3] == 0)
							var_weapon_int[0] = enemy.heads - enemy.heads / var_weapon_int[3];
						else
							var_weapon_int[0] = 0;
					}
					else
					{
						var_weapon_int[3] = 0;

						while (1)
						{
							cout << endl << "How many heads does the weapon cut? ";
							cin >> var_weapon_int[0];

							if (var_weapon_int[0] < 1)
								cout << endl << "Invalid input." << endl << endl;
							else
								break;
						}
					}

					while (1)
					{
						cout << endl << "Is the hydra's head growth multiplicative? (0/1)" << endl;
						cin >> var_weapon_bool[2];

						if ((var_weapon_bool[2] == 1) | (var_weapon_bool[2] == 0))
							break;
						else
							cout << endl << "Invalid input." << endl << endl;
					}

					if (var_weapon_bool[2])
						var_weapon_int[1] = var_weapon_int[0] * 2;
					else
					{
						while (1)
						{
							cout << endl << "How many heads will grow back? ";
							cin >> var_weapon_int[1];

							if (var_weapon_int[1] < 0)
								cout << endl << "Invalid input." << endl << endl;
							else
								break;
						}
					}

					var_weapon_int[2] = var_weapon_int[0] - var_weapon_int[1];
					weapons[ID].set_Weapon(ID, var_weapon_int[0], var_weapon_int[1], var_weapon_int[2], var_weapon_int[3], var_weapon_bool[0], var_weapon_bool[1], var_weapon_bool[2]);
					delete[] var_weapon_int;
					delete[] var_weapon_bool;
					break;
				}
			}

			if (ID == 8)
			{
				cout << "No space left for any weapons.";
				sleep;
				clear;
			}
			else
				is_done("Weapon", 1);
		}
		else if (*var_choice_1 == 2)
		{
			int *var_weapon_id = new int;

			while (1)
			{
				cout << "ID of weapon to be deleted: ";
				cin >> *var_weapon_id;

				if ((*var_weapon_id < 0) | (*var_weapon_id > 7))
					cout << endl << "Invalid input." << endl << endl;
				else
					break;
			}

			if (!weapons[*var_weapon_id].exists)
			{
				cout << endl << "Weapon already does not exist.";
				sleep;
				clear;
				delete var_weapon_id;
			}
			else
			{
				weapons[*var_weapon_id].exists = false;
				is_done("Weapon", 0);
				delete var_weapon_id;
			}
		}
		else if (*var_choice_1 == 3)
		{
			cout << "ID\tcut\tgrow\ts_cut_factor" << endl << endl;

			for (int ID = 0; ID < 8; ++ID)
			{
				if (weapons[ID].exists)
				{
					if (weapons[ID].s_cut)
						cout << weapons[ID].id << "\t" << weapons[ID].cut << "\t" << weapons[ID].grow << "\t" << weapons[ID].s_cut_factor << endl;
					else
						cout << weapons[ID].id << "\t" << weapons[ID].cut << "\t" << weapons[ID].grow << endl;
				}
			}

			exit_keypress();
		}
		else if (*var_choice_1 == 4)
		{
			int *var_heads_2 = new int;

			while (1)
			{
				cout << "How many heads does the hydra have? ";
				cin >> *var_heads_2;

				if (*var_heads_2 < 0)
					cout << endl << "Invalid input." << endl << endl;
				else
					break;
			}

			enemy.heads = *var_heads_2;
			test.heads = *var_heads_2;
			delete var_heads_2;
			is_done("Hydra", 1);
		}
		else if (*var_choice_1 == 5)
		{
			delete var_choice_1;
			break;
		}
		else
		{
			cout << "Invalid input.";
			sleep;
			clear;
		}

		delete var_choice_1;
	}
}
// Created by madhav427 on 2/18/2018.
// Copyright © 2018 madhav427. All rights reserved.
/**************************************************************************/
/* Function: This program is for implementation of the Simulated Anneling */
/*				Algorithm for Balanced Bipartition                        */
/* Author : madhav427                                                     */
/* Date: FEB/18/2018				    								  */
/**************************************************************************/
#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include "math.h"
#include <random>
#include<sstream>
//#include <chrono>
/**************************************************************************/
/* Function: This function is used to implement the perturbation in the   */
/* 			 Simulated Annealing Algorithm			  					  */
/**************************************************************************/
std::vector<bool> perturb(std::vector<bool> cur_sol, unsigned int m) {
	std::vector<unsigned int>vect_a;
	std::vector<unsigned int>vect_b;
	for (unsigned int i = 0; i < cur_sol.size(); i++) {
		if (cur_sol[i] == 0) {
			vect_a.push_back(i);
		}
		else {
			vect_b.push_back(i);
		}
	}
	std::random_shuffle(vect_a.begin(), vect_a.end()); //pertrubation of vector A partition
	std::random_shuffle(vect_b.begin(), vect_b.end()); //pertrubation of vector B partition
	for (unsigned int i = 0; i < m; i++) {
		cur_sol[vect_a[m]] = 1;
		cur_sol[vect_b[m]] = 0;
	}
	vect_a.clear();
	vect_b.clear();
	return cur_sol;
}
/**************************************************************************/
/* Function: This function is used to implement the calculation of the    */
/* 			 size of the cutset                                           */
/**************************************************************************/
unsigned int cutset(unsigned int &nodes, std::vector<unsigned int> &rows, std::vector<bool>
	&next_sol, std::vector<unsigned int> &cols, std::vector<unsigned int>&values)
{
	std::vector<unsigned int>external(nodes, 0);
	//create external
	unsigned int val_i = 0;
	for (unsigned int r_i = 0; r_i < nodes; r_i++) { ///r_i indicates the row index
		unsigned int count = rows[r_i + 1] - rows[r_i];
		if (count != 0) {
			unsigned int i = 0;
			while (val_i + i<val_i + count)
			{
				if (((next_sol[r_i] == 1) && (next_sol[cols[val_i + i]] == 0)) ||
					((next_sol[r_i] == 0) && (next_sol[cols[val_i + i]] == 1))) {
					external[r_i] = external[r_i] + values[val_i + i];
					external[cols[val_i + i]] = external[cols[val_i + i]] +
						values[val_i + i];
				}
				i++;
			}
			val_i = val_i + count;
		}
	}
	unsigned int sum = 0;
	for (unsigned int i = 0; i < nodes; i++)
	{
		if (next_sol[i] == 0)
		{
			sum = sum + external[i];
		}
	}
	return sum;
}
/**************************************************************************/
/* Function: This function is used to implement cool down of the          */
/* 				Temperature                                               */
/**************************************************************************/
void cool_down(double &Temp) {
	Temp = 0.9985*Temp;
}
/**************************************************************************/
/* Function: This function is used to implement the random decimal number */
/* 				Generation using the random library of CPP                */
/**************************************************************************/
double get_random()
{
	static std::default_random_engine e;
	static std::uniform_real_distribution<> dis(0, 1); // range 0 - 1
	return dis(e);
}
/**************************************************************************/
/* Function: This function is used to implement the accept move of the    */
/* 				Simulated Annealing Algorithm                             */
/**************************************************************************/
bool accept_move(int &cost_delta, double & Temp) {
	if (cost_delta < 0) {
		return true;
	}
	else {
		double boltz, K = 1;//Can change the K value
		boltz = exp((-1)*cost_delta / (K*Temp));
		double r = get_random();
		if (r < boltz) {
			return true;
		}
		else {
			return false;
		}
	}
}
/**************************************************************************/
/* Function: This function is the main function taking inputs through     */
/* 				Command line and save the outputs in the text file        */
/**************************************************************************/
int main(int argc, char *argv[])
{
/**************************************************************************/
/* Snippet: This code snippet is used to initialize the time elapsed      */
/* 				During the execution of the program	           	          */
/**************************************************************************/
	//auto start =std::chrono::system_clock::now();
	unsigned int nets, nodes;
	std::ifstream infile(argv[1]);
	infile >> nodes;
	//std::cout << "The number of nodes are ";
	//std::cout << nodes << std::endl;
	infile >> nets;
	//std::cout << "The number of nets are ";
	//std::cout << nets << std::endl;
	//std::cout << "Creating the vector" << std::endl;
	std::vector<unsigned int> values;
	//vector implementation of the values
	std::vector<unsigned int> cols;
	//vector implementation of coloums
	std::vector<unsigned int> rows(nodes + 1, 0);
	//vector implementation of row index rows
	//size is nodes + 1
	std::vector <std::vector<unsigned int>> matrix;
	matrix.assign(nodes, std::vector < unsigned int >(nodes));
/**************************************************************************/
/* Snippet: This code snippet is used to implement the matrix which       */
/*       	creates the lower triangular matrix vector                    */
/**************************************************************************/
	for (unsigned int i = 0; i < nodes; i++) {
		for (unsigned int j = 0; j <= i; j++) {
			matrix[i].resize(j + 1);
		}
	}
/**************************************************************************/
/* Snippet: This code snippet is used to store the inputs into the        */
/**************************************************************************/
	while (nets)
	{
		unsigned int x1, x2;
		infile >> x1;
		infile >> x2;
		//std::cout << x1 << " " << x2 << std::endl;
		x1 = x1 - 1;
		x2 = x2 - 1;
		if (x1 < x2) {
			matrix[x2][x1] = matrix[x2][x1] + 1;
		}
		if (x1 >= x2) {
			matrix[x1][x2] = matrix[x1][x2] + 1;
		}
		nets--;
	}
	infile.close();//closing the file
/**************************************************************************/
/* Snippet: This code snippet is used to initialize the Direct Sequence   */
/* Sparse Matrix. The contents are rows,coloums,values  		  */
/**************************************************************************/
	for (unsigned int i = 0; i < nodes; i++) {
		unsigned int count = 0; //changed here from unsigned to signed
		for (unsigned int j = 0; j <= i; j++) {
			if (matrix[i][j] != 0) {
				values.push_back(matrix[i][j]);
				cols.push_back(j);
				count++;
			}
			rows[i + 1] = rows[i] + count;
		}
	}
/**************************************************************************/
/*Snippet: This code snippet is used to delete the matrix-lower triangular*/
/*      	To save the dynamic space                                 */
/**************************************************************************/
	matrix.clear();
/**************************************************************************/
/*Snippet: This code snippet is used to create the current solution       */
/*      	the initial partitions containing equal number of nodes   */
/**************************************************************************/
	std::vector<bool> cur_sol(nodes, 1);//vector implementation of flags for the partition
	/////////Creating intial partition by assigning values ot the flags////////
	for (unsigned int i = 0; i < nodes; i++)
	{
		if (i % 2 == 0)
		{
			cur_sol[i] = 0;
		}
	}
/**************************************************************************/
/*Snippet: This code snippet is used to initialize the next solution      */
/*       	the current solution is stored into the next solution     */
/**************************************************************************/
	std::vector<bool> next_sol(nodes);//vector implementation of flags for the partition
	next_sol = cur_sol;
/**************************************************************************/
/*Snippet: The Simulated Engine starts from here        	          */
/**************************************************************************/
	double Temp = 40000, Temp_crit = 0.1;
	unsigned int cur_sum = cutset(nodes, rows, cur_sol, cols, values);//cur_sum will hold the value
	unsigned int iterations = 0;
/**************************************************************************/
/*Snippet: As long as the Temperature is greater than the critical        */
/*      	temperature the next solution is found out                */
/**************************************************************************/
	int cost_delta = 0;
	while (Temp > Temp_crit) {
		unsigned int goodmoves = 0;
		//short unsigned int m= (nodes <= 100) ? nodes / 10 * 3 : 22;
		for (unsigned int i = 1; i <= nodes / 10 * 3; i++) {
/**************************************************************************/
/*Snippet: Calling the perturb function to randomly arrange the next      */
/*			Solution                                          */
/**************************************************************************/
			next_sol = perturb(cur_sol, 1);
			unsigned int next_sum = cutset(nodes, rows, next_sol, cols, values);
			cost_delta = next_sum - cur_sum;//Calculating the Cost_delta value
/**************************************************************************/
/*Snippet: Calling the accept_move function to accept or reject the next  */
/*       	Solution            			         	  */
/**************************************************************************/
			if (accept_move(cost_delta, Temp)) {
				cur_sol = next_sol;
				cur_sum = next_sum;
				goodmoves++;
			}
			iterations++;
		}
/**************************************************************************/
/*Snippet: Calling the cooldown function                                  */
/**************************************************************************/
		cool_down(Temp);
	}
/**************************************************************************/
/*Snippet: Printing The Output file                                       */
/**************************************************************************/
	std::ofstream outfile(argv[2]);
	unsigned int sum = cutset(nodes, rows, cur_sol, cols, values);
	outfile << sum << std::endl;
	for (unsigned int i = 0; i < cur_sol.size(); i++) {
		if (cur_sol[i] == 0) {
			outfile << i + 1 << " ";
		}
	}
	outfile << std::endl;
	for (unsigned int i = 0; i < cur_sol.size(); i++) {
		if (cur_sol[i] == 1) {
			outfile << i + 1 << " ";
		}
	}
	outfile << std::endl;
	outfile.close();
/**************************************************************************/
/*Snippet:Find the end execution time                                     */
/**************************************************************************/
	//auto end=std::chrono::system_clock::now();
	//std::chrono::duration<double> execution_time =end-start;
	//std::cout<<execution_time.count()/60<<"m"<<std::endl;
	//std::cout << "Iterations = " << iterations << std::endl;
/**************************************************************************/
/*Snippet:End of The Simulation Annealing 				                  */
/**************************************************************************/
	return 0;
}


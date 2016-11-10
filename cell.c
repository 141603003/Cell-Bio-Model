/*****************************************************************************

 * Copyright (C) 2016 Vinay P. Desai desaivinay1997@gmail.com COEP biologicay group
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
 *
 *****************************************************************************/
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
typedef FILE DNA;
typedef FILE LigandCommand;
typedef struct CELL{
	char name[200];       /*Name of the cell*/
	char parent[200];     /*Parent of the cell*/
	DNA* dna;             /*DNA sequence*/
	int dead;             /*Indicates whether cell is dead*/
	LigandCommand* ligand;/*ligand*/
	int ligandpointer;    /*points to next ligand command*/	
}CELL;

void copyCELL(CELL* t1, CELL* t2);
void copyDNA(char* file1, char* file2);
int cellDivisionGene(FILE* fp);
int noOfLigandCommand();
int cellDeadGene(FILE* fp);
void copyarray(CELL* a, CELL*b, int n);
int main(){

	CELL c1, child1, child2;
	CELL temp;
	char command;
	char tempstr[100];
	char ligandstr[100];
	CELL *cellarray, *tempcellarray;
	int noOfCell = 1, i, j, t, noOfCommands;
	strcpy(c1.name, "CELL 0");    /*Creating first cell*/
	strcpy(c1.parent, "Not known");
	strcpy(tempstr, "./DNA/");
	strcat(tempstr, c1.name);
	noOfCommands = noOfLigandCommand();
	c1.dna = fopen(tempstr, "r");
	strcpy(ligandstr, "./ligand/");
	strcat(ligandstr, "LIGAND");
	cellarray = (CELL*)malloc(sizeof(CELL) * 1000);
	tempcellarray = (CELL*)malloc(sizeof(CELL) * 1000);
	LigandCommand* ligand = fopen(ligandstr, "r");
	if((!ligand) || (!c1.dna)) {
		perror("ss");
		printf("No DNA or ligand file present\n");
		return 0;
	}
	c1.ligandpointer = 0;
	c1.dead = 0;
	cellarray[0] = c1;
	system("clear");
	while(1) {
		printf("===========================================================================================================================================\n\n\n\n\n");
		j = 0;
		for(i = 0; i < noOfCell; i++) {
			copyCELL(&temp, &cellarray[i]);
			fseek(ligand, temp.ligandpointer, SEEK_SET);
			if(temp.ligandpointer >= noOfCommands) {
				
				temp.dead = 0;
				copyCELL(&tempcellarray[j], &temp);
				j++;
				
			}
			else {
				fread(&command, sizeof(char), 1, ligand);
				temp.ligandpointer++;

				if(command == ' ') {/*No command*/
					copyCELL(&tempcellarray[j], &temp);
					j++;
					continue;
				}
				else if(command == 'B') {/*Cell division command*/
					printf("B (Division) command to %s\n", temp.name);
					if(cellDivisionGene(temp.dna)) { /*If the gene is found*/
						printf("CELL divided..%s\n", temp.name);

						copyCELL(&child1, &temp);/*Creating child 1*/
						strcat(child1.name, ".1");
						strcpy(child1.parent, temp.name);
						copyDNA(child1.name, temp.name);
						copyCELL(&tempcellarray[j], &child1);
						j++;
		
						copyCELL(&child2, &temp);/*Creating child 2*/
						strcat(child2.name, ".2");
						strcpy(child2.parent, temp.name);
						copyDNA(child2.name, temp.name);
						child2.ligandpointer = 0;
						copyCELL(&tempcellarray[j], &child2);
						j++;
						temp.dead = 1;/*Let the parent cell die*/
					}
					else {
						copyCELL(&tempcellarray[j], &temp);
						j++;
					}
				}
				else if(command == 'D') {/*CEll killing command*/
					printf("D (Killing) command to %s\n", temp.name);
					if(cellDeadGene(temp.dna)){
						printf("CELL dead.. %s\n", temp.name);

						temp.dead = 1;/*Indicates that the cell is dead*/
					}
					else {
						temp.dead = 0;
						copyCELL(&tempcellarray[j], &temp);
						j++;
					}
				}
			}	
		}
		copyarray(cellarray, tempcellarray, j);
		noOfCell = j;
		printf("\n%-50s%-50sSTATUS\n", "NAME", "PARENT");
		for(i = 0; i < noOfCell; i++) {
			if(!cellarray[i].dead)
				printf("%-50s%-50s LIVE\n", cellarray[i].name, cellarray[i].parent);
		}

		printf("Total cell live: %d\n", noOfCell);
		printf("\n(Press ENTER key)\n");
		getchar();
		system("clear");
	}
	return 0;
}
void copyarray(CELL* a, CELL*b, int n) {
	int i = 0;
	for(i = 0; i < n; i++)
		a[i] = b[i];

}
/*To copy cell information*/
void copyCELL(CELL* t1, CELL* t2){
	strcpy(t1->name, t2->name);
	strcpy(t1->parent, t2->parent);
	t1->dna = t2->dna;
	t1->dead = t2->dead;
	t1->ligand = t2->ligand;
	t1->ligandpointer = t2->ligandpointer;
}

/*Copies the DNA to daugter cell*/
void copyDNA(char* file1, char* file2) {

	FILE *s, *t;
	char ch;
	char f1[100];
	char f2[100];
	strcpy(f1, "./DNA/");
	strcpy(f2, "./DNA/");
	strcat(f1, file1);
	strcat(f2, file2);
	s = fopen(f2, "r");
	t = fopen(f1, "w");
	
	if(!s || !t) {
		perror("");
		printf("Error while copying dna\n");
		return;
     	}
	while(fread(&ch, sizeof(char), 1, s)) {

		fwrite(&ch, sizeof(char), 1, t);
	}
	fclose(s);
	fclose(t);
}

/*Finds whether cell dividing gene is present or not*/
int cellDivisionGene(FILE* fp){
	char str[5];
	fseek(fp,2, SEEK_SET);/*search gene at position 2*/
	fread(str, sizeof(char), 4, fp);
	str[4] = '\0';
	if(!strcmp(str, "ATGC")){ /*Cell division gene*/
		printf("%100s", "Cell division gene found\n");
		return 1;
	}
	else {
		printf("%100s", "Cell division gene not found\n");
	
		return 0;
	}
}

/*Finds whether cell dead gene is present or not*/
int cellDeadGene(FILE* fp){
	char str[5];
	fseek(fp,10, SEEK_SET);/*search gene at position 10*/
	fread(str, sizeof(char), 4, fp);
	str[4] ='\0';

	if(!strcmp(str, "ATTC")){ /*Cell killing gene*/
		printf("%100s", "Dead gene found\n");
		return 1;
	}
	else {
		printf("%100s", "Dead gene not found\n");
		return 0;
	}
}

/*Gives the number of ligand commands in ligand file*/
int noOfLigandCommand() {
	FILE *fp;
	int i = 0;
	char ch;
	fp = fopen("./ligand/LIGAND", "r");
	if(!fp) {
		printf("No ligand file\n");
		exit(1);
	}
	while(fread(&ch, sizeof(char), 1, fp)) {
		i++;
	}
	i--;
	fclose(fp);
	return i;
}

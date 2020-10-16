#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <stdio.h>
#include <string.h>

struct fifo{
	char name[20];
	struct elem* head;
	int pr;
};

struct elem{
	char name[20];
	int clients[100];
	int weight;
	struct elem* next;
};

int size = 0;
struct fifo fifos[100];
int activeF = -1;
struct elem* activeG = NULL;
int norm = -1;
int sum = -1;

void printGroups(int num);

void newFifo () {
	char enter;
	struct elem* head = (struct elem*)malloc(sizeof(struct elem));
	head->next = NULL;
	printf("Enter name of new fifo ");
	char name[20];
	scanf("%s%c", &name, &enter);
	printf("Enter priority of new fifo ");
	int pr;
	scanf("%d", &pr);
	size++;
	strcpy(fifos[size - 1].name, name);
	fifos[size - 1].pr = pr;
	fifos[size - 1].head = head;
	activeF = -1;
	activeG = NULL;
}

void printFifos () {
	for (int i = 0; i < size; i++)
	{
		printf("%d) name: %s    priority: %d\n", i, fifos[i].name, fifos[i].pr);
		printGroups(i);
	}
}

void newGroup () {
	char enter;
	struct elem* elem = (struct elem*) malloc(sizeof(struct elem));
	printf("Enter name of the group ");
	char name[20];
	scanf("%s%c", &name, &enter);
	printf("Enter weight of the group ");
	int weight;
	scanf("%d", &weight);
	strcpy(elem->name, name);
	elem->weight = weight;
	elem->next = NULL;
	for (int i = 0; i < 100; i++)
		elem->clients[i] = 0;
	printf("Enter number of the fifo that group belongs to ");
	int num;
	scanf("%d", &num);
	struct elem* current = fifos[num].head;
	while (current->next != NULL)
		current = current->next;
	current->next = elem;
	activeF = -1;
	sum = -1;
	norm = -1;
	activeG = NULL;
}

void printGroups(int num) {

	struct elem* current = fifos[num].head;
	while (current->next != NULL)
	{
		current = current->next;
		printf("   %s  (%d)   ", current->name, current->weight);
		int i = 0;
		while (current->clients[i] != 0)
		{
			printf("%d ", current->clients[i]);
			i++;
		}
		printf("\n");
	}
}

void newElem() {
	char enter;
	printf("Enter the number ");
	int ch;
	scanf("%d", &ch);
	printf("Enter number of the fifo that number belongs to ");
	int num;
	scanf("%d", &num);
	printf("Enter name of the group that number belongs to ");
	char name[20];
	scanf("%s%c", &name, &enter);
	struct elem* current = fifos[num].head;
	while (strcmp(current->name, name) != 0)
		current = current->next;
	int i = 0;
	while (current->clients[i] != 0)
		i++;
	current->clients[i] = ch;
	activeF = -1;
	sum = -1;
	norm = -1;
	activeG = NULL;
}

void getElem() {
	if (activeF == -1) 
	{
		for (int i = 0; i <= size - 1; i++)
			if (((activeF == -1) || (fifos[i].pr < fifos[activeF].pr)) &&
				(fifos[i].head->next != NULL))
			{
				struct elem* current = fifos[i].head;
				int flag = 0;
				while ((current->next != NULL) && (flag == 0))
				{
					current = current->next;
					if (current->clients[0] != 0) flag = 1;
				}
				if (flag == 1) activeF = i;
			}
		if (activeF == -1)
		{
			printf("No elements!\n");
			return;
		}

		sum = 0;
		struct elem* current = fifos[activeF].head;
		while (current->next != NULL)
		{
			current = current->next;
			sum += current->weight;
		}
    }

	if (activeG == NULL)
	{
		activeG = fifos[activeF].head->next;
		while (activeG->clients[0] == 0)
			activeG = activeG->next;
		norm = ceil(activeG->weight*10/sum);
	}

	printf("%d (%s %s)\n", activeG->clients[0], fifos[activeF].name, activeG->name);
	norm--;
	activeG->clients[0] = 0;
	int i = 0;
	while (activeG->clients[i + 1] != 0)
	{
		activeG->clients[i] = activeG->clients[i + 1];
		i++;
	}
	activeG->clients[i] = 0;
	if ((i == 0) || (norm == 0))
	{
		if (activeG->next != NULL)
			activeG = activeG->next; else
			{
				activeF = -1;
				sum = -1;
				norm = -1;
				activeG = NULL;
			}
		if (activeG != NULL)
		{
			while ((activeG->clients[0] == 0) && (activeG->next != NULL))
				activeG = activeG->next;
			if (activeG->clients[0] != 0) norm = ceil(activeG->weight * 10 / sum); else
			{
				activeF = -1;
				sum = -1;
				norm = -1;
				activeG = NULL;
			}
		}
	}
}

void main()
{
	int choose;
	int exit = 0;

	while (exit == 0) {
		printf("(1) add fifo   (2) add group   (3) add elem   (4) print fifos   (5) get elem   (6) exit\n");
		scanf("%d", &choose);
		switch (choose) {
		case 1: newFifo(); break;
		case 2: newGroup(); break;
		case 3: newElem(); break;
		case 4: printFifos(); break;
		case 5: getElem(); break;
		case 6: exit = 1;
		}
	}

}

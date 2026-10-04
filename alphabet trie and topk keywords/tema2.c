// Savlovschi Andrei-Bogdan 311CB
// 117/120 points
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct filedata {
	char *ID;
	char **keywords;
	int score;
	int nr_keywords;
} TFileData;

typedef struct NextPrevCell {
	struct NextPrevCell *prev, *next;
	TFileData file;
} TNextPrevCell, *TNextPrevList;

typedef struct start_end_list {
	TNextPrevCell *start, *end;
} *TStart_End_List;

typedef struct pointer_list {
	struct pointer_list *next;
	TNextPrevList pointer;
} TPCell, *TPList;

typedef struct multicai {
	struct multicai *down, *right;
	char letter;
	TPList list;
} TTCell, *Ttree;

typedef int (*TFCmp)(TNextPrevList, TNextPrevList);

typedef struct Heap {
	int nrMax, nrElem;
	TNextPrevList *v;
	TFCmp comp;
} THeap;


int Compare_heap(TNextPrevList a, TNextPrevList b) {
	if (a->file.score != b->file.score)
		return a->file.score > b->file.score;
	return strcmp(a->file.ID, b->file.ID) < 0;
}

void Insert_heap(THeap *h, TNextPrevList val) {
	TNextPrevList current, parent;
	h->nrElem++;
	int pos = h->nrElem - 1;
	h->v[pos] = val;


	current = val;
	parent = h->v[(pos - 1) / 2];
	while (pos > 0 && h->comp(current, parent)) {
		h->v[pos] = parent;
		h->v[(pos - 1) / 2] = current;

		pos = (pos - 1) / 2;
		// current = parent;
		parent = h->v[(pos - 1) / 2];
	}


	return;
}

TNextPrevList ExtrHeap(THeap *h) {
	TNextPrevList result = h->v[0];
	h->v[0] = h->v[h->nrElem - 1];
	h->v[h->nrElem - 1] = NULL;
	h->nrElem--;

	int current_pos = 0;
	TNextPrevList copil_st = h->v[current_pos * 2 + 1];
	TNextPrevList copil_dr = h->v[current_pos * 2 + 2];

	while (current_pos * 2 + 1 < h->nrElem && h->comp(copil_st, h->v[current_pos])) {

		if (current_pos * 2 + 2 < h->nrElem &&  h->comp(copil_dr, copil_st)) {
			h->v[current_pos * 2 + 2] = h->v[current_pos];
			h->v[current_pos] = copil_dr;
			current_pos = current_pos * 2 + 2;

		} else {
			h->v[current_pos * 2 + 1] = h->v[current_pos];
			h->v[current_pos] = copil_st;
			current_pos = current_pos * 2 + 1;
		}


		copil_st = h->v[current_pos * 2 + 1];
		copil_dr = h->v[current_pos * 2 + 2];
	}

	return result;
}

THeap* Allocate_Heap(int nrMax, TFCmp comp) {
	THeap* h = (THeap*) malloc(sizeof(struct Heap));
	if (!h) {
		return NULL;
	}

	h->v = (TNextPrevList *) calloc(nrMax, sizeof(TNextPrevList));
	if (!h->v) {
		free(h);
		return NULL;
	}

	h->nrMax = nrMax;
	h->nrElem = 0;
	h->comp = comp;

	return h;
}

void Destroy_Heap(THeap **h) {
	free((*h)->v);
	free(*h);
	*h = NULL;
}

TStart_End_List allocate_Start_End_List() {
	TStart_End_List s_e_list = (TStart_End_List)calloc(2, sizeof(TNextPrevList));
	if (!s_e_list) {
		perror("Allocation failed");
		exit(77);
	}
	s_e_list->start = NULL;
	s_e_list->end = NULL;

	return s_e_list;
}

TNextPrevList Aloc_NextPrevCell(TFileData new_file) {
	TNextPrevList new_cell = (TNextPrevList)malloc(sizeof(TNextPrevCell));
	if (!new_cell) {
		perror("Allocation failed");
		exit(77);
	}

	new_cell->file = new_file;
	new_cell->next = NULL;
	new_cell->prev = NULL;

	return new_cell;
}

TPList allocate_list_cell(TNextPrevList list_pointer) {
	TPList cell = (TPList)calloc(1, sizeof(TPCell));
	if (!cell) {
		perror("Allocation failed");
		exit(77);
	}
	cell->pointer = list_pointer;
	return cell;
}

char **allocate_keywords(int number_kw) {

	char **keywords = (char **)malloc(number_kw * sizeof(char *));
	if (!keywords) {
		perror("Allocation failed");
		exit(77);
	}
	for (int i = 0; i < number_kw; i++) {
		keywords[i] = (char *)calloc(100, 1);
		if (!keywords[i]) {
			perror("Allocation failed");
			for (int j = 0; j < i; j++) {
				free(keywords[j]);
			}
			exit(77);
		}
	}
	return keywords;
}

char *allocate_buffer() {
	char *buffer = (char *)calloc(100, 1);
	if (!buffer) {
		perror("Allocation failed");
		exit(77);
	}
	return buffer;
}

Ttree Init_tree() {
	Ttree tree = (Ttree)calloc(1, sizeof(TTCell));
	if (!tree) {
		perror("Allocation failed");
		exit(77);
	}
	return tree;
}

int add_file_to_list (TStart_End_List s_e_list, TFileData new_file) {

	// if first cell
	if (s_e_list->end == NULL) {
		s_e_list->end = Aloc_NextPrevCell(new_file);
		s_e_list->start = s_e_list->end;
		return 1;
	}
	//else
	TNextPrevList aux = s_e_list->start;
	// if it already exists
	while (aux != s_e_list->end) {
		if (strcmp(aux->file.ID, new_file.ID) == 0) {
			for (int i = 0; i < new_file.nr_keywords; i++)
				free(new_file.keywords[i]);
			free(new_file.keywords);
			free(new_file.ID);
			return 0;
		}
		aux = aux->next;
	}
		if (strcmp(aux->file.ID, new_file.ID) == 0) {
			for (int i = 0; i < new_file.nr_keywords; i++)
				free(new_file.keywords[i]);
			free(new_file.keywords);
			free(new_file.ID);
			return 0;
		}

	aux = s_e_list->start;
	while (aux->next != NULL) {
		if (strcmp(new_file.ID, aux->next->file.ID) > 0)
			break;
		aux = aux->next;
	}

	s_e_list->end->next = Aloc_NextPrevCell(new_file);
	s_e_list->end->next->prev = s_e_list->end;
	s_e_list->end = s_e_list->end->next;
	return 1;
}

void destroy_plist(TPList plist) {
	TPList aux;
	while (plist) {
		aux = plist;
		plist = plist->next;
		free(aux);
	}
}

void destroy_tree(Ttree tree) {
	if (tree == NULL)
		return;
	destroy_plist(tree->list);
	tree->list = NULL;
	destroy_tree(tree->down);
	destroy_tree(tree->right);
	free(tree);
}

void destroy_NextPrevList (TStart_End_List s_e_list) {

	TNextPrevList start_cell = s_e_list->start;
	TNextPrevList end_cell = s_e_list->end;
	TNextPrevList aux;

	if (start_cell == NULL)
		return;


	while (start_cell != end_cell) {

		for (int i = 0; i <start_cell->file.nr_keywords; i++) {
			free(start_cell->file.keywords[i]);
		}
		free(start_cell->file.keywords);
		free(start_cell->file.ID);
		aux = start_cell;
		start_cell = start_cell->next;
		free(aux);

	}
	// last cell
	free(start_cell->file.ID);
	for (int i = 0; i <start_cell->file.nr_keywords; i++) {
		free(start_cell->file.keywords[i]);
	}
	free(start_cell->file.keywords);
	aux = start_cell;
	free(aux);


	s_e_list->end = NULL;
	s_e_list->start = NULL;
}

Ttree allocate_tree_cell(char letter) {
	Ttree cell = (Ttree)calloc(1, sizeof(TTCell));
	if (!cell) {
		perror("Allocation failed");
		exit(77);
	}
	cell->letter = letter;
	return cell;
}

void add_keyword_to_tree (Ttree tree, char *keyword) {
	if (keyword[0] == 0)
		return;
	if (tree->down == NULL) {
		tree->down = allocate_tree_cell(keyword[0]);
		add_keyword_to_tree(tree->down, &keyword[1]);
	}
	Ttree aux = tree->down;
	while (aux) {
		if (aux->letter == keyword[0]) {
			add_keyword_to_tree(aux, &keyword[1]);
			break;
		}
		aux = aux->right;
	}
	if (aux == NULL) {
		aux = tree->down;
		if (aux->letter - keyword[0] > 0) {
			Ttree cell = allocate_tree_cell(keyword[0]);
			cell->right = aux;
			tree->down = cell;
			add_keyword_to_tree(cell, &keyword[1]);
			return;
		}

		while (aux->right) {
			if (aux->right->letter - keyword[0] > 0)
				break;
			aux = aux->right;
		}
		Ttree cell = allocate_tree_cell(keyword[0]);
		cell->right = aux->right;
		aux->right = cell;

		add_keyword_to_tree(cell, &keyword[1]);
	}
}

TPList Insert_prefix_list (TPList plist, TNextPrevList list_pointer) {
	if (plist == NULL) {
		plist = allocate_list_cell(list_pointer);
		return plist;
	}
	TPList aux = plist;
	if (strcmp(plist->pointer->file.ID, list_pointer->file.ID) > 0) {
		TPList cell = allocate_list_cell(list_pointer);
		cell->next = plist;
		return cell;
	}
	if (strcmp(plist->pointer->file.ID, list_pointer->file.ID) == 0) {
			return aux;
	}
	while (plist->next) {
		if (strcmp(plist->next->pointer->file.ID, list_pointer->file.ID) > 0) {
			break;
		}
		if (strcmp(plist->next->pointer->file.ID, list_pointer->file.ID) == 0) {
			return aux;
		}
		plist = plist->next;
	}
	TPList cell = allocate_list_cell(list_pointer);
	cell->next = plist->next;
	plist->next = cell;
	return aux;
}

TPList Insert_list(TPList plist, TNextPrevList list_pointer) {
	if (plist == NULL) {
		plist = allocate_list_cell(list_pointer);
		return plist;
	}
	TPList aux = plist;
	if (strcmp(plist->pointer->file.ID, list_pointer->file.ID) > 0) {
		TPList cell = allocate_list_cell(list_pointer);
		cell->next = plist;
		return cell;
	}
	while (plist->next) {
		if (strcmp(plist->next->pointer->file.ID, list_pointer->file.ID) > 0) {
			break;
		}
		plist = plist->next;
	}
	TPList cell = allocate_list_cell(list_pointer);
	cell->next = plist->next;
	plist->next = cell;
	return aux;
}

TPList delete_from_list(TPList plist, TNextPrevList list_pointer) {
	if (plist == NULL)
		printf("how did we end up here\n");

	TPList start = plist;
	if (plist->pointer == list_pointer) {
		plist = plist->next;
		free(start);
		return plist;
	}
	while (plist->next) {
		if (plist->next->pointer == list_pointer) {
			break;
		}
		plist = plist->next;
	}
	TPList aux = plist->next;
	plist->next = aux->next;
	free(aux);



	return start;
}

void add_refrence(Ttree tree, char *keyword, int level, TNextPrevList list_pointer) {
	if (tree == NULL && level == 0)
		return;
	while (tree->letter != keyword[level]) {
		tree = tree->right;
	}
	if ((int)strlen(keyword) == level + 1) {
		tree->list = Insert_list(tree->list, list_pointer);
		return;
	}
	add_refrence(tree->down, keyword, level + 1, list_pointer);

}

void delete_refrence(Ttree tree, char *keyword, int level, TNextPrevList cell) {
	if (tree == NULL && level == 0)
		return;
	while (tree->letter != keyword[level]) {
		tree = tree->right;
	}
	if ((int)strlen(keyword) == level + 1) {
		tree->list = delete_from_list(tree->list, cell);
		return;
	}
	delete_refrence(tree->down, keyword, level + 1, cell);
}

void ADD(TStart_End_List s_e_list, Ttree tree, FILE *input, FILE *output) {

	TFileData new_file;

	new_file.ID = (char *)calloc(100, 1);
	if (!new_file.ID) {
		perror("Allocation failed");
		exit(77);
	}

	fscanf(input, "%s", new_file.ID);

	fscanf(input, "%d", &new_file.score);

	fscanf(input, "%d", &new_file.nr_keywords);
	new_file.keywords = allocate_keywords(new_file.nr_keywords);

	for (int i = 0; i < new_file.nr_keywords; i++) {
		fscanf(input, "%s", new_file.keywords[i]);
	}

	if (add_file_to_list(s_e_list, new_file)) {
		fprintf (output, "OK\n");
	} else {
		fprintf (output, "EXISTS\n");
		return;
	}

	int k = 0;
	for (int i = 0; i < new_file.nr_keywords; i++) {
		k = 0;
		for (int j = 0; j < i; j++) {
			if (strcmp(new_file.keywords[j], new_file.keywords[i]) == 0)
				k = 1;
		}
		if (k == 0) {
			add_keyword_to_tree(tree, new_file.keywords[i]);
			add_refrence(tree->down, new_file.keywords[i], 0, s_e_list->end);
		}

	}

}

void remove_empty_branches(Ttree *tree_pointer) {
	Ttree tree = *tree_pointer;

	if (tree == NULL)
		return;

	remove_empty_branches(&(tree->down));
	remove_empty_branches(&(tree->right));

	if (tree->down == NULL && tree->list == NULL) {
		*tree_pointer = tree->right;
		free(tree);
	}

}

void DEL(TStart_End_List s_e_list, Ttree tree, FILE *input, FILE *output) {
	char *buffer = allocate_buffer();
	fscanf(input, "%s", buffer);

	TNextPrevList start = s_e_list->start;
	TNextPrevList end = s_e_list->end;
	if (start == NULL) {
		fprintf(output, "NOT FOUND\n");
		free(buffer);
		return;
	}

	if (strcmp(start->file.ID, buffer) == 0) {
		for (int i = 0; i < start->file.nr_keywords; i++) {
			delete_refrence(tree->down, start->file.keywords[i] , 0, start);
			remove_empty_branches(&(tree->down));
		}
		if (start->next == NULL)
			s_e_list->end = NULL;
		s_e_list->start = start->next;
		free(start->file.ID);
		for (int i = 0; i < start->file.nr_keywords; i++) {
			free(start->file.keywords[i]);
		}
		free(start->file.keywords);
		free(start);
		fprintf(output, "OK\n");
		free(buffer);
		return;
	}

	while (start != end) {
		if (strcmp(start->file.ID, buffer) == 0)
			break;
		start = start->next;
	}
	if (strcmp(start->file.ID, buffer) != 0) {
		fprintf(output, "NOT FOUND\n");
		free(buffer);
		return;
	}
	for (int i = 0; i < start->file.nr_keywords; i++) {
		delete_refrence(tree->down, start->file.keywords[i] , 0, start);
		remove_empty_branches(&tree);
	}
	TNextPrevList previous = start->prev;
	previous->next = start->next;
	if (start->next)
		start->next->prev = previous; // if not last file
	else
		s_e_list->end = previous; //if last file

	free(start->file.ID);
	for (int i = 0; i < start->file.nr_keywords; i++) {
		free(start->file.keywords[i]);
	}
	free(start->file.keywords);
	free(start);


	fprintf(output, "OK\n");
	free(buffer);
}

void ADDKW(TStart_End_List s_e_list, Ttree tree, FILE *input, FILE *output) {

	char *new_kw = allocate_buffer();
	char *file_name = allocate_buffer();
	fscanf(input, "%s%s", file_name, new_kw);

	TNextPrevList file_cell = s_e_list->start;
	TNextPrevList end = s_e_list->end;
	while (file_cell != end) {
		if (strcmp(file_cell->file.ID, file_name) == 0)
			break;
		file_cell = file_cell->next;
	}
	// if file doesn't exist
	if (file_cell == NULL ||strcmp(file_cell->file.ID, file_name) != 0) {
		fprintf(output, "NOT FOUND\n");
		free(file_name);
		free(new_kw);
		return;
	}
	// if keyword exists
	for (int i = 0; i < file_cell->file.nr_keywords; i++) {
		if (strcmp(new_kw, file_cell->file.keywords[i]) == 0) {
			fprintf(output, "OK\n");
			free(file_name);
			free(new_kw);
			return;
		}
	}
	// reallocating kw memory
	file_cell->file.nr_keywords++;
	file_cell->file.keywords = realloc(file_cell->file.keywords, file_cell->file.nr_keywords * sizeof(char *));
	file_cell->file.keywords[file_cell->file.nr_keywords - 1] = (char *)calloc(100, 1);
	strcpy(file_cell->file.keywords[file_cell->file.nr_keywords - 1], new_kw);

	add_keyword_to_tree(tree, new_kw);
	add_refrence(tree->down, new_kw, 0, file_cell);
	fprintf(output, "OK\n");

	free(file_name);
	free(new_kw);
}

void DELKW(TStart_End_List s_e_list, Ttree tree, FILE *input, FILE *output) {
	char *rem_kw = allocate_buffer();
	char *file_name = allocate_buffer();
	fscanf(input, "%s%s", file_name, rem_kw);

	TNextPrevList file_cell = s_e_list->start;
	TNextPrevList end = s_e_list->end;
	while (file_cell != end) {
		if (strcmp(file_cell->file.ID, file_name) == 0)
			break;
		file_cell = file_cell->next;
	}
	// if file doesn't exist
	if (strcmp(file_cell->file.ID, file_name) != 0) {
		fprintf(output, "NOT FOUND\n");
		free(file_name);
		free(rem_kw);
		return;
	}
	// if keyword exists
	for (int i = 0; i < file_cell->file.nr_keywords; i++) {
		if (strcmp(rem_kw, file_cell->file.keywords[i]) == 0) {
			delete_refrence(tree->down, file_cell->file.keywords[i] , 0, file_cell);

			strcpy(file_cell->file.keywords[i], file_cell->file.keywords[file_cell->file.nr_keywords - 1]);
			free(file_cell->file.keywords[file_cell->file.nr_keywords - 1]);
			file_cell->file.nr_keywords--;
			remove_empty_branches(&tree);
			fprintf(output, "OK\n");
			free(file_name);
			free(rem_kw);
			return;
		}
	}

	fprintf(output, "OK\n");
	free(file_name);
	free(rem_kw);
}

void FIND(Ttree tree, FILE *input, FILE *output) {
	char keyword[100];
	char my_keyword[100]; // the keyword I am building from the tree
	fscanf(input, "%s", keyword);

	Ttree last_tree;

	int len = strlen(keyword);
	for (int i = 0; i < len; i++) {
		while (tree) {
			if (tree->letter == keyword[i]) {
				my_keyword[i] = tree->letter;
				last_tree = tree;
				tree = tree->down;
				break;
			}
			tree = tree->right;
		}
	}
	my_keyword[len] = 0;
	int counter = 0;
	if (strcmp(my_keyword, keyword) == 0) {
		tree = last_tree;
		for(TPList i = tree->list; i != NULL; i = i->next)
			counter++;
		fprintf(output, "%d ", counter);

		for(TPList i = tree->list; i != NULL; i = i->next)
			fprintf(output, "%s ", i->pointer->file.ID);
		fprintf(output, "\n");
	} else {
		fprintf(output, "EMPTY\n");
	}

}

//almost the same with find until the word has been found in the tree
void TOPK(Ttree tree, FILE *input, FILE *output) {
	char keyword[100];
	char my_keyword[100]; // the keyword I am building from the tree
	int nr_topk;
	fscanf(input, "%s%d", keyword, &nr_topk);

	Ttree last_tree;

	int len = strlen(keyword);
	for (int i = 0; i < len; i++) {
		while (tree) {
			if (tree->letter == keyword[i]) {
				my_keyword[i] = tree->letter;
				last_tree = tree;
				tree = tree->down;
				break;
			}
			tree = tree->right;
		}
	}
	my_keyword[len] = 0;
	int total_refrences = 0;
	if (strcmp(my_keyword, keyword) == 0) {
		tree = last_tree;
		for(TPList i = tree->list; i != NULL; i = i->next)
			total_refrences++;
		THeap *heap = Allocate_Heap(100, Compare_heap);

		for (TPList i = tree->list; i != NULL; i = i->next)
			Insert_heap(heap, i->pointer);

		if (nr_topk > total_refrences)
			nr_topk = total_refrences;
		if (nr_topk)
			fprintf(output, "%d ", nr_topk);
		else
			fprintf(output, "EMPTY\n");

		TNextPrevList refrence;
		for (int i = 0; i < nr_topk; i++) {
			refrence = ExtrHeap(heap);
			fprintf(output, "%s ", refrence->file.ID);
		}
		fprintf(output, "\n");

		Destroy_Heap(&heap);

	} else {
		fprintf(output, "EMPTY\n");
	}

}

void PRINT(Ttree tree, char *buffer, int level, FILE *output) {
	if (!tree){
		return;
	}
	buffer[level] = tree->letter;
	int counter = 0;
	if (tree->list) {

		buffer[level + 1] = 0;
		fprintf(output, "%s ", buffer);

		for(TPList i = tree->list; i != NULL; i = i->next)
			counter++;
		fprintf(output, "%d ", counter);

		for(TPList i = tree->list; i != NULL; i = i->next)
			fprintf(output, "%s ", i->pointer->file.ID);
		fprintf(output, "\n");
	}
	PRINT(tree->down, buffer, level + 1, output);
	PRINT(tree->right, buffer, level, output);

}

void PRINT_prefix_variation(Ttree tree, TPList *prefix_list, FILE *output) {
	if (!tree){
		return;
	}
	if (tree->list)
		for(TPList i = tree->list; i != NULL; i = i->next)
			*prefix_list = Insert_prefix_list(*prefix_list, i->pointer);

	PRINT_prefix_variation(tree->down, prefix_list, output);
	PRINT_prefix_variation(tree->right, prefix_list, output);

}

// PREFIX is just reusing code from the other operations:
// the code from "FIND prefix" gets the location of the prefix in the tree;
// using that, with the algorithm from print,
// I make a separate list of all the refrences to the files,
// excluding the repeating ones
// (almost the same as when adding a keyword to the tree)
void PREFIX(Ttree tree, FILE *input, FILE *output) {
	char keyword[100];
	char my_keyword[100]; // the keyword I am building from the tree
	fscanf(input, "%s", keyword);

	Ttree last_tree;

	int len = strlen(keyword);
	for (int i = 0; i < len; i++) {
		while (tree) {
			if (tree->letter == keyword[i]) {
				my_keyword[i] = tree->letter;
				last_tree = tree;
				tree = tree->down;
				break;
			}
			tree = tree->right;
		}
	}
	my_keyword[len] = 0;
	int counter = 0;
	TPList prefix_list = allocate_list_cell(NULL);
	if (strcmp(my_keyword, keyword) == 0) {
		for(TPList i = last_tree->list; i != NULL; i = i->next)
			prefix_list->next = Insert_prefix_list(prefix_list->next, i->pointer);
		PRINT_prefix_variation(tree, &(prefix_list->next), output);

		for(TPList i = prefix_list->next; i != NULL; i = i->next)
			counter++;
		fprintf(output, "%d ", counter);

		for(TPList i = prefix_list->next; i != NULL; i = i->next)
			fprintf(output, "%s ", i->pointer->file.ID);
		fprintf(output, "\n");

		destroy_plist(prefix_list);
	} else {
		free(prefix_list);
		fprintf(output, "EMPTY\n");
	}

}

int main(void) {

	TStart_End_List s_e_list = allocate_Start_End_List();
	Ttree tree = Init_tree();
	FILE *input = fopen("indexare.in", "rt");
	FILE *output = fopen("indexare.out", "wt");
	char command[10];
	int nr_commands;
	fscanf(input, "%d", &nr_commands);


	for(int i = 0; i < nr_commands; i++) {
		fscanf(input, "%s", command);

		if (strcmp("ADD", command) == 0) {
			ADD(s_e_list, tree, input, output);
		} else if (strcmp("DEL", command) == 0) {
			DEL(s_e_list, tree, input, output);
		} else if (strcmp("ADDKW", command) == 0) {
			ADDKW(s_e_list, tree, input, output);
		} else if (strcmp("DELKW", command) == 0) {
			DELKW(s_e_list, tree, input, output);
		} else if (strcmp("FIND", command) == 0) {
			FIND(tree->down, input, output);
		} else if (strcmp("TOPK", command) == 0) {
			TOPK(tree->down, input, output);
		} else if (strcmp("PREFIX", command) == 0) {
			PREFIX(tree->down, input, output);
		} else if (strcmp("PRINT", command) == 0) {
			if (tree->down == NULL)
				fprintf(output, "EMPTY\n");
			char *buffer = allocate_buffer();
			PRINT(tree->down, buffer, 0, output);
			free(buffer);
		}

	}

	fclose(input);
	fclose(output);
	destroy_tree(tree);
	destroy_NextPrevList(s_e_list);
	free(s_e_list);
	return 0;
}

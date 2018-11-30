/**
 * Represents the server for the game "tic tac toe".
 * Using char* array[] to represent the game board.
 * Using Minimax algorithm to get the next move.
 * In this way, the server will never lose the game.
 */
#include <stdio.h>
#include <ulfius.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

#define PORT 8080
#define BOARD_SIZE 9
#define FIRST_CHESS "x"
#define SECOND_CHESS "o"
#define EMPTY " "
#define CLIENT_FIRST 1
#define SERVER_FIRST 0
#define WIN 10
#define LOSE -10
#define DRAW 0
#define DUMB_CELL -1
#define UPPER_BOUND 20;
#define LOWER_BOUND -20;

// Define a structure to store move information.
struct move {
	int score;
	int index;
};
typedef struct move Move;

void array_to_json(json_auto_t * json_body, char* array[], int client_first);
void json_to_array(json_auto_t * json_body, char* array[]);

Move next_move(char* array[], char* player, char* client_chess, char* server_chess);

/**
 * Gets the board from the client, and if the game is not over,
 * using minimax algorithm to get the next move, put the chess,
 * and pass the new board to the client.
 */
int callback_process_order(const struct _u_request * request, struct _u_response * response, void * user_data) {
  json_auto_t * json_board = ulfius_get_json_body_request(request, NULL);
  int who_first = json_integer_value(json_object_get(json_board, "clientFirst"));
  char* client_chess;
  char* server_chess;
  if (who_first == 1) {
  	client_chess = "x";
  	server_chess = "o";
  } else {
  	client_chess = "o";
  	server_chess = "x";
  }
  char* player = server_chess;
  char* board[BOARD_SIZE];
  json_to_array(json_board, board);
  Move next = next_move(board,player,client_chess,server_chess);
  board[next.index] = player;
  json_auto_t *new_board_json = json_object();
  array_to_json(new_board_json, board, who_first);

  ulfius_set_json_body_response(response, 200, new_board_json);

  return U_CALLBACK_CONTINUE;
}

/**
 * If server goes first, randomly chooses a cell, puts the chess in the cell,
 * passes the board into json object and sends it to client.
 */
int callback_create_order(const struct _u_request * request, struct _u_response * response, void * user_data) {
	srand(time(NULL));  
	int r = rand() % BOARD_SIZE; 
	char* result[BOARD_SIZE];
	int i;
	for (i = 0; i < BOARD_SIZE; i++) {
		if (i == 7) {
			result[i] = FIRST_CHESS;
		} else {
			result[i] = " ";
		}		
	}

  json_auto_t * board = json_object();

  array_to_json(board, result, SERVER_FIRST);
 
  ulfius_set_json_body_response(response, 200, board);

  return U_CALLBACK_CONTINUE;
}

/**
 * Fills a json object with char* array[] and int who_first.
 * Input: json object, char* array[] and int who_first
 */
void array_to_json(json_auto_t * json_body, char* array[], int who_first) {
	json_auto_t * json_cell1 = json_string(array[0]);
	json_auto_t * json_cell2 = json_string(array[1]);
	json_auto_t * json_cell3 = json_string(array[2]);
	json_auto_t * json_cell4 = json_string(array[3]);
	json_auto_t * json_cell5 = json_string(array[4]);
	json_auto_t * json_cell6 = json_string(array[5]);
	json_auto_t * json_cell7 = json_string(array[6]);
	json_auto_t * json_cell8 = json_string(array[7]);
	json_auto_t * json_cell9 = json_string(array[8]);
	json_auto_t * json_who_first = json_integer(who_first);
	json_object_set(json_body,"c1",json_cell1);
	json_object_set(json_body,"c2",json_cell2);
	json_object_set(json_body,"c3",json_cell3);
	json_object_set(json_body,"c4",json_cell4);
	json_object_set(json_body,"c5",json_cell5);
	json_object_set(json_body,"c6",json_cell6);
	json_object_set(json_body,"c7",json_cell7);
	json_object_set(json_body,"c8",json_cell8);
	json_object_set(json_body,"c9",json_cell9);
	json_object_set(json_body,"clientFirst",json_who_first);
}

/**
 * Fills a char* array[] with a json object.
 * Input: json object, char* array[]
 */
void json_to_array(json_auto_t * json_body, char* array[]) {
  
	array[0] = (char*)json_string_value(json_object_get(json_body,"c1"));
	array[1] = (char*)json_string_value(json_object_get(json_body,"c2"));
	array[2] = (char*)json_string_value(json_object_get(json_body,"c3"));
	array[3] = (char*)json_string_value(json_object_get(json_body,"c4"));
	array[4] = (char*)json_string_value(json_object_get(json_body,"c5"));
	array[5] = (char*)json_string_value(json_object_get(json_body,"c6"));
	array[6] = (char*)json_string_value(json_object_get(json_body,"c7"));
	array[7] = (char*)json_string_value(json_object_get(json_body,"c8"));
	array[8] = (char*)json_string_value(json_object_get(json_body,"c9"));

}

/**
 * Tells if one player has won the game.
 * Input: char* array[] to represents the board and char* player to represent
 * current player's chess
 * Returns: 1 if the current player has one the game, and 0 otherwise
 */
int has_won(char* array[], char* player) {
	if ((strcmp(array[0],player) == 0 && strcmp(array[1],player) == 0
		&& strcmp(array[2],player) == 0) ||
		(strcmp(array[3],player) == 0 && strcmp(array[4],player) == 0
		&& strcmp(array[5],player) == 0) ||
		(strcmp(array[6],player) == 0 && strcmp(array[7],player) == 0
		&& strcmp(array[8],player) == 0) ||
		(strcmp(array[0],player) == 0 && strcmp(array[3],player) == 0
		&& strcmp(array[6],player) == 0) ||
		(strcmp(array[1],player) == 0 && strcmp(array[4],player) == 0
		&& strcmp(array[7],player) == 0) ||
		(strcmp(array[2],player) == 0 && strcmp(array[5],player) == 0
		&& strcmp(array[8],player) == 0) ||
		(strcmp(array[0],player) == 0 && strcmp(array[4],player) == 0
		&& strcmp(array[8],player) == 0) ||
		(strcmp(array[2],player) == 0 && strcmp(array[4],player) == 0
		&& strcmp(array[6],player) == 0)){
  		return 1;
  	}
  	return 0;
}

/**
 * Counts the empty cells of a board.
 * Input: char* array[] to represents the board 
 * Returns: the number of empty cells
 */
int count_empty_cells(char* array[]) {
	int i;
	int count = 0;
	for (i = 0; i < BOARD_SIZE; i++) {
		if (strcmp(array[i], FIRST_CHESS) != 0 && strcmp(array[i], SECOND_CHESS) != 0) {
			count++;
		}
	}
	return count;
}

/**
 * Gets the best move using Minimax algorithm.
 * Input: char* array[] representing the board, char* player representing
 * the current player's chess, char* client_chess and char* server_chess
 * Returns: the next best move
 */
Move next_move(char* array[], char* player, char* client_chess, char* server_chess) {
	int empty_cells = count_empty_cells(array);
	
	if (has_won(array,player) == 1) {
		if (strcmp(player,client_chess) == 0){
			Move move = {LOSE, DUMB_CELL};
			return move;
		}
		if (strcmp(player,server_chess) == 0){
			Move move = {WIN, DUMB_CELL};
			return move;
		}
	}

	if (empty_cells == 0) {
		Move move = {DRAW, DUMB_CELL};
		return move;
	}

	Move moves[empty_cells];
	int moves_index = 0;
	int i;
	for (i = 0; i < BOARD_SIZE; i++) {
		if (strcmp(array[i], FIRST_CHESS) != 0 && strcmp(array[i], SECOND_CHESS) != 0) {
			array[i] = player;
			if (strcmp(player, client_chess) == 0) {
				Move move = next_move(array, server_chess, client_chess, server_chess);
				move.index = i;
				moves[moves_index] = move;
				moves_index++;
			  array[i] = EMPTY;
			} else {
				Move move = next_move(array, client_chess, client_chess, server_chess);
				move.index = i;
				moves[moves_index] = move;
				moves_index++;
			  array[i] = EMPTY;
			}

		}
	}

	Move best_move;
	if (strcmp(player, client_chess) == 0) {
		int best_score = UPPER_BOUND;
		for (i = 0; i < empty_cells; i++) {
			if (moves[i].score < best_score) {
				best_score = moves[i].score;
				best_move = moves[i];
			}
		}
	} else {
		int best_score = LOWER_BOUND;
		for (i = 0; i < empty_cells; i++) {
			if (moves[i].score > best_score) {
				best_score = moves[i].score;
				best_move = moves[i];
			}
		}
	}
  return best_move;
}

/**
 * main function to run the server.
 */
int main(void) {
  struct _u_instance instance;

  // Initialize instance with the port number
  if (ulfius_init_instance(&instance, PORT, NULL, NULL) != U_OK) {
    fprintf(stderr, "Error ulfius_init_instance, abort\n");
    return(1);
  }

  // Endpoint list declaration
  ulfius_add_endpoint_by_val(&instance, "GET", "/helloworld", NULL, 0, &callback_create_order, NULL);

  // Endpoint list declaration
  ulfius_add_endpoint_by_val(&instance, "POST", "/helloworld", NULL, 0, &callback_process_order, NULL);

  // Start the framework
  if (ulfius_start_framework(&instance) == U_OK) {
    printf("Start framework on port %d\n", instance.port);

    printf("Press <enter> to exit the server.\n");

    // Wait for the user to press <enter> on the console to quit the application
    getchar();
   

  } else {
    fprintf(stderr, "Error starting framework\n");
  }
  printf("End framework\n");

  ulfius_stop_framework(&instance);
  ulfius_clean_instance(&instance);

  return 0;
}

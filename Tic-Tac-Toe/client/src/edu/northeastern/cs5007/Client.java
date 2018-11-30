package edu.northeastern.cs5007;

import com.google.gson.Gson;
import com.google.gson.GsonBuilder;
import java.io.*;
import java.util.Random;
import java.util.Scanner;
import org.apache.http.HttpEntity;
import org.apache.http.HttpResponse;
import org.apache.http.entity.StringEntity;
import org.apache.http.client.HttpClient;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.impl.client.DefaultHttpClient;

/* Represents the client class for "tic tac toe"(using inner class Board to
   represents the game board), which will do the following things:
(1) Randomly choose who goes first;
(2) Get user input as next move;
(3) make one move, update the board, transfer board to Gson and pass it to server;
(4) receive json from server, transfer to board and make the next move;
(5) check if the game is over and the result.
*/

public class Client {
	private static final int BASE = 100;
	private static final char FIRST_CHESS = 'x';
	private static final char SECOND_CHESS = 'o';
	private static final int CLIENT_FIRST = 1;
	private static final int SERVER_FIRST = 0;
	private static Board newBoard = new Board(CLIENT_FIRST);
	private static char clientChess;
	private static char serverChess;
	private static int inputRow;
	private static int inputColumn;
	
  private static String URL = "http://localhost:8080/helloworld";

  //main function to run the game
  public static void main(String[] args) {
    System.out.println();
    System.out.println("***********************************"+
    "******************");
    System.out.println("Welcome to the Tic Tac Toe game!");
    System.out.println("To quit the game, press <q> or <Q>" + 
      " and press <enter>.\n");

    System.out.println("***********************************"+
    "******************");
  	Random random = new Random();
  	int randomNumber = random.nextInt(BASE) % 2;
  	if (randomNumber  == CLIENT_FIRST) {
  		clientChess = FIRST_CHESS;
  		serverChess = SECOND_CHESS; 

  	} else {
  		serverChess = FIRST_CHESS;
  		clientChess = SECOND_CHESS; 
  		updateBoard();
  	}

  	while (true) {
  		getUserInput(clientChess);
  		gameOver(clientChess, serverChess);
  		passBoardToServer(clientChess, serverChess);
  	}
  	
	}

  // Receives a new board from server if the server goes first.
	private static void updateBoard() {
		HttpClient httpClient = new DefaultHttpClient();
    String completePayload = "";
    try {
      HttpGet httpGetRequest = new HttpGet(URL);
      HttpResponse httpResponse = httpClient.execute(httpGetRequest);

      System.out.println("----------------------------------------");
      System.out.println(httpResponse.getStatusLine());
      System.out.println("----------------------------------------");

      HttpEntity entity = httpResponse.getEntity();

      byte[] buffer = new byte[1024];

      if (entity != null) {
        InputStream inputStream = entity.getContent();
        
        try {
          int bytesRead = 0;
          BufferedInputStream bis = new BufferedInputStream(inputStream);
          while ((bytesRead = bis.read(buffer)) != -1) {
            String chunk = new String(buffer, 0, bytesRead);
            completePayload += chunk;
          }
        } catch (Exception e) {
          e.printStackTrace();
        } finally {
          try { inputStream.close(); } catch (Exception ignore) {}
        }
      }

      // Now, try to turn that string into actual JSON
      Gson gson = new GsonBuilder().create();
      newBoard = gson.fromJson(completePayload, Board.class);
      System.out.println("Server goes first! This is from the server:");
      newBoard.printBoard();

    } catch (Exception e) {
      e.printStackTrace();
    } finally {
      httpClient.getConnectionManager().shutdown();
    } 
	}

  // Gets user input and changes board status
	private static void getUserInput(char clientChess) {
		Scanner reader = new Scanner(System.in);  // Reading from System.in
    System.out.println("Please enter a row number(1,2 or 3): ");
    String buffer = reader.next();
    while (!checkInput(buffer)) {
    	System.out.println("Invalid input!\nPlease enter a row number(1,2 or 3): ");
    	buffer = reader.next();
    }
    int n = Integer.parseInt(buffer);
    System.out.println("The input rom number you entered: " + n);
    // Scans the next token of the
    System.out.println("Please enter a column number(1,2 or 3): "); 
    buffer = reader.next();
    while (!checkInput(buffer)) {
    	System.out.println("Invalid input!\nPlease enter a column number(1,2 or 3): ");
    	buffer = reader.next();
    }
    int m = Integer.parseInt(buffer);
    System.out.println("The input column number your entered: " + m);

    // check if the given cell position is already occupied
    int index = 3 * (n - 1) + m;
    char[] array = boardToArray(newBoard);
    if (array[index - 1] == 'o' || array[index - 1] == 'x') {
    	System.out.println("The cell has been occupied! Please give another cell position!");
    	getUserInput(clientChess);
    } else {
    inputRow = n;
    inputColumn = m;// Scans the next token of the input as an int.
    //once finished
    System.out.println("Your input is: Row: " + inputRow +" Column: " + inputColumn);
    }
    index = 3 * (inputRow - 1) + inputColumn;
    array[index - 1] = clientChess;
    int first = newBoard.clientFirst;
    newBoard = arrayToBoard(array,first);
    System.out.println("You has made a move! This is the new board: ");
    newBoard.printBoard();
	}
  
  /* Checks if the user's input is valid.
   * Input: the user's input string
   * Return: true if the input is valid, and false otherwise
   */
	private static boolean checkInput(String buffer) {
    if (buffer.equals("Q") || buffer.equals("q")) {
      System.exit(0);
    }
		if (buffer.length() != 1) {
			return false;
		}
		if (isInteger(buffer)) {
			int n = Integer.parseInt(buffer);
			if (n >= 1 && n <= 3) {
				return true;
			}
		}
		return false;
	}

  /* Checks if a string represents an integer.
   * Input: the user's input string
   * Return: true if the string represents an integer, and false otherwise
   */
	private static boolean isInteger(String s) {
    try { 
        Integer.parseInt(s); 
    } catch(NumberFormatException e) { 
        return false; 
    } catch(NullPointerException e) {
        return false;
    }
    // only got here if we didn't return false
    return true;
  }
  
  /* Transfer a board object to a char array
   * Input: a board object
   * Return: a char array with value of the nine cells of the given board
   */
  private static char[] boardToArray(Board board) {
  	return new char[]{board.c1, board.c2, board.c3, board.c4,
  		board.c5, board.c6, board.c7, board.c8, board.c9};
  }
  
  /* Transfer a char array to a board object
   * Input: a char array and a integer representing who goes first
   * Return: a board object
   */
  private static Board arrayToBoard(char[] array, int first) {
  	return new Board(array[0], array[1], array[2], array[3], array[4],
  		array[5], array[6], array[7], array[8], first); 	
  }

  /* Tells if one player has won the game(if the board has three chesses in a line)
   * Input: a char array with 9 cells' value and a integer representing who goes first
   * Return: a board object
   */
  private static boolean hasWon(char[] array, char player) {
  	if ((array[0] == player && array[1] == player && array[2] == player)
  		|| (array[3] == player && array[4] == player && array[5] == player)
  		|| (array[6] == player && array[7] == player && array[8] == player)
  		|| (array[0] == player && array[3] == player && array[6] == player)
  		|| (array[1] == player && array[4] == player && array[7] == player)
  		|| (array[2] == player && array[5] == player && array[8] == player)
  		|| (array[0] == player && array[4] == player && array[8] == player)
  		|| (array[2] == player && array[4] == player && array[6] == player)){
  		return true;
  	}
  	return false;
  }

  /* Tells if the board is full of chesses(no cell to go).
   * Input: a char array with 9 cells' value
   * Return: true if the board is full and false otherwise
   */
  private static boolean isFull(char[] array) {
  		for (char c : array) {
  			if (c == ' ') {
  				return false;
  			}
  		}
  	return true;
  }

  /* Tells if the game is over(either play has won or the board is full).
   * Input: a char array with 9 cells' value
   * SideEffect: output game result.
   */
  private static void gameOver(char clientChess, char serverChess) {
  	char[] array = boardToArray(newBoard);
  	if (hasWon(array, clientChess)) {
  		System.out.println("Game over! Client won!");
  		System.exit(0);
  	}
  	else if (hasWon(array, serverChess)) {
  		System.out.println("Game over! Server won!");
  		System.exit(0);
  	}
  	else if (isFull(array)) {
  		System.out.println("Game over! There is a draw!");
  		System.exit(0);
  	}
  }


  /* Passes the board to server and receive the board from server throw Gson.
   * Input: a char representing the chess for one movement
   * Returns: void, but update the field newBoard with the received board from
   * the server
   */
  private static void passBoardToServer(char clientChess, char serverChess) {
  	HttpClient httpClient = new DefaultHttpClient();
    String completePayload = "";
    try {
      HttpPost httpPostRequest = new HttpPost(URL);
      Gson gson = new GsonBuilder().create();
      String result = gson.toJson(newBoard);
      StringEntity entity = new StringEntity(result);
      entity.setContentType("application/json");
      httpPostRequest.setEntity(entity);
      HttpResponse httpResponse = httpClient.execute(httpPostRequest);

      System.out.println("----------------------------------------");
      System.out.println(httpResponse.getStatusLine());
      System.out.println("----------------------------------------");

      HttpEntity newEntity = httpResponse.getEntity();

      byte[] buffer = new byte[1024];

      if (newEntity != null) {
        InputStream inputStream = newEntity.getContent();
        try {
          int bytesRead = 0;
          BufferedInputStream bis = new BufferedInputStream(inputStream);
          while ((bytesRead = bis.read(buffer)) != -1) {
            String chunk = new String(buffer, 0, bytesRead);
            //System.out.println(chunk);
            completePayload += chunk;
          }
        } catch (Exception e) {
          e.printStackTrace();
        } finally {
          try { inputStream.close(); } catch (Exception ignore) {}
        }
      }

      // Now, try to turn that string into actual JSON
      newBoard = gson.fromJson(completePayload, Board.class);
      System.out.println("Server has made a move and this is the new board: ");
      newBoard.printBoard();
      gameOver(clientChess, serverChess);

    } catch (Exception e) {
      e.printStackTrace();
    } finally {
      httpClient.getConnectionManager().shutdown();
    } 

  }



  /* Represents the board of "tic tac toe".
   * Input: a char array with 9 cells' value
   * SideEffect: output game result.
   */
  private static class Board {
    char c1;
    char c2;
    char c3;
    char c4;
    char c5;
    char c6;
    char c7;
    char c8;
    char c9;
    // if clientFirst == 1, then client goes first;
    // if clientFirst == 0, then server goes first.
    int clientFirst;

    // default constructor for empty board(only set if client goes first)
    public Board(int clientFirst) {
    	this.c1 = ' ';
    	this.c2 = ' ';
    	this.c3 = ' ';
    	this.c4 = ' ';
    	this.c5 = ' ';
    	this.c6 = ' ';
    	this.c7 = ' ';
    	this.c8 = ' ';
    	this.c9 = ' ';
    	this.clientFirst = clientFirst;
    }

    // the second constructor for a board with all cells value and int clientFirst
    public Board(char c1, char c2, char c3, char c4,char c5, char c6, char c7,
    	char c8, char c9, int clientFirst){
    	this.c1 = c1;
    	this.c2 = c2;
    	this.c3 = c3;
    	this.c4 = c4;
    	this.c5 = c5;
    	this.c6 = c6;
    	this.c7 = c7;
    	this.c8 = c8;
    	this.c9 = c9;
    	this.clientFirst = clientFirst;
    }
    
    //Prints out the board in console
    public void printBoard() {
    	System.out.println();
      System.out.println("   " + "1" + " " + " " + " " + "2" + " " + " " + " " + "3" + " ");
    	System.out.println("1 [" + c1 + "]" + " " + "[" + c2 + "]" + " " + "[" + c3 + "]");
    	System.out.println("2 [" + c4 + "]" + " " + "[" + c5 + "]" + " " + "[" + c6 + "]");
    	System.out.println("3 [" + c7 + "]" + " " + "[" + c8 + "]" + " " + "[" + c9 + "]");
    	System.out.println();
    }
    
  }
}

#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#define PORT 8080

//Display the main menu
void main_menu(){
    printf("Library Management System\n");
    printf("1. Enter 1 if you are the Library Administrator.\n");
    printf("2. Enter 2 if you are an existing member.\n");
    printf("3. Enter 3 if you are a new member.\n");
    printf("0. Enter 0 to exit.\n\n");
}


void admin_menu(){
	printf("Library Management System\n");
	printf("1. Add a new book.\n");
	printf("2. Delete an existing book.\n");
	printf("3. Modify the book count.\n");
	printf("4. Search for book details.\n");
	printf("5. Display all the member details.\n");
	printf("6. Remove an existing member.\n");
	printf("0. Enter 0 to logout.\n\n");
}

void member_menu(){
	printf("Library Management System\n");
	printf("1. Borrow a book.\n");
	printf("2. Return a book.\n");
	printf("0. Enter 0 to logout.\n\n");
}

//function to clear the input buffer
void clear_Inp_Buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}


void addbook(int client_fd) {
    char id[10];
    char name[50];
    char author[50];
    char count[5];
    char sep[2] = ",";
	//the request number
    char new_book[400] = "2";
    strcat(new_book, sep);

	//Input all the details
    printf("Enter Book ID: \n");
    scanf("%s", id);
    strcat(new_book, id);
    strcat(new_book, sep);

    clear_Inp_Buffer();

    printf("Enter Book Name: \n");
    fgets(name, sizeof(name), stdin);
    strtok(name, "\n"); 
    strcat(new_book, name);
    strcat(new_book, sep);

    printf("Enter Author Name: \n");
    fgets(author, sizeof(author), stdin);
    strtok(author, "\n"); 
    strcat(new_book, author);
    strcat(new_book, sep);

    printf("Enter count of Book: \n");
    scanf("%s", count);
    strcat(new_book, count);
    send(client_fd, new_book, strlen(new_book), 0);

	char buffer[20];
	read(client_fd, buffer,10);

	//Error handling if adding failed
	if (!strcmp(buffer,"fail")){
		printf("Book could not be added.\n\n");
		return;
	}
	printf("Book successfuly added!\n\n");

}


void removebook(int client_fd){
	//Request NUmber 3
	char removebook[20] = "3,";	
	char bid[20];
	printf("Enter ID of book to be deleted: \n");
	scanf("%s",bid);
	strcat(removebook,bid);

	send(client_fd, removebook, strlen(removebook), 0);

	char buffer[20];
	read(client_fd, buffer,20);

	if (!strcmp(buffer,"success")){
		printf("Book successfully removed!\n\n");
		return;
	}
	printf("Book could not be removed.\n\n");
}

void modifybook(int client_fd) {
    char book_id[10];
    char new_count[5];
	//Request Number 5
    char request[20] = "5,";
    char sep[2] = ",";
    
    printf("Enter Book ID: \n");
    scanf("%s", book_id);
    strcat(request, book_id);
    strcat(request, sep);

    printf("Enter new count of Book: \n");
    scanf("%s", new_count);
    strcat(request, new_count);

    send(client_fd, request, strlen(request), 0);

    char buffer[20];
    read(client_fd, buffer, 20);

    if (!strcmp(buffer, "success")) {
        printf("Book count successfully modified!\n\n");
    } else {
        printf("Failed to modify book count.\n\n");
    }
}



void searchbook(int client_fd) {
	//Req no. 4
    char searchbook[20] = "4,";
    char book_id[20];

    printf("Enter ID or name of the book to be searched: ");
    scanf("%s", book_id);
    strcat(searchbook, book_id);

    send(client_fd, searchbook, strlen(searchbook), 0);

    char buffer[1024];
    ssize_t bytes_received = recv(client_fd, buffer, sizeof(buffer), 0);

    
	buffer[bytes_received] = '\0'; 
	printf("BookID,Book Name,Author Name,Copies: %s\n", buffer);
    
}

void displaymembers(int client_fd){
	//Req char
	char displaybook[10] = "b";
	send(client_fd, displaybook, strlen(displaybook), 0);
	char buffer[1024];
	if (strlen(buffer)==0)	return;
    ssize_t bytes_received = recv(client_fd, buffer, sizeof(buffer), 0);
	
	buffer[bytes_received] = '\0'; 
	printf("Library Members:\nUsernames, Passwords:\n%s\n\n", buffer);
}

void displaybooks(int client_fd){
	char displaybook[10] = "7";	//Request Number
	send(client_fd, displaybook, strlen(displaybook), 0);
	char buffer[1024];
    ssize_t bytes_received = recv(client_fd, buffer, sizeof(buffer), 0);
	buffer[bytes_received] = '\0'; 
	printf("Books available:\nBookID,Book Name,Author Name,Copies:\n%s\n\n", buffer);
}

void removemember(int client_fd){
	//Req Char
	char removebook[20] = "c,";			
	char uname[20];
	printf("Enter username of member to be removed: \n");
	scanf("%s",uname);
	strcat(removebook,uname);

	send(client_fd, removebook, strlen(removebook), 0);

	char buffer[20];
	read(client_fd, buffer,20);

	if (!strcmp(buffer,"success")){
		printf("Member successfully removed!\n\n");
		return;
	}
	printf("Member could not be removed.\n\n");
}

void borrowbook(int client_fd, char name[]){
	displaybooks(client_fd);

	char book_id[10];
	// Request No. 8
    char request[20] = "8,"; 
    char sep[2] = ",";
	
    printf("Enter Book ID: \n");
    scanf("%s", book_id);
    strcat(request, book_id);
	strcat(request,sep);
	strcat(request,name);
	send(client_fd, request, strlen(request), 0);

	char buffer[20];
    read(client_fd, buffer, 20);

    if (!strcmp(buffer, "success")) {
        printf("Book borrowed!\n\n");
    } else {
        printf("Failed to borrow book.\n\n");
    }


}

void returnbook(int client_fd, char name[]){
	displaybooks(client_fd);

	char book_id[10];
	// Request Number
    char request[20] = "a,"; 
    char sep[2] = ",";
	
    printf("Enter Book ID: \n");
    scanf("%s", book_id);
    strcat(request, book_id);
	strcat(request,sep);
	strcat(request,name);
	send(client_fd, request, strlen(request), 0);

	char buffer[20];
    read(client_fd, buffer, 20);

    if (!strcmp(buffer, "success")) {
        printf("Book returned!\n\n");
    } else {
        printf("The book was never borrowed.\n\n");
    }
}


void admin(int client_fd) {
    char name[20];
    char pwd[20];
    char sep[2] = ",";
    
    // Construct the request for login verification
    char request[200] = "1";    
    strcat(request, sep);

    printf("Enter name: \n");
    scanf("%19s", name);
    strcat(request, name);
    strcat(request, sep);

    printf("Enter password: \n");
    scanf("%19s", pwd);
    strcat(request, pwd);
    
    // Send the login request to the server
    send(client_fd, request, strlen(request), 0);

    // Receive the server's response
    char buffer[20];
    recv(client_fd, buffer, sizeof(buffer) - 1, 0);
    buffer[19] = '\0';  // Ensure null-termination

    if (strcmp(buffer, "success") == 0) {
        printf("Login Successful!\n\n");

        while (1) {
            admin_menu();
            int choice;
            scanf("%d", &choice);

            switch (choice) {
                case 1:
                    addbook(client_fd);
                    break;
                case 2:
                    removebook(client_fd);
                    break;
                case 3:
                    modifybook(client_fd);
                    break;
                case 4:
                    searchbook(client_fd);
                    break;
                case 5:
                    displaymembers(client_fd);
                    break;
                case 6:
                    removemember(client_fd);
                    break;
                case 0:
                    printf("Logging out...\n\n");
                    return;
                default:
                    printf("Invalid choice.\n\n");
                    break;
            }
        }            
    } else {
        printf("Invalid credentials.\n\n");
        return;
    }
}



void old_mem(int client_fd){
	char name[20];
    char pwd[20];
	char sep[2] = ",";
	//Request Number 9
	char request[200] = "9";	
	strcat(request,sep);

    printf("Enter name: \n");
    scanf("%s",name);
	strcat(request,name);
	strcat(request,sep);

    printf("Enter password: \n");
    scanf("%s",pwd);
	strcat(request,pwd);
	
    send(client_fd, request, strlen(request), 0);

	char buffer[20];
	recv(client_fd, buffer, 10, 0);

	if (strcmp(buffer,"success")==0){
		printf("Login Successful!\n\n");

		while(1){
			member_menu();
			int choice;
			scanf("%d",&choice);
			if (choice == 1)	borrowbook(client_fd,name);
			else if (choice == 2)	returnbook(client_fd,name);
			else if (choice == 0){
				printf("Logging out...\n\n");
				return;
			}
			else	printf("Invalid choice.\n\n");
		}			
	}
	else{
		printf("Invalid credentials.\n\n");
		return;
	}
}


void new_mem(int client_fd){
	char name[20];
    char pwd[20];
	char sep[2] = ",";
		//Request Number 6
	char request[200] = "6";
	strcat(request,sep);

    printf("Enter name: \n");
    scanf("%s",name);
	strcat(request,name);
	strcat(request,sep);

    printf("Enter password: \n");
    scanf("%s",pwd);
	strcat(request,pwd);
	
    send(client_fd, request, strlen(request), 0);
	char buffer[20];
	read(client_fd, buffer,10);

	if (strcmp(buffer,"success")==0){
		printf("Member successfuly registered!\n\n");
		while(1){
			member_menu();
			int choice;
			scanf("%d",&choice);
			if (choice == 1)	borrowbook(client_fd,name);
			else if (choice == 2)	returnbook(client_fd,name);
			else if (choice == 0){
				printf("Logging out...\n\n");
				return;
			}
			else	printf("Invalid choice.\n\n");
		}
	}
	printf("Member could not be added.\n\n");

}


int main(int argc, char const* argv[])
{
	int status, valread, client_fd;
	struct sockaddr_in serv_addr;
	// char* hello = "Hello from client";
	char buffer[1024] = { 0 };
	if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("\n Socket creation error \n");
		return -1;
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);

	// Convert IPv4 and IPv6 addresses to binary
	if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)
		<= 0) {
		printf(
			"\nAddress not supported \n");
		return -1;
	}

	if ((status
		= connect(client_fd, (struct sockaddr*)&serv_addr,
				sizeof(serv_addr)))
		< 0) {
		printf("\nConnection Failed \n");
		return -1;
	}

	while (1){
		main_menu();
		int choice;
		scanf("%d",&choice); //take input of choice

		if (choice == 1)  admin(client_fd);
		else if (choice == 2) old_mem(client_fd);
		else if (choice == 3)   new_mem(client_fd);
		else if (choice == 0)   exit(0);

		else printf("Invalid choice. Please try again\n\n"); //error handling for choosing invalid option
		
	}
	close(client_fd);
	return 0;
}

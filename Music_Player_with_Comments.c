/* MusicPlayer using C,
This code can play any wav files and only certain types of mp3 files
If cant play, the user is notified about it
The app directly scans all the music available in the directory
Do not forget to add winmm in linkers of the project.

CURRENT MUSIC CAPACITY IS 50.Please change the list and build again, if needed more.
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<windows.h> //This Include file for basic system operations
#include<dirent.h>  //This Include header is required for directory scanning operations
#include<conio.h>
#include"MMSystem.h" // Main Header for Playing music

struct node
{
    /* Basic structure declaration for linked list, data is filename, which is a string,
    and two pointers, next and prev */
    char file_name[75];
    struct node *next;
    struct node *prev;
};

struct node* head=NULL;  //Head is a pointer that has the start of the linked list.
struct node* now_playing; //This contains the current song that is being played
/*
different counts and flags being declared.
total_songs=count of all available songs in directory.
play_status=indicates whether a song is being played or no. 1 means affirmative.
playlist_end=flag to indicate, if linked list has  reached either ends, 1 is affirmative.
is_mp3=flag will be set to 1, if file is mp3
error= flag which is set to 0, if no error in opening file
*/
int total_songs=0,play_status=0,playlist_end=0,is_mp3=0,error=0;
char songs_list[50][75]; //Array of strings to have list of all songs.Max of 40 can be stored. please increase the number for more songs.


void append(struct node** head_ref, char new_data[])
{
    //Basic append function to add data at end of an array.
    struct node* new_node = (struct node*) malloc(sizeof(struct node));

    struct node *last = *head_ref;
    strcpy(new_node->file_name ,new_data);
    new_node->next = NULL;
    if (*head_ref == NULL)
    {
        new_node->prev = NULL;
        *head_ref = new_node;
        return;
    }
    while (last->next != NULL)
        last = last->next;
    last->next = new_node;
    new_node->prev = last;
    return;
}

void printList(struct node *node)
{
    //Function to print all the elements in the linked_list.
    //Basically used to print the song list.
    int n=1;
    if (node!=NULL)
    {   printf("The songs in the list are: \n");
        while (node != NULL)
        {
            printf("%d.%s\n",n,node->file_name);
            n++;
            node = node->next;
        }
    }
    else
    printf("Playlist Empty\n");

}
void ClearList()
 {
     //Function to delete all elements in the linked list
     //Used when u need to update the songs_list during run of app.
    struct node* current=head;
    struct node* previous;
    while (current!=NULL)
    {
        previous=current;
        current=current->next;
        free(previous);
    }
    head=NULL;
 }
void checkfile(char current[])
{
    /*
    This function is used to recognize, whether the current file name passed is mp3/wav
    If true, adds it to the linked list else, does nothing.
    */
    char format[]=".wav",format2[]=".mp3";
    int count1=0,count2=0,i,j,flag,n,x;
    //This part recognizes .wav files
    count1=strlen(current);
    count2=strlen(format);
    for(i=0;i<=count1-count2;i++)
    {
        for(j=i; j< i+count2;j++)
        {
         flag=1;
         if(current[j]!=format[j-i])
         {
             flag=0;
             break;
         }

        }
        if(flag==1)
            {
                is_mp3=0;
                goto here;
            }
    }
    //This part recognizes .mp3 files
    count1=strlen(current);
    count2=strlen(format2);
    for(i=0;i<=count1-count2;i++)
    {
        for(j=i; j< i+count2;j++)
        {
         flag=1;
         if(current[j]!=format2[j-i])
         {
             flag=0;
             break;
         }

        }
        if(flag==1)
        {   is_mp3=1;
            break;
        }
    }
    here: if (flag==1)
    {
        //If its an mp3/wav file, its added to the linked list.
                strcpy(songs_list[total_songs],current);
                append(&head,current);
                total_songs++;
    }


}
void UpdateList()
{
    /*
    This function, gets the files present in the current directory,
    passes each of them to the checkfile function, and updates the song list.

    */
    DIR *d; //Directory Structure type, for storing file information.
    struct dirent *dir;
    char current[60];//Current file variable
    d=opendir(".");
    ClearList();//Clear before updating, to avoid copies of same file
    total_songs=0;
    if(d)
    {  //The directory type is auto-increment, so once the file_name is accessed it increments
        //automatically. Null indicates end of the structure
        while((dir=readdir(d))!= NULL)
        {
            strcpy(current,dir->d_name);
            checkfile(current);
        }
        closedir(d);//Close the directory
        printf("Updated the List, it has %d Songs \n", total_songs);
        now_playing=head;//Point the song to the beginning of the list.
    }

}

void PrintCurrentSong()
{
// This function, prints the details of the current playing song.
 if(play_status==1)
 printf(" ####  NOW Playing: %s #### \n",now_playing->file_name);
}

void play()
{/*Main function to play the music. A function called mciSendString is Used.
   Syntax: * mciSendString("open "filename" alias 'aliasname' ", NULL, 0, 0);
   alias is optional. returns a zero if successful, else other integers.
   To play: * mciSendString("play filename/aliasname,NULL,0,0);

    */
    char open_file_string[115]="",buff[95]="";
    if(now_playing!=NULL)//if Null, indication that, it has reached the end
    {
        //Series of string operations, to add the extra texts to the file_name to open it.
        strcpy(buff,"open \"");
        strcat(buff,now_playing->file_name);
        strcpy(open_file_string,buff);
        strcat(open_file_string,"\" alias MY_SND");//Alias is chosen as MY_SND
        error=mciSendString(open_file_string,NULL,0,0); //Open the file
        mciSendString("play MY_SND notify",NULL,0,0);
        if(error==0)
        {//If no error, play the song, enable the flag.
            play_status=1;
            PrintCurrentSong();
        }
        else
        {//if error persists, inform the user abt it, and give him other options
            printf("Can't play song, please try some other song: \n $$ Please Navigate using 5,6,7 $$ \n");
        }
    }
    else
    {//id reached end of playlist, enable the flag, inform the user.
        //Chance the pointer to the beginning
        printf("Reached End of the Playlist \n The Song-Pointer is at start now.\n");
        playlist_end=1;
        now_playing=head;
    }
}
void pause()
{
    /*
    Function to pause the play_back.
    To pause: * mciSendString("pause filename/aliasname,NULL,0,0);
    */
    play_status=0;//Change flag as song is paused.
    mciSendString("pause MY_SND",NULL,0,NULL);
    PrintCurrentSong();
    printf("###  Paused  ###  \n");

}
void resume()
{
    /*
    Function to resume the play_back.
    To resume: * mciSendString("resume filename/aliasname,NULL,0,0);
    */
    play_status=1;
    mciSendString("resume MY_SND",NULL,0,NULL);
    PrintCurrentSong();
}
void stop()
{
    /*
    Function to stop the play_back.
    To stop: * mciSendString("stop filename/aliasname,NULL,0,0);
    After stopping, the file is closed, Inorder to avoid error
    */
 play_status=0;
 mciSendString("stop MY_SND",NULL,0,NULL);
 mciSendString("close MY_SND",NULL,0,NULL);
}

void go_next()
{
    //If user opts for next song, stop the current one
    //Then navigate to next node of linked list
    //Play
    stop();
    now_playing=now_playing->next;
    play();

}

void go_previous()
{
    //If user opts for precious song, stop the current one
    //Then navigate to previous node of linked list
    //Play
   stop();
   now_playing=now_playing->prev;
   play();

}
void playNum(int num)
{/* This function plays the song, the user has opted acc to a choice.
    The list is shown according to order in the linked_list.
    SO, the linked list is traversed n times till it reaches the users choice, then played.
    if the number entered by user is more than total_songs, a wrong choice error is thrown.

    */
 int nodecount=0;
 struct node* current=head;
 if (num<=total_songs)
 {  while((nodecount!=num-1) && (current->next!=NULL))
    {
        current=current->next;
        nodecount++;
    }

    now_playing=current;
    if(play_status==1) stop();//Stop the song, if its being played
    play();
 }
 else
    printf("Wrong choice\n");
}

void AskForExit()
{
    //Function to confirm, exit by user. User has to press a 'y' to exit.
    //To cancel, he can use the normal music player choices.
    char a;
    a=getc(stdin);
    if(a=='y')
        exit(0);

}

void ShowAllChoices()
{
    //A simple function, to show the choices to the user, while a song is not being played.
if(error==0)
    printf("1.Play From Beginning\n");
if(error!=0) //If there is an error, option(1), must be disabled.
    printf("5.Next Song \t 6.Previous Song \n");
printf("7.Play by selecting the song to play\n");
printf("8.Exit \t");
printf("9.Update the Song List\n");
}

void ShowPlayChoices()
{
    //A simple function, to show choices, while a song is being played.
    //If the song is paused, the resume has to be shown, the play_status flag is made used here.
  if (play_status==1)
    printf("2.Pause \t");
  else
    printf("3.Resume \t");
  printf("4.Stop \n");
  printf("5.Next Song \t");
  printf("6.Previous Song \t");
  printf("8.Exit\n");

}

 void ExecuteChoice(int choice)
{
    /*
    Function to execute the Choice entered by the User.
    */
  int num;
  switch(choice)
  {
    case 1:
        //If user presses 1, play the current song from beginning.
        play();
        if(error==0)
        ShowPlayChoices();
        else//if error exists show him, other set of choices
        ShowAllChoices();
        break;
    case 2:
        //Press 2 to pause the playback. show the choices
        pause();
        ShowPlayChoices();
        break;
    case 3:
        resume();
        //Resume the Playback. Show the choices.
        ShowPlayChoices();
        break;
    case 4:
        //Stop the Playback.Show the choices
        stop();
        ShowAllChoices();
        break;
    case 5:
        //GO next in the linked list.
        go_next();
        if(playlist_end!=1 && error==0) //Show the current playing choices, only if end is not reached and no error
            ShowPlayChoices();
        else if(error!=0)
            ShowAllChoices();
        else
            ShowAllChoices();
        break;
    case 6:
        //GO previous in the linked list.
        go_previous();
        if(playlist_end!=1 && error==0) //Show the current playing choices, only if end is not reached and no error
            ShowPlayChoices();
        else if(error!=0)
            ShowAllChoices();
        else
            ShowAllChoices();
        break;
    case 7:
        //Play the song of users choice
        //Print the song_list, ask for the choice using a variable
        //Pass it to function, to traverse and play it.
        printList(head);
        printf("Select the song using number to play : ");
        scanf("%d",&num);
        playNum(num);
        if (error==0)
        ShowPlayChoices();
        if(error!=0)
            ShowAllChoices();
        break;
    case 8:
        //Case for the exiting the music_player. request for 'y' to confirm it.
        AskForExit();
        printf(" ** Do you really want to exit??? (Press y) ** \n To cancel, Just enter the Music Player Choices :  ");
        break;
    case 9:
        //Case to update the music_list while the app is open.
        UpdateList();
        break;
    case 10:
        //Just a extra case to show choices to the user
        ShowAllChoices();
        break;
    default:
        //Default invalid case.
        printf("Invalid Choice, Please Press 10 to show available choices :");
        break;
  }
}
int main()
{
    /*
    MAIN function.
    It starts by updating the list by default and showing the start choices
    Enters an infinite loop, that keeps scanning for inputs
    And executes those choices.
    */
  int choice;
  UpdateList();
  ShowAllChoices();
  printf("Enter Choice: \nPress 10 to show all choices, at any time of program  : ");
  now_playing=head;
  while(1)
  {
      //Keep scanning and executing the choices infinitely till user terminates.
      scanf("%d",&choice);
      ExecuteChoice(choice);
  }
  return 0;
}


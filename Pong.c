/****************************PONG*******************************
 * Project Name: Pong                                          *
 * Author: Sheraz Mahmood                                      *
 * Purpose: To show beginners how to write pong. Intended as a *
 *          first game for beginners to write.                 *
 * Notes: This game uses the Allegro library.                  *
 *                                   1997 (c) Sheraz Mahmood   *
 **************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <allegro.h>

/*Include the pong header file for the finding things in the datafile*/
#include "pong.h"

/*Direction consts*/
#define DOWN_RIGHT 0
#define UP_RIGHT   1
#define DOWN_LEFT  2
#define UP_LEFT    3

/*Get the Pong DataFile*/
DATAFILE *pong_datafile=NULL;

/*Game variables*/
int bar1_y=30;       //Player 1 Bar position
int bar2_y=430;      //Player 2 Bar position

int ball_x;          //The location of the Ball
int ball_y;          //The location of the Ball
int direction=0;     //The direction the Ball is going

int score_p1=0;      //Player 1 score
int score_p2=0;      //Plater 2 score

BITMAP *buffer;      //The screen buffer of double buffering

int speed=2;         //The speed of the Ball

int amout_of_hits=0; //Used to set the speed of the Ball

int up_ran_speed=0;  //Random speed

/********************************************************
 *NAME:Boing                                            *
 *PURPOSE:To play the sound file when the ball is hit   *
 *INPUT:  N/A                                           *
 *RETURN: N/A                                           *
 *******************************************************/
void boing(void)
{
   play_sample(pong_datafile[pong_boing].dat,255,255,1000,0);
   return;
}

/********************************************************
 *NAME:Random Direction                                 *
 *PURPOSE:To send back a random direction for the ball  *
 *INPUT:  N/A                                           *
 *RETURN: An int for the direction                      *
 *******************************************************/
int random_direction(void)
{
   up_ran_speed=random()%3;
   return random()%3;
}

/********************************************************
 *NAME:Move Ball                                        *
 *PURPOSE:To Move the ball and to bounce the ball       *
 *INPUT:  N/A                                           *
 *RETURN: N/A                                           *
 *******************************************************/
void move_ball(void)
{
   /*Set the speed according to the amout of hits*/
   if(amout_of_hits%10==0 && amout_of_hits!=0)
   {
             speed++;
             amout_of_hits++;
   }

   /*Respond to the direction the ball is traviling*/
   switch(direction)
   {
      case DOWN_RIGHT:
           ball_x+=speed+up_ran_speed;
           ball_y+=speed;
           if(ball_x>600)
           {
              if((ball_y>=bar2_y-40) &&( ball_y<=bar2_y+65))
              {
                       direction=DOWN_LEFT;
                       amout_of_hits++;
                       boing();
              }
              else
              {
                       score_p1++;
                       ball_x=SCREEN_W/2;
                       ball_y=35;
                       direction=random_direction();
              }
           }
           else
           {

                 if(ball_y>420)
                            direction=UP_RIGHT;
           }



           break;
      case UP_LEFT:
           ball_x-=speed;
           ball_y-=speed+up_ran_speed;
           if(ball_x<5)
           {
              if((ball_y>=bar1_y-40) &&( ball_y<=bar1_y+65))
              {
                       direction=UP_RIGHT;
                       amout_of_hits++;
                       boing();
              }
              else
              {
                       direction=random_direction();
                       score_p2++;
                       ball_x=SCREEN_W/2;
                       ball_y=35;

              }
           }
           else
           {
            if(ball_y<30)
                        direction=DOWN_LEFT;
           }
           break;
      case UP_RIGHT:
           ball_x+=speed+up_ran_speed;
           ball_y-=speed;
           if(ball_x>600)
           {
               if((ball_y>=bar2_y-40) &&( ball_y<=bar2_y+65))
               {
                       direction=UP_LEFT;
                       amout_of_hits++;
                       boing();
               }
               else
               {
                        score_p1++;
                        ball_x=SCREEN_W/2;
                        ball_y=35;
                        direction=random_direction();
               }
           }
           else
           if(ball_y<30)
                        direction=DOWN_RIGHT;
           break;
      case DOWN_LEFT:
           ball_x-=speed;
           ball_y+=speed+up_ran_speed;
           if(ball_x<5)
           {
               if((ball_y>=bar1_y-40) &&( ball_y<=bar1_y+65))
               {
                        direction=DOWN_RIGHT;
                       amout_of_hits++;
                       boing();
               }
               else
               {
                        score_p2++;
                        ball_x=SCREEN_W/2;
                        ball_y=35;
                        direction=random_direction();
               }
           }
           else
           {
               if(ball_y>420)
                        direction=UP_LEFT;
           }

   }
   /*Display the ball*/
   draw_rle_sprite(buffer,pong_datafile[pong_ball].dat,ball_x,ball_y);

   return;
}

/********************************************************
 *NAME:Key Respond                                      *
 *PURPOSE:To respond to the input from the users        *
 *INPUT:  N/A                                           *
 *RETURN: N/A                                           *
 *******************************************************/
void key_respond(void)
{
     /*Update the joystick*/
     poll_joystick();

   /*Respond to the input*/
    if(key[KEY_DOWN])
       bar1_y+=6+up_ran_speed;

    if(key[KEY_UP])
       bar1_y-=6+up_ran_speed;

    if(bar1_y<=30)
       bar1_y=30;

    if(bar1_y>430)
       bar1_y=430;

    if(joy_down || key[KEY_Z])
       bar2_y+=6+up_ran_speed;

    if(joy_up || key[KEY_A])
       bar2_y-=6+up_ran_speed;

    if(bar2_y<=30)
       bar2_y=30;

    if(bar2_y>430)
       bar2_y=430;

    /*Display the bars on the screen*/
    draw_rle_sprite(buffer,pong_datafile[pong_bar].dat,0,bar1_y);
    draw_rle_sprite(buffer,pong_datafile[pong_bar].dat,635,bar2_y);


   return;
}

/********************************************************
 *NAME:Play Game                                        *
 *PURPOSE:To Control the acctual the game               *
 *INPUT:  N/A                                           *
 *RETURN: N/A                                           *
 *******************************************************/
void pong_game(void)
{
   /*Set the first random direction the ball will be traviling*/
   direction=random_direction();

   /*Play the game while the ESC key is not pressed*/
   while(!key[KEY_ESC])
   {
      /*Move the Ball*/
      move_ball();
      /*Respond to the input*/
      key_respond();

      /*Put the information on the Screen*/
      textout(buffer,pong_datafile[pong_text].dat,"Player 1 Score:",150,0,254);
      textout(buffer,pong_datafile[pong_text].dat,itoa(score_p1,NULL,10),text_length(pong_datafile[pong_text].dat,"Player 1 Score:")+150,0,10);
      textout(buffer,pong_datafile[pong_text].dat,"Player 2 Score:",350,0,254);
      textout(buffer,pong_datafile[pong_text].dat,itoa(score_p2,NULL,10),text_length(pong_datafile[pong_text].dat,"Player 2 Score:")+350,0,10);

      textout(buffer,pong_datafile[pong_text].dat,"keyboard",0,0,255);
      textout(buffer,pong_datafile[pong_text].dat,"joystick",640-text_length(pong_datafile[pong_text].dat,"joystick"),0,255);

      /*Draw a line to set the boundries*/
      line(buffer,0,30,640,30,10);

      /*Put the buffer screen on the screen*/
      blit(buffer,screen,0,0,0,0,640,480);

      /*Clear the buffer*/
      clear(buffer);
   }
   return;
}

int main(void)
{
  /*Init Allegro equipment*/
 allegro_init();

 /*Install the timer*/
 install_timer();

 /*Install the keyboard*/
 install_keyboard();


 /*Initizlise the joystick*/
 initialise_joystick();

 /*Install the mouse*/
 install_mouse();


 if((pong_datafile=load_datafile("pong.dat"))==NULL)
 {
         /*Shutdown Allegro*/
         allegro_exit();
         /*Print error message*/
         printf("Error loading \"pong.dat\"\n%s\n\n", allegro_error);
         exit(1);
 }

 /*Set the sound system*/
 printf("Setting up the Sound.");

 if(install_sound(DIGI_AUTODETECT,MIDI_AUTODETECT,NULL) != 0)
 {
         /*Shutdown Allegro*/
         allegro_exit();
         /*Print error message*/
         printf("Error setting up Sound\n%s\n\n", allegro_error);
//         exit(1);

 }

 /*Set the graphice mode*/
 printf("Setting up graphics mode 640x480.\n");

 if (set_gfx_mode(GFX_AUTODETECT, 640, 480, 0, 0) != 0)
 {
         /*Shutdown Allegro*/
         allegro_exit();
         /*Print error message*/
         printf("Error setting graphics mode\n%s\n\n", allegro_error);
         exit(1);
 }


 /*Set the pallete for the sprites*/
 set_pallete(pong_datafile[pong_pal].dat);

 /*Clear and Create the buffer*/
 buffer=create_bitmap(640,480);
 clear(buffer);

 /*Set the Ball position*/
 ball_x=SCREEN_W/2;
 ball_y=35;

 /*Play the Game*/
 pong_game();

 /*Shut down Allegro*/
 allegro_exit();

 /*Output the information*/
 if(score_p1>score_p2)
            printf("Player 1 beat Player 2\n");
 else
            printf("Player 2 beat Player 1\n");

 return;
}


/* MT4 Properties ----------------------------------------------------------- */
#property copyright "Copyright 2017, Nicholas Buckeridge."
#property link      "https://mcqueen256.com/"
#property version   "1.00"
#property strict

/* Inputs ------------------------------------------------------------------- */
input int number;

/* Globals ------------------------------------------------------------------ */
int robot_instance;

/* Libraries ---------------------------------------------------------------- */
#include <nb/CommandInterpreter.h>
#include <nb/Status.h>

/* DLL Functions ------------------------------------------------------------ */
#import "NBRobot.dll"
  int initialise(int number);
  void analyse(int instance, double open, double high, double low, double close);
  void bar(int instance, double open, double high, double low, double close);
  char getStreamCharacter(int instance);
  void sendStreamCharacter(int instance, char c);
#import

/* Global Objects  ---------------------------------------------------------- */
CommandInterpreter interpreter("NBRobot");
Status orders("NBRobot");

/* Initialisation ----------------------------------------------------------- */
int OnInit() {
  /* Send settings to the dll */
  robot_instance = initialise(number);

  /* Analyse buffer history */
  for (int i = ArraySize(Close) - 1; i > 0; i--) {
    analyse(robot_instance, Open[i], High[i], Low[i], Close[i]);
  }

  return(INIT_SUCCEEDED);
}

/* New Data to Analyse ------------------------------------------------------ */
void OnTick() {
  /* Track when a new bar is formed */
  static long tick_volume_tracker = 0;

  if (tick_volume_tracker < Volume[0]) {
    /* Volume is still increasing */
    tick_volume_tracker = Volume[0];
  } else {
    /* New bar */
    bar(robot_instance, Open[1], High[1], Low[1], Close[1]);
    tick_volume_tracker = 0;
  }
  analyse(robot_instance, Open[0], High[0], Low[0], Close[0]);

  /* Exchange commands */
  string status = orders.activeOrders();
  for (int i = 0; i < StringLen(status); i++) {
    sendStreamCharacter(robot_instance, StringGetCharacter(status, i));
  }
  sendStreamCharacter(robot_instance, '\0');
  
  char c;
  while ((c  = getStreamCharacter(robot_instance)) != '\0') {
    interpreter.push(c);
  }
  interpreter.push(c);


  status = orders.activeOrders();
  for (int i = 0; i < StringLen(status); i++) {
    sendStreamCharacter(robot_instance, StringGetCharacter(status, i));
  }
  sendStreamCharacter(robot_instance, '\0');
}

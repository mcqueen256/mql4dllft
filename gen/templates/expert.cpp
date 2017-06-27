/* MT4 Properties ----------------------------------------------------------- */
#property copyright "Copyright 2017, Nicholas Buckeridge."
#property link      "https://mcqueen256.com/"
#property version   "1.00"
#property strict


/* Inputs ------------------------------------------------------------------- */
{%
  for type, name, default in inputs:
    if default is not None:
      default = ' = {}'.format(default);
    else:
      default = '';

    line = 'input {} {}{};'.format(type, name, default);
%}
{{ line }}
{% end for %}

/* Globals ------------------------------------------------------------------ */
int robot_instance; // reference number for the robot in the dll

/* Libraries ---------------------------------------------------------------- */
#include <...>

/* DLL Functions ------------------------------------------------------------ */
#import "NBRobot.dll"
  int initialise(int number);
  int initialise(string name);
#import

/* Global Objects  ---------------------------------------------------------- */


/* Initialisation ----------------------------------------------------------- */
int OnInit() {
  /* Send settings to the dll */
  robot_instance = initialise(number);

  return(INIT_SUCCEEDED);
}

/* New Data to Analyse ------------------------------------------------------ */
void OnTick() {

}
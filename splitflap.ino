#include<WHATEVERTHEMOTORIS.h>

const int button = 0;

/*
 * Inintialize various motor states
 */

int FlipSpeed = 60;	// Rotation speed of flip display
int CurrentCard = 1;
int bFlip = 0;
int Movement;
int resetcount = 0;
int cardoffset[6];	// Number of card states
int d2 = 1;
int leftover = 0;
 
void setup() {
  if(debug) {
    Serial.begin(57600);
    Serial.println("Debug started");
  }
  else {
    // pinMode stuff
  }

  cardoffset[0] = _;
  cardoffset[1] = _;
  cardoffset[2] = _;
  cardoffset[3] = _;
  cardoffset[4] = _;
  cardoffset[5] = _;
}


void loop() {
  bsFlip = digitalRead(Flip);
  bsLeft = digitalRead(Left);
  bsRight = digitalRead(Right);
  x = random(1,maxcard+1);
  // Check for both bottons pressed
  // 4 seconds hold is door close
  // 8 seconds hold is door open
  if (bsLeft == HIGH && bsRight ==HIGH)  {
    if (debug) { Serial.println("Both bottons pressed"); }
    beep(100);
    resetcount = 0;
    while (bsLeft ==HIGH && bsRight == HIGH) {
      resetcount++;       
      if (resetcount == 400) { beep(50); }
      if (resetcount == 800) { beep(50); }
      bsLeft = digitalRead(Left);
      bsRight = digitalRead(Right);
      delay(8);
    }
    if (debug) { Serial.println(resetcount); }

    if (resetcount > 800) { 
      DoorOpen(); 
    }
    else
      {
	if (resetcount > 400) { 
	  DoorClose(); 
	}
      }
  }

  if (bsLeft != bLeft) {
    if (bsLeft == HIGH) {
      if (debug) { Serial.print(state); Serial.println(" - Left pressed"); }
      bLeft = bsLeft;  
      if (debug2) {
	d2 = CurrentCard +1;
	if (d2>40) d2=1;
	if (debug) { Serial.print("DEBUG2: +1 -> "); Serial.println(d2);}
	FlipCard(d2);  
	while ((bsLeft != HIGH && bsRight !=HIGH)) {  // make sure buttons are unpressed before continue
	  bsLeft = digitalRead(Left);
	  bsRight = digitalRead(Right);
	}
      }
      else
	{
	  switch(state) {
	  case 0: 
	    statechange = true; newstate =1;
	    break;
	  case 1: 
	    statechange = true; newstate =2;
	    break;
	  case 2: 
	    answer = false;
	    statechange = true; newstate =2;
	    break;
	  case 3: 
	    answer = false;
	    statechange = true; newstate =3;
	    break;
          }
	}
    }
    else
      {
        
	bLeft = bsLeft;
      }
       
  }
  
  if (bsRight != bRight) {
    Serial.println(bsRight);
    if (bsRight == 1 ) 
      { 
        if (debug) { Serial.println("Right pressed"); }
        bRight = bsRight; 
        if (debug2) {
          d2 = CurrentCard +10;
          if (d2>40) d2 = 40-d2;
          if (debug) { Serial.print("DEBUG2: +10 -> "); Serial.println(d2);}
          FlipCard(d2);  
	  while ((bsLeft != HIGH && bsRight !=HIGH)) {  // make sure buttons are unpressed before continue
	    bsLeft = digitalRead(Left);
	    bsRight = digitalRead(Right);
	  }
	}
	else
	  {
	    switch(state) {
	    case 0: 
	      statechange = true; newstate =1;
	      break;
	    case 1: 
	      statechange = true; newstate =2;
	      break;
	    case 2: 
	      answer = true;
	      statechange = true; newstate =2;
	      break;
	    case 3: 
	      answer = true;
	      statechange = true; newstate =3;
	      break;
	    }
	  }
      }
    else
      {
      
	bRight = bsRight;
      }
  }
  
  // Execute actions
  // state 0 - Start stage with welcome screen - CARD 1
  // state 1 - Explain game                    - CARD 40
  // state 2 - Game in progress                - Randon
  
  if (statechange) {
    statechange = false;
    // if (debug) { Serial.print("State change : "); Serial.print(state); Serial.print (" - > "); Serial.println(newstate); }
    state = newstate;
    switch(state) {
    case 1: 
      FlipCard(2);
      break;
    case 2: 
      options=false;
      for (int i=1; i<=maxcard; i++) {
	if (cards[i] == false) {options = true;}
      }
      if (options == false) {
	for(int i=1; i<=maxcard; i++) { cards[i] = false;}
        if (debug) { Serial.println("all cards asked, resetting"); }
      }
      notfound = true;
      while (notfound) {
	randomcard = random(1,maxcard+1);
	if (cards[randomcard] == false) {
	  notfound=false;
	  cards[randomcard]=true;
	}
      }
      Serial.print ("Random selected card : ");
      Serial.println(randomcard);
      FlipCard(randomcard+2);
      state =3;
      break;
    case 3: 
      // check answer
       
      // correct answer
      if (answer == cardanswer[randomcard]) {
	if (debug) {Serial.print ("Correct answer! "); Serial.println(randomcard);}
	score = score +1;
	if (score == 5) { //winner winner, chicken dinner, lets open the door
	  if (debug) {Serial.print ("Winner, lets open the door "); }
	  FlipCard(40);
	  updatescore(score);  
	  DoorOpen();
	  while (1 == 1) {
	    updatescore(0);
	    delay(500);
	    updatescore(5);
	    delay(500);
              
	  }
            
	}
	else
	  {
	    FlipCard(39);
	    updatescore(score);       
	  }
      }
      else // INCORRECT ANSWER
        {
	  if (debug) {Serial.print ("Incorrect answer! anserwer given: "); Serial.print(answer); Serial.print("Card: "); Serial.print(randomcard);}
	  FlipCard(38); 
	  score=0;
	  updatescore(score);
        }
        
      state =2;
      break;
    
    }
    
    
        
    
    while ((bsLeft != HIGH && bsRight !=HIGH)) {  // make sure buttons are unpressed before continue
      bsLeft = digitalRead(Left);
      bsRight = digitalRead(Right);
    }
    //if (debug) { Serial.println("Button released"); }
    
  } // end statechanges
  

} // end loop

void FlipCard(int nr) {
 if (nr != CurrentCard) {  
   if (nr > CurrentCard) {
      Movement = leftover + ((nr - CurrentCard) * 20);
   }
   else
   {
     Movement = leftover + (((40-CurrentCard)+nr) * 20);
   }

   FlipFlap.step(Movement-10+cardoffset[nr]);
  leftover = 10-cardoffset[nr];
   if (debug) {Serial.print ("Card: "); Serial.print(nr);Serial.print (" Left over "); Serial.print(leftover); Serial.print (" Offset "); Serial.print(cardoffset[nr]);Serial.print (" Movement:"); Serial.println(Movement);}
 
   CurrentCard = nr;
 }
}

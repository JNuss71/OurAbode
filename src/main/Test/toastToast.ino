
void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200)
  randomSeed(analogRead(0));
  
}

void loop()
{
  // put your main code here, to run repeatedly:
  int newTime(int timeHigh,int timeLow); //determines next time to toast the toast
  rateToast(char rating); //Rating of toast affects time
  void toastToast(int toastTime);//Causes the toaster to toast 
  
  int main()
  {
  char rating; // Should be either D/d N/n or L/l
  int timeHigh(300), timeLow(30), toastTime; //defaults to 300 seconds highest and 30 seconds 
 
  toastTime = random(timeLow,timeHigh); // Creates first random toast time 
 
  toastToast(toastTime);
  rateToast(rating);
  newTime(timeHigh, timeLow);
  toastToast(toastTime);
  rateToast(rating);
  newTime(timeHigh, timeLow);
 
  }

  void rateToast(char rating)
  {

   Serial.println(";How would you rate this toast? Please input (D) for  if you would like it darker, (L) for  if you would like itlighter, or (N) for neutral.");
   
   rating = Serial.read();
   
   while ( ( ( ((rating != 'D')||(rating != 'd')) || ((rating != 'N')||(rating != 'n')) ) || ((rating != 'L')||(rating != 'l')) ) )
   {
      Serial.println(";Please input either D,N,or L.");
      rating = Serial.read();
   
   }

   if((rating == 'D')||(rating == 'd')) //D for darker, increases length of toasting time
   {
      timeHigh += 5;
      timeLow += 5;
   }
   
   if((rating == 'N')||(rating == 'n') // N for neutral, closes in the times to find "optimal toasting time"
   {
      timeHigh -= 5;
      timeLow += 5;
   }
   
   if((rating == 'L')||(rating == 'l') // L for lighter, decreases length of toasting time
   {
      timeHigh -= 5;
      timeLow -= 5;
   }

  }

  int newTime(int timeHigh,int timeLow)
  {

      toastTime = random(timeLow,timeHigh);

      return toastTime;

  }

  void toastToast(int toastTime)
  {

      Serial.println(";Toasting toast for "+toastTime+" seconds.");

      //Here would be the stuff that tells the toaster to toast the toast

      Serial.println(";Toasting complete!");

  }

 }

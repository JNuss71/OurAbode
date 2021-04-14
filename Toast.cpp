#include<iostream>
#include<cstdlib>
using namespace std;


int newTime(int timeHigh,int timeLow); //determines next time to toast the toast
void rateToast(char rating); //Rating of toast affects time
void toastToast(int toastTime);//Causes the toaster to toast 

int main()
{
 char rating; // Should be either D/d N/n or L/l
 int timeHigh(300), timeLow(30), toastTime; //defaults to 300 seconds highest and 30 seconds 
 
 toastTime = rand() % timeLow + timeHigh; // Creates first random toast time 
 
 toastToast(toastTime);
 rateToast(rating);
 newTime(timeHigh, timeLow);
 toastToast(toastTime);
 rateToast(rating);
 newTime(timeHigh, timeLow);
 
}

void rateToast(char rating)
{

   cout << "How would you rate this toast? Please input (D) for darker, (L) for lighter, or (N) for neutral." << endl;
   
   cin >> rating;
   
   while ( ( ( ((rating != 'D')||(rating != 'd')) || ((rating != 'N')||(rating != 'n')) ) || ((rating != 'L')||(rating != 'l')) ) )
   {
      cout << "Please input either D,N,or L." << endl;
      cin >> rating;
   
   }

   if((rating == 'D')||(rating == 'd')) //D for darker, increases length of toasting time
   {
      timeHigh += 1;
      timeLow += 1;
   }
   
   if((rating == 'N')||(rating == 'n') // N for neutral, closes in the times to find "optimal toasting time"
   {
      timeHigh -= 1;
      timeLow += 1;
   }
   
   if((rating == 'L')||(rating == 'l') // L for lighter, decreases length of toasting time
   {
      timeHigh -= 1;
      timeLow -= 1;
   }

}

int newTime(int timeHigh,int timeLow)
{

      toastTime = rand() % timeLow + timeHigh;

      return toastTime;

}

void toastToast(int toastTime)
{

      cout >> "Toasting toast for " >> toastTime >> " seconds." >> endl;

      //Here would be the stuff that tells the toaster to toast the toast

      cout >> "Toasting complete!" >> endl;

}


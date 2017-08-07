// 2-dimensional array of row pin numbers:
int R[] = {2,7,A5,5,13,A4,12,A2};  
// 2-dimensional array of column pin numbers:
int C[] = {6,11,10,3,A3,4,8,9};    
  
unsigned char biglove[8][8] =     //the big "heart"   
{  
  0,0,0,0,0,0,0,0,  
  0,1,1,0,0,1,1,0,  
  1,1,1,1,1,1,1,1,  
  1,1,1,1,1,1,1,1,  
  1,1,1,1,1,1,1,1,  
  0,1,1,1,1,1,1,0,  
  0,0,1,1,1,1,0,0,  
  0,0,0,1,1,0,0,0,  
};  
  
unsigned char smalllove[8][8] =      //the small "heart" 
{  
  0,0,0,0,0,0,0,0,  
  0,0,0,0,0,0,0,0,  
  0,0,1,0,0,1,0,0,  
  0,1,1,1,1,1,1,0,  
  0,1,1,1,1,1,1,0,  
  0,0,1,1,1,1,0,0,  
  0,0,0,1,1,0,0,0,  
  0,0,0,0,0,0,0,0,  
};  
  
void setup()  
{  
   // iterate over the pins:
  for(int i = 0;i<8;i++)  
  // initialize the output pins:
  {  
    pinMode(R[i],OUTPUT);  
    pinMode(C[i],OUTPUT);  
  }  
}  
  
void loop()  
{  
  for(int i = 0 ; i < 100 ; i++)        //Loop display 100 times 
  {  
    Display(biglove);                   //Display the "Big Heart"  
  }  
  for(int i = 0 ; i < 50 ; i++)         //Loop display 50 times
  {     
    Display(smalllove);                 //Display the "small Heart" 
  }  
}  
  
void Display(unsigned char dat[8][8])    
{  
  for(int c = 0; c<8;c++)  
  {  
    digitalWrite(C[c],LOW);//use thr column 
    //loop
    for(int r = 0;r<8;r++)  
    {  
      digitalWrite(R[r],dat[r][c]);  
    }  
    delay(1);  
    Clear();  //Remove empty display light
  }  
}  
  
void Clear()                          //清空显示  
{  
  for(int i = 0;i<8;i++)  
  {  
    digitalWrite(R[i],LOW);  
    digitalWrite(C[i],HIGH);  
  }  
}  


//========================================================//
//  predictor.c                                           //
//  Source file for the Branch Predictor                  //
//                                                        //
//  Implement the various branch predictors below as      //
//  described in the README                               //
//========================================================//
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "predictor.h"

//
// TODO:Student Information
//
const char *studentName = "Naga Harshini Voruganti";
const char *studentID   = "A53247771";
const char *email       = "nvorugan@ucsd.edu";

//------------------------------------//
//      Predictor Configuration       //
//------------------------------------//

// Handy Global for use in output routines
const char *bpName[4] = { "Static", "Gshare",
                          "Tournament", "Custom" };

int ghistoryBits ; // Number of bits used for Global History
int lhistoryBits; // Number of bits used for Local History
int pcIndexBits;  // Number of bits used for PC index
int bpType;       // Branch Prediction Type
int verbose;

uint32_t global_history; //Can take a max of 32 bits. 

//------------------------------------//
//      Predictor Data Structures     //
//------------------------------------//
uint8_t *BHT;
uint32_t Gshare_index;
uint8_t pred;
uint8_t *global_pred;
uint8_t *choice_pred;
uint8_t *local_pred;
uint32_t *Local_History_Table;
uint8_t my_choice_prediction;
uint8_t my_global_prediction;
uint8_t my_local_prediction;
uint8_t my_prediction;
uint32_t Local_History_Table_Index;
int count =0;
//
//TODO: Add your own Branch Predictor data structures here
//

uint8_t global_predictor (uint32_t global_history)
{
  uint32_t temp_index;
  temp_index = global_history & ((int)(pow(2,ghistoryBits))-1);
 if ((global_pred[temp_index]==0) || (global_pred[temp_index]==1))
  return 0;
else
  return 1;
}
uint8_t local_predictor (uint32_t pc)
{
 uint32_t temp_index;
 temp_index = Local_History_Table[pc&((int)(pow(2,pcIndexBits))-1)];
 if ((local_pred[temp_index]==0)|| (local_pred[temp_index]==1)) 
  return 0;
else 
  return 1;
}
void init_tournament()
{
    global_pred = malloc((int)(pow(2,ghistoryBits))*sizeof(uint8_t));
    choice_pred = malloc((int)(pow(2,ghistoryBits))*sizeof(uint8_t));
    local_pred = malloc((int)(pow(2,lhistoryBits))*sizeof(uint8_t));
    Local_History_Table =malloc((int)(pow(2,pcIndexBits))*sizeof(uint32_t));
    memset(global_pred,1,sizeof(global_pred));
    memset(local_pred,1,sizeof(local_pred));
    memset(Local_History_Table,0,sizeof(Local_History_Table));
    memset(choice_pred,2,sizeof(choice_pred));
}

int i;
void
init_predictor()
{
  //
  //TODO: Initialize Branch Predictor Data Structures
  //
  if (bpType == GSHARE)
  {
    global_history=0;
    //int temp =(int)(pow(2,ghistoryBits));
    //printf("%d\n",temp);
    BHT = malloc((int)(pow(2,ghistoryBits))*sizeof(uint8_t)); // 0 - Strong NT; 1 - Weak NT; 2 - Weak Taken; 3 - Strong Taken
    memset(BHT,1,sizeof(BHT));
    //for(i=0;i<temp;i++) // Check to see if all the elements of BHT are properly being set
    //printf("%d\n",BHT[i]);
  } 
  if (bpType == TOURNAMENT)
  {
   global_history=0;
   init_tournament();
  }
}

// Make a prediction for conditional branch instruction at PC 'pc'
// Returning TAKEN indicates a prediction of taken; returning NOTTAKEN
// indicates a prediction of not taken
//
uint8_t
make_prediction(uint32_t pc)
{
  //
  //TODO: Implement prediction scheme
  //
  
  // Make a prediction based on the bpType
  switch (bpType) {
    case STATIC:
      return TAKEN;
    case GSHARE:
      Gshare_index= (pc ^ global_history)&((int)(pow(2,ghistoryBits)) -1);
      pred = BHT[Gshare_index];
      if (pred == 0 || pred ==1 )
        return NOTTAKEN;
      else
        return TAKEN;        
    case TOURNAMENT:
      my_global_prediction = global_predictor(global_history);
      my_local_prediction = local_predictor(pc);
      my_choice_prediction = choice_pred[global_history&((int)(pow(2,ghistoryBits)) -1)];
      //if (count <5) {printf("global = %d local = %d choice = %d \n ",my_global_prediction,my_local_prediction,my_choice_prediction); count++;}
      if ((my_choice_prediction == 0) || (my_choice_prediction ==1))
        my_prediction = my_local_prediction;
      else
        my_prediction = my_global_prediction;
      //printf("my_prediction: %d\n", my_prediction );
      return my_prediction;
    case CUSTOM:
    default:
      break;
  }

  // If there is not a compatable bpType then return NOTTAKEN
  return NOTTAKEN;
}

// Train the predictor the last executed branch at PC 'pc' and with
// outcome 'outcome' (true indicates that the branch was taken, false
// indicates that the branch was not taken)
//
uint32_t temp;
void
train_predictor(uint32_t pc, uint8_t outcome)
{
  //
  //TODO: Implement Predictor training
  //
  //printf("%d\n",outcome);
  switch (bpType) {


    case GSHARE:
      temp = (pc ^ global_history)&((int)(pow(2,ghistoryBits))-1);

      if (outcome == 0)
      {
        if (BHT[temp] != 0)
          BHT[temp] = BHT[temp] -1;
          }
        
      if (outcome == 1)
         {
          if (BHT[temp]!= 3 )
            BHT[temp] = BHT[temp] +1;
            }
      global_history = (global_history << 1);  
      global_history= global_history | outcome;


    case TOURNAMENT:
      
      //Updating my Choice Predictor
      if (my_global_prediction != my_local_prediction)
      {
        temp = global_history&((int)(pow(2,ghistoryBits)) -1); 
        if (outcome == my_local_prediction) 
       {
         if (choice_pred[temp]!=0)
          choice_pred[temp]--;
       }
       else
       {
        if (choice_pred[temp]!=3)
          choice_pred[temp]++;
       }
       }

      //Updating my Local Predictor
      Local_History_Table_Index = pc&((int)(pow(2,pcIndexBits))-1);  
      temp = Local_History_Table[Local_History_Table_Index];
      if (outcome == 0)
      {
        if (local_pred[temp] != 0)
          local_pred[temp]--;
      }        
      if (outcome == 1)
         {
          if (local_pred[temp]!= 3 )
            local_pred[temp]++;
          }
      //Updating my Local Branch History Table
         //Local_History_Table_Index = pc&((int)(pow(2,pcIndexBits))-1);
         Local_History_Table[Local_History_Table_Index] = (Local_History_Table[Local_History_Table_Index] << 1);
         Local_History_Table[Local_History_Table_Index] = Local_History_Table[Local_History_Table_Index] &((int)(pow(2,lhistoryBits))-1);
         Local_History_Table[Local_History_Table_Index] = Local_History_Table[Local_History_Table_Index] | outcome;

      //Updating my global predictors
      temp = global_history&((int)(pow(2,ghistoryBits)) -1);    
      if (outcome == 0)
      {
        if (global_pred[temp] != 0)
          global_pred[temp]--;
      }
      if (outcome == 1)
         {
          if (global_pred[temp]!= 3 )
            global_pred[temp]++;
          }
      //Updating my global history
      global_history = (global_history << 1)&((int)(pow(2,ghistoryBits))-1) ;  
      global_history= global_history | outcome;
      
    default:
      break;
  }

}

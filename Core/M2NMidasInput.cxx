
// M2N
#include "M2NMidasInput.h"
#include "M2NRootOutput.h"
//STL
#include <fstream>
#include <Riostream.h>
#include <Byteswap.h>
//ROOT
#include "TRandom.h"

using namespace std;

////////////////////////////////////////////////////////////////////////////////
M2N::MidasInput::MidasInput(){
  m_DetectorManager=0;
  m_FileName="not-set";
  m_Tree= M2N::RootOutput::getInstance()->GetTree();
}
////////////////////////////////////////////////////////////////////////////////
M2N::MidasInput::~MidasInput(){
}
////////////////////////////////////////////////////////////////////////////////
void M2N::MidasInput::SetDetectorManager(M2N::DetectorManager* mgr){
  m_DetectorManager = mgr;
}
////////////////////////////////////////////////////////////////////////////////
void M2N::MidasInput::SetFileName(string filename){
  m_FileName=filename;
}
////////////////////////////////////////////////////////////////////////////////
void M2N::MidasInput::TreatFile(){
  // Minor adaptation of Raw_Tree.C macro from University of York group 
/*   if(unzip) {
    TString zipcmd = Form("gunzip %s.gz", f_in.Data());
    gSystem->Exec (zipcmd);
  }
  */
  //int adcN, tdcN, scalarN, evtNum;
  int evtNum; 
  int scalar[32];

  ifstream fin;
  fin.open(m_FileName,ifstream::binary);

  const int blkSize = 16384;
  const int nWdChar = blkSize / sizeof(char);
  const int nWdInt = blkSize / sizeof(int);
  char bytes[blkSize];
  char *bytesPtr = bytes;

  int blocks, events, evt_len;
  short group,item,address, *end_event;
  short *half,*end_block,end_data;

  blocks=0;
  events=0;

  while (!fin.eof()){
    if(fin.read(bytesPtr,nWdChar)){
      blocks++;
      half=(short *) bytesPtr;

      for(int j=0;j < 4;j++)half[j]=Rbswap_16(half[j]);
      for(int j=13;j < nWdInt*2;j++)half[j]=Rbswap_16(half[j]);

      half+=10;
      end_data=*(half)/2+13;
      end_block=half+end_data;
      half++;

      while (half<end_block){
        ++half;
        evt_len=(*half-2)/2;
        end_event=half+evt_len;
        if (evt_len>1000)break;

        if(evt_len>0){
          evtNum=0;

          for (int i=0;i<32;++i){
            scalar[i]=0;
          }

          ++half;

          while((half < end_event)){
            group=*half & 0x00ff;
            item=*half >> 8 & 0x003f;
            if(group > 0 && group !=31){

              address = 32 * (group - 1) + item;
              ++half;
              if(*half>0 && *half<4095){
                m_DetectorManager->Fill(address,*half);
              }
            }
            else if (group == 31){
              half=end_event;
            }
                  ++half;
          }
          events++;
          evtNum=events-1;
          m_Tree->Fill();
          half=end_event;
        }
      }

      if(!(blocks%500)){
        cout << "\rProcessing Block:  " << blocks;
        cout.flush();
      }
    }
  }

  cout << "\r----------------------Processed All Blocks-----------------------" << endl;
  cout << "blocks " << blocks << endl;

  fin.close();
}
////////////////////////////////////////////////////////////////////////////////
void M2N::MidasInput::SimulateTreat(int event, int cmin , int cmax, int vmin,int vmax){
  unsigned int count=0;
  cout << "Starting Simulation " << endl;
  TRandom rand;

  while( count++ < event){
  int channel;
  int value;
  m_DetectorManager->Clear();
  if(count%500==0)
    cout <<"\r **** Simulating treat : " << count*100.0/event  << "% ****" ;
    // Random channel
    channel = rand.Uniform(cmin,cmax);
    
    // Random value
    value = rand.Uniform(vmin,vmax);
     m_DetectorManager->Fill(channel,value);
     m_Tree->Fill();
      
  }
  cout << endl;
}


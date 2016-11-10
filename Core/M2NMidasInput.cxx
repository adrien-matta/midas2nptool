
// M2N
#include "M2NMidasInput.h"
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
  int adcN, tdcN, scalarN, evtNum;
  int scalar[32];

  unsigned short SPpos, SPde, SPwire, SPplasp, SPplasg;
  unsigned short adcList[512], tdcList[512], adcData[512], tdcData[512];

  const Int_t blkSize = 16384;
  const Int_t nWdChar = blkSize / sizeof(Char_t);
  const Int_t nWdInt = blkSize / sizeof(Int_t);
  char bytes[blkSize];
  char* bytesPtr = bytes;

  int blocks, events, evt_len;
  short group,item,address; 
  short* end_event;
  short* half;
  short* end_block;
  short end_data;

  blocks=0;
  events=0;

  ifstream fin;
  fin.open(m_FileName.c_str(),ifstream::binary);

  while (!fin.eof()){
    if(fin.read(bytesPtr,nWdChar)){
      blocks++;
      half=(Short_t *) bytesPtr;

      for(Int_t j=0;j < 4;j++)half[j]=Rbswap_16(half[j]);
      for(Int_t j=13;j < nWdInt*2;j++)half[j]=Rbswap_16(half[j]);

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
          adcN = 0;
          tdcN = 0;
          scalarN = 0;
          evtNum=0;

          SPpos = 0;
          SPde = 0;
          SPwire = 0;
          SPplasp = 0;
          SPplasg = 0;

          for (Int_t i=0;i<32;++i){
            scalar[i]=0;
          }

          (void) memset(adcList,0,sizeof(UShort_t) * 512);
          (void) memset(tdcList,0,sizeof(UShort_t) * 512);
          (void) memset(adcData,0,sizeof(UShort_t) * 512);
          (void) memset(tdcData,0,sizeof(UShort_t) * 512);

          ++half;

          while((half < end_event)){
            group=*half & 0x00ff;
            item=*half >> 8 & 0x003f;

            if(group > 0 && group < 20){
              if(group==4){
                if(item<16) address = 32 * (group - 1) + item + 16;
                if(item>15) address = 32 * (group - 1) + item - 16;
                ++half;
              }
              else{
                address = 32 * (group - 1) + item;
                ++half;
              }
              if(*half<4095 && address < 384){
                adcList[adcN]=address;
                adcData[adcN]=*half;

                if(adcList[adcN]==192)SPpos=*half;
                if(adcList[adcN]==193)SPde=*half;
                if(adcList[adcN]==194)SPwire=*half;
                if(adcList[adcN]==195)SPplasp=*half;
                if(adcList[adcN]==196)SPplasg=*half;
              }
              ++adcN;
            }

            else if (group > 19 && group < 29){
              address=64*(group - 24) + item;
              ++half;

              tdcList[tdcN]=address;
              tdcData[tdcN]=*half;
              ++tdcN;
            }

            else if (group == 30){
              for(Int_t i=0; i<3 ;i++){
                half++;
                if(*half< 0) scalar[i] = 65536 + (*half);
                else scalar[i]=*half;

                half+=2;
                scalar[i] += (*half) * 65536;
                if (i==2) scalarN = scalar[i];
                half++;
              }
              half--;
            }

            else if (group == 31){
              half=end_event;
            }
            ++half;
          }

          events++;
          evtNum=events-1;
          //tout->Fill();
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

  //tout->Write();
 // fout->Close();
  fin.close();




}
////////////////////////////////////////////////////////////////////////////////
void M2N::MidasInput::SimulateTreat(int event, int cmin , int cmax, int vmin,int vmax){
  unsigned int count;
  cout << "Starting Simulation " << endl;
  TRandom rand;

  while( count++ < event){
  int channel;
  int value;
  if(count%500==0)
    cout <<"\r **** Simulating treat : " << count*100.0/event  << "% ****" ;

    // Random channel
    channel = rand.Uniform(cmin,cmax);
    
    // Random value
    value = rand.Uniform(vmin,vmax);
    m_DetectorManager->Fill(channel,value);
  }
  cout << endl;
}


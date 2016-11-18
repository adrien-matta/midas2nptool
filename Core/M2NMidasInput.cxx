
// M2N
#include "M2NMidasInput.h"
#include "M2NRootOutput.h"
//STL
#include <fstream>
#include <Riostream.h>
#include <Byteswap.h>
#include <bitset>
using namespace std;

//ROOT
#include "TRandom.h"


////////////////////////////////////////////////////////////////////////////////
M2N::MidasInput::MidasInput(){
  m_DetectorManager=0;
  m_FileName="not-set";
  m_Tree= M2N::RootOutput::getInstance()->GetTree();
  m_HitPattern = new TH1F("HitPattern","Hit Pattern",2500,0,2500);
  m_blocks = 0; 
  m_events = 0;
  // Check the header size is correct:
  M2N::DATA_HEADER block;
  double header_size = sizeof(block.header_id)+
    sizeof(block.header_sequence)+
    sizeof(block.header_stream)+
    sizeof(block.header_tape)+
    sizeof(block.header_MyEndian)+
    sizeof(block.header_DataEndian)+
    sizeof(block.header_dataLen);

  if(header_size!=24){
    cout << "ERROR: The header block size is " << header_size << "instead of 24 bytes " << endl;
    exit(1);
  }
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
bool M2N::MidasInput::ReadBlockHeader(ifstream& fin){
  m_bytes_read = 0;
  strcpy(m_header.header_id,"        \0");
  static char buffer1[5];
  static char buffer2[5];
  strcpy(buffer1,"    \0");
  strcpy(buffer2,"    \0");
  while(strcmp(m_header.header_id,"EBYEDATA")!=0 && !fin.eof()){     
    // there is some padding at the end of the block to be ignored
    // because a block is always 16K but an event cannot by split on two blocks
    fin.read(buffer1,4);
    if(strcmp(buffer1,"EBYE")==0){
      fin.read(buffer2,4);
      for(unsigned int i = 0 ; i < 4 ;i++)
        m_header.header_id[i] = buffer1[i];
      for(unsigned int i = 0 ; i < 4 ;i++)
        m_header.header_id[i+4] = buffer2[i];
    }

  } 
  if(fin.eof()) 
    return false;
  else{
    m_blocks++;
    fin.read((char*)&m_header.header_sequence,sizeof(m_header.header_sequence));
    fin.read((char*)&m_header.header_stream,sizeof(m_header.header_stream));
    fin.read((char*)&m_header.header_tape,sizeof(m_header.header_tape));
    fin.read((char*)&m_header.header_MyEndian,sizeof(m_header.header_MyEndian));
    fin.read((char*)&m_header.header_DataEndian,sizeof(m_header.header_DataEndian));
    fin.read((char*)&m_header.header_dataLen,sizeof(m_header.header_dataLen));
    return true;
  }
}
////////////////////////////////////////////////////////////////////////////////
unsigned short M2N::MidasInput::Swap(unsigned short value){
  bitset<16> bsvalue(value);
  bitset<16> bsword(0);
  for(unsigned int i = 0 ; i < 16 ; i++){
    bsword[15-i] = bsvalue[i];
  }

  unsigned short word = bsword.to_ulong();
  return word;
}


////////////////////////////////////////////////////////////////////////////////
unsigned short M2N::MidasInput::ReadWord(ifstream& fin){
  m_bytes_read+=2;
  unsigned short word = 0; 
  fin.read((char*)&word,sizeof(word));
  if(m_header.header_DataEndian){
    word = Rbswap_16(word);
    word = Swap(word); 
  }
  return word;
}
////////////////////////////////////////////////////////////////////////////////
unsigned short M2N::MidasInput::ReadMiniWord(ifstream& fin){
  m_bytes_read+=1;
  unsigned short word = 0; 
  fin.read((char*)&word,1);
  return word;
}


////////////////////////////////////////////////////////////////////////////////
unsigned short M2N::MidasInput::BitMask(unsigned short a, unsigned short b){
  unsigned short r = 0;
  for (unsigned i=a; i<=b; i++)
    r |= 1 << i;
  return r;
}
////////////////////////////////////////////////////////////////////////////////
bool M2N::MidasInput::ReadBlock(ifstream& fin){
  // Read the block header
  if(!ReadBlockHeader(fin)){
    return false;
  }

  // Read the block 
  unsigned short int whole;
  unsigned short int address;
  unsigned short int item;
  unsigned short int value;
  unsigned short int control;
  unsigned short int count;
  unsigned short int group;
  unsigned short int min;
  unsigned short int max;
  while(1){
    // read the first word
    whole = ReadWord(fin);
    unsigned short int ctrl = whole & BitMask(14,15) >> 14;
    if(ctrl == 3){ // begin of event or end of block
      unsigned short int check1 = (whole & BitMask(8,13))>>8;
      unsigned short int check2 = (whole & BitMask(0,7));

      value = ReadWord(fin);
      if(check1==0x3f && check2==0xff){
        if(value == 0){ // end of block (end of current event)
          return true;
        }
        else{ // begin of event (end of previous event)
          m_Tree->Fill();
          m_DetectorManager->Clear();
          if(m_events++%5000 ==0)
            cout << "\r Blocks treated: " << m_blocks << "\t Event treated: " << m_events;
        }
      }
      else{
        cout << "Shit Happen!" << endl;
      }
    }

    else if(ctrl == 0){ // Simple data word
      value = ReadWord(fin);
      address = Swap(whole);
      group=address & 0x00ff;
      item=address >> 8 & 0x003f;
      address = 32 * (group - 1) + item;

      m_DetectorManager->Fill(address,Swap(value));
      m_HitPattern->Fill(address);
    }

    else if(ctrl == 1){ // Group data item
      count = (whole & BitMask(8,13));
      group = (whole & BitMask(0,7));
      for(unsigned int i = 0 ; i < count ; i++){
        value = ReadWord(fin);
      }
    }
    else if(ctrl == 2){ // Extended group data item
      count = (whole & BitMask(0,13));
      group = ReadWord(fin);
      if(count == 0){
        min = ReadWord(fin);
        max = ReadWord(fin);
        count = max - min;
      }

      for(unsigned int i = 0 ; i < count ; i++){
        value = ReadWord(fin);
      } 
    }
    else
      cout << "Control shit happen : " << ctrl << endl;
  }
}

////////////////////////////////////////////////////////////////////////////////
void M2N::MidasInput::TreatFile(){

  cout << "\r------------------------Starting Process-------------------------" << endl;

  // Minor adaptation of Raw_Tree.C macro from University of York group 
  ifstream fin;
  fin.open(m_FileName,ifstream::binary);
  while(ReadBlock(fin)){
    // Read all the block. ReadBlock return false on eof
  }
  cout << "\r----------------------Processed All Blocks-----------------------" << endl;
  cout << "Treated blocks: " << m_blocks << "\t Treated Events: " << m_events << endl;

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


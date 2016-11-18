void Show(){
  TFile* file = new TFile("PR3_1.root");
  TTree* tree = (TTree*) file->FindObjectAny("T40Data");
  TH1*   h    = (TH1*)    file->FindObjectAny("HitPattern");
  h->Draw();
  gPad->SetLogy();
  cout << tree->GetEntries() << endl;
}

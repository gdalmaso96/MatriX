void recover(TString name){
	TFile* F = TFile::Open(name + ".root");
	TTree* T = (TTree*) F->Get("T");

	double ein = 0, edep = 0, delta = 0;
	int eventID = 0, NgammaSec = 0;

	vector<int>* Channel_ptr = 0;
	vector<int>* Cells_ptr = 0;
	vector<double>* CellTime_ptr = 0;
	vector<int>* OCTflag_ptr = 0;
	vector<int>* DNflag_ptr = 0;

	vector<int> Channel;
	vector<int> Cells;
	vector<double> CellTime;
	vector<int> OCTflag;
	vector<int> DNflag;

	double GunTime = 0;

	T->SetBranchAddress(      "ein",          &ein);
	T->SetBranchAddress(     "edep",         &edep);
	T->SetBranchAddress(    "delta",        &delta);
	T->SetBranchAddress(  "eventID",      &eventID);
	T->SetBranchAddress("NgammaSec",    &NgammaSec);
	T->SetBranchAddress(  "Channel",  &Channel_ptr);
	T->SetBranchAddress(    "Cells",    &Cells_ptr);
	T->SetBranchAddress( "CellTime", &CellTime_ptr);
	T->SetBranchAddress(  "OCTflag",  &OCTflag_ptr);
	T->SetBranchAddress(   "DNflag",   &DNflag_ptr);
	T->SetBranchAddress(  "GunTime",      &GunTime);

	TFile* N = TFile::Open(name + "new.root", "RECREATE");
	TTree* Tnew = new TTree("T", "new");

	Tnew->Branch(      "ein",       &ein);
	Tnew->Branch(     "edep",      &edep);
	Tnew->Branch(    "delta",     &delta);
	Tnew->Branch(  "eventID",   &eventID);
	Tnew->Branch("NgammaSec", &NgammaSec);
	Tnew->Branch(  "Channel",   &Channel);
	Tnew->Branch(    "Cells",     &Cells);
	Tnew->Branch( "CellTime",  &CellTime);
	Tnew->Branch(  "OCTflag",   &OCTflag);
	Tnew->Branch(   "DNflag",    &DNflag);
	Tnew->Branch(  "GunTime",   &GunTime);

	int n_entries = T->GetEntries();
	for(int i = 0; i < n_entries; i++){
		T->GetEntry(i);
		Channel  = *Channel_ptr;
		Cells    = *Cells_ptr;
		CellTime = *CellTime_ptr;
		OCTflag  = *OCTflag_ptr;
		DNflag   = *DNflag_ptr;
		Tnew->Fill();
	}

	F->Close();
	N->cd();
	Tnew->Write("T", TObject::kOverwrite);
	N->Close();
}

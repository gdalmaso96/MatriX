#include "./merge.C"
#include "./signalsLite.C"
#include "./readgio.cpp"

void merging(){
	TString flavour[2] = {"pos", "mu"};
	vector<TString> ein;
	vector<TString> rate;
	vector<TString> rate2;
	TString mask[2] = {"", "No"};
	vector<TString> SiPM;

	ein.push_back("0220");
	ein.push_back("5280");
	rate.push_back("10e6");
	rate.push_back("10e7");
	rate.push_back("10e8");
	rate.push_back("19e9");
	rate2.push_back("50e6");
	rate2.push_back("70e6");
	rate2.push_back("90e6");
	SiPM.push_back("25");
	SiPM.push_back("50");
/*
	for(int i = 0; i < rate.size(); i++){
		for(int j = 0; j < ein.size(); j++){
			for(int l = 0; l < 2; l++){
				for(int k = 0; k < SiPM.size(); k++){
					TString name = flavour[0] + ein.at(j) + "Mev" + rate.at(i) + mask[l] + "Mask" + SiPM.at(k);
					merge(name, 4);
				}
			}
		}
	}

	for(int i = 0; i < 2; i++){
		for(int j = 0; j < SiPM.size(); j++){
			TString name = flavour[1] + "0370MeV" + rate.at(2) + mask[i] + "Mask" + SiPM.at(j);
			merge(name, 4);
		}
	}

*/
//	for(int i = 0; i < 2; i++){
		//for(int j = 0; j < SiPM.size(); j++){
			for(int k = 0; k < rate2.size(); k++){
				TString name = flavour[0] + "0001Mev" + rate2.at(k) + mask[0] + "Mask" + SiPM.at(1);
				merge(name, 4);
			}
		//}
//	}
}



void analysis(){
	TString flavour[2] = {"pos", "mu"};
	vector<TString> ein;
	vector<TString> rate;
	vector<TString> rate2;
	TString mask[2] = {"", "No"};
	vector<TString> SiPM;

	ein.push_back("0220");
	ein.push_back("5280");
	rate.push_back("10e6");
	rate.push_back("10e7");
	rate.push_back("10e8");
	rate.push_back("19e9");
	rate2.push_back("50e6");
	rate2.push_back("70e6");
	rate2.push_back("90e6");
	SiPM.push_back("25");
	SiPM.push_back("50");

	

	for(int i = 0; i < rate.size(); i++){
		for(int j = 0; j < ein.size(); j++){
			for(int l = 0; l < 2; l++){
				for(int k = 0; k < SiPM.size(); k++){
					TString name = flavour[0] + ein.at(j) + "Mev" + rate.at(i) + mask[l] + "Mask" + SiPM.at(k);
					signals("./merge/merge" + name + ".root", "./out/" + name);
				}
			}
		}
	}

	for(int i = 0; i < 2; i++){
		for(int j = 0; j < SiPM.size(); j++){
			TString name =  flavour[1] + "0370MeV" + rate.at(2) + mask[i] + "Mask" + SiPM.at(j);
			signals("./merge/merge" + name + ".root", "./out/" + name);
		}
	}

//	for(int i = 0; i < 2; i++){
		//for(int j = 0; j < SiPM.size(); j++){
			for(int k = 0; k < rate2.size(); k++){
				TString name = flavour[0] + "0001MeV" + rate2.at(k) + mask[0] + "Mask" + SiPM.at(1);
				signals("./merge/merge" + name + ".root", "./out/" + name);
			}
		//}
//	}
}

void analysis1(){
	TString flavour[2] = {"pos", "mu"};
	vector<TString> ein;
	vector<TString> rate;
	vector<TString> rate2;
	TString mask[2] = {"", "No"};
	vector<TString> SiPM;

	ein.push_back("0220");
	ein.push_back("5280");
	rate.push_back("10e6");
	rate.push_back("10e7");
	rate.push_back("10e8");
	rate.push_back("19e9");
	rate2.push_back("50e6");
	rate2.push_back("70e6");
	rate2.push_back("90e6");
	SiPM.push_back("25");
	SiPM.push_back("50");

	

	for(int i = 0; i < rate.size(); i++){
		for(int j = 0; j < ein.size(); j++){
			TString name = flavour[0] + ein.at(j) + "Mev" + rate.at(i) + mask[0] + "Mask" + SiPM.at(0);
			signals("./merge/merge" + name + ".root", "./out/" + name);
		}
	}

	TString name =  flavour[1] + "0370MeV" + rate.at(2) + mask[0] + "Mask" + SiPM.at(0);
	signals("./merge/merge" + name + ".root", "./out/" + name);

	for(int k = 0; k < rate2.size(); k++){
		TString name = flavour[0] + "0001MeV" + rate2.at(k) + mask[0] + "Mask" + SiPM.at(1);
		signals("./merge/merge" + name + ".root", "./out/" + name);
	}
}

void analysis2(){
	TString flavour[2] = {"pos", "mu"};
	vector<TString> ein;
	vector<TString> rate;
	vector<TString> rate2;
	TString mask[2] = {"", "No"};
	vector<TString> SiPM;

	ein.push_back("0220");
	ein.push_back("5280");
	rate.push_back("10e6");
	rate.push_back("10e7");
	rate.push_back("10e8");
	rate.push_back("19e9");
	rate2.push_back("50e6");
	rate2.push_back("70e6");
	rate2.push_back("90e6");
	SiPM.push_back("25");
	SiPM.push_back("50");

	

	for(int i = 0; i < rate.size(); i++){
		for(int j = 0; j < ein.size(); j++){
			TString name = flavour[0] + ein.at(j) + "Mev" + rate.at(i) + mask[0] + "Mask" + SiPM.at(1);
			signals("./merge/merge" + name + ".root", "./out/" + name);
		}
	}

	TString name =  flavour[1] + "0370MeV" + rate.at(2) + mask[0] + "Mask" + SiPM.at(1);
	signals("./merge/merge" + name + ".root", "./out/" + name);

}

void analysis3(){
	TString flavour[2] = {"pos", "mu"};
	vector<TString> ein;
	vector<TString> rate;
	vector<TString> rate2;
	TString mask[2] = {"", "No"};
	vector<TString> SiPM;

	ein.push_back("0220");
	ein.push_back("5280");
	rate.push_back("10e6");
	rate.push_back("10e7");
	rate.push_back("10e8");
	rate.push_back("19e9");
	rate2.push_back("50e6");
	rate2.push_back("70e6");
	rate2.push_back("90e6");
	SiPM.push_back("25");
	SiPM.push_back("50");

	

	for(int i = 0; i < rate.size(); i++){
		for(int j = 0; j < ein.size(); j++){
			TString name = flavour[0] + ein.at(j) + "Mev" + rate.at(i) + mask[1] + "Mask" + SiPM.at(0);
			signals("./merge/merge" + name + ".root", "./out/" + name);
		}
	}

	TString name =  flavour[1] + "0370MeV" + rate.at(2) + mask[1] + "Mask" + SiPM.at(0);
	signals("./merge/merge" + name + ".root", "./out/" + name);
}


void analysis4(){
	TString flavour[2] = {"pos", "mu"};
	vector<TString> ein;
	vector<TString> rate;
	vector<TString> rate2;
	TString mask[2] = {"", "No"};
	vector<TString> SiPM;

	ein.push_back("0220");
	ein.push_back("5280");
	rate.push_back("10e6");
	rate.push_back("10e7");
	rate.push_back("10e8");
	rate.push_back("19e9");
	rate2.push_back("50e6");
	rate2.push_back("70e6");
	rate2.push_back("90e6");
	SiPM.push_back("25");
	SiPM.push_back("50");

	

	for(int i = 0; i < rate.size(); i++){
		for(int j = 0; j < ein.size(); j++){
			TString name = flavour[0] + ein.at(j) + "Mev" + rate.at(i) + mask[1] + "Mask" + SiPM.at(1);
			signals("./merge/merge" + name + ".root", "./out/" + name);
		}
	}

	TString name =  flavour[1] + "0370MeV" + rate.at(2) + mask[1] + "Mask" + SiPM.at(1);
	signals("./merge/merge" + name + ".root", "./out/" + name);
}


void rate(){
	TString flavour[2] = {"pos", "mu"};
	vector<TString> ein;
	vector<TString> rate;
	vector<TString> rate2;
	vector<double> rateD;
	vector<double> rateD2;
	TString mask[2] = {"", "No"};
	vector<TString> SiPM;

	ein.push_back("0220");
	ein.push_back("5280");
	rate.push_back("10e6");
	rate.push_back("10e7");
	rate.push_back("10e8");
	rate.push_back("19e9");
	rate2.push_back("50e6");
	rate2.push_back("70e6");
	rate2.push_back("90e6");
	rateD.push_back(1e6);
	rateD.push_back(1e7);
	rateD.push_back(1e8);
	rateD.push_back(1.9e9);
	rateD2.push_back(50e6);
	rateD2.push_back(70e6);
	rateD2.push_back(90e6);
	SiPM.push_back("25");
	SiPM.push_back("50");

	TGraphErrors* G[8];
	TGraphErrors* G2 = new TGraphErrors(3);
	double R[2];

	for(int i = 0; i < 8; i++) G[i] = new TGraphErrors(4);

	for(int i = 0; i < rate.size(); i++){
		for(int j = 0; j < ein.size(); j++){
			for(int l = 0; l < 2; l++){ // mask index
				for(int k = 0; k < SiPM.size(); k++){
					TString name = flavour[0] + ein.at(j) + "Mev" + rate.at(i) + mask[l] + "Mask" + SiPM.at(k);
					readMatrix("./out/" + (std::string) name + ".txt", R);
					G[j * 4 + l * 2 + k]->SetPoint(i, rateD.at(i), R[0] / rateD.at(i));
					G[j * 4 + l * 2 + k]->SetPointError(i, 0, R[1] / rateD.at(i));
				}
			}
		}
	}

	TFile* N = TFile::Open("results/results.root", "UPDATE");
	for(int i = 0; i < 8; i++){
		N->cd();
		G[i]->Write(ein[i/4] + "MeV" + mask[(i%4)/2] + "Mask" + SiPM.at(i%2), TObject::kOverwrite);
	}

	for(int i = 0; i < 2; i++){
		for(int j = 0; j < SiPM.size(); j++){
			TString name =  flavour[1] + "0370MeV" + rate.at(2) + mask[i] + "Mask" + SiPM.at(j);
			readMatrix("./out/" + (std::string) name + ".txt");
		}
	}

//	for(int i = 0; i < 2; i++){
		//for(int j = 0; j < SiPM.size(); j++){
			for(int k = 0; k < rate2.size(); k++){
				TString name = flavour[0] + "0001MeV" + rate2.at(k) + mask[0] + "Mask" + SiPM.at(1);
				reprocess(2, "./out/" + (std::string) name);
				readMatrix("./out/" + (std::string) name + ".txt", R);
				G2->SetPoint(k, rateD2.at(k), R[0] / rateD2.at(k));
				G2->SetPointError(k, 0, R[1] / rateD.at(k));
			}
		//}
//	}


	N->cd();
	G2->Write("0001MeVMask50", TObject::kOverwrite);
	N->Close();

}

void rate(double thr, double thr2){
	TString flavour[2] = {"pos", "mu"};
	vector<TString> ein;
	vector<TString> rate;
	vector<TString> rate2;
	vector<double> rateD;
	vector<double> rateD2;
	TString mask[2] = {"", "No"};
	vector<TString> SiPM;

	ein.push_back("0220");
	ein.push_back("5280");
	rate.push_back("10e6");
	rate.push_back("10e7");
	rate.push_back("10e8");
	rate.push_back("19e9");
	rate2.push_back("50e6");
	rate2.push_back("70e6");
	rate2.push_back("90e6");
	rateD.push_back(1e6);
	rateD.push_back(1e7);
	rateD.push_back(1e8);
	rateD.push_back(1.9e9);
	rateD2.push_back(50e6);
	rateD2.push_back(70e6);
	rateD2.push_back(90e6);
	SiPM.push_back("25");
	SiPM.push_back("50");

	TGraphErrors* G[8];
	TGraphErrors* G2 = new TGraphErrors(3);
	double R[2];

	for(int i = 0; i < 8; i++) G[i] = new TGraphErrors(4);

	for(int i = 0; i < rate.size(); i++){
		for(int j = 0; j < ein.size(); j++){
			for(int l = 0; l < 2; l++){ // mask index
				for(int k = 0; k < SiPM.size(); k++){
					TString name = flavour[0] + ein.at(j) + "Mev" + rate.at(i) + mask[l] + "Mask" + SiPM.at(k);
					reprocess(thr, "./out/" + (std::string) name);
					readMatrix("./out/" + (std::string) name + ".txt", R);
					G[j * 4 + l * 2 + k]->SetPoint(i, rateD.at(i), R[0] / rateD.at(i));
					G[j * 4 + l * 2 + k]->SetPointError(i, 0, R[1] / rateD.at(i));
				}
			}
		}
	}

	TFile* N = TFile::Open("results/results.root", "UPDATE");
	for(int i = 0; i < 8; i++){
		N->cd();
		G[i]->Write(ein[i/4] + "MeV" + mask[(i%4)/2] + "Mask" + SiPM.at(i%2), TObject::kOverwrite);
	}

	for(int i = 0; i < 2; i++){
		for(int j = 0; j < SiPM.size(); j++){
			TString name =  flavour[1] + "0370MeV" + rate.at(2) + mask[i] + "Mask" + SiPM.at(j);
			reprocess(thr, "./out/" + (std::string) name);
			readMatrix("./out/" + (std::string) name + ".txt");
		}
	}

//	for(int i = 0; i < 2; i++){
		//for(int j = 0; j < SiPM.size(); j++){
			for(int k = 0; k < rate2.size(); k++){
				TString name = flavour[0] + "0001MeV" + rate2.at(k) + mask[0] + "Mask" + SiPM.at(1);
				reprocess(thr2, "./out/" + (std::string) name);
				readMatrix("./out/" + (std::string) name + ".txt", R);
				G2->SetPoint(k, rateD2.at(k), R[0] / rateD2.at(k));
				G2->SetPointError(k, 0, R[1] / rateD.at(k));
			}
		//}
//	}


	N->cd();
	G2->Write("0001MeVMask50", TObject::kOverwrite);
	N->Close();

}

void amplitude(){
	TString flavour[2] = {"pos", "mu"};
	vector<TString> ein;
	vector<TString> rate;
	vector<TString> rate2;
	TString mask[2] = {"", "No"};
	vector<TString> SiPM;

	ein.push_back("0220");
	ein.push_back("5280");
	rate.push_back("10e6");
	rate.push_back("10e7");
	rate.push_back("10e8");
	rate.push_back("19e9");
	rate2.push_back("50e6");
	rate2.push_back("70e6");
	rate2.push_back("90e6");
	SiPM.push_back("25");
	SiPM.push_back("50");

	
	TFile* N = TFile::Open("results/results.root", "UPDATE");
	for(int i = 0; i < rate.size(); i++){
		for(int j = 0; j < ein.size(); j++){
			for(int l = 0; l < 2; l++){
				for(int k = 0; k < SiPM.size(); k++){
					TString name = flavour[0] + ein.at(j) + "Mev" + rate.at(i) + mask[l] + "Mask" + SiPM.at(k);
					TFile* F = TFile::Open("./out/" + name + ".root");
					TH1F* H = new TH1F(name, name, 200, 0, 200);
					((TTree*) F->Get("waves"))->Draw("Amplitude>>" + name);
					N->cd();
					H->Write("amplitude"+name, TObject::kOverwrite);
					F->Close();
				}
			}
		}
	}

	for(int i = 0; i < 2; i++){
		for(int j = 0; j < SiPM.size(); j++){
			TString name =  flavour[1] + "0370MeV" + rate.at(2) + mask[i] + "Mask" + SiPM.at(j);
			TFile* F = TFile::Open("./out/" + name + ".root");
			TH1F* H = new TH1F(name, name, 200, 0, 200);
			((TTree*) F->Get("waves"))->Draw("Amplitude>>" + name);
			N->cd();
			H->Write("amplitude"+name, TObject::kOverwrite);
			F->Close();
		}
	}

//	for(int i = 0; i < 2; i++){
		//for(int j = 0; j < SiPM.size(); j++){
			for(int k = 0; k < rate2.size(); k++){
				TString name = flavour[0] + "0001MeV" + rate2.at(k) + mask[0] + "Mask" + SiPM.at(1);
				TFile* F = TFile::Open("./out/" + name + ".root");
				TH1F* H = new TH1F(name, name, 200, 0, 20);
				((TTree*) F->Get("waves"))->Draw("Amplitude>>" + name);
				N->cd();
				H->Write("amplitude"+name, TObject::kOverwrite);
				F->Close();
			}
		//}
//	}

	N->Close();
}

void charge(){
	TString flavour[2] = {"pos", "mu"};
	vector<TString> ein;
	vector<TString> rate;
	vector<TString> rate2;
	TString mask[2] = {"", "No"};
	vector<TString> SiPM;

	ein.push_back("0220");
	ein.push_back("5280");
	rate.push_back("10e6");
	rate.push_back("10e7");
	rate.push_back("10e8");
	rate.push_back("19e9");
	rate2.push_back("50e6");
	rate2.push_back("70e6");
	rate2.push_back("90e6");
	SiPM.push_back("25");
	SiPM.push_back("50");

	
	TFile* N = TFile::Open("results/results.root", "UPDATE");
	for(int i = 0; i < rate.size(); i++){
		for(int j = 0; j < ein.size(); j++){
			for(int l = 0; l < 2; l++){
				for(int k = 0; k < SiPM.size(); k++){
					TString name = flavour[0] + ein.at(j) + "Mev" + rate.at(i) + mask[l] + "Mask" + SiPM.at(k);
					TFile* F = TFile::Open("./out/" + name + ".root");
					TH1F* H = new TH1F(name, name, 200, 0, 200);
					((TTree*) F->Get("waves"))->Draw("Charge>>" + name);
					N->cd();
					H->Write("charge"+name, TObject::kOverwrite);
					F->Close();
				}
			}
		}
	}

	for(int i = 0; i < 2; i++){
		for(int j = 0; j < SiPM.size(); j++){
			TString name =  flavour[1] + "0370MeV" + rate.at(2) + mask[i] + "Mask" + SiPM.at(j);
			TFile* F = TFile::Open("./out/" + name + ".root");
			TH1F* H = new TH1F(name, name, 200, 0, 200);
			((TTree*) F->Get("waves"))->Draw("Charge>>" + name);
			N->cd();
			H->Write("charge"+name, TObject::kOverwrite);
			F->Close();
		}
	}

//	for(int i = 0; i < 2; i++){
		//for(int j = 0; j < SiPM.size(); j++){
			for(int k = 0; k < rate2.size(); k++){
				TString name = flavour[0] + "0001MeV" + rate2.at(k) + mask[0] + "Mask" + SiPM.at(1);
				TFile* F = TFile::Open("./out/" + name + ".root");
				TH1F* H = new TH1F(name, name, 200, 0, 20);
				((TTree*) F->Get("waves"))->Draw("Charge>>" + name);
				N->cd();
				H->Write("charge"+name, TObject::kOverwrite);
				F->Close();
			}
		//}
//	}

	N->Close();
}

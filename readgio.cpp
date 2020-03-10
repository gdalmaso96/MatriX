
int iparX[3] = { 0,
                 1,
                 2
};

int iparY[3] = { 0,
                 3,
                 4
};

// Create the GlobalCHi2 structure
struct GlobalChi2 {
   GlobalChi2(  ROOT::Math::IMultiGenFunction & f1,
                ROOT::Math::IMultiGenFunction & f2) :
      fChi2_1(&f1), fChi2_2(&f2) {}
   // parameter vector is first background (in common 1 and 2)
   // and then is signal (only in 2)
   double operator() (const double *par) const {
      double p1[3];
      for (int i = 0; i < 3; ++i) p1[i] = par[iparX[i]];
      double p2[3];
      for (int i = 0; i < 3; ++i) p2[i] = par[iparY[i]];
      return (*fChi2_1)(p1) + (*fChi2_2)(p2);
   }
   const  ROOT::Math::IMultiGenFunction * fChi2_1;
   const  ROOT::Math::IMultiGenFunction * fChi2_2;
};


int readSciFi(string filename) {
   const float fibreWidth = 0.25; //mm
   const float separation = 5; //mm

   TF1* fit = 0;

   ifstream file(filename);

   string name = filename;

   size_t pos = name.rfind('/');
   if (pos != string::npos) name.replace(0, pos + 1, "");
   pos = name.rfind('.');
   if (pos != string::npos) name.replace(pos, name.length(), "");

   string line;

   Int_t value = 0;
   Float_t *xRate = new Float_t[21];
   Float_t *yRate = new Float_t[21];
   Float_t *dxRate = new Float_t[21];
   Float_t *dyRate = new Float_t[21];
   Int_t time;
   Int_t beamRate;

   // Read the file data
   Int_t iLine = 0;
   while(getline(file, line)) {
      if (iLine < 21) {
	      sscanf(line.c_str(), "%d", &value);
	      xRate[iLine] = value;
          dxRate[iLine] = sqrt(value);
      }
      
      else if (iLine < 42) {
	      sscanf(line.c_str(), "%d", &value);
	      yRate[iLine- 21] = value;
          dyRate[iLine - 21] = sqrt(value);
      }
      
      else if (iLine == 42) {
	      sscanf(line.c_str(), "%d", &beamRate);
      }
      
      else if (iLine == 43) {
	      sscanf(line.c_str(), "%d", &time);
      }
      
      ++iLine;
   }

   // position of the fibres (to be updated)
   Float_t *xPos = new Float_t[21];
   Float_t *yPos = new Float_t[21];
   for (Int_t i = 0; i < 21; ++i) {
      xPos[i] = i * 5 - 50;
      yPos[i] = i * 5 - 50;
   }

   Float_t zero[21];
   for (int i = 0; i < 21; ++i) zero[i] = 0;
   // Graphs
   TGraphErrors* gXProfile = new TGraphErrors(21, xPos, xRate, zero, dxRate);
   TGraphErrors* gYProfile = new TGraphErrors(21, yPos, yRate, zero, dyRate);

    cout << "Graphs created" << endl;
    //Defining fit function

    TF1* fx = new TF1("fx","[0] * TMath::Gaus(x, [1], [2], 1)", -50, 50);
    TF1* fy = new TF1("fy","[0] * TMath::Gaus(x, [1], [2], 1)", -50, 50);

    TF2* fxy = new TF2("fxy", "[0] * TMath::Gaus(x, [1], [2], 1) * TMath::Gaus(y, [3], [4], 1)", -50, 50, -50, 50);
    
    cout << "Functions created" << endl;
    ROOT::Math::WrappedMultiTF1 wfX(*fx,1);
    ROOT::Math::WrappedMultiTF1 wfY(*fy,1);
    

    
    // set the data range
    

    
    // set the data range
    ROOT::Fit::DataOptions opt;
    
    ROOT::Fit::DataRange rangeX;
    rangeX.SetRange( -50, 50);
    ROOT::Fit::BinData dataX(opt,rangeX);
    ROOT::Fit::FillData(dataX, gXProfile);
    
    ROOT::Fit::DataRange rangeY;
    rangeY.SetRange( -50, 50);
    ROOT::Fit::BinData dataY(opt,rangeY);
    ROOT::Fit::FillData(dataY, gYProfile);
    
    ROOT::Fit::Chi2Function chi2_X(dataX, wfX);
    ROOT::Fit::Chi2Function chi2_Y(dataY, wfY);
    
    GlobalChi2 globalChi2(chi2_X, chi2_Y);
    
    cout << "create Fitter" << endl;
    ROOT::Fit::Fitter fitter;
    
    const int Npar = 5;
    double par0[Npar] = { 1e+8,0, 20, 0, 20};
    // create before the parameter settings in order to fix or set range on them
    fitter.Config().SetParamsSettings(5,par0);
    
    fitter.Config().MinimizerOptions().SetPrintLevel(0);
    
    // fit FCN function directly
    // (specify optionally data size and flag to indicate that is a chi2 fit)
    fitter.FitFCN(5,globalChi2,0,dataX.Size()+dataY.Size(),true);
    ROOT::Fit::FitResult result = fitter.Result();
    result.Print(std::cout);

    cout << "Done fitting" << endl;
    
    // Graphs and fitting etc.
    gXProfile->SetMarkerSize(1.5);
    gXProfile->SetMarkerStyle(21);
    gYProfile->SetMarkerSize(1.5);
    gYProfile->SetMarkerStyle(21);
    

    TCanvas* cx = new TCanvas((name + "x").c_str(), "xProfile");
    
    cx->cd();
    fx->SetFitResult( result, iparX);
    fx->SetRange(rangeX().first, rangeX().second);
    fx->SetLineColor(kBlue);
    TPaveText *xText = new TPaveText(0.65, 0.6, 0.9, 0.9, "brNDC");
    xText->AddText(TString::Format("#mu = %0.2f", result.Parameter(1)));
    xText->AddText(TString::Format("#sigma = %0.2f", result.Parameter(2)));
    xText->AddText(TString::Format("Ntot = %0.2e", result.Parameter(0) / fibreWidth));
    xText->AddText(TString::Format("Rate = %0.2e", result.Parameter(0) / fibreWidth / time * 1e6));
    xText->AddText(TString::Format("Normalized Rate = %0.2e",  0.22 / 0.18 * result.Parameter(0) / fibreWidth /time * 1e6));
    cout << beamRate  << " " << time<< endl;
    //xText->SetTextSize(0.02);
    
   
    gXProfile->GetListOfFunctions()->Add(fx);
    gXProfile->SetTitle((name + " Horizontal;x (in mm);rate").c_str());
    gXProfile->Draw("ap");
    xText->Draw();
   
    //fx->Draw("same");
    cx->SaveAs((name + "gioX.C").c_str());
    cx->SaveAs((name + "gioX.pdf").c_str());

    
    
    TCanvas* cy = new TCanvas((name + "y").c_str(), "yProfile");
    cy->cd();
    fy->SetFitResult( result, iparY);
    fy->SetRange(rangeY().first, rangeY().second);
    fy->SetLineColor(kBlue);
    TPaveText *yText = new TPaveText(0.65, 0.6, 0.9, 0.9, "brNDC");
    yText->AddText(TString::Format("#mu = %0.2f", result.Parameter(3)));
    yText->AddText(TString::Format("#sigma = %0.2f", result.Parameter(4)));
    yText->AddText(TString::Format("Ntot = %0.2e", result.Parameter(0) / fibreWidth));
    yText->AddText(TString::Format("Rate = %0.2e",  result.Parameter(0) / fibreWidth / time * 1e6));
    yText->AddText(TString::Format("Normalized Rate = %0.2e",  0.22 / 0.18 * result.Parameter(0) / fibreWidth / time * 1e6));
    
    cout << "Beamrate = " << beamRate << endl;

    gYProfile->GetListOfFunctions()->Add(fy);
    gYProfile->SetTitle((name + " Horizontal;y (in mm);rate").c_str());
    gYProfile->Draw("ap");
    yText->Draw();
   //fy->Draw("same");
    cy->SaveAs((name + "gioY.C").c_str());
    cy->SaveAs((name + "gioY.pdf").c_str());


    TLegend* legend = new TLegend(0.5,0.7,0.9,0.9);
    legend->AddEntry(fxy, TString::Format("Fit: "), "");
    legend->AddEntry(fxy, TString::Format("     #mu = (%.2lf,%.2lf)", result.Parameter(1), result.Parameter(3)), "");
    legend->AddEntry(fxy, TString::Format("     #sigma = (%.2lf,%.2lf)", result.Parameter(2), result.Parameter(4)), "");
    legend->AddEntry(fxy, TString::Format("     Rate = %.2e mu/s",   0.22 / 0.18 * result.Parameter(0) / fibreWidth / time * 1e6), "");
    TCanvas* cxy = new TCanvas((name + "xy").c_str(), "xyProfile");
    cxy->cd();
    fxy->SetTitle((name + " Beam profile;x (in mm);y (in mm);Rate").c_str());
    fxy->GetXaxis()->SetTickLength(0.01);
    fxy->GetXaxis()->SetTitleOffset(1.2);
    fxy->GetYaxis()->SetTickLength(0.01);
    fxy->GetZaxis()->SetTickLength(0.01);
    fxy->SetFitResult(result);
    fxy->Draw("surf2");
    cxy->SaveAs((name + "gioXY.C").c_str());
    cxy->SaveAs((name + "gioXY.pdf").c_str());
    legend->Draw();


    return 0;
}

TH2Poly* getMatrixLayout(const char* name) {

    TH2Poly* h = new TH2Poly(name, name, -30, 30, -30, 30);
    for (float y = 25; y > -25.5; y -= 6) {
        for (float x = -25.05; x < 25.05; x += 6) {
	 h->AddBin(x, y, x + 2, y + 2);
        }
    }

    return h;
}

int readMatrix(string filename) {
    const float crystalSize = 1.65; // mm

    float time = 0;
    float beamRate = 0;

    ifstream file(filename);

    string name = filename;

    size_t pos = name.rfind('/');
    if (pos != string::npos) name.replace(0, pos + 1, "");
    pos = name.rfind('.');
    if (pos != string::npos) name.replace(pos, name.length(), "");

    string line;

    TGraph2D *profile = new TGraph2D;
    profile->SetName((name + "profile").c_str());
    profile->SetTitle("Beam Profile;x (in mm);y (in mm)");

    TH2Poly *prof2 = getMatrixLayout((name + "poly").c_str());

    Int_t value = 0;
    // Read the file data
    Int_t iBin = 0;
    Float_t x = -25.05;
    Float_t y = 25;

    while(file >> value) {
        if (iBin < 81) {
	 profile->SetPoint(iBin, x + 6 * (iBin % 9) + 1, y - 6 * (iBin / 9) + 1, value); 
	 prof2->Fill(x + 6 * (iBin % 9) + 1, y - 6 * (iBin / 9) + 1, value);
            ++iBin;
        } else if (iBin == 81) {
	 beamRate = value;
	 ++iBin;
        } else if (iBin == 82) {
	 time = value;
	 ++iBin;
        }
        
    }

    string oldAlgo = ROOT::Math::MinimizerOptions::DefaultMinimizerAlgo();
    string oldMeth = ROOT::Math::MinimizerOptions::DefaultMinimizerType();

    ROOT::Math::MinimizerOptions::SetDefaultMinimizer("Fumili");

    TF2* fit = new TF2("fit", "bigaus", -50, 50, -50 ,50);
    // Starting Parameters for fit - they might need some adaption to the specific profile.
    fit->SetParameters(profile->GetMaximum(), 0, 6, 0, 18, 0.1);
    //fit->SetParLimits(1, -25, 25);
    //fit->SetParLimits(2, 0, 20);
    //fit->SetParLimits(3, -25, 25);
    //fit->SetParLimits(4, 0, 20);
    //fit->SetParLimits(6, -1, 1);
    profile->Fit(fit, "0");


    TF2* theFit = (TF2*)profile->FindObject("fit");
    Float_t N = theFit->GetMaximum();
    theFit->SetContour(7);
    for (int i = 0; i < 7; ++i) theFit->SetContourLevel(i, N * TMath::Gaus(3 - 0.5 * i));
    TCanvas* c1 = new TCanvas(name.c_str() , name.c_str());

    prof2->Draw("col");
    theFit->Draw("same cont3");

    // Some calculations:

    Float_t Ntot = 2 * TMath::Pi() * fit->Eval(theFit->GetParameter(1), theFit->GetParameter(3)) * theFit->GetParameter(2) * theFit->GetParameter(4) * TMath::Sqrt(1 - theFit->GetParameter(5) * theFit->GetParameter(5)) / (crystalSize * crystalSize);
    Float_t muonRate = Ntot * 0.22 / beamRate * 1e6;

    TLegend* legend = new TLegend(0.5,0.7,0.9,0.9);
    legend->AddEntry(fit, TString::Format("Fit: "), "");
    legend->AddEntry(fit, TString::Format("     #mu = (%.2lf,%.2lf)", theFit->GetParameter(1), theFit->GetParameter(3)), "");
    legend->AddEntry(fit, TString::Format("     #sigma = (%.2lf,%.2lf)", theFit->GetParameter(2), theFit->GetParameter(4)), "");
    legend->AddEntry(fit, TString::Format("     Rate = %.2e mu/s",muonRate), "");
    TCanvas* c2 = new TCanvas((name + "2d" ).c_str(), name.c_str());
    fit->SetTitle((name + "Beam Profile;x [mm];y [mm];Rate [Hz]" ).c_str());
    fit->Draw("surf2");
    legend->Draw();

    cout << "stdv X: " << theFit->GetParameter(2) << " +- " << fit->GetParError(2) << endl;
    cout << "stdv y: " << theFit->GetParameter(4) << " +- " << fit->GetParError(4) << endl;
    cout << "xy cor: " << theFit->GetParameter(5) << " +- " << fit->GetParError(5) << endl;
    cout << "Ntot: " << Ntot << endl;
    cout << "Rate: " << muonRate << " mu / s" << endl;
    cout << "Time: " << time << endl;

    c1->SaveAs((name + "profile.C").c_str());
    c1->SaveAs((name + "profile.pdf").c_str());
    c2->SaveAs((name + "2dprofile.C").c_str());
    c2->SaveAs((name + "2dprofile.pdf").c_str());

    ROOT::Math::MinimizerOptions::SetDefaultMinimizer(oldMeth.c_str(), oldAlgo.c_str());
    return 0;
}




void readgio() {
//    readSciFi("./SciFi/0007-0.dat");
}


#include <TChain.h>
#include <TFile.h>
#include <TH1F.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TLegend.h>
#include <TString.h>
#include <map>
#include <cmath>
#include <iostream>

using namespace std;

void BetaECrash_npronti()
{
    gStyle->SetOptStat(0);
    gSystem->Load("libPhysics");

    TChain mc("KS_3PI0/h1");
    mc.Add("/home/nadusia/Documents/KLOE-2_analysis/mc/*.root");

    TChain data("KS_3PI0/h1");
    data.Add("/home/nadusia/Documents/KLOE-2_analysis/data/*.root");

    Float_t beta;
    Float_t poso[5];
    Char_t npronti;
    UChar_t nsel[4];
    UInt_t filfowd;
    UShort_t Ecltag;
    Int_t vetocos;
    Int_t Crflag;
    UInt_t phiwordmc;

    mc.SetBranchAddress("beta", &beta);
    mc.SetBranchAddress("poso", poso);
    mc.SetBranchAddress("npronti", &npronti);
    mc.SetBranchAddress("nsel", nsel);
    mc.SetBranchAddress("vetocos",&vetocos);
    mc.SetBranchAddress("Crflag",&Crflag);
    mc.SetBranchAddress("filfowd", &filfowd);
    mc.SetBranchAddress("Ecltag", &Ecltag);
    mc.SetBranchAddress("phiwordmc",&phiwordmc);

    data.SetBranchAddress("beta", &beta);
    data.SetBranchAddress("poso", poso);
    data.SetBranchAddress("npronti", &npronti);
    data.SetBranchAddress("nsel", nsel);
    data.SetBranchAddress("vetocos",&vetocos);
    data.SetBranchAddress("Crflag",&Crflag);
    data.SetBranchAddress("filfowd", &filfowd);
    data.SetBranchAddress("Ecltag", &Ecltag);

    TH1F *hBetaMC_All[11];
    TH1F *hBetaMC_Veto[11];

    TH1F *hBetaData_All[11];
    TH1F *hBetaData_Veto[11];

    TH1F *hEcrashMC_All[11];
    TH1F *hEcrashMC_Veto[11];

    TH1F *hEcrashData_All[11];
    TH1F *hEcrashData_Veto[11];

    std::map<Char_t, std::map<UInt_t, Long64_t>> PhiCounts;
    std::map<Char_t, std::map<UInt_t, Long64_t>> PhiCountsVeto;

    for(int n=0; n<=10; n++)
    {
        hBetaMC_All[n] =
            new TH1F(Form("hBetaMC_All_npronti%d",n),
                     Form("#beta, npronti = %d;#beta;Events",n),
                     120,0.17,0.28);

        hBetaMC_Veto[n] =
            new TH1F(Form("hBetaMC_Veto_npronti%d",n),
                     Form("#beta after track veto, npronti = %d;#beta;Events",n),
                     120,0.17,0.28);


        hBetaData_All[n] =
            new TH1F(Form("hBetaData_All_npronti%d",n),
                     Form("#beta, npronti = %d;#beta;Events",n),
                     120,0.17,0.28);

        hBetaData_Veto[n] =
            new TH1F(Form("hBetaData_Veto_npronti%d",n),
                     Form("#beta after track veto, npronti = %d;#beta;Events",n),
                     120,0.17,0.28);


        hEcrashMC_All[n] =
            new TH1F(Form("hEcrashMC_All_npronti%d",n),
                     Form("MC KL crash energy, npronti = %d;E_{crash} [MeV];Events",n),
                     100,650,2500);

        hEcrashMC_Veto[n] =
            new TH1F(Form("hEcrashMC_Veto_npronti%d",n),
                     Form("MC KL crash energy after track veto, npronti = %d;E_{crash} [MeV];Events",n),
                     100,650,2500);


        hEcrashData_All[n] =
            new TH1F(Form("hEcrashData_All_npronti%d",n),
                     Form("DATA KL crash energy, npronti = %d;E_{crash} [MeV];Events",n),
                     100,650,2500);

        hEcrashData_Veto[n] =
            new TH1F(Form("hEcrashData_Veto_npronti%d",n),
                     Form("DATA KL crash energy after track veto, npronti = %d;E_{crash} [MeV];Events",n),
                     100,650,2500);
    }

    Long64_t nBetaMC   = 0;
    Long64_t nBetaMCveto = 0;
    Long64_t nBetaData   = 0;
    Long64_t nBetaDataveto = 0;
    Long64_t nEcrashMC   = 0;
    Long64_t nEcrashMCveto = 0;
    Long64_t nEcrashData   = 0;
    Long64_t nEcrashDataveto = 0;

    Long64_t nMC = mc.GetEntries();
    Long64_t nData = data.GetEntries();

    // Wypisanie phiwordmc dla energii większej od 900 MeV
    for(Long64_t iev = 0; iev < nMC; iev++)
    {
        mc.GetEntry(iev);

        double Ecrash = poso[4];

        if(npronti != 1 && npronti != 2 && npronti != 3 && npronti != 4 && npronti != 5 &&
            npronti != 6 && npronti != 7 && npronti != 8 && npronti!= 9) continue;

        if(Ecrash < 650) continue;

        PhiCounts[npronti][phiwordmc]++;

        if(nsel[0] > 0) continue;

        PhiCountsVeto[npronti][phiwordmc]++;
    }

    for(int np = 1; np <= 9; np++)
    {
        cout << endl;
        cout << "              npronti = " << np << endl;
        cout << "Phiwordmc                 Events" << endl;
        cout << "--------------------------------------------------------" << endl;

        for(auto const& entry : PhiCounts[np])
        {
            cout << entry.first
                 << "                       "
                 << entry.second
                 << endl;
        }
        // Z TRACK VETO
        cout << endl;
        cout << "           npronti = " << np << "  Z TRACK VETO" << endl;
        cout << "Phiwordmc                 Events" << endl;
        cout << "--------------------------------------------------------" << endl;

        for(auto const& entry : PhiCountsVeto[np])
        {
            cout << entry.first
                 << "                       "
                 << entry.second
                 << endl;
        }
    }

    for(Long64_t ie=0; ie<nMC; ie++)
    {
        mc.GetEntry(ie);

        double Ecrash = poso[4];

        // !!! sprawdzenie
        if(Ecrash < 650) continue;

        if(npronti < 0 || npronti > 10) continue;

        //---ECLTAG---//
        if(Ecltag == 0) continue;

        //---FILFOWD---//
        if ((filfowd & (1 << 20)) == 0) continue;

        //---CRFLAG---//
        if(Crflag<=0) continue;

        //---VETOCOS---//
        if(vetocos>0) continue;

        hBetaMC_All[npronti]->Fill(beta);
        hEcrashMC_All[npronti]->Fill(Ecrash);

        // if(npronti==7){
        // nBetaMC++;
        // nEcrashMC++;
        // }

        //---TRACK VETO---//
        if(nsel[0] > 0) continue;

        hBetaMC_Veto[npronti]->Fill(beta);
        hEcrashMC_Veto[npronti]->Fill(Ecrash);

        // if(npronti==7){
        // nBetaMCveto++;
        // nEcrashMCveto++;
        // }
    }


    for(Long64_t ie=0; ie<nData; ie++)
    {
        data.GetEntry(ie);

        double Ecrash = poso[4];

        // !!! sprawdzenie
        if(Ecrash < 650) continue;

        if(npronti < 0 || npronti > 10) continue;

        //---ECLTAG---//
        if(Ecltag == 0) continue;

        //---FILFOWD---//
        if ((filfowd & (1 << 20)) == 0) continue;

        //---CRFLAG---//
        if(Crflag<=0) continue;

        //---VETOCOS---//
        if(vetocos>0) continue;

        hBetaData_All[npronti]->Fill(beta);
        hEcrashData_All[npronti]->Fill(Ecrash);

        // if(npronti==7){
        // nBetaData++;
        // nEcrashData++;
        // }

        //---TRACK VETO---//
        if(nsel[0] > 0) continue;

        hBetaData_Veto[npronti]->Fill(beta);
        hEcrashData_Veto[npronti]->Fill(Ecrash);

        // if(npronti==7){
        // nBetaDataveto++;
        // nEcrashDataveto++;
        // }
    }

    // Sprawdzenie liczby zdarzeń dla danych i mc dla npronti = 7
    // cout << endl;
    // cout << "Events beta mc      : " << nBetaMC << "  ;Events beta mc veto    : " << nBetaMCveto << endl;
    // cout << "Events beta data    : " << nBetaData << "  ;Events beta data veto  : " << nBetaDataveto << endl;
    // cout << "Events Ecrash mc    : " << nEcrashMC << "  ;Events Ecrash mc veto  : " << nEcrashMCveto << endl;
    // cout << "Events Ecrash data  : " << nEcrashData << "  ;Events Ecrash data veto: " << nEcrashDataveto << endl;
    
    // Sprawdzenie overflow w histogramach
    // cout << endl;
    // cout << "Overflow = " << hEcrashMC_All[7]->GetBinContent(hEcrashMC_All[7]->GetNbinsX() + 1) << endl;
    // cout << "Overflow = " << hEcrashMC_Veto[7]->GetBinContent(hEcrashMC_Veto[7]->GetNbinsX() + 1) << endl;
    // cout << "Overflow = " << hEcrashData_All[7]->GetBinContent(hEcrashData_All[7]->GetNbinsX() + 1) << endl;
    // cout << "Overflow = " << hEcrashData_Veto[7]->GetBinContent(hEcrashData_Veto[7]->GetNbinsX() + 1) << endl;
    // cout << endl;

    TFile *out = new TFile("KLOE_analysis_results/Beta_Ecrash_vs_npronti.root","RECREATE");

    for(int n=0; n<=10; n++)
    {
        hBetaMC_All[n]->Write();
        hBetaMC_Veto[n]->Write();

        hBetaData_All[n]->Write();
        hBetaData_Veto[n]->Write();

        hEcrashMC_All[n]->Write();
        hEcrashMC_Veto[n]->Write();

        hEcrashData_All[n]->Write();
        hEcrashData_Veto[n]->Write();
    }

    out->Close();

    TCanvas *c = new TCanvas("c", "c", 900, 700);

    TString pdf = "KLOE_analysis_results/Beta_Ecrash_vs_npronti_BIGENERGY.pdf";

    c->Print(pdf + "[");

    for(int n=1; n<=10; n++)
    {
        c->Clear();

        hBetaMC_All[n]->SetLineColor(kRed);
        hBetaMC_All[n]->SetLineWidth(2);

        hBetaMC_Veto[n]->SetLineColor(kRed+1);
        hBetaMC_Veto[n]->SetLineStyle(2);
        hBetaMC_Veto[n]->SetLineWidth(2);

        hBetaData_All[n]->SetMarkerStyle(20);
        hBetaData_All[n]->SetMarkerSize(0.7);
        hBetaData_All[n]->SetMarkerColor(kBlack);
        hBetaData_All[n]->SetLineColor(kBlack);

        hBetaData_Veto[n]->SetMarkerStyle(20);
        hBetaData_Veto[n]->SetMarkerSize(0.7);
        hBetaData_Veto[n]->SetMarkerColor(kAzure+2);
        hBetaData_Veto[n]->SetLineColor(kAzure+2);

        hBetaMC_All[n]->SetStats(0);

        hBetaMC_All[n]->SetMinimum(0);
        hEcrashMC_All[n]->SetMinimum(0);
        hBetaData_All[n]->SetMinimum(0);


        // if(n == 1)
        // {
        //     hBetaData_All[n]->Draw("E1");
        //     hBetaData_Veto[n]->Draw("E1 SAME");

        //     hBetaMC_All[n]->Draw("HIST SAME");
        //     hBetaMC_Veto[n]->Draw("HIST SAME");
        // }
        // else
        // {
        //     hBetaMC_All[n]->Draw("HIST");
        //     hBetaMC_Veto[n]->Draw("HIST SAME");

        //     hBetaData_All[n]->Draw("E1 SAME");
        //     hBetaData_Veto[n]->Draw("E1 SAME");
        // }

        hBetaMC_All[n]->Draw("HIST");
        hBetaMC_Veto[n]->Draw("HIST SAME");

        hBetaData_All[n]->Draw("E1 SAME");
        hBetaData_Veto[n]->Draw("E1 SAME");

        TLegend *leg = new TLegend(0.65,0.68,0.88,0.88);

        leg->AddEntry(hBetaMC_All[n],"MC","l");
        leg->AddEntry(hBetaMC_Veto[n],"MC after track veto","l");
        leg->AddEntry(hBetaData_All[n],"DATA","lep");
        leg->AddEntry(hBetaData_Veto[n],"DATA after track veto","lep");

        leg->SetBorderSize(0);
        leg->Draw();

        c->Modified();
        c->Update();

        c->Print(pdf);

        delete leg;
    }

    for(int n=1; n<=10; n++)
    {
        c->Clear();
        // c->SetLogy();

        hEcrashMC_All[n]->SetLineColor(kRed);
        hEcrashMC_All[n]->SetLineWidth(2);

        hEcrashMC_Veto[n]->SetLineColor(kRed+1);
        hEcrashMC_Veto[n]->SetLineStyle(2);
        hEcrashMC_Veto[n]->SetLineWidth(2);

        hEcrashData_All[n]->SetMarkerStyle(20);
        hEcrashData_All[n]->SetMarkerSize(0.7);
        hEcrashData_All[n]->SetMarkerColor(kBlack);
        hEcrashData_All[n]->SetLineColor(kBlack);

        hEcrashData_Veto[n]->SetMarkerStyle(20);
        hEcrashData_Veto[n]->SetMarkerSize(0.7);
        hEcrashData_Veto[n]->SetMarkerColor(kAzure+2);
        hEcrashData_Veto[n]->SetLineColor(kAzure+2);

        hEcrashMC_All[n]->SetStats(0);

        hEcrashMC_All[n]->Draw("HIST");
        hEcrashMC_Veto[n]->Draw("HIST SAME");

        hEcrashData_All[n]->Draw("E1 SAME");
        hEcrashData_Veto[n]->Draw("E1 SAME");

        TLegend *leg = new TLegend(0.65,0.68,0.88,0.88);

        leg->AddEntry(hEcrashMC_All[n],"MC","l");
        leg->AddEntry(hEcrashMC_Veto[n],"MC after track veto","l");
        leg->AddEntry(hEcrashData_All[n],"DATA","lep");
        leg->AddEntry(hEcrashData_Veto[n],"DATA after track veto","lep");

        leg->SetBorderSize(0);
        leg->Draw();

        c->Modified();
        c->Update();

        c->Print(pdf);

        delete leg;
    }

    c->Print(pdf + "]");

    delete c;

}
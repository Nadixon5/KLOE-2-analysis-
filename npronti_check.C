#include "TSystem.h"
#include "TChain.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TLegend.h"
#include <map>

void npronti_check()
{
    gSystem->Load("libPhysics");
    gStyle->SetOptStat(0);

    Char_t npronti;
    UInt_t Kswordmc;
    UInt_t phiwordmc;
    UChar_t nsel[4];
    Float_t beta;
    Float_t poso[5];
    UInt_t filfowd;
    UShort_t Ecltag;
    Int_t vetocos;
    Int_t Crflag;

    TChain mc("KS_3PI0/h1");
    mc.Add("/home/nadusia/Documents/KLOE-2_analysis/mc/*.root");

    TChain data("KS_3PI0/h1");
    data.Add("/home/nadusia/Documents/KLOE-2_analysis/data/*.root");

    mc.SetBranchAddress("Kswordmc",&Kswordmc);
    mc.SetBranchAddress("phiwordmc",&phiwordmc);
    mc.SetBranchAddress("npronti", &npronti);
    mc.SetBranchAddress("nsel", nsel);
    mc.SetBranchAddress("beta",&beta);
    mc.SetBranchAddress("poso",poso);
    mc.SetBranchAddress("vetocos",&vetocos);
    mc.SetBranchAddress("Crflag",&Crflag);
    mc.SetBranchAddress("filfowd", &filfowd);
    mc.SetBranchAddress("Ecltag", &Ecltag);

    data.SetBranchAddress("npronti", &npronti);
    data.SetBranchAddress("nsel", nsel);
    data.SetBranchAddress("beta",&beta);
    data.SetBranchAddress("poso",poso);
    data.SetBranchAddress("vetocos",&vetocos);
    data.SetBranchAddress("Crflag",&Crflag);
    data.SetBranchAddress("filfowd", &filfowd);
    data.SetBranchAddress("Ecltag", &Ecltag);

    TH1F *hnprontiMCAll =
    new TH1F("hnprontiMCAll","npronti;N_{prompt};Events",11, -0.5, 10.5);

    TH1F *hnprontiMCAllveto =
    new TH1F("hnprontiMCAllveto","npronti;N_{prompt};Events",11, -0.5, 10.5);

    TH1F *hnprontiMCKs =
    new TH1F("hnprontiMCKs","npronti;N_{prompt};Events",11, -0.5, 10.5);

    TH1F *hnprontiMCKsveto =
    new TH1F("hnprontiMCKsveto","npronti;N_{prompt};Events",11, -0.5, 10.5);

    TH1F *hnprontiData =
    new TH1F("hnprontiData","npronti;N_{prompt};Events",11, -0.5, 10.5);

    TH1F *hnprontiDataveto =
    new TH1F("hnprontiDataveto","npronti;N_{prompt};Events",11, -0.5, 10.5);

    Long64_t nprontiCount[11] = {0};
    Long64_t Nmc = mc.GetEntries();
    Long64_t Ndata = data.GetEntries();

    for(Long64_t iev = 0; iev < Ndata; iev++)
    {
        data.GetEntry(iev);

        if(npronti >= 0 && npronti <= 10)
            nprontiCount[(int)npronti]++;
    }

    for(int i = 0; i <= 10; i++)
    {
        cout << "npronti = " << i
            << " : " << nprontiCount[i]
            << endl;
    }

    for(Long64_t iev = 0; iev < Nmc; iev++)
    {
        mc.GetEntry(iev);

        if(npronti < 0 || npronti > 10) continue;

        //--- ECLTAG ---//
        if(Ecltag == 0) continue;

        //--- FILFOWD ---//
        if((filfowd & (1 << 20)) == 0) continue;

        //--- CRFLAG ---//
        if(Crflag <= 0) continue;

        //--- VETOCOS ---//
        if(vetocos > 0) continue;

        hnprontiMCAll->Fill(npronti);

        if(Kswordmc == 1799)
            hnprontiMCKs->Fill(npronti);

        //--- Track veto + dodatkowe warunki ---//
        if(nsel[0] > 0) continue;
        //if(poso[4] < 150.) continue;
        //if(beta < 0.2 || beta > 0.225) continue;

        hnprontiMCAllveto->Fill(npronti);

        if(Kswordmc == 1799)
            hnprontiMCKsveto->Fill(npronti);
    }

    for(Long64_t iev = 0; iev < Ndata; iev++)
    {
        data.GetEntry(iev);

        if(npronti < 0 || npronti > 10) continue;

        //--- ECLTAG ---//
        if(Ecltag == 0) continue;

        //--- FILFOWD ---//
        if((filfowd & (1 << 20)) == 0) continue;

        //--- CRFLAG ---//
        if(Crflag <= 0) continue;

        //--- VETOCOS ---//
        if(vetocos > 0) continue;

        hnprontiData->Fill(npronti);

        //--- Track veto + dodatkowe warunki ---//
        if(nsel[0] > 0) continue;
        //if(poso[4] < 150.) continue;
        //if(beta < 0.2 || beta > 0.225) continue;

        hnprontiDataveto->Fill(npronti);
    }

    TCanvas *c1 = new TCanvas("c1","",900,700);

    hnprontiMCAll->SetLineColor(kBlue);
    hnprontiMCAll->SetLineWidth(2);

    hnprontiMCAllveto->SetLineColor(kBlue+1);
    hnprontiMCAllveto->SetLineStyle(2);
    hnprontiMCAllveto->SetLineWidth(2);

    hnprontiMCKs->SetLineColor(kRed);
    hnprontiMCKs->SetLineWidth(2);

    hnprontiMCKsveto->SetLineColor(kRed+1);
    hnprontiMCKsveto->SetLineStyle(2);
    hnprontiMCKsveto->SetLineWidth(2);

    hnprontiData->SetMarkerStyle(20);
    hnprontiData->SetMarkerSize(0.7);
    hnprontiData->SetMarkerColor(kBlack);
    hnprontiData->SetLineColor(kBlack);

    hnprontiDataveto->SetMarkerStyle(20);
    hnprontiDataveto->SetMarkerSize(0.7);
    hnprontiDataveto->SetMarkerColor(kAzure+2);
    hnprontiDataveto->SetLineColor(kAzure+2);

    hnprontiMCAll->Draw("HIST");
    hnprontiMCAllveto->Draw("HIST SAME");
    hnprontiMCKs->Draw("HIST SAME");
    hnprontiMCKsveto->Draw("HIST SAME");
    hnprontiData->Draw("E1 SAME");
    hnprontiDataveto->Draw("E1 SAME");


    TLegend *legN = new TLegend(0.6,0.65,0.88,0.88);

    legN->SetTextSize(0.02);

    legN->AddEntry(hnprontiMCAll,"MC all","l");
    legN->AddEntry(hnprontiMCAllveto,"MC all after veto","l");
    legN->AddEntry(hnprontiMCKs,"MC: K_{S} #rightarrow 2#pi^{0}","l");
    legN->AddEntry(hnprontiMCKsveto,"MC: K_{S} #rightarrow 2#pi^{0} after veto","l");
    legN->AddEntry(hnprontiData,"DATA","lep");
    legN->AddEntry(hnprontiDataveto,"DATA after veto","lep");

    legN->Draw();

    c1->Update();

    c1->Print("KLOE_analysis_results/npronti_TRACKVETO_BASICCUTS.pdf");

}

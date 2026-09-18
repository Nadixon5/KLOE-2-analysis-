#include "TSystem.h"
#include "TChain.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TLegend.h"
#include <map>
#include <iostream>

void phiwordmc_check()
{
    gSystem->Load("libPhysics");
    gStyle->SetOptStat(0);

    Char_t npronti;
    UInt_t phiwordmc;
    UChar_t nsel[4];
    UInt_t filfowd;
    UShort_t Ecltag;
    Int_t vetocos;
    Int_t Crflag;

    TChain mc("KS_3PI0/h1");
    mc.Add("/home/nadusia/Documents/KLOE-2_analysis/mc/*.root");

    TChain data("KS_3PI0/h1");
    data.Add("/home/nadusia/Documents/KLOE-2_analysis/data/*.root");

    mc.SetBranchAddress("phiwordmc", &phiwordmc);
    mc.SetBranchAddress("npronti", &npronti);
    mc.SetBranchAddress("nsel", nsel);
    mc.SetBranchAddress("vetocos",&vetocos);
    mc.SetBranchAddress("Crflag",&Crflag);
    mc.SetBranchAddress("filfowd", &filfowd);
    mc.SetBranchAddress("Ecltag", &Ecltag);

    Long64_t Nmc = mc.GetEntries();

    std::map<Char_t, std::map<UInt_t, Long64_t>> phiCounts;
    std::map<Char_t, std::map<UInt_t, Long64_t>> phiCountsVeto;

    for(Long64_t iev = 0; iev < Nmc; iev++)
    {
        mc.GetEntry(iev);

        if(npronti != 1 && npronti != 2 && npronti != 3 && npronti != 4 && 
           npronti != 5 && npronti != 6 && npronti != 7 && npronti != 8)
            continue;

        //---ECLTAG---//
        if(Ecltag == 0) continue;

        //---FILFOWD---//
        if ((filfowd & (1 << 20)) == 0) continue;

        //---CRFLAG---//
        if(Crflag<=0) continue;

        //---VETOCOS---//
        if(vetocos>0) continue;

        phiCounts[npronti][phiwordmc]++;

        //---TRACK VETO---//
        if(nsel[0] > 0) continue;

        phiCountsVeto[npronti][phiwordmc]++;
    }


    for(int np = 1; np <= 8; np++)
    {
        cout << endl;
        cout << "========================================================" << endl;
        cout << "              npronti = " << np << endl;
        cout << "========================================================" << endl;
        cout << "phiwordmc                 Events" << endl;
        cout << "--------------------------------------------------------" << endl;

        for(auto const& entry : phiCounts[np])
        {
            cout << entry.first
                 << "                       "
                 << entry.second
                 << endl;
        }

        // Z TRACK VETO
        // cout << endl;
        // cout << "========================================================" << endl;
        // cout << "           npronti = " << np << "  Z TRACK VETO" << endl;
        // cout << "========================================================" << endl;
        // cout << "phiwordmc                 Events" << endl;
        // cout << "--------------------------------------------------------" << endl;

        // for(auto const& entry : phiCountsVeto[np])
        // {
        //     cout << entry.first
        //          << "                       "
        //          << entry.second
        //          << endl;
        // }
    }
}

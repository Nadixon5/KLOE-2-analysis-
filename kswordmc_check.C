#include "TSystem.h"
#include "TChain.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TLegend.h"
#include <map>
#include <iostream>

void kswordmc_check()
{
    gSystem->Load("libPhysics");
    gStyle->SetOptStat(0);

    Char_t npronti;
    UInt_t Kswordmc;
    UChar_t nsel[4];

    TChain mc("KS_3PI0/h1");
    mc.Add("/home/nadusia/Documents/KLOE-2_analysis/mc/*.root");

    TChain data("KS_3PI0/h1");
    data.Add("/home/nadusia/Documents/KLOE-2_analysis/data/*.root");

    mc.SetBranchAddress("Kswordmc", &Kswordmc);
    mc.SetBranchAddress("npronti", &npronti);
    mc.SetBranchAddress("nsel", nsel);

    data.SetBranchAddress("npronti", &npronti);
    data.SetBranchAddress("nsel", nsel);

    Long64_t Nmc = mc.GetEntries();
    Long64_t Ndata = data.GetEntries();

    std::map<Char_t, std::map<UInt_t, Long64_t>> KsCounts;
    std::map<Char_t, std::map<UInt_t, Long64_t>> KsCountsVeto;

    for(Long64_t iev = 0; iev < Nmc; iev++)
    {
        mc.GetEntry(iev);

        if(npronti != 1 && npronti != 2 && npronti != 3 && npronti != 4 && 
           npronti != 5 && npronti != 6 && npronti != 7 && npronti != 8)
            continue;

        KsCounts[npronti][Kswordmc]++;

        if(nsel[0] > 0)
            continue;

        KsCountsVeto[npronti][Kswordmc]++;
    }


    for(int np = 1; np <= 8; np++)
    {
        // cout << endl;
        // cout << "========================================================" << endl;
        // cout << "              npronti = " << np << endl;
        // cout << "========================================================" << endl;
        // cout << "Kswordmc                 Events" << endl;
        // cout << "--------------------------------------------------------" << endl;

        // for(auto const& entry : KsCounts[np])
        // {
        //     cout << entry.first
        //          << "                       "
        //          << entry.second
        //          << endl;
        // }

        // Z TRACK VETO
        cout << endl;
        cout << "========================================================" << endl;
        cout << "           npronti = " << np << "  Z TRACK VETO" << endl;
        cout << "========================================================" << endl;
        cout << "Kswordmc                 Events" << endl;
        cout << "--------------------------------------------------------" << endl;

        for(auto const& entry : KsCountsVeto[np])
        {
            cout << entry.first
                 << "                       "
                 << entry.second
                 << endl;
        }
    }
}

using namespace std;

void SetMCStyle(TH1 *h) {
    if (!h) return;
    h->SetLineColor(kRed + 1);
    h->SetLineWidth(2);
    h->SetStats(0);
}

void SetDataStyle(TH1 *h) {
    if (!h) return;
    h->SetMarkerStyle(20);
    h->SetMarkerSize(0.7);
    h->SetLineColor(kBlack);
    h->SetMarkerColor(kBlack);
    h->SetStats(0);
}

void DrawMCData(TH1 *mc, TH1 *data, TCanvas *c, const char *pdf) {
    if (!mc || !data) return;

    SetMCStyle(mc);
    SetDataStyle(data);

    c->Clear();
    data->Draw("E1");
    mc->Draw("HIST SAME");
    // mc->Draw("HIST");
    // data->Draw("E1 SAME");

    c->Modified();
    c->Update();
    c->Print(pdf);
}

void accidental_check(const char *output = "accidental_check.root"){

    gSystem->Load("libPhysics");
    gStyle->SetOptStat(0);

    const double wMC = 1.0 * 400;
    const double wData = 1.0 * 400;

    TChain mc("KS_3PI0/h1");
    mc.Add("/home/nadusia/Documents/KLOE-2_analysis/mc/*.root");

    TChain data("KS_3PI0/h1");
    data.Add("/home/nadusia/Documents/KLOE-2_analysis/data/*.root");

    Float_t coord[10][5];
    Float_t energ[10];
    Float_t beta;
    Float_t poso[5];
    Char_t npronti;
    Int_t vetocos;
    Int_t Crflag;
    UInt_t filfowd;
    UShort_t Ecltag;
    UChar_t nsel[4];
    Int_t pnum1[100];
    Int_t speng[10];
    Float_t pshortcr[4];
    Float_t Vp[3];

    mc.SetBranchAddress("beta",&beta);
    mc.SetBranchAddress("npronti", &npronti);
    mc.SetBranchAddress("nsel", nsel);
    mc.SetBranchAddress("poso", poso);
    mc.SetBranchAddress("coord", coord);
    mc.SetBranchAddress("energ", energ);
    mc.SetBranchAddress("Vp",Vp);
    mc.SetBranchAddress("pshortcr",pshortcr);
    mc.SetBranchAddress("filfowd", &filfowd);
    mc.SetBranchAddress("Ecltag", &Ecltag);
    mc.SetBranchAddress("vetocos", &vetocos);
    mc.SetBranchAddress("Crflag", &Crflag);
    mc.SetBranchAddress("pnum1", pnum1);
    mc.SetBranchAddress("speng", speng);

    data.SetBranchAddress("beta",&beta);
    data.SetBranchAddress("poso",poso);
    data.SetBranchAddress("coord",coord);
    data.SetBranchAddress("energ",energ);
    data.SetBranchAddress("Vp",Vp);
    data.SetBranchAddress("pshortcr",pshortcr);
    data.SetBranchAddress("npronti",&npronti);
    data.SetBranchAddress("vetocos",&vetocos);
    data.SetBranchAddress("Crflag",&Crflag);
    data.SetBranchAddress("filfowd", &filfowd);
    data.SetBranchAddress("Ecltag", &Ecltag);
    data.SetBranchAddress("nsel", nsel);

    TFile fout(output, "RECREATE");

    // Energia klastra dla niskiej i wysokiej energii crashu KL
    TH1F *hClusterELowPnum0 =
    new TH1F("hClusterELowPnum0","E_{crash}<650 MeV;E_{cluster} [MeV];Clusters",120, 0, 700);

    TH1F *hClusterELowPnumNonZero =
    new TH1F("hClusterELowPnumNonZero","E_{crash}<650 MeV;E_{cluster} [MeV];Clusters",120, 0, 700);

    TH1F *hClusterEHighPnum0 =
    new TH1F("hClusterEHighPnum0","E_{crash}>650 MeV;E_{cluster} [MeV];Clusters",120, 0, 700);

    TH1F *hClusterEHighPnumNonZero =
    new TH1F("hClusterEHighPnumNonZero","E_{crash}>650 MeV;E_{cluster} [MeV];Clusters",120, 0, 700);

    // Ile klastrów z pnum1=0 przypada na jedno zdarzenie 4-gamma?
    TH1F *hNAccCandidatesLow =
    new TH1F("hNAccCandidatesLow","E_{crash}<650 MeV;N_{clusters} with pnum1=0;Events",5, 0, 5);

    TH1F *hNAccCandidatesHigh =
    new TH1F("hNAccCandidatesHigh","E_{crash}>650 MeV;N_{clusters} with pnum1=0;Events",5, 0, 5);

    // Pozostałe histogramy
    TH1F *hDeltaPxMC =
    new TH1F("hDeltaPxMC","Delta P_{x};#Delta P_{x} [MeV/c];Events",200,-300,300);

    TH1F *hDeltaPyMC =
    new TH1F("hDeltaPyMC","Delta P_{y};#Delta P_{y} [MeV/c];Events",200,-300,300);

    TH1F *hDeltaPzMC =
    new TH1F("hDeltaPzMC","Delta P_{z};#Delta P_{z} [MeV/c];Events",200,-300,300);

    TH1F *hDeltaPxData =
    new TH1F("hDeltaPxData","Delta P_{x};#Delta P_{x} [MeV/c];Events",200,-300,300);

    TH1F *hDeltaPyData =
    new TH1F("hDeltaPyData","Delta P_{y};#Delta P_{y} [MeV/c];Events",200,-300,300);

    TH1F *hDeltaPzData =
    new TH1F("hDeltaPzData","Delta P_{z};#Delta P_{z} [MeV/c];Events",200,-300,300);

    TH1F *hBetaMC =
    new TH1F("hBetaMC","#beta;#beta;Events",120,0.18,0.30);

    TH1F *hBetaData =
    new TH1F("hBetaData","#beta;#beta;Events",120,0.18,0.30);

    TH1F *hPosoEnergyMC =
    new TH1F("hPosoEnergyMC","KL crash energy;E_{KL crash} [MeV];Events",100,0,800);

    TH1F *hPosoEnergyData =
    new TH1F("hPosoEnergyData","KL crash energy;E_{KL crash} [MeV];Events",100,0,800);

    TH1F *hDeltaEMC =
    new TH1F("hDeltaEMC","#Delta E_{K_{S}};E_{K_{S}}^{crash}-E_{K_{S}}^{4#gamma} [MeV];Events",200,-300,300);

    TH1F *hDeltaEData =
    new TH1F("hDeltaEData","#Delta E_{K_{S}};E_{K_{S}}^{crash}-E_{K_{S}}^{4#gamma} [MeV];Events",200,-300,300);

    // Long64_t nLowPnum0 = 0;
    // Long64_t nLowPnumNonZero = 0;
    // Long64_t nHighPnum0 = 0;
    // Long64_t nHighPnumNonZero = 0;
    // Long64_t nSelectedEvents = 0;

    // cout << "Before event loop" << endl;

    Long64_t Nmc = mc.GetEntries();
    for(Long64_t iev = 0; iev < Nmc; ++iev)
    {
        mc.GetEntry(iev);

        if(Ecltag == 0) continue;
        if((filfowd & (1 << 20)) == 0) continue;
        if(Crflag <= 0) continue;
        if(vetocos > 0) continue;
        if(npronti != 4) continue;
        if(nsel[0] > 0) continue;

        if (poso[4] > 650.0) continue;

        // Sprawdzamy wszystkie cztery klastry
        bool noAccidentals = true;

        for (int j = 0; j < 4; ++j) {
            int cluster = speng[j];

            if (cluster < 0 || cluster >= 100 ||
                pnum1[cluster] != 0) {
                noAccidentals = false;
                break;
            }
        }
        if (!noAccidentals) continue;

        //++nSelectedEvents;

        // // Surowa energia crashu KL, przed korektą jej skali
        // double EcrashRaw = poso[4];

        // bool lowE = EcrashRaw < 650.0;
        // bool highE = EcrashRaw > 650.0;

        // // Sprawdź najpierw indeksy wszystkich czterech klastrów
        // int clusterIndex[4];
        // bool validIndices = true;

        // for(int j = 0; j < 4; ++j)
        // {
        //     clusterIndex[j] = speng[j];

        //     if(clusterIndex[j] < 0 || clusterIndex[j] >= 100)
        //     {
        //         validIndices = false;
        //         break;
        //     }
        // }

        // Analizujemy cztery klastry fotonowe w każdym zdarzeniu
        // for(int j = 0; j < 4; ++j)
        // {
        //     int cluster = clusterIndex[j];

        //     // Robocza klasyfikacja na podstawie pnum1
        //     bool candidateAccidental = (pnum1[cluster] == 0);

        //     if(candidateAccidental)
        //         ++nAccCandidates;

        //     // Lokalna kopia energii klastra.
        //     // Stosujemy tę samą korektę skali energii co w głównej analizie,
        //     // ale nie zmieniamy tablicy energ[j].
        //     // double Ecluster = energ[j];

        //     // int energyBin = (int)(Ecluster / 10.0);

        //     // double Rxy = sqrt(
        //     //     coord[j][0] * coord[j][0] +
        //     //     coord[j][1] * coord[j][1]
        //     // );

        //     // if(Rxy >= 200.0)
        //     //     Ecluster *= (1.053 - 0.001 * energyBin);
        //     // else
        //     //     Ecluster *= (1.050 - 0.001 * energyBin);

        //     // Podział według energii crashu oraz pnum1
        //     if(lowE)
        //     {
        //         if(candidateAccidental)
        //         {
        //             hClusterELowPnum0->Fill(energ[j]);
        //             //++nLowPnum0;
        //         }
        //         else
        //         {
        //             hClusterELowPnumNonZero->Fill(energ[j]);
        //             //++nLowPnumNonZero;
        //         }
        //     }
        //     else // highE
        //     {
        //         if(candidateAccidental)
        //         {
        //             hClusterEHighPnum0->Fill(energ[j]);
        //             //++nHighPnum0;
        //         }
        //         else
        //         {
        //             hClusterEHighPnumNonZero->Fill(energ[j]);
        //             //++nHighPnumNonZero;
        //         }
        //     }
        // }

        // Liczba kandydatów na accidentale w całym zdarzeniu
        // if(lowE)
        //     hNAccCandidatesLow->Fill(nAccCandidates);
        // else
        //     hNAccCandidatesHigh->Fill(nAccCandidates);

        //-----------------------//
        //    KS from 4 gamma    //
        //-----------------------//

        TLorentzVector KS(0.,0.,0.,0.);
        TLorentzVector gamma[4];

        for(int i=0;i<4;i++)
        {
            double x = coord[i][0];
            double y = coord[i][1];
            double z = coord[i][2];
            double E = energ[i];

            TVector3 r(
                x - Vp[0],
                y - Vp[1],
                z - Vp[2]);

            TVector3 dir = r.Unit();

            gamma[i].SetPxPyPzE(
                E*dir.X(),
                E*dir.Y(),
                E*dir.Z(),
                E);

            KS += gamma[i];
        }

        // ----------------------//
        //    KS from KL crash   //
        // ----------------------//

        TLorentzVector KScr;

        KScr.SetPxPyPzE(
            pshortcr[0],
            pshortcr[1],
            pshortcr[2],
            pshortcr[3]
        );

        double deltaE = KScr.E() - KS.E();
        double deltaPx = KScr.Px() - KS.Px();
        double deltaPy = KScr.Py() - KS.Py();
        double deltaPz = KScr.Pz() - KS.Pz();

        hDeltaEMC->Fill(deltaE,  wMC);
        hDeltaPxMC->Fill(deltaPx, wMC);
        hDeltaPyMC->Fill(deltaPy, wMC);
        hDeltaPzMC->Fill(deltaPz, wMC);

        hBetaMC->Fill(beta,wMC);
        hPosoEnergyMC->Fill(poso[4],wMC);
    }

    Long64_t Ndata = data.GetEntries();
    for(Long64_t iev=0; iev<Ndata; iev++)
    {
        data.GetEntry(iev);

        if(Ecltag == 0) continue;
        if((filfowd & (1 << 20)) == 0) continue;
        if(Crflag <= 0) continue;
        if(vetocos > 0) continue;
        if(npronti != 4) continue;
        if(nsel[0] > 0) continue;

        if (poso[4] > 650.0) continue;

        TLorentzVector KS(0.,0.,0.,0.);
        TLorentzVector gamma[4];

        for(int i=0;i<4;i++)
        {
            double x = coord[i][0];
            double y = coord[i][1];
            double z = coord[i][2];
            double E = energ[i];

            TVector3 r(
                x - Vp[0],
                y - Vp[1],
                z - Vp[2]);

            TVector3 dir = r.Unit();

            gamma[i].SetPxPyPzE(
                E*dir.X(),
                E*dir.Y(),
                E*dir.Z(),
                E);

            KS += gamma[i];
        }

        //-------------------------
        // KS from KL crash
        //-------------------------

        TLorentzVector KScr;

        KScr.SetPxPyPzE(
            pshortcr[0],
            pshortcr[1],
            pshortcr[2],
            pshortcr[3]);

        double deltaE = KScr.E() - KS.E();
        double deltaPx = KScr.Px() - KS.Px();
        double deltaPy = KScr.Py() - KS.Py();
        double deltaPz = KScr.Pz() - KS.Pz();

        hDeltaEData->Fill(deltaE,  wData);
        hDeltaPxData->Fill(deltaPx, wData);
        hDeltaPyData->Fill(deltaPy, wData);
        hDeltaPzData->Fill(deltaPz, wData);

        hBetaData->Fill(beta,wData);
        hPosoEnergyData->Fill(poso[4],wData);

    }

    //cout << "After event loop" << endl;

    // cout << "\n=== ACCIDENTAL CHECK ===\n";
    // cout << "MC entries: " << Nmc << endl;
    // cout << "Events passing basic selection: "
    //     << nSelectedEvents << endl;

    // cout << "\nEcrash < 650 MeV:\n";
    // cout << "  pnum1 == 0: " << nLowPnum0 << " clusters\n";
    // cout << "  pnum1 != 0: " << nLowPnumNonZero << " clusters\n";

    // cout << "\nEcrash > 650 MeV:\n";
    // cout << "  pnum1 == 0: " << nHighPnum0 << " clusters\n";
    // cout << "  pnum1 != 0: " << nHighPnumNonZero << " clusters\n";

    TCanvas *c = new TCanvas("cComparison", "MC vs Data", 790,700);

    const char *pdf =
        "KLOE_analysis_results/accidentals_under650MeV_pnum1_EQUAL_zero.pdf";

    c->Print(Form("%s[", pdf));

    // Delta P
    hDeltaPxMC->GetXaxis()->SetRangeUser(-200, 200);
    DrawMCData(hDeltaPxMC, hDeltaPxData, c, pdf);

    hDeltaPyMC->GetXaxis()->SetRangeUser(-200, 200);
    DrawMCData(hDeltaPyMC, hDeltaPyData, c, pdf);

    hDeltaPzMC->GetXaxis()->SetRangeUser(-200, 200);
    DrawMCData(hDeltaPzMC, hDeltaPzData, c, pdf);

    // Beta
    DrawMCData(hBetaMC, hBetaData, c, pdf);

    // Energia crashu KL
    hPosoEnergyMC->GetXaxis()->SetRangeUser(100, 800);
    DrawMCData(hPosoEnergyMC, hPosoEnergyData, c, pdf);

    // Delta E
    DrawMCData(hDeltaEMC, hDeltaEData, c, pdf);

    c->Print(Form("%s]", pdf));

    delete c;
    // fout.Close();

    // TCanvas *c1 = new TCanvas("c1", "Cluster energy spectra", 1200, 550);
    // c1->Divide(2, 1);

    // hClusterELowPnum0->SetLineColor(kRed + 1);
    // hClusterELowPnumNonZero->SetLineColor(kBlue + 1);
    // hClusterEHighPnum0->SetLineColor(kRed + 1);
    // hClusterEHighPnumNonZero->SetLineColor(kBlue + 1);

    // hClusterELowPnum0->SetLineWidth(2);
    // hClusterELowPnumNonZero->SetLineWidth(2);
    // hClusterEHighPnum0->SetLineWidth(2);
    // hClusterEHighPnumNonZero->SetLineWidth(2);

    // c1->cd(1);

    // double maxLow = max(hClusterELowPnum0->GetMaximum(),hClusterELowPnumNonZero->GetMaximum());

    // hClusterELowPnum0->SetMaximum(1.15 * maxLow);
    // hClusterELowPnum0->Draw("HIST");
    // hClusterELowPnumNonZero->Draw("HIST SAME");

    // TLegend *legLow = new TLegend(0.58, 0.72, 0.88, 0.88);
    // legLow->AddEntry(hClusterELowPnum0, "pnum1 = 0", "l");
    // legLow->AddEntry(hClusterELowPnumNonZero, "pnum1 != 0", "l");
    // legLow->Draw();

    // c1->cd(2);

    // double maxHigh = max(hClusterEHighPnum0->GetMaximum(),hClusterEHighPnumNonZero->GetMaximum());

    // hClusterEHighPnum0->SetMaximum(1.15 * maxHigh);
    // hClusterEHighPnum0->Draw("HIST");
    // hClusterEHighPnumNonZero->Draw("HIST SAME");

    // TLegend *legHigh = new TLegend(0.58, 0.72, 0.88, 0.88);
    // legHigh->AddEntry(hClusterEHighPnum0, "pnum1 = 0", "l");
    // legHigh->AddEntry(hClusterEHighPnumNonZero, "pnum1 != 0", "l");
    // legHigh->Draw();


    // TCanvas *c2 = new TCanvas("c2", "Accidental candidates per event", 1000, 500);
    // c2->Divide(2, 1);

    // c2->cd(1);
    // hNAccCandidatesLow->SetLineColor(kRed + 1);
    // hNAccCandidatesLow->SetLineWidth(2);
    // hNAccCandidatesLow->Draw("HIST");

    // c2->cd(2);
    // hNAccCandidatesHigh->SetLineColor(kBlue + 1);
    // hNAccCandidatesHigh->SetLineWidth(2);
    // hNAccCandidatesHigh->Draw("HIST");


    // cout << "\n=== HISTOGRAM DIAGNOSTICS ===" << endl;

    // cout << "Low pnum0 entries: "
    //     << hClusterELowPnum0->GetEntries() << endl;

    // cout << "Low pnumNonZero entries: "
    //     << hClusterELowPnumNonZero->GetEntries() << endl;

    // cout << "High pnum0 entries: "
    //     << hClusterEHighPnum0->GetEntries() << endl;

    // cout << "High pnumNonZero entries: "
    //     << hClusterEHighPnumNonZero->GetEntries() << endl;

    // cout << "Low histogram maximum: "
    //     << hClusterELowPnum0->GetMaximum() << endl;

    // cout << "High histogram maximum: "
    //     << hClusterEHighPnum0->GetMaximum() << endl;

    //     cout << "Underflow: "
    //     << hClusterELowPnum0->GetBinContent(0) << endl;

    // cout << "Overflow: "
    //     << hClusterELowPnum0->GetBinContent(
    //             hClusterELowPnum0->GetNbinsX() + 1) << endl;

    // // cout << "Before canvas 1 SaveAs" << endl;
    // c1->SaveAs("KLOE_analysis_results/accidental_check_1.pdf");

    // // cout << "Before canvas 2 SaveAs" << endl;
    // c2->SaveAs("KLOE_analysis_results/accidental_check_2.pdf");

    // cout << "Before writing histograms" << endl;
    // fout.cd();

    hClusterELowPnum0->Write();
    hClusterELowPnumNonZero->Write();
    hClusterEHighPnum0->Write();
    hClusterEHighPnumNonZero->Write();

    hNAccCandidatesLow->Write();
    hNAccCandidatesHigh->Write();

    hDeltaPxMC->Write();
    hDeltaPxData->Write();
    hDeltaPyMC->Write();
    hDeltaPyData->Write();
    hDeltaPzMC->Write();
    hDeltaPzData->Write();

    hBetaMC->Write();
    hBetaData->Write();
    hPosoEnergyMC->Write();
    hPosoEnergyData->Write();
    hDeltaEMC->Write();
    hDeltaEData->Write();

    // cout << "Before closing output file" << endl;
    fout.Close();

}
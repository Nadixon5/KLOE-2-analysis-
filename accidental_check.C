using namespace std;

void SetMCStyle(TH1 *h) {
    if (!h) return;
    h->SetLineColor(kRed + 1);
    h->SetLineWidth(2);
}

void SetDataStyle(TH1 *h) {
    if (!h) return;
    h->SetMarkerStyle(20);
    h->SetMarkerSize(0.7);
    h->SetLineColor(kBlack);
    h->SetMarkerColor(kBlack);
}

void DrawMCData(TH1 *mc, TH1 *mcacc0, TH1 *acc1, TH1 *acc2,
                TH1 *data, TCanvas *c, const char *pdf) {
    if (!mc || !mcacc0 || !acc1 || !acc2 || !data || !c) return;

    SetMCStyle(mc);
    SetDataStyle(data);

    mcacc0->SetLineColor(kOrange +7);
    mcacc0->SetLineWidth(2);
    mcacc0->SetLineStyle(9);

    acc1->SetLineColor(kAzure +2);
    acc1->SetLineWidth(2);
    acc1->SetLineStyle(2);

    acc2->SetLineColor(kSpring -7);
    acc2->SetLineWidth(2);
    acc2->SetLineStyle(4);

    mc->SetStats(0);
    mcacc0->SetStats(0);
    acc1->SetStats(0);
    acc2->SetStats(0);
    data->SetStats(0);

    c->Clear();

    // double ymax = max(
    //     max(mc->GetMaximum(), acc1->GetMaximum()),
    //     max(acc2->GetMaximum(), data->GetMaximum())
    // );

    // mc->SetMaximum(ymax * 1.25);

    mc->Draw("HIST");
    mcacc0->Draw("HIST SAME");
    acc1->Draw("HIST SAME");
    acc2->Draw("HIST SAME");
    data->Draw("E1 SAME");

    TLegend *leg = new TLegend(0.59, 0.67, 0.89, 0.89);
    leg->AddEntry(mc, "MC: All events", "l");
    leg->AddEntry(mcacc0, "MC: zero accidentals", "l");
    leg->AddEntry(acc1, "MC: exactly 1 accidental", "l");
    leg->AddEntry(acc2, "MC: exactly 2 accidentals", "l");
    leg->AddEntry(data, "Data", "lep");
    leg->Draw();

    c->Modified();
    c->Update();
    c->Print(pdf);

    delete leg;
}

void DrawStackData(TH1 *mc1, TH1 *mc2,TH1 *data, TCanvas *c, const char *pdf, const char *title)
{
    if (!mc1 || !mc2 || !data || !c) return;

    mc1->SetFillColor(kBlue -9);
    mc1->SetLineColor(kBlue -6);

    mc2->SetFillColor(kMagenta +1);
    mc2->SetLineColor(kMagenta +3);

    data->SetMarkerStyle(20);
    data->SetMarkerSize(0.7);
    data->SetMarkerColor(kBlack);
    data->SetLineColor(kBlack);

    mc1->SetStats(0);
    mc2->SetStats(0);
    data->SetStats(0);

    THStack *hs = new THStack("hs_stack", title);

    hs->Add(mc1);
    hs->Add(mc2);

    c->Clear();

    hs->Draw("HIST");
    hs->GetXaxis()->SetTitle(mc1->GetXaxis()->GetTitle());
    hs->GetYaxis()->SetTitle("Events");
    data->Draw("E1 SAME");

    TLegend *leg = new TLegend(0.67, 0.77, 0.89, 0.89);
    leg->AddEntry(mc1, "MC: 1 accidental", "f");
    leg->AddEntry(mc2, "MC: 2 accidentals", "f");
    leg->AddEntry(data, "Data", "lep");
    leg->Draw();

    c->Modified();
    c->Update();
    c->Print(pdf);

    delete leg;
    delete hs;
}

void accidental_check(){

    gSystem->Load("libPhysics");

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

    TH1F *hDeltaPxacc0MC =
    new TH1F("hDeltaPxacc0MC","Delta P_{x};#Delta P_{x} [MeV/c];Events",200,-300,300);
    TH1F *hDeltaPx_1Acc =    
    new TH1F("hDeltaPx_1Acc","Exactly 1 accidental;#Delta P_{x} [MeV/c];Events",200, -300, 300);
    TH1F *hDeltaPx_2Acc = 
    new TH1F("hDeltaPx_2Acc","Exactly 2 accidentals;#Delta P_{x} [MeV/c];Events",200, -300, 300);

    TH1F *hDeltaPyacc0MC =
    new TH1F("hDeltaPyacc0MC","Delta P_{y};#Delta P_{y} [MeV/c];Events",200,-300,300);
    TH1F *hDeltaPy_1Acc =    
    new TH1F("hDeltaPy_1Acc","Exactly 1 accidental;#Delta P_{y} [MeV/c];Events",200, -300, 300);
    TH1F *hDeltaPy_2Acc = 
    new TH1F("hDeltaPy_2Acc","Exactly 2 accidentals;#Delta P_{y} [MeV/c];Events",200, -300, 300);

    TH1F *hDeltaPzacc0MC =
    new TH1F("hDeltaPzacc0MC","Delta P_{z};#Delta P_{z} [MeV/c];Events",200,-300,300);
    TH1F *hDeltaPz_1Acc =    
    new TH1F("hDeltaPz_1Acc","Exactly 1 accidental;#Delta P_{z} [MeV/c];Events",200, -300, 300);
    TH1F *hDeltaPz_2Acc = 
    new TH1F("hDeltaPz_2Acc","Exactly 2 accidentals;#Delta P_{z} [MeV/c];Events",200, -300, 300);

    TH1F *hBetaMC =
    new TH1F("hBetaMC","#beta;#beta;Events",120,0.18,0.28);
    TH1F *hBetaData =
    new TH1F("hBetaData","#beta;#beta;Events",120,0.18,0.28);

    TH1F *hBetaacc0MC =
    new TH1F("hBetaacc0MC","#beta;#beta;Events",120,0.18,0.28);
    TH1F *hBeta_1Acc =    
    new TH1F("hBeta_1Acc","Exactly 1 accidental;#beta;Events",120,0.18,0.28);
    TH1F *hBeta_2Acc = 
    new TH1F("hBeta_2Acc","Exactly 2 accidentals;#beta;Events",120,0.18,0.28);

    TH1F *hPosoEnergyMC =
    new TH1F("hPosoEnergyMC","KL crash energy;E_{KL crash} [MeV];Events",100,0,800);
    TH1F *hPosoEnergyData =
    new TH1F("hPosoEnergyData","KL crash energy;E_{KL crash} [MeV];Events",100,0,800);

    TH1F *hPosoEnergyacc0MC =
    new TH1F("hPosoEnergyacc0MC","KL crash energy;E_{KL crash} [MeV];Events",100,0,800);
    TH1F *hPosoEnergy_1Acc =    
    new TH1F("hPosoEnergy_1Acc","Exactly 1 accidental;E_{KL crash} [MeV];Events",100,0,800);
    TH1F *hPosoEnergy_2Acc = 
    new TH1F("hPosoEnergy_2Acc","Exactly 2 accidentals;E_{KL crash} [MeV];Events",100,0,800);

    TH1F *hDeltaEMC =
    new TH1F("hDeltaEMC","#Delta E_{K_{S}};E_{K_{S}}^{crash}-E_{K_{S}}^{4#gamma} [MeV];Events",200,-300,300);
    TH1F *hDeltaEData =
    new TH1F("hDeltaEData","#Delta E_{K_{S}};E_{K_{S}}^{crash}-E_{K_{S}}^{4#gamma} [MeV];Events",200,-300,300);

    TH1F *hDeltaEacc0MC =
    new TH1F("hDeltaEacc0MC","#Delta E_{K_{S}};E_{K_{S}}^{crash}-E_{K_{S}}^{4#gamma} [MeV];Events",200,-300,300);
    TH1F *hDeltaE_1Acc =    
    new TH1F("hDeltaE_1Acc","Exactly 1 accidental;#Delta E [MeV];Events",200, -300, 300);
    TH1F *hDeltaE_2Acc = 
    new TH1F("hDeltaE_2Acc","Exactly 2 accidentals;#Delta E [MeV];Events",200, -300, 300);


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

        int nAcc = 0;
        bool validEvent = true;

        for (int i = 0; i < npronti; i++) {
            int cluster = speng[i];

            if (cluster < 0 || cluster >= 100) {
                validEvent = false;
                break;
            }

            if (pnum1[cluster] == 0) {
                ++nAcc;
            }
        }
        // czy zdarzenie pozostało poprawne? Wykrzyknik oznacza negację.
        if (!validEvent) continue;

        //-----------------------//
        //    KS from 4 gamma    //
        //-----------------------//

        TLorentzVector KS(0.,0.,0.,0.);
        TLorentzVector gamma[4];

        for(int i = 0; i < 4; i++)
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

        if(nAcc == 0){
            hDeltaEacc0MC->Fill(deltaE,  wMC);
            hDeltaPxacc0MC->Fill(deltaPx, wMC);
            hDeltaPyacc0MC->Fill(deltaPy, wMC);
            hDeltaPzacc0MC->Fill(deltaPz, wMC);
            hBetaacc0MC->Fill(beta,wMC);
            hPosoEnergyacc0MC->Fill(poso[4],wMC);
        }
        else if(nAcc == 1){
            hDeltaE_1Acc->Fill(deltaE, wMC);
            hDeltaPx_1Acc->Fill(deltaPx, wMC);
            hDeltaPy_1Acc->Fill(deltaPy, wMC);
            hDeltaPz_1Acc->Fill(deltaPz, wMC);
            hBeta_1Acc->Fill(beta, wMC);
            hPosoEnergy_1Acc->Fill(poso[4], wMC);
        }
        else if(nAcc == 2){
            hDeltaE_2Acc->Fill(deltaE, wMC);
            hDeltaPx_2Acc->Fill(deltaPx, wMC);
            hDeltaPy_2Acc->Fill(deltaPy, wMC);
            hDeltaPz_2Acc->Fill(deltaPz, wMC);
            hBeta_2Acc->Fill(beta, wMC);
            hPosoEnergy_2Acc->Fill(poso[4], wMC);
        }
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

    TCanvas *c = new TCanvas("cComparison", "MC vs Data", 900, 700);

    const char *pdf = "KLOE_analysis_results/accidentals_comparison.pdf";

    c->Print(Form("%s[", pdf));

    // Delta Px
    hDeltaPxMC->GetXaxis()->SetRangeUser(-200, 200);
    DrawMCData(hDeltaPxMC, hDeltaPxacc0MC, hDeltaPx_1Acc, hDeltaPx_2Acc,
            hDeltaPxData, c, pdf);

    // Delta Py
    hDeltaPyMC->GetXaxis()->SetRangeUser(-200, 200);
    DrawMCData(hDeltaPyMC, hDeltaPyacc0MC, hDeltaPy_1Acc, hDeltaPy_2Acc,
            hDeltaPyData, c, pdf);

    // Delta Pz
    hDeltaPzMC->GetXaxis()->SetRangeUser(-200, 200);
    DrawMCData(hDeltaPzMC, hDeltaPzacc0MC, hDeltaPz_1Acc, hDeltaPz_2Acc,
            hDeltaPzData, c, pdf);

    // Beta
    DrawMCData(hBetaMC, hBetaacc0MC, hBeta_1Acc, hBeta_2Acc,
            hBetaData, c, pdf);

    // Energia crashu
    hPosoEnergyMC->GetXaxis()->SetRangeUser(100, 800);
    DrawMCData(hPosoEnergyMC, hPosoEnergyacc0MC, hPosoEnergy_1Acc, hPosoEnergy_2Acc,
            hPosoEnergyData, c, pdf);

    // Delta E
    DrawMCData(hDeltaEMC, hDeltaEacc0MC, hDeltaE_1Acc, hDeltaE_2Acc,
            hDeltaEData, c, pdf);


    // Delta Px
    DrawStackData(hDeltaPx_1Acc,hDeltaPx_2Acc,hDeltaPxData,c,pdf,
        "#Delta P_{x};#Delta P_{x} [MeV/c];Events");

    // Delta Py
    DrawStackData(hDeltaPy_1Acc,hDeltaPy_2Acc,hDeltaPyData,c,pdf,
        "#Delta P_{y};#Delta P_{y} [MeV/c];Events");

    // Delta Pz
    DrawStackData(hDeltaPz_1Acc,hDeltaPz_2Acc,hDeltaPzData,c,pdf,
        "#Delta P_{z};#Delta P_{z} [MeV/c];Events");

    // Beta
    DrawStackData(hBeta_1Acc,hBeta_2Acc,hBetaData,c,pdf,
        "#beta;#beta;Events");

    // Energia crashu
    DrawStackData(hPosoEnergy_1Acc,hPosoEnergy_2Acc,hPosoEnergyData,c,pdf,
        "KL crash energy;E_{KL crash} [MeV];Events");

    // Delta E
    DrawStackData(hDeltaE_1Acc,hDeltaE_2Acc,hDeltaEData,c,pdf,
        "#Delta E;#Delta E [MeV];Events");

    c->Print(Form("%s]", pdf));

    delete c;
    fout.Close();

}
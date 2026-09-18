#include "PlotStyle.C"
#include <iostream>
#include <cmath>
#include "TSystem.h"
#include "TChain.h"
#include "TFile.h"
#include "TH1F.h"
#include "TVector3.h"
#include "TLorentzVector.h"

using namespace std;

void KSReco_v1(const char *output="KS_Reconstructed.root")
{
    gSystem->Load("libPhysics");
    gStyle->SetOptStat(0);

    // Stałe
    const double M_PI0 = 134.9768;   // MeV
    const double c = 29.9792458;   // cm/ns  

    // Wagi (tak musi być!)
    const double wMC = 0.68 * 400;
    const double wData = 1.0 * 400;

    // Z dopasowania funkcji Gaussa
    const double pionMass1MeanMC  = 137.157;
    const double pionMass1SigmaMC = 21.1367;
    const double pionMass2MeanMC  = 136.595;
    const double pionMass2SigmaMC = 21.0655;

    const double pionMass1MeanData  = 136.654;
    const double pionMass1SigmaData = 22.2932;
    const double pionMass2MeanData  = 136.649;
    const double pionMass2SigmaData = 21.9706;

    const double sigmaEKS_MC = 46.0933;
    const double sigmaPxKS_MC = 26.182;
    const double sigmaPyKS_MC = 28.415;
    const double sigmaPzKS_MC = 25.121;
    
    const double sigmaEKS_Data = 49.4114;
    const double sigmaPxKS_Data = 28.209;
    const double sigmaPyKS_Data = 30.585;
    const double sigmaPzKS_Data = 26.499;

    const double pionAngleSigmaMC = 3.46682;
    const double pionAngleSigmaData = 3.64829;

    // tak musi być, żebym nie połączyła wszystkie pliki w jeden histogram!
    TChain mc("KS_3PI0/h1");
    mc.Add("/home/nadusia/Documents/KLOE-2_analysis/mc/*.root");

    TChain data("KS_3PI0/h1");
    data.Add("/home/nadusia/Documents/KLOE-2_analysis/data/*.root");

    // Zmienne
    Float_t coord[10][5];
    Float_t energ[10];
    Float_t Vp[3];
    Float_t pshortcr[4];
    Float_t beta;
    Float_t poso[5];
    Char_t npronti;
    Int_t vetocos;
    Int_t Crflag;
    UInt_t phiwordmc;
    UInt_t Kswordmc;
    UInt_t Klwordmc;
    Int_t ncl;
    UInt_t filfowd;
    UShort_t Ecltag;
    UChar_t nsel[4];
    Int_t phidec;
    Float_t lxy;

    // raz dla mc tła i raz dla danych
    mc.SetBranchAddress("coord",coord);
    mc.SetBranchAddress("energ",energ);
    mc.SetBranchAddress("Vp",Vp);
    mc.SetBranchAddress("pshortcr",pshortcr);
    mc.SetBranchAddress("npronti",&npronti);
    mc.SetBranchAddress("vetocos",&vetocos);
    mc.SetBranchAddress("Crflag",&Crflag);
    mc.SetBranchAddress("beta",&beta);
    mc.SetBranchAddress("poso",poso);
    mc.SetBranchAddress("filfowd", &filfowd);
    mc.SetBranchAddress("phiwordmc",&phiwordmc);
    mc.SetBranchAddress("Kswordmc",&Kswordmc);
    mc.SetBranchAddress("Ecltag", &Ecltag);
    mc.SetBranchAddress("nsel", nsel);
    mc.SetBranchAddress("Klwordmc", &Klwordmc);
    mc.SetBranchAddress("phidec", &phidec);
    mc.SetBranchAddress("lxy", &lxy);
    mc.SetBranchAddress("ncl", &ncl);

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
    data.SetBranchAddress("ncl", &ncl);

    // Histogramy
    // TH1F("nazwa", "tytuł", liczba_binów, minimum_X, maksimum_X)

    TH1F *hMassMC =
    new TH1F("hMassMC","K_{S} mass;M_{4#gamma} [MeV/c^{2}];Events",250,200,700);

    TH1F *hMassData =
    new TH1F("hMassData","K_{S} from 4#gamma;M_{K_{S}} [MeV/c^{2}];Events",250,200,700);

    TH1F *hPxMC =
    new TH1F("hPxMC","P_{x};P_{x} [MeV/c];Events",200,-250,250);

    TH1F *hPxData =
    new TH1F("hPxData","P_{x};P_{x} [MeV/c];Events",200,-250,250);

    TH1F *hPyMC =
    new TH1F("hPyMC","P_{y};P_{y} [MeV/c];Events",200,-250,250);

    TH1F *hPyData =
    new TH1F("hPyData","P_{y};P_{y} [MeV/c];Events",200,-250,250);

    TH1F *hPzMC =
    new TH1F("hPzMC","P_{z};P_{z} [MeV/c];Events",200,-250,250);

    TH1F *hPzData =
    new TH1F("hPzData","P_{z};P_{z} [MeV/c];Events",200,-250,250);

    TH1F *hPMC =
    new TH1F("hPMC","|P|;|P| [MeV/c];Events",200,0,250);

    TH1F *hPData =
    new TH1F("hPData","|P|;|P| [MeV/c];Events",200,0,250);

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

    TH1F *hNprontiMC =
    new TH1F("hNprontiMC","Number of prompt photons;N_{prompt};Events",11,-0.5,10.5);

    TH1F *hNprontiData =
    new TH1F("hNprontiData","Number of prompt photons;N_{prompt};Events",11,-0.5,10.5);

    TH1F *hBetaMC =
    new TH1F("hBetaMC","#beta;#beta;Events",120,0.18,0.30);

    TH1F *hBetaData =
    new TH1F("hBetaData","#beta;#beta;Events",120,0.18,0.30);

    TH1F *hPosoEnergyMC =
    new TH1F("hPosoEnergyMC","KL crash energy;E_{KL crash} [MeV];Events",100,0,800);

    TH1F *hPosoEnergyData =
    new TH1F("hPosoEnergyData","KL crash energy;E_{KL crash} [MeV];Events",100,0,800);
    
    TH1F *hRminMC = 
    new TH1F("hRminMC", "R_{min};R_{min} [cm];Events",250,0,400);

    TH1F *hRminData = 
    new TH1F("hRminData", "R_{min};R_{min} [cm];Events",250,0,400);

    TH1F *hDeltaTMaxMC =
    new TH1F("hDeltaTMaxMC","Max time difference;#Delta t_{max} [ns];Events",200, -2, 4);

    TH1F *hDeltaTMaxData =
    new TH1F("hDeltaTMaxData","Max time difference;#Delta t_{max} [ns];Events",200, -2, 4);

    TH1F *hDeltaTMC =
    new TH1F("hDeltaTMC","Time differences between clusters;#Delta t [ns];Events",200, -4, 4);

    TH1F *hDeltaTData =
    new TH1F("hDeltaTData","Time differences between clusters;#Delta t [ns];Events",200, -4, 4);

    TH1F *hTGammaMC =
    new TH1F("hTGammaMC","t_{#gamma} = t - R/c; t_{#gamma} [ns];Events",250, -4, 4);

    TH1F *hTGammaData =
    new TH1F("hTGammaData","t_{#gamma} = t - R/c; t_{#gamma} [ns];Events",250, -4, 4);

    TH1F *hChi2_2piMC =
    new TH1F("hChi2_2piMC","#chi^{2}_{2#pi};#chi^{2}_{2#pi};Events",200, -50, 140);
    
    TH1F *hChi2_2piData =
    new TH1F("hChi2_2piData","#chi^{2}_{2#pi};#chi^{2}_{2#pi};Events",200, -50, 140);

    TH1F *hPionAngleMC =
    new TH1F("hPionAngleMC","Angle between #pi^{0} and #pi^{0};#angle(#pi^{0},#pi^{0}) [deg];Events",180, 0, 260);

    TH1F *hPionAngleData =
    new TH1F("hPionAngleData","Angle between #pi^{0} and #pi^{0};#angle(#pi^{0},#pi^{0}) [deg];Events",180, 0, 260);

    TH1F *hPionMass1MC =
    new TH1F("hPionMass1MC", "First #pi^{0} mass;M_{#gamma#gamma} [MeV/c^{2}];Events",180, 0, 300);

    TH1F *hPionMass2MC =
    new TH1F("hPionMass2MC", "Second #pi^{0} mass;M_{#gamma#gamma} [MeV/c^{2}];Events",180, 0, 300);

    TH1F *hPionMass1Data =
    new TH1F("hPionMass1Data","First #pi^{0} mass;M_{#gamma#gamma} [MeV/c^{2}];Events",180, 0, 300);

    TH1F *hPionMass2Data =
    new TH1F("hPionMass2Data","Second #pi^{0} mass;M_{#gamma#gamma} [MeV/c^{2}];Events",180, 0, 300);

    TH1F *hDeltaEMC =
    new TH1F("hDeltaEMC","#Delta E_{K_{S}};E_{K_{S}}^{crash}-E_{K_{S}}^{4#gamma} [MeV];Events",200,-300,300);

    TH1F *hDeltaEData =
    new TH1F("hDeltaEData","#Delta E_{K_{S}};E_{K_{S}}^{crash}-E_{K_{S}}^{4#gamma} [MeV];Events",200,-300,300);

    TH2D *hBetaVsEcrashMC = 
    new TH2D("hBetaVsEcrashMC","MC;#beta;E_{KL crash} [MeV]",100, 0.18, 0.28,100, 100, 1100);

    TH2D *hBetaVsEcrashData = 
    new TH2D("hBetaVsEcrashData","Data;#beta;E_{KL crash} [MeV]",100, 0.18, 0.28,100, 100, 800);

    // Counters
    Long64_t nAllEventsMC   = 0;
    Long64_t nAftervetocosMC = 0;
    Long64_t nAfterCrflagMC = 0;
    Long64_t nAfterNprontiMC  = 0;
    Long64_t nAfterBetaMC    = 0;
    Long64_t nAfterCorrKLCrashEnergyMC = 0;
    Long64_t nAfterFilfowdMC = 0;
    Long64_t nAfterECLMC = 0; 
    Long64_t nAfterTrackVetoMC = 0; 
    Long64_t nAfterPhiwordmcRozne = 0;
    Long64_t nAfterPhiwordmcRowne = 0;
    Long64_t nAfterKswordmcRozne = 0;
    Long64_t nAfterKswordmcRowne = 0;

    Long64_t nAllEventsData   = 0;
    Long64_t nAftervetocosData = 0;
    Long64_t nAfterCrflagData = 0;
    Long64_t nAfterNprontiData  = 0;
    Long64_t nAfterBetaData    = 0;
    Long64_t nCutData = 0;
    Long64_t nAfterFilfowdData = 0;
    Long64_t nAfterECLData = 0; 
    Long64_t nAfterTrackVetoData = 0; 

    //-------------------------------------------------------//
    // -------------------Event loop dla mc------------------//
    //-------------------------------------------------------//

    Long64_t Nmc = mc.GetEntries();
    for(Long64_t iev=0; iev<Nmc; iev++)
    {

        mc.GetEntry(iev);

        hNprontiMC->Fill(npronti,wMC);

        nAllEventsMC++;

        //-----------! WARUNKI !-----------//

        //---Track veto---//
        // if(nsel[2] > 0) continue;
        // nAfterTrackVetoMC++;

        //---NOWE TRACK VETO JEZUS MARIA---//
        if(nsel[0] > 0) continue;

        //---ECLTAG---//
        if(Ecltag == 0) continue;
        nAfterECLMC++;

        //---FILFOWD---//
        if ((filfowd & (1 << 20)) == 0) continue;
        nAfterFilfowdMC++;

        //---CRFLAG---//
        if(Crflag<=0) continue;
        nAfterCrflagMC++;

        //---VETOCOS---//
        if(vetocos>0) continue;
        nAftervetocosMC++;

        //---NPRONTI---//
        if(npronti!=4) continue;
        nAfterNprontiMC++;

        //---BETA SMEARING---//
        double betaSmeared = beta * (1.0 + 0.0142* gRandom->Gaus(0.0, 1.0)) + 0.00022;

        //---KSWORDMC---//

        // if (Kswordmc == 1799) continue;
        // nAfterKswordmcRozne++;
        // if (Kswordmc != 1799) continue;
        // nAfterKswordmcRowne++;

        //---PHIWORDMC---//

        // if (phiwordmc == 1034) continue;
        // nAfterPhiwordmcRozne++;
        // if (phiwordmc != 1034) continue;
        // nAfterPhiwordmcRowne++;
        

        //---KOREKCJA SKALI ENERGII---//
        for(int i=0; i<npronti; i++) {

            int j = (int)(energ[i] / 10.0);

            double Rxy = sqrt(
                coord[i][0]*coord[i][0] +
                coord[i][1]*coord[i][1]
            );

            if(Rxy >= 200.0) {

                energ[i] *= (1.053 - 0.001*j);

            } else {

                energ[i] *= (1.050 - 0.001*j);
            }
        }

        //--- KOREKCJA ENERGII KL CRASH ---//

        double lxy    = sqrt(poso[0]*poso[0] + poso[1]*poso[1]);
        double Ecrash = poso[4];

        // Czy event jest FAKE?
        bool fake =
            (nsel[0] > 0 && Klwordmc != 0 && Klwordmc != 10)
            || phidec != 2
            || Kswordmc == 2312;

        if (!fake)
        {
            // TRUE KL-CRASH
            if (lxy > 200 && lxy < 226)
            {
                int iCrash = (int)(Ecrash - 100.0);

                Ecrash *= (1.06 + iCrash * 0.0001);
            }
            // else: energia pozostaje bez zmian
        }

        poso[4] = Ecrash;

        //--- CUT NA SKORYGOWANĄ ENERGIĘ ---//
        //if (poso[4] < 150.) continue;
        //nAfterCorrKLCrashEnergyMC++;

        //---BETA---//
        //if(beta < 0.2 || beta > 0.225) continue;
        //nAfterBetaMC++;
        
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

        // ------------------------------------------------------------
        // BOOST TO KS REST FRAME
        //
        // KScr = KS four-momentum determined from KL crash.
        // We boost all photons by -beta(KS).
        // ------------------------------------------------------------

        TVector3 betaKS = KScr.BoostVector();

        TLorentzVector gammaKSframe[4];

        for(int i = 0; i < 4; i++)
        {
            gammaKSframe[i] = gamma[i];

            // transformation to KS rest frame
            gammaKSframe[i].Boost(-betaKS);
        }

        int pairs[3][4] = {
            {0,1,2,3},
            {0,2,1,3},
            {0,3,1,2}
        };

        double bestChi2 = 1e30;
        int bestCombination = -1;

        double bestMass1 = -1.;
        double bestMass2 = -1.;
        double bestAngle = -1.;

        // LOOP OVER 3 POSSIBLE pi0-pi0 COMBINATIONS
        for(int c = 0; c < 3; c++)
        {
            int i1 = pairs[c][0];
            int i2 = pairs[c][1];
            int i3 = pairs[c][2];
            int i4 = pairs[c][3];

            // pi0 four-momenta
            TLorentzVector pion1 = gammaKSframe[i1] + gammaKSframe[i2];
            TLorentzVector pion2 = gammaKSframe[i3] + gammaKSframe[i4];

            // reconstructed pi0 masses
            double m1 = pion1.M();
            double m2 = pion2.M();

            // angle between pi0 momenta in KS rest frame
            double angle = pion1.Vect().Angle(pion2.Vect()) * 180.0 / TMath::Pi();

            // --------------------------------------------------------
            // CHI2
            // --------------------------------------------------------

            double chi2Mass1 = pow((m1 - M_PI0)/ pionMass1SigmaMC,2);
            double chi2Mass2 = pow((m2 - M_PI0)/ pionMass2SigmaMC,2);

            double chi2Angle = pow((angle - 180.0)/ pionAngleSigmaMC,2);

            double sumGammaE = 0.0;
            double sumGammaPx = 0.0;
            double sumGammaPy = 0.0;
            double sumGammaPz = 0.0;

            for(int i = 0; i < 4; i++)
            {
                sumGammaE  += gamma[i].E();
                sumGammaPx += gamma[i].Px();
                sumGammaPy += gamma[i].Py();
                sumGammaPz += gamma[i].Pz();
            }

            double deltaE = KScr.E() - sumGammaE;
            double chi2Energy = pow(deltaE / sigmaEKS_MC,2);

            double deltaPx = KScr.Px() - sumGammaPx;
            double chi2Px = pow(deltaPx / sigmaPxKS_MC,2);

            double deltaPy = KScr.Py() - sumGammaPy;
            double chi2Py = pow(deltaPy / sigmaPyKS_MC,2);

            double deltaPz = KScr.Pz() - sumGammaPz;
            double chi2Pz = pow(deltaPz / sigmaPzKS_MC,2);


            double chi2 = chi2Mass1 + chi2Mass2 + chi2Angle + chi2Energy                    
                         + chi2Px + chi2Py + chi2Pz;

            // choose the best combination

            if(chi2 < bestChi2)
            {
                bestChi2 = chi2;
                bestCombination = c;

                bestMass1 = m1;
                bestMass2 = m2;
                bestAngle = angle;
            }
        }

        // SAVE BEST COMBINATION

        if(bestCombination >= 0)
        {
            hPionMass1MC->Fill(bestMass1, wMC);
            hPionMass2MC->Fill(bestMass2, wMC);

            hPionAngleMC->Fill(bestAngle, wMC);

            hChi2_2piMC->Fill(bestChi2, wMC);
        }

        
        // Fill histograms
        hMassMC->Fill(KS.M(),wMC);
        hPxMC->Fill(KS.Px(),wMC);
        hPyMC->Fill(KS.Py(),wMC);
        hPzMC->Fill(KS.Pz(),wMC);
        hPMC->Fill(KS.P(),wMC);
        hBetaMC->Fill(betaSmeared,wMC);
        hPosoEnergyMC->Fill(poso[4],wMC);
        hBetaVsEcrashMC->Fill(betaSmeared, poso[4], wMC);

        double tGamma[10];

        // coord[i][3] = R [cm]
        // coord[i][4] = t [ns]
        // t_gamma = t - R/c

        for(int i = 0; i < npronti; i++)
        {
            double R = coord[i][3];   
            double t = coord[i][4];   

            tGamma[i] = t - R/c;

            hTGammaMC->Fill(tGamma[i],wMC);
        }

        // WSZYSTKIE RÓŻNICE CZASOWE MIĘDZY KLASTRAMI

        double maxDeltaT = 0.0;

        for(int i=0; i<npronti; i++) {

            for(int j=i+1; j<npronti; j++) {

                double deltaT = tGamma[i] - tGamma[j];

                // wszystkie różnice - ze znakiem
                hDeltaTMC->Fill(deltaT,wMC);

                // największa wartość bezwzględna
                double absDeltaT = fabs(deltaT);

                if(absDeltaT > maxDeltaT)
                    maxDeltaT = absDeltaT;
            }
        }

        // rozkład maksymalnej różnicy w zdarzeniu
        hDeltaTMaxMC->Fill(maxDeltaT,wMC);

        double Rmin = 1e9;

        for(int i = 0; i < npronti; i++) {

            for(int j = i + 1; j < npronti; j++) {

                double dx = coord[i][0] - coord[j][0];
                double dy = coord[i][1] - coord[j][1];
                double dz = coord[i][2] - coord[j][2];

                double distance = sqrt(dx*dx + dy*dy + dz*dz);

                if(distance < Rmin)
                    Rmin = distance;
            }
        }

        //if(Rmin < 1e8)
            hRminMC->Fill(Rmin,wMC);

    }

    //-------------------------------------------------------//
    // -----------------Event loop dla data------------------//
    //-------------------------------------------------------//

    Long64_t Ndata = data.GetEntries();
    for(Long64_t iev=0; iev<Ndata; iev++)
    {

        data.GetEntry(iev);

        hNprontiData->Fill(npronti,wData);

        nAllEventsData++;

        //---Track veto---//
        // if(nsel[2] > 0) continue;
        // nAfterTrackVetoData++;

        //---NOWE TRACK VETO JEZUS MARIA---//
        if(nsel[0] > 0) continue;

        //---ECLTAG---//
        if(Ecltag == 0) continue;
        nAfterECLData++;

        //---FILFOWD---//
        if ((filfowd & (1 << 20)) == 0) continue;
        nAfterFilfowdData++;

        //---CRFLAG---//
        if(Crflag<=0) continue;
        nAfterCrflagData++;

        //---VETOCOS---//
        if(vetocos>0) continue;
        nAftervetocosData++;

        //---NPRONTI---//
        if(npronti!=4) continue;
        nAfterNprontiData++;

        //if (poso[4] < 150.) continue;
        //nCutData++;

        //if(beta < 0.2 || beta > 0.225) continue;
        //nAfterBetaData++;

        //-------------------------
        //    KS from 4 gamma    //
        //-------------------------

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

        // ------------------------------------------------------------
        // BOOST TO KS REST FRAME
        //
        // KScr = KS four-momentum determined from KL crash.
        // We boost all photons by -beta(KS).
        // ------------------------------------------------------------

        TVector3 betaKS = KScr.BoostVector();

        TLorentzVector gammaKSframe[4];

        for(int i = 0; i < 4; i++)
        {
            gammaKSframe[i] = gamma[i];

            // transformation to KS rest frame
            gammaKSframe[i].Boost(-betaKS);
        }

        int pairs[3][4] = {
            {0,1,2,3},
            {0,2,1,3},
            {0,3,1,2}
        };

        double bestChi2 = 1e30;
        int bestCombination = -1;

        double bestMass1 = -1.;
        double bestMass2 = -1.;
        double bestAngle = -1.;

        // LOOP OVER 3 POSSIBLE pi0-pi0 COMBINATIONS
        for(int c = 0; c < 3; c++)
        {
            int i1 = pairs[c][0];
            int i2 = pairs[c][1];
            int i3 = pairs[c][2];
            int i4 = pairs[c][3];

            // pi0 four-momenta
            TLorentzVector pion1 = gammaKSframe[i1] + gammaKSframe[i2];
            TLorentzVector pion2 = gammaKSframe[i3] + gammaKSframe[i4];

            // reconstructed pi0 masses
            double m1 = pion1.M();
            double m2 = pion2.M();

            // angle between pi0 momenta in KS rest frame
            double angle = pion1.Vect().Angle(pion2.Vect()) * 180.0 / TMath::Pi();

            // --------------------------------------------------------
            // CHI2
            // --------------------------------------------------------

            double chi2Mass1 = pow((m1 - M_PI0)/ pionMass1SigmaData,2);
            double chi2Mass2 = pow((m2 - M_PI0)/ pionMass2SigmaData,2);

            double chi2Angle = pow((angle - 180.0)/ pionAngleSigmaData,2);

            double sumGammaE = 0.0;
            double sumGammaPx = 0.0;
            double sumGammaPy = 0.0;
            double sumGammaPz = 0.0;

            for(int i = 0; i < 4; i++)
            {
                sumGammaE  += gamma[i].E();
                sumGammaPx += gamma[i].Px();
                sumGammaPy += gamma[i].Py();
                sumGammaPz += gamma[i].Pz();
            }

            double deltaE = KScr.E() - sumGammaE;
            double chi2Energy = pow(deltaE / sigmaEKS_Data,2);

            double deltaPx = KScr.Px() - sumGammaPx;
            double chi2Px = pow(deltaPx / sigmaPxKS_Data,2);

            double deltaPy = KScr.Py() - sumGammaPy;
            double chi2Py = pow(deltaPy / sigmaPyKS_Data,2);

            double deltaPz = KScr.Pz() - sumGammaPz;
            double chi2Pz = pow(deltaPz / sigmaPzKS_Data,2);


            double chi2 = chi2Mass1 + chi2Mass2 + chi2Angle + chi2Energy                    
                         + chi2Px + chi2Py + chi2Pz;

            // choose the best combination

            if(chi2 < bestChi2)
            {
                bestChi2 = chi2;
                bestCombination = c;

                bestMass1 = m1;
                bestMass2 = m2;
                bestAngle = angle;
            }
        }

        // SAVE BEST COMBINATION

        if(bestCombination >= 0)
        {
            hPionMass1Data->Fill(bestMass1, wData);
            hPionMass2Data->Fill(bestMass2, wData);

            hPionAngleData->Fill(bestAngle, wData);

            hChi2_2piData->Fill(bestChi2, wData);
        }
        
        // Fill histograms
        hMassData->Fill(KS.M(),wData);
        hPxData->Fill(KS.Px(),wData);
        hPyData->Fill(KS.Py(),wData);
        hPzData->Fill(KS.Pz(),wData);
        hPData->Fill(KS.P(),wData);

        hBetaData->Fill(beta,wData);
        hPosoEnergyData->Fill(poso[4],wData);
        hBetaVsEcrashData->Fill(beta, poso[4], wData);

        double tGamma[10];

        for(int i = 0; i < npronti; i++)
        {
            double R = coord[i][3];   // cm
            double t = coord[i][4];   // ns

            tGamma[i] = t - R/c;

            hTGammaData->Fill(tGamma[i],wData);
        }

        // WSZYSTKIE RÓŻNICE CZASOWE MIĘDZY KLASTRAMI

        double maxDeltaT = 0.0;

        for(int i=0; i<npronti; i++) {

            for(int j=i+1; j<npronti; j++) {

                double deltaT = tGamma[i] - tGamma[j];

                hDeltaTData->Fill(deltaT,wData);

                double absDeltaT = fabs(deltaT);

                if(absDeltaT > maxDeltaT)
                    maxDeltaT = absDeltaT;
            }
        }

        hDeltaTMaxData->Fill(maxDeltaT,wData);

        double Rmin = 1e9;

        for(int i = 0; i < npronti; i++) {

            for(int j = i + 1; j < npronti; j++) {

                double dx = coord[i][0] - coord[j][0];
                double dy = coord[i][1] - coord[j][1];
                double dz = coord[i][2] - coord[j][2];

                double distance = sqrt(dx*dx + dy*dy + dz*dz);

                if(distance < Rmin)
                    Rmin = distance;
            }
        }

        //if(Rmin < 1e8)
            hRminData->Fill(Rmin,wData);

    }

/////////////////////////////////////////////////////////////////////////////

// cout << endl;
// cout << "=== CUT CHECK MC ===" << endl;
// cout << "All events          : " << nAllEventsMC << endl;
// cout << "After track veto    : " << nAfterTrackVetoMC << endl;
// cout << "After ecltag        : " << nAfterECLMC << endl;
// cout << "After filfowd       : " << nAfterFilfowdMC << endl;
// cout << "After crflag        : " << nAfterCrflagMC << endl;
// cout << "After vetocos       : " << nAftervetocosMC << endl;
// cout << "After npronti       : " << nAfterNprontiMC << endl;
//cout << "After kl crash energy cut    : " << nAfterCorrKLCrashEnergyMC << endl;
//cout << "After beta          : " << nAfterBetaMC << endl;
// cout << "After phiwordmc = 1034 without vetocos : " << nAfterPhiwordmcRowne << endl;
// cout << "After phiwordmc /= 1034 without vetocos : " << nAfterPhiwordmcRozne << endl;

// cout << "After kswordmc = 1799 without vetocos : " << nAfterKswordmcRowne << endl;
// cout << "After kswordmc /= 1799 without vetocos : " << nAfterKswordmcRozne << endl;

// cout << endl;
// cout << "=== CUT CHECK DATA ===" << endl;
// cout << "All events          : " << nAllEventsData << endl;
// cout << "After track veto    : " << nAfterTrackVetoData << endl;
// cout << "After ecltag        : " << nAfterECLData << endl;
// cout << "After filfowd       : " << nAfterFilfowdData << endl;
// cout << "After crflag        : " << nAfterCrflagData << endl;
// cout << "After vetocos       : " << nAftervetocosData << endl;
// cout << "After npronti       : " << nAfterNprontiData << endl;
//cout << "After kl crash energy cut: " << nCutData << endl;
//cout << "After beta          : " << nAfterBetaData << endl;


TFile fout(output,"RECREATE");
fout.cd();

    hMassMC->Write();
    hPxMC->Write();
    hPyMC->Write();
    hPzMC->Write();
    hPMC->Write();

    hMassData->Write();
    hPxData->Write();
    hPyData->Write();
    hPzData->Write();
    hPData->Write();

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
    hNprontiMC->Write();
    hNprontiData->Write();

    hRminMC->Write();
    hRminData->Write();

    hDeltaTMaxMC->Write();
    hDeltaTMaxData->Write();
    hDeltaTMC->Write();
    hDeltaTData->Write();
    hTGammaMC->Write();
    hTGammaData->Write();

    hChi2_2piMC->Write();
    hChi2_2piData->Write();
    hPionAngleMC->Write();
    hPionAngleData->Write();
    hPionMass1MC->Write();
    hPionMass1Data->Write();
    hPionMass2MC->Write();
    hPionMass2Data->Write();
    hDeltaEMC->Write();
    hDeltaEData->Write();
    hBetaVsEcrashMC->Write();
    hBetaVsEcrashData->Write();


fout.Close();

}

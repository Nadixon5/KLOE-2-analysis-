#include <iostream>
#include "TFile.h"
#include "TH1F.h"
#include "TF1.h"
#include "TH2D.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TStyle.h"
#include "TROOT.h"

using namespace std;

void SetMCStyle(TH1 *h)
{
    if(!h) return;
    h->SetLineColor(kRed+1);
    h->SetLineWidth(2);
    h->SetMarkerStyle(1);
    h->SetMarkerSize(0);
    h->SetStats(0);
}

void SetDataStyle(TH1 *h)
{
    if(!h) return;
    h->SetMarkerStyle(20);
    h->SetMarkerSize(0.7);
    h->SetMarkerColor(kBlack);
    h->SetLineColor(kBlack);
    h->SetStats(0);
}

void DrawMCData(
    TH1 *hMC,
    TH1 *hData,
    // TH1 *hData,
    // TH1 *hMC,
    TCanvas *c,
    const char *pdf,
    bool normalizeMC = false) //ture i false dotyczą normalizacji!!!!!!!!!
{
    if(!hMC || !hData)
    {
        cout << "ERROR: missing histogram!" << endl;
        return;
    }

    //SetDataStyle(hData);
    SetMCStyle(hMC);
    SetDataStyle(hData);

    //----------------------------------------------------------
    // Opcjonalna normalizacja MC do liczby zdarzeń DATA
    //----------------------------------------------------------

    // if(normalizeMC)
    // {
    //     double intMC   = hMC->Integral();
    //     double intData = hData->Integral();
    //     if(intMC > 0)
    //         hMC->Scale(intData/intMC);
    // }

    c->Clear();

    hMC->Draw("hist");
    hData->Draw("E1 SAME");
    // hData->Draw("hist");
    // hMC->Draw("E1 SAME");

    c->Modified();
    c->Update();
    c->Print(pdf);
}

// GŁÓWNE MACRO
void PlotStyle(
    const char *input = "KS_Reconstructed.root",
    const char *output = "KLOE_analysis_results/ks3pi0_basic_cuts_trackveto_energycut650MeV_v2_no_klcrenergycorr.pdf")
{
    gROOT->SetBatch(kTRUE);

    gStyle->SetOptStat(0);
    gStyle->SetCanvasDefW(800);
    gStyle->SetCanvasDefH(800);

    gStyle->SetPadLeftMargin(0.12);
    gStyle->SetPadRightMargin(0.05);
    gStyle->SetPadBottomMargin(0.12);
    gStyle->SetPadTopMargin(0.073);

    gStyle->SetTitleSize(0.041,"XY");
    gStyle->SetLabelSize(0.040,"XY");
    gStyle->SetTitleOffset(1.2,"X");
    gStyle->SetTitleOffset(1.3,"Y");

    TFile *f = TFile::Open(input);

    TH1F *hMassMC = (TH1F*)f->Get("hMassMC");
    TH1F *hMassData = (TH1F*)f->Get("hMassData");

    TH1F *hDeltaPxMC = (TH1F*)f->Get("hDeltaPxMC");
    TH1F *hDeltaPxData = (TH1F*)f->Get("hDeltaPxData");
    TH1F *hDeltaPyMC = (TH1F*)f->Get("hDeltaPyMC");
    TH1F *hDeltaPyData = (TH1F*)f->Get("hDeltaPyData");
    TH1F *hDeltaPzMC = (TH1F*)f->Get("hDeltaPzMC");
    TH1F *hDeltaPzData = (TH1F*)f->Get("hDeltaPzData");

    TH1F *hBetaMC = (TH1F*)f->Get("hBetaMC");
    TH1F *hBetaData = (TH1F*)f->Get("hBetaData");

    TH1F *hPosoEnergyMC = (TH1F*)f->Get("hPosoEnergyMC");
    TH1F *hPosoEnergyData = (TH1F*)f->Get("hPosoEnergyData");

    // TH1F *hNprontiMC = (TH1F*)f->Get("hNprontiMC");
    // TH1F *hNprontiData = (TH1F*)f->Get("hNprontiData");

    TH1F *hRminMC = (TH1F*)f->Get("hRminMC");
    TH1F *hRminData = (TH1F*)f->Get("hRminData");

    TH1F *hTGammaMC = (TH1F*)f->Get("hTGammaMC");
    TH1F *hTGammaData = (TH1F*)f->Get("hTGammaData");
    TH1F *hDeltaTMC = (TH1F*)f->Get("hDeltaTMC");
    TH1F *hDeltaTData = (TH1F*)f->Get("hDeltaTData");
    TH1F *hDeltaTMaxMC = (TH1F*)f->Get("hDeltaTMaxMC");
    TH1F *hDeltaTMaxData = (TH1F*)f->Get("hDeltaTMaxData");

    TH1F *hChi2_2piMC = (TH1F*)f->Get("hChi2_2piMC");
    TH1F *hChi2_2piData = (TH1F*)f->Get("hChi2_2piData");

    TH1F *hPionAngleMC = (TH1F*)f->Get("hPionAngleMC");
    TH1F *hPionAngleData = (TH1F*)f->Get("hPionAngleData");

    TH1F *hPionMass1MC = (TH1F*)f->Get("hPionMass1MC");
    TH1F *hPionMass1Data = (TH1F*)f->Get("hPionMass1Data");
    TH1F *hPionMass2MC = (TH1F*)f->Get("hPionMass2MC");
    TH1F *hPionMass2Data = (TH1F*)f->Get("hPionMass2Data");
    
    TH1F *hDeltaEMC = (TH1F*)f->Get("hDeltaEMC");
    TH1F *hDeltaEData = (TH1F*)f->Get("hDeltaEData");

    TH2D *hBetaVsEcrashMC = (TH2D*)f->Get("hBetaVsEcrashMC");
    TH2D *hBetaVsEcrashData = (TH2D*)f->Get("hBetaVsEcrashData");

    TCanvas *c = new TCanvas("c","KS analysis",790,700);

    c->Print(Form("%s[",output));

    // ========================================================
    // 1. MASA KS - 4 GAMMY
    // ========================================================

    if(hMassMC && hMassData){
        DrawMCData(hMassMC,hMassData,c,output,false);
        //DrawMCData(hMassData,hMassMC,c,output,false);
    }

    // ========================================================
    // 2. NUMBER OF PROMPT PHOTONS
    // ========================================================

    // if(hNprontiMC && hNprontiData){
    //     hNprontiMC->GetXaxis()->SetRangeUser(0,9);
    //     DrawMCData(hNprontiMC,hNprontiData,c,output,false);
    //     //DrawMCData(hNprontiData,hNprontiMC,c,output,false);
    // }

    // ========================================================
    // 3. DELTA Px
    // ========================================================

    if(hDeltaPxMC && hDeltaPxData){
        hDeltaPxMC->GetXaxis()->SetRangeUser(-200,200);
        DrawMCData(hDeltaPxMC,hDeltaPxData,c,output,true);
        //DrawMCData(hDeltaPxData,hDeltaPxMC,c,output,true);
    }

    // ========================================================
    // 4. DELTA Py
    // ========================================================

    if(hDeltaPyMC && hDeltaPyData){
        hDeltaPyMC->GetXaxis()->SetRangeUser(-200,200);
        DrawMCData(hDeltaPyMC,hDeltaPyData,c,output,true);
        //DrawMCData(hDeltaPyData,hDeltaPyMC,c,output,true);
    }

    // ========================================================
    // 5. DELTA Pz
    // ========================================================

    if(hDeltaPzMC && hDeltaPzData){
        hDeltaPzMC->GetXaxis()->SetRangeUser(-200,200);
        DrawMCData(hDeltaPzMC,hDeltaPzData,c,output,true);
        //DrawMCData(hDeltaPzData,hDeltaPzMC,c,output,true);
    }

    // ========================================================
    // 6. BETA
    // ========================================================

    if(hBetaMC && hBetaData){
        hBetaMC->GetXaxis()->SetRangeUser(0.18,0.30);
        hBetaData->GetXaxis()->SetRangeUser(0.18,0.30);
        hBetaMC->GetYaxis()->SetRangeUser(0,9800000);
        DrawMCData(hBetaMC,hBetaData,c,output,true);
        //DrawMCData(hBetaData,hBetaMC,c,output,true);
    }

    // ========================================================
    // 7. ENERGIA KL CRASH 
    // ========================================================

    if(hPosoEnergyMC && hPosoEnergyData){
        hPosoEnergyMC->GetXaxis()->SetRangeUser(100,800);
        hPosoEnergyData->GetXaxis()->SetRangeUser(100,800);
        DrawMCData(hPosoEnergyMC,hPosoEnergyData,c,output,true);
        //DrawMCData(hPosoEnergyData,hPosoEnergyMC,c,output,true);
    }

    // ========================================================
    // 8. MINIMUM DISTANCE BETWEEN CLUSTERS
    // ========================================================

    if(hRminMC && hRminData){
        //hRminMC->GetXaxis()->SetRangeUser(0,400);
        DrawMCData(hRminMC,hRminData,c,output,true);
        //DrawMCData(hRminData,hRminMC,c,output,true);
    }

    // ========================================================
    // 9. GAMMA PRODUCTION TIME
    // ========================================================

    if(hTGammaMC && hTGammaData){
        DrawMCData(hTGammaMC,hTGammaData,c,output,true);
        //DrawMCData(hTGammaData,hTGammaMC,c,output,true);
    }

    // ========================================================
    // 10. WSZYSTKIE RÓŻNICE CZASOWE
    // ========================================================

    if(hDeltaTMC && hDeltaTData){
        DrawMCData(hDeltaTMC,hDeltaTData,c,output,true);
        //DrawMCData(hDeltaTData,hDeltaTMC,c,output,true);
    }

    // ========================================================
    // 11. MAKSYMALNA RÓŻNICA CZASOWA W ZDARZENIU
    // ========================================================

    if(hDeltaTMaxMC && hDeltaTMaxData){
        DrawMCData(hDeltaTMaxMC,hDeltaTMaxData,c,output,true);
        //DrawMCData(hDeltaTMaxData,hDeltaTMaxMC,c,output,true);
    }

    // ========================================================
    // 12. Chi^2_2pi
    // ========================================================

    if(hChi2_2piMC && hChi2_2piData){
        hChi2_2piMC->GetXaxis()->SetRangeUser(-5,140);
        hChi2_2piData->GetXaxis()->SetRangeUser(-5,140);
        DrawMCData(hChi2_2piMC,hChi2_2piData,c,output,true);
        //DrawMCData(hChi2_2piData,hChi2_2piMC,c,output,true);
    }

    // ========================================================
    // 13. Angle between pions
    // ========================================================

    if(hPionAngleMC && hPionAngleData){
        DrawMCData(hPionAngleMC,hPionAngleData,c,output,true);
        //DrawMCData(hPionAngleData,hPionAngleMC,c,output,true);
    }

    // ========================================================
    // 14. Mass of first pion
    // ========================================================

    if(hPionMass1MC && hPionMass1Data){
        DrawMCData(hPionMass1MC,hPionMass1Data,c,output,true);
        //DrawMCData(hPionMass1Data,hPionMass1MC,c,output,true);
    }

    // ========================================================
    // 15. Mass of second pion
    // ========================================================

    if(hPionMass2MC && hPionMass2Data){
        DrawMCData(hPionMass2MC,hPionMass2Data,c,output,true);
        //DrawMCData(hPionMass2Data,hPionMass2MC,c,output,true);
    }

    // ========================================================
    // 16. Energy of fourmomentum
    // ========================================================

    if(hDeltaEMC && hDeltaEData){
        DrawMCData(hDeltaEMC,hDeltaEData,c,output,true);
        //DrawMCData(hDeltaEData,hDeltaEMC,c,output,true);
    }

    // ========================================================
    // 17. Beta vs KL crash energy
    // ========================================================

    if(hBetaVsEcrashMC)
    {
        TCanvas *cBetaE_MC =
            new TCanvas("cBetaE_MC",
                        "Beta vs KL crash energy MC",
                        800, 700);

        hBetaVsEcrashMC->SetStats(0);
        hBetaVsEcrashMC->Draw("COLZ");

        cBetaE_MC->Modified();
        cBetaE_MC->Update();
        cBetaE_MC->Print(output);

        delete cBetaE_MC;
    }


    if(hBetaVsEcrashData)
    {
        TCanvas *cBetaE_Data =
            new TCanvas("cBetaE_Data",
                        "Beta vs KL crash energy DATA",
                        800, 700);

        hBetaVsEcrashData->SetStats(0);
        hBetaVsEcrashData->Draw("COLZ");

        cBetaE_Data->Modified();
        cBetaE_Data->Update();
        cBetaE_Data->Print(output);

        delete cBetaE_Data;
    }



    //----------- WYNIKI DOPASOWANIA -----------//


    // ============================================================
    //                  Delta Px FIT
    // ============================================================

    TF1 *fitDeltaPxMC = new TF1("fitDeltaPxMC", "gaus", -50, 50);
    hDeltaPxMC->Fit(fitDeltaPxMC, "RQ");

    TF1 *fitDeltaPxData = new TF1("fitDeltaPxData", "gaus", -50, 50);
    hDeltaPxData->Fit(fitDeltaPxData, "RQ");

    double deltaPxMeanMC     = fitDeltaPxMC->GetParameter(1);
    double deltaPxMeanMC_err = fitDeltaPxMC->GetParError(1);

    double deltaPxSigmaMC     = fitDeltaPxMC->GetParameter(2);
    double deltaPxSigmaMC_err = fitDeltaPxMC->GetParError(2);

    double deltaPxMeanData     = fitDeltaPxData->GetParameter(1);
    double deltaPxMeanData_err = fitDeltaPxData->GetParError(1);

    double deltaPxSigmaData     = fitDeltaPxData->GetParameter(2);
    double deltaPxSigmaData_err = fitDeltaPxData->GetParError(2);

    cout << endl;
    cout << "======================================" << endl;
    cout << "            Delta Px FIT              " << endl;
    cout << "======================================" << endl;

    cout << "Mean MC   = " << deltaPxMeanMC
        << " +/- " << deltaPxMeanMC_err << endl;

    cout << "Sigma MC  = " << deltaPxSigmaMC
        << " +/- " << deltaPxSigmaMC_err << endl;

    cout << "Mean Data = " << deltaPxMeanData
        << " +/- " << deltaPxMeanData_err << endl;

    cout << "Sigma Data = " << deltaPxSigmaData
        << " +/- " << deltaPxSigmaData_err << endl;

    cout << "======================================" << endl;


    // ============================================================
    //                  Delta Py FIT
    // ============================================================

    TF1 *fitDeltaPyMC = new TF1("fitDeltaPyMC", "gaus", -50, 50);
    hDeltaPyMC->Fit(fitDeltaPyMC, "RQ");

    TF1 *fitDeltaPyData = new TF1("fitDeltaPyData", "gaus", -50, 50);
    hDeltaPyData->Fit(fitDeltaPyData, "RQ");

    double deltaPyMeanMC     = fitDeltaPyMC->GetParameter(1);
    double deltaPyMeanMC_err = fitDeltaPyMC->GetParError(1);

    double deltaPySigmaMC     = fitDeltaPyMC->GetParameter(2);
    double deltaPySigmaMC_err = fitDeltaPyMC->GetParError(2);

    double deltaPyMeanData     = fitDeltaPyData->GetParameter(1);
    double deltaPyMeanData_err = fitDeltaPyData->GetParError(1);

    double deltaPySigmaData     = fitDeltaPyData->GetParameter(2);
    double deltaPySigmaData_err = fitDeltaPyData->GetParError(2);

    cout << endl;
    cout << "======================================" << endl;
    cout << "            Delta Py FIT              " << endl;
    cout << "======================================" << endl;

    cout << "Mean MC   = " << deltaPyMeanMC
        << " +/- " << deltaPyMeanMC_err << endl;

    cout << "Sigma MC  = " << deltaPySigmaMC
        << " +/- " << deltaPySigmaMC_err << endl;

    cout << "Mean Data = " << deltaPyMeanData
        << " +/- " << deltaPyMeanData_err << endl;

    cout << "Sigma Data = " << deltaPySigmaData
        << " +/- " << deltaPySigmaData_err << endl;

    cout << "======================================" << endl;


    // ============================================================
    //                  Delta Pz FIT
    // ============================================================

    TF1 *fitDeltaPzMC = new TF1("fitDeltaPzMC", "gaus", -50, 50);
    hDeltaPzMC->Fit(fitDeltaPzMC, "RQ");

    TF1 *fitDeltaPzData = new TF1("fitDeltaPzData", "gaus", -50, 50);
    hDeltaPzData->Fit(fitDeltaPzData, "RQ");

    double deltaPzMeanMC     = fitDeltaPzMC->GetParameter(1);
    double deltaPzMeanMC_err = fitDeltaPzMC->GetParError(1);

    double deltaPzSigmaMC     = fitDeltaPzMC->GetParameter(2);
    double deltaPzSigmaMC_err = fitDeltaPzMC->GetParError(2);

    double deltaPzMeanData     = fitDeltaPzData->GetParameter(1);
    double deltaPzMeanData_err = fitDeltaPzData->GetParError(1);

    double deltaPzSigmaData     = fitDeltaPzData->GetParameter(2);
    double deltaPzSigmaData_err = fitDeltaPzData->GetParError(2);

    cout << endl;
    cout << "======================================" << endl;
    cout << "            Delta Pz FIT              " << endl;
    cout << "======================================" << endl;

    cout << "Mean MC   = " << deltaPzMeanMC
        << " +/- " << deltaPzMeanMC_err << endl;

    cout << "Sigma MC  = " << deltaPzSigmaMC
        << " +/- " << deltaPzSigmaMC_err << endl;

    cout << "Mean Data = " << deltaPzMeanData
        << " +/- " << deltaPzMeanData_err << endl;

    cout << "Sigma Data = " << deltaPzSigmaData
        << " +/- " << deltaPzSigmaData_err << endl;

    cout << "======================================" << endl;

    // ============================================================
    //                    Delta E FIT
    // ============================================================

    TF1 *fitDeltaEMC = new TF1("fitDeltaEMC", "gaus", -90, 100);
    hDeltaEMC->Fit(fitDeltaEMC, "RQ");

    TF1 *fitDeltaEData = new TF1("fitDeltaEData", "gaus", -90, 100);
    hDeltaEData->Fit(fitDeltaEData, "RQ");

    double deltaEMeanMC     = fitDeltaEMC->GetParameter(1);
    double deltaEMeanMC_err = fitDeltaEMC->GetParError(1);

    double deltaESigmaMC     = fitDeltaEMC->GetParameter(2);
    double deltaESigmaMC_err = fitDeltaEMC->GetParError(2);

    double deltaEMeanData     = fitDeltaEData->GetParameter(1);
    double deltaEMeanData_err = fitDeltaEData->GetParError(1);

    double deltaESigmaData     = fitDeltaEData->GetParameter(2);
    double deltaESigmaData_err = fitDeltaEData->GetParError(2);

    cout << endl;
    cout << "======================================" << endl;
    cout << "             Delta E FIT              " << endl;
    cout << "======================================" << endl;

    cout << "Mean MC   = " << deltaEMeanMC
        << " +/- " << deltaEMeanMC_err << endl;

    cout << "Sigma MC  = " << deltaESigmaMC
        << " +/- " << deltaESigmaMC_err << endl;

    cout << "Mean Data = " << deltaEMeanData
        << " +/- " << deltaEMeanData_err << endl;

    cout << "Sigma Data = " << deltaESigmaData
        << " +/- " << deltaESigmaData_err << endl;

    cout << "======================================" << endl;



    // ============================================================
    //                  PI0 MASS FIT MC
    // ============================================================

    TF1 *fitPionMass1MC = new TF1("fitPionMass1MC", "gaus", 100, 175);
    hPionMass1MC->Fit(fitPionMass1MC, "RQ");

    TF1 *fitPionMass2MC = new TF1("fitPionMass2MC", "gaus", 100, 175);
    hPionMass2MC->Fit(fitPionMass2MC, "RQ");

    double pionMass1MeanMC     = fitPionMass1MC->GetParameter(1);
    double pionMass1MeanMC_err = fitPionMass1MC->GetParError(1);

    double pionMass1SigmaMC     = fitPionMass1MC->GetParameter(2);
    double pionMass1SigmaMC_err = fitPionMass1MC->GetParError(2);

    double pionMass2MeanMC     = fitPionMass2MC->GetParameter(1);
    double pionMass2MeanMC_err = fitPionMass2MC->GetParError(1);

    double pionMass2SigmaMC     = fitPionMass2MC->GetParameter(2);
    double pionMass2SigmaMC_err = fitPionMass2MC->GetParError(2);

    cout << endl;
    cout << "======================================" << endl;
    cout << "            PI0 MASS FIT MC           " << endl;
    cout << "======================================" << endl;

    cout << "Mean1  = " << pionMass1MeanMC
        << " +/- " << pionMass1MeanMC_err << " MeV" << endl;

    cout << "Sigma1 = " << pionMass1SigmaMC
        << " +/- " << pionMass1SigmaMC_err << " MeV" << endl;

    cout << "Mean2  = " << pionMass2MeanMC
        << " +/- " << pionMass2MeanMC_err << " MeV" << endl;

    cout << "Sigma2 = " << pionMass2SigmaMC
        << " +/- " << pionMass2SigmaMC_err << " MeV" << endl;

    cout << "======================================" << endl;


    // ============================================================
    //                 PI0 MASS FIT DATA
    // ============================================================

    TF1 *fitPionMass1Data = new TF1("fitPionMass1Data", "gaus", 100, 175);
    hPionMass1Data->Fit(fitPionMass1Data, "RQ");

    TF1 *fitPionMass2Data = new TF1("fitPionMass2Data", "gaus", 100, 175);
    hPionMass2Data->Fit(fitPionMass2Data, "RQ");

    double pionMass1MeanData     = fitPionMass1Data->GetParameter(1);
    double pionMass1MeanData_err = fitPionMass1Data->GetParError(1);

    double pionMass1SigmaData     = fitPionMass1Data->GetParameter(2);
    double pionMass1SigmaData_err = fitPionMass1Data->GetParError(2);

    double pionMass2MeanData     = fitPionMass2Data->GetParameter(1);
    double pionMass2MeanData_err = fitPionMass2Data->GetParError(1);

    double pionMass2SigmaData     = fitPionMass2Data->GetParameter(2);
    double pionMass2SigmaData_err = fitPionMass2Data->GetParError(2);

    cout << endl;
    cout << "======================================" << endl;
    cout << "          PI0 MASS FIT DATA           " << endl;
    cout << "======================================" << endl;

    cout << "Mean1  = " << pionMass1MeanData
        << " +/- " << pionMass1MeanData_err << " MeV" << endl;

    cout << "Sigma1 = " << pionMass1SigmaData
        << " +/- " << pionMass1SigmaData_err << " MeV" << endl;

    cout << "Mean2  = " << pionMass2MeanData
        << " +/- " << pionMass2MeanData_err << " MeV" << endl;

    cout << "Sigma2 = " << pionMass2SigmaData
        << " +/- " << pionMass2SigmaData_err << " MeV" << endl;

    cout << "======================================" << endl;


    // ============================================================
    //                   PI0 ANGLE FIT
    // ============================================================

    TF1 *fitPionAngleMC = new TF1("fitPionAngleMC", "gaus", 165, 180);
    hPionAngleMC->Fit(fitPionAngleMC, "RQ");

    TF1 *fitPionAngleData = new TF1("fitPionAngleData", "gaus", 165, 180);
    hPionAngleData->Fit(fitPionAngleData, "RQ");

    double pionAngleMeanMC     = fitPionAngleMC->GetParameter(1);
    double pionAngleMeanMC_err = fitPionAngleMC->GetParError(1);

    double pionAngleSigmaMC     = fitPionAngleMC->GetParameter(2);
    double pionAngleSigmaMC_err = fitPionAngleMC->GetParError(2);

    double pionAngleMeanData     = fitPionAngleData->GetParameter(1);
    double pionAngleMeanData_err = fitPionAngleData->GetParError(1);

    double pionAngleSigmaData     = fitPionAngleData->GetParameter(2);
    double pionAngleSigmaData_err = fitPionAngleData->GetParError(2);

    cout << endl;
    cout << "======================================" << endl;
    cout << "             PI0 Angle FIT            " << endl;
    cout << "======================================" << endl;

    cout << "Mean MC  = " << pionAngleMeanMC
        << " +/- " << pionAngleMeanMC_err << " deg" << endl;

    cout << "Sigma MC = " << pionAngleSigmaMC
        << " +/- " << pionAngleSigmaMC_err << " deg" << endl;

    cout << "Mean Data = " << pionAngleMeanData
        << " +/- " << pionAngleMeanData_err << " deg" << endl;

    cout << "Sigma Data = " << pionAngleSigmaData
        << " +/- " << pionAngleSigmaData_err << " deg" << endl;

    cout << "======================================" << endl;


    // TF1 *fitbetaMC = new TF1("fitbetaMC", "gaus", 0.2, 0.225);
    // hBetaMC->Fit(fitbetaMC, "RQ");

    // TF1 *fitbetaData = new TF1("fitbetaData", "gaus", 0.2, 0.225);
    // hBetaData->Fit(fitbetaData, "RQ");

    // double betaMeanMC      = fitbetaMC->GetParameter(1);
    // double betaMeanMC_err  = fitbetaMC->GetParError(1);

    // double betaSigmaMC     = fitbetaMC->GetParameter(2);
    // double betaSigmaMC_err = fitbetaMC->GetParError(2);

    // double betaMeanData      = fitbetaData->GetParameter(1);
    // double betaMeanData_err  = fitbetaData->GetParError(1);

    // double betaSigmaData     = fitbetaData->GetParameter(2);
    // double betaSigmaData_err = fitbetaData->GetParError(2);

    // cout << endl;
    // cout << "======================================" << endl;
    // cout << "              Beta FIT                " << endl;
    // cout << "======================================" << endl;

    // cout << "Mean MC   = " << betaMeanMC
    //     << " +/- " << betaMeanMC_err << endl;

    // cout << "Sigma MC  = " << betaSigmaMC
    //     << " +/- " << betaSigmaMC_err << endl;

    // cout << "Mean Data = " << betaMeanData
    //     << " +/- " << betaMeanData_err << endl;

    // cout << "Sigma Data = " << betaSigmaData
    //     << " +/- " << betaSigmaData_err << endl;

    // cout << "======================================" << endl;


    c->Print(Form("%s]",output));

    f->Close();

    delete c;

}
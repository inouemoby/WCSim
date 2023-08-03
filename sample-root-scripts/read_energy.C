/*
#include <iostream>
#include <TH1F.h>
#include <stdio.h>
#include <stdlib.h>
#include "TPad.h"
#include "/home/zhw1999/WCSim-build/ROOT5/nuPRISM/develop/include/WCSimRootEvent.hh"
#include "/home/zhw1999/WCSim-build/ROOT5/nuPRISM/develop/include/WCSimRootGeom.hh"
#include "/home/zhw1999/WCSim-build/ROOT5/nuPRISM/develop/include/WCSimRootOptions.hh"
#include "/home/zhw1999/WCSim-build/ROOT5/nuPRISM/develop/include/WCSimRootTools.hh"
*/

void read_energy2(const char *filename = "../rootfile/wcsim_output.root")
{
    char *wcsimdirenv;
    wcsimdirenv = getenv("WCSIMDIR");
    if (wcsimdirenv != NULL)
    {
        gSystem->Load("${WCSIMDIR}/libWCSimRoot.so");
    }
    else
    {
        gSystem->Load("../libWCSimRoot.so");
    }

    // 打开ROOT文件
    TFile *file = TFile::Open(filename);
    if (!file || file->IsZombie())
    {
        // 处理无法打开文件的情况
        return;
    }

    // 获取TTree "wcsimT"
    TTree *tree = (TTree *)file->Get("wcsimT");
    if (!tree)
    {
        // 处理找不到"wcsimT"的情况
        file->Close();
        return;
    }

    // 声明需要读取的变量
    WCSimRootEvent *wcsimrootsuperevent = new WCSimRootEvent();
    tree->SetBranchAddress("wcsimrootevent", &wcsimrootsuperevent);

    // 创建TH1F来保存能量分布和动量分布
    TH1F *energyHist = new TH1F("Energy Distribution", "Energy Distribution", 100, 0, 10);       // 假设能量范围在0到10之间
    TH1F *momentumHist = new TH1F("Momentum Distribution", "Momentum Distribution", 100, 0, 10); // 假设动量范围在0到10之间

    // 获取总事件数
    int numEvents = tree->GetEntries();

    // 循环读取每个事件的信息
    for (int i = 0; i < numEvents; ++i)
    {
        tree->GetEntry(i);

        // 获取WCSimRootTrigger对象，该对象包含每个事件的track信息
        WCSimRootTrigger *wcsimrootevent = wcsimrootsuperevent->GetTrigger(0);

        // 定义每个事件的能量和动量
        double eventEnergy = 0;
        double eventMomentum = 0;

        // 循环读取每个track的能量和动量，并累加到事件的能量和动量中
        for (int j = 0; j < wcsimrootevent->GetNtrack(); ++j)
        {
            WCSimRootTrack *wcsimroottrack = (WCSimRootTrack *)wcsimrootevent->GetTracks()->At(j);

            // 只处理最初生成的粒子的track (Id = 1)
            if (wcsimroottrack->GetId() == 1)
            {
                // 获取粒子的能量和动量
                double energy = wcsimroottrack->GetE();
                double momentum = wcsimroottrack->GetP();

                // 累加到事件的能量和动量中
                eventEnergy += energy;
                eventMomentum += momentum;
            }
        }
        std::cout << i << " event total energy is " << eventEnergy << std::endl;

        // 将每个事件的能量和动量累积到对应的直方图中
        energyHist->Fill(eventEnergy);
        momentumHist->Fill(eventMomentum);
    }

    energyHist->Print();
    momentumHist->Print();

    // 画出能量分布和动量分布的直方图

    /*
    // 创建两个画布，并在不同的画布上绘制直方图
    TCanvas *c1 = new TCanvas("c1", "Energy Distribution", 800, 600);
    c1->Draw();
    //c1->cd(1);
    energyHist->Draw();

    TCanvas *c2 = new TCanvas("c2", "Momentum Distribution", 800, 600);
    c2->Draw();
    //c2->cd(1);
    momentumHist->Draw();
    */
    TCanvas *c1 = new TCanvas("c1", "Energy and Momentum Distribution", 1200, 600);
    c1->Divide(2, 1); // 将画布分成1行2列

    c1->cd(1); // 切换到第一个子画布
    energyHist->Draw();

    c1->cd(2); // 切换到第二个子画布
    momentumHist->Draw();

    c1->Update();
    // gPad->Modified();
    //  gPad->Update();

    // 关闭文件
    file->Close();
}

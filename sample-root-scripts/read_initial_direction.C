#include <iostream>
#include <TH1F.h>
#include <TCanvas.h>
#include <TFile.h>
#include <TTree.h>
#include "/home/zhw1999/WCSim-build/ROOT5/nuPRISM/develop/include/WCSimRootEvent.hh"
#include "/home/zhw1999/WCSim-build/ROOT5/nuPRISM/develop/include/WCSimRootGeom.hh"
#include "/home/zhw1999/WCSim-build/ROOT5/nuPRISM/develop/include/WCSimRootOptions.hh"
#include "/home/zhw1999/WCSim-build/ROOT5/nuPRISM/develop/include/WCSimRootTools.hh"
#include <vector>

void read_initial_direction(const char* filename = "../rootfile/wcsim_output.root")
{
    char* wcsimdirenv;
    wcsimdirenv = getenv ("WCSIMDIR");
    if (wcsimdirenv != NULL) {
        gSystem->Load("${WCSIMDIR}/libWCSimRoot.so");
    } else {
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
    TTree *tree = (TTree*)file->Get("wcsimT");
    if (!tree)
    {
        // 处理找不到"wcsimT"的情况
        file->Close();
        return;
    }

    // 声明需要读取的变量
    WCSimRootEvent *wcsimrootsuperevent = new WCSimRootEvent();
    tree->SetBranchAddress("wcsimrootevent", &wcsimrootsuperevent);

    // 创建一个vector来保存所有事件的初始轨迹方向信息
    std::vector<double> dirXs;
    std::vector<double> dirYs;
    std::vector<double> dirZs;

    // 获取总事件数
    int numEvents = tree->GetEntries();

    // 循环读取每个事件的信息
    for (int i = 0; i < numEvents; ++i)
    {
        tree->GetEntry(i);

        // 获取WCSimRootTrigger对象，该对象包含每个事件的track信息
        WCSimRootTrigger *wcsimrootevent = wcsimrootsuperevent->GetTrigger(0);

        // 循环读取每个track的初始方向信息
        for (int j = 0; j < wcsimrootevent->GetNtrack(); ++j)
        {
            WCSimRootTrack *wcsimroottrack = (WCSimRootTrack*)wcsimrootevent->GetTracks()->At(j);

            // 获取粒子的初始方向信息
            double dirX = wcsimroottrack->GetDir(0);
            double dirY = wcsimroottrack->GetDir(1);
            double dirZ = wcsimroottrack->GetDir(2);

            // 将方向信息添加到vector中
            dirXs.push_back(dirX);
            dirYs.push_back(dirY);
            dirZs.push_back(dirZ);
        }
    }

    // 创建Canvas并绘制直方图
    TCanvas *c1 = new TCanvas("c1", "Initial Direction", 800, 800);
    TH1F *hDirX = new TH1F("hDirX", "Initial Direction in X", 100, -1.5, 1.5);
    TH1F *hDirY = new TH1F("hDirY", "Initial Direction in Y", 100, -1.5, 1.5);
    TH1F *hDirZ = new TH1F("hDirZ", "Initial Direction in Z", 100, -1.5, 1.5);

    for (size_t k = 0; k < dirXs.size(); ++k)
    {
        hDirX->Fill(dirXs[k]);
        hDirY->Fill(dirYs[k]);
        hDirZ->Fill(dirZs[k]);
    }

    c1->Divide(2, 2);
    c1->cd(1);
    hDirX->Draw();
    c1->cd(2);
    hDirY->Draw();
    c1->cd(3);
    hDirZ->Draw();

    c1->Update();

    // 关闭文件
    file->Close();
}

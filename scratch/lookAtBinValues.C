
DT1->SetLineColor(kRed+2);
DT2->SetLineColor(kBlue+2);
DT3->SetLineColor(kGreen+2);
DT4->SetLineColor(kMagenta+2);

double lwidth=3;
DT1->SetLineWidth(lwidth);
DT2->SetLineWidth(lwidth);
DT3->SetLineWidth(lwidth);
DT4->SetLineWidth(lwidth);

DT1->Project3D("z")->Draw();
DT2->Project3D("z")->Draw("same")
DT3->Project3D("z")->Draw("same");
DT4->Project3D("z")->Draw("same");


DT1->Project3D("x")->Draw();
DT2->Project3D("x")->Draw("same")
DT3->Project3D("x")->Draw("same");
DT4->Project3D("x")->Draw("same");


DT1->SetLineColor(1);
DT2->SetLineColor(2);
DT3->SetLineColor(3);
DT4->SetLineColor(4);

DT1->Project3D("z")->Draw();
DT2->Project3D("z")->Draw("same")
DT3->Project3D("z")->Draw("same");
DT4->Project3D("z")->Draw("same");


DT1->Project3D("x")->Draw();
DT2->Project3D("x")->Draw("same")
DT3->Project3D("x")->Draw("same");
DT4->Project3D("x")->Draw("same");

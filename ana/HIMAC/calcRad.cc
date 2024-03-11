const double dose_2424 = 6.67; // kGy/h (Measurement value)
const double dose_2423 = 8.18; // kGy/h (Measurement value)
const double times = 136.0; // hours
const double rho_Si = 2.3; // Si density [g/cm3]
const double rho_Si_kg = 2.3e3; // Si density [kg/m3]
const double w = 6.21e-6; // Si sensor weight [kg]

void calcRad(){

  double dose = dose_2424;
  // ----- Path Length ---------- //
  double tAG = 1.1e-2; // [m]
  double l_CIGS = 2.0e-3; // [m]
  cout<<"<< ------- Dose Correction ---------- >>"<<endl;
  cout<<"Dose #2423 "<<dose_2423<<" kGy/h "<<" total doze "<<dose_2423*times<<endl; // kGy/h
  cout<<"Dose #2424 "<<dose_2424<<" kGy/h "<<" total doze "<<dose_2424*times<<endl; // kGy/h
  cout<<"Diff (#2423 - #2424) : "<<(dose_2423 - dose_2424)<<" kGy/h"<<" total Diff "<<(dose_2423-dose_2424)*times<<endl;
  cout<<"Diff Lenght : "<<tAG*100. + 1.1 <<" [cm]"<<endl;

  cout<<endl;


  // ----- Si Parameters --------- //  
  double mu = 8.22 ; // [m]
  double tSi = 3.0e-3; // thickness (Si) [m]
  double ASi =  exp(-mu*tSi);

  
  // ----- Cu Parameters --------- //
  //  double Cu_1_10 = 
  double mu_Cu  = 0.578e2; //[1/m]
  double mu_Cu_m = 8.93e-1; // [m2/kg]
  double rho_Cu_kg = 8.96e3; // [kg/m3]
  double tCu = 1.0e-3; // [m]
  //  double ACu =  exp(-mu_Cu*tCu);
  //  cout<<"ACu "<<ACu<<endl;

  double ACu =  9.897e-1; // reffered 
  // -----  Fe Parameters -------- //
  double Fe_1_10 = 6.7e-2; // 1/10 [m]
  double mu_Fe   = -1.*(log(1./10.)/Fe_1_10);
  double rho_Fe_kg = 7.874e3; // [kg/m3]
  double tFe = 1.0e-3; // [m]
  double mu_Fe_m   = mu_Fe/rho_Fe_kg;
  double AFe = exp(-mu_Fe*tFe);

  // ----------------------------- //


  AFe = 0.987;
  ACu = 0.990;
  

  double a = (7.570 - 6.800)/(7.3e-2 - 5.6e-2);
  //  double dL = tAG/2. + tFe + tCu + 2.0e-3; // for CIGS
   double dL = tAG/2. + tFe + 0.01 + 2.0e-3; // for GaN
  double dose_dL =dL*a;

  
  //  double total_dose = dose_2423*times*AFe*ACu - dose_dL*times;
    double total_dose = dose_2423*times*ACu - dose_dL*times;
  //  double total_dose = dose*times/AFe;



  
  cout<<"========= < Results > ==================="<<endl;
  cout<<"Absorb (Fe) 1 mm "<<(1.- AFe)*total_dose<<" (kGy)"<<endl;
  cout<<"Absorb (Cu) 1 mm "<<(1.- ACu)*total_dose<<" (kGya)"<<endl;
  cout<<"Absorbed Dose (kGy) "<<dose_2423*times*AFe*ACu<<endl;
  cout<<" total dose_dL : "<<dose_dL*times<<" (kGy)"<<endl;
  cout<<"Amino Gray Total Doze (kGy) : "<<dose_2423*times<<endl;
  cout<<"CIGS Total Doze (kGy) : "<<total_dose<<endl;
  cout<<"=========================================="<<endl;





  // ======== Calc CIGS Dose from BackSide ======= //

  double dL2 = tAG/2.;


  
}

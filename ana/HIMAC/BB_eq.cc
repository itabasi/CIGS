extern double CalcI(double Z);

#define Xe

void BB_eq(){

  double u   = 132.;
  double Ebeam = 400.*u;
  double M =132.; //incident particle mass
  
#ifdef Xe
  u = 132.;
  Ebeam = 400.*u;
  M =
    //    z = 
#endif
  
  double Pbeam = sqrt(Ebeam*Ebam - M*M);
  double pi = acos(-1.);
  double me = 0.51;  // MeV
  double z =1; // Number of irradiated charge
  double Na = 6.022e23; // Avogadro constant
  double Z  =1 ; // Nuber of proton
  double A  =1; // Nuber of nuceon
  double pho =2.3; // density [g/cm^3]
  double Mu = ; // Molar mass constant
  double n = Na*Z*rho/(A*Mu); // electronical density
  double e = 1.6e-19; // elementary charge [C]
  double beta = 1;
  double  I=CalcI(Z);
  double epsilon = 8.86r-12; // F/m
  double re =2.817e-13; //classical electron radius  [cm]
  double gamma = 1/sqrt(1-beta*beta);

  double s =me/M;
  double eta =beta*gamma;
  double Wmax = 2.*me*eta*eta/(1.+2.*sqrt(1.+eta*eta+s*s));


  //  double dEdx = 4.*pi/me*n*z*z/(beta*beta)*pow(e*e/(4.*pi*epsilon),2)
  //    *log(2.*me*beta*beta/(I*(1-beta*beata))-beta*beta);

  double dEdx =2.*pi*Na*re*re*me*rho*Z/A*z*z/beta/beta*(log(2.*me*gamma*gamma*v*v*Wmax/I/I )-2.beta*beta) ;

  

}


double CalcI(double Z ){


  double I;
  if(Z<13){
    I = 12.*Z + 7.; // eV
  }else{
    I = 9.76+ 58.8* pow(Z,-1.19); // eV
  }

  I *=1.e-6; // eV to MeV
  return I;

}

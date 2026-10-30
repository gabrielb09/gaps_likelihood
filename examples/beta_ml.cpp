#include "detector.hh"

int main(){
    // step 1: define detectors
    gaps_ml::Detector sili = gaps_ml::Detector(gaps_ml::silicon, gaps_ml::sili_wid*gaps_ml::rho_si, 0.1);
    gaps_ml::Detector tof = gaps_ml::Detector(gaps_ml::pvt, gaps_ml::tof_wid*gaps_ml::rho_pvt, 0.25);

    // step 2:

    return 0;
}
